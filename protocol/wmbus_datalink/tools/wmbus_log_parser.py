#!/usr/bin/env python3
"""Parse WMBus datalink UART logs.

This tool is intentionally conservative: it parses the fixed tag format used by
WMBUS_DBG_MSG_FLOW / WMBUS_DBG_MSG_ERROR and reports the first obvious problem
with nearby context.
"""

from __future__ import annotations

import argparse
import contextlib
import glob
import io
import json
import re
import sys
from collections import Counter
from dataclasses import dataclass, field
from datetime import datetime
from pathlib import Path
from typing import Iterable


# Match tagged WMBus events inside a line. A UART log line may contain more
# than one event if a firmware log message was truncated before its newline.
# The first tag must be a known WMBus log role to avoid matching incidental
# Python/C type annotations such as list[str].
TAG_RE = re.compile(
    r"((?:\[(?:GW|MTR|CONN)\](?:\[[A-Za-z0-9_]+\])+))"
)
ONE_TAG_RE = re.compile(r"\[([A-Za-z0-9_]+)\]")
KV_RE = re.compile(r"([A-Za-z0-9_]+)=([^,\s]+)")
TIMESTAMP_RE = re.compile(r"^\[([A-Za-z]{3} [A-Za-z]{3} +\d+ \d\d:\d\d:\d\d\.\d+ \d{4})\]")
BOOT_MARKER_RE = re.compile(r"\bWISE_CORE_V2\b|Built@")
MANUAL_BOUNDARY_RE = re.compile(r"\bwm\s+(?:stop|fsm)\b")
RAW_GW_RX_RE = re.compile(r"\bGW receives data\b.*\bfrom ID:(0x[0-9A-Fa-f]+)")
RAW_MTR_RX_RE = re.compile(r"\bMTR receives raw data\b")
RAW_MTR_TX_DONE_RE = re.compile(r"\bTx done\b")
RAW_MTR_SESSION_END_RE = re.compile(r"\bReceived NKE so finish this session\b")
SOURCE_DEV_RE = re.compile(r"(?:0x)?([0-9A-Fa-f]{8})")


@dataclass
class Event:
    source: str
    line_no: int
    raw: str
    tags: list[str]
    fields: dict[str, str]
    timestamp_ms: int | None = None

    @property
    def role(self) -> str:
        return self.tags[0] if self.tags else "UNKNOWN"

    @property
    def kind(self) -> str:
        if "ERR" in self.tags:
            return "ERR"
        if "WARN" in self.tags:
            return "WARN"
        return self.tags[1] if len(self.tags) >= 2 else "UNKNOWN"

    @property
    def reason(self) -> str:
        for marker in ("ERR", "WARN"):
            if marker in self.tags:
                marker_index = self.tags.index(marker)
                if marker_index + 1 < len(self.tags):
                    return self.tags[marker_index + 1]
                if marker_index > 1:
                    return self.tags[marker_index - 1]
                return marker
        return self.kind

    @property
    def dev(self) -> str:
        return self.fields.get("dev", "unknown")


@dataclass
class SourceReport:
    name: str
    path: Path
    total_lines: int = 0
    parsed_events: int = 0
    counters: Counter[str] = field(default_factory=Counter)
    errors: list[Event] = field(default_factory=list)
    warn_events: list[Event] = field(default_factory=list)
    parser_diagnostics: list["ParserDiagnostic"] = field(default_factory=list)
    warnings: list[str] = field(default_factory=list)
    lines: list[str] = field(default_factory=list)


@dataclass
class ParserDiagnostic:
    source: str
    line_no: int
    level: str
    code: str
    message: str
    raw_line: str | None = None


@dataclass
class MeterWatchdogState:
    last_trigger: Event | None = None
    last_liveness: Event | None = None
    liveness_seen_since_trigger: bool = False
    session_stuck_reported: bool = False


def source_name(prefix: str, path: Path) -> str:
    stem = path.stem.replace(" ", "_")
    if stem.lower().startswith(prefix.lower()):
        return stem
    return f"{prefix}_{stem}"


def expand_paths(patterns: Iterable[Path]) -> list[Path]:
    paths: list[Path] = []
    seen: set[str] = set()

    for pattern in patterns:
        pattern_text = str(pattern)
        matches = sorted(Path(match) for match in glob.glob(pattern_text))
        if not matches:
            matches = [pattern]

        for path in matches:
            key = str(path.resolve()) if path.exists() else str(path)
            if key in seen:
                continue
            seen.add(key)
            paths.append(path)

    return paths


def parse_input_arg(value: str) -> tuple[str, Path]:
    if "=" not in value:
        raise argparse.ArgumentTypeError("expected PREFIX=LOG_FILE")

    prefix, path_text = value.split("=", 1)
    prefix = prefix.strip()
    path_text = path_text.strip()

    if not prefix:
        raise argparse.ArgumentTypeError("input prefix must not be empty")
    if not path_text:
        raise argparse.ArgumentTypeError("input log file must not be empty")

    return prefix, Path(path_text)


def parse_timestamp_ms(raw: str) -> int | None:
    match = TIMESTAMP_RE.match(raw)
    if not match:
        return None

    try:
        timestamp = datetime.strptime(match.group(1), "%a %b %d %H:%M:%S.%f %Y")
    except ValueError:
        return None

    return int(timestamp.timestamp() * 1000)


def parse_line(source: str, line_no: int, raw: str) -> list[Event]:
    matches = list(TAG_RE.finditer(raw))
    events: list[Event] = []
    timestamp_ms = parse_timestamp_ms(raw)

    for idx, match in enumerate(matches):
        event_end = matches[idx + 1].start() if idx + 1 < len(matches) else len(raw)
        event_raw = raw[match.start():event_end].rstrip("\n")
        payload = raw[match.end():event_end]
        tags = ONE_TAG_RE.findall(match.group(1))
        if not tags:
            continue

        fields = dict(KV_RE.findall(payload))
        events.append(Event(source=source, line_no=line_no, raw=event_raw, tags=tags, fields=fields, timestamp_ms=timestamp_ms))

    return events


def infer_source_dev(name: str) -> str | None:
    match = SOURCE_DEV_RE.search(name)
    if not match:
        return None
    return f"0x{match.group(1).lower()}"


def event_key(event: Event) -> str:
    if event.kind == "ERR":
        return f"{event.role}.ERR.{event.reason}"
    if event.kind == "WARN":
        return f"{event.role}.WARN.{event.reason}"
    if event.role == "CONN":
        return f"CONN.{event.kind}"
    return f"{event.role}.{event.kind}"


def read_events(name: str, path: Path) -> tuple[SourceReport, list[Event]]:
    report = SourceReport(name=name, path=path)
    events: list[Event] = []

    with path.open("r", encoding="utf-8", errors="replace") as log_file:
        for line_no, line in enumerate(log_file, start=1):
            report.total_lines += 1
            report.lines.append(line.rstrip("\n"))

            line_events = parse_line(name, line_no, line)
            if not line_events:
                continue

            for event in line_events:
                events.append(event)
                report.parsed_events += 1
                report.counters[event_key(event)] += 1

                if event.kind == "ERR":
                    report.errors.append(event)
                elif event.kind == "WARN":
                    report.warn_events.append(event)

    return report, events


def check_source(report: SourceReport, events: Iterable[Event]) -> None:
    events = list(events)
    conn_active: set[str] = set()

    for event in events:
        if event.role == "CONN" and event.kind == "ADD":
            if event.dev in conn_active:
                report.warnings.append(
                    f"{event.source}:{event.line_no}: duplicate active connection dev={event.dev}"
                )
            else:
                conn_active.add(event.dev)
        elif event.role == "CONN" and event.kind == "REMOVE":
            if event.dev not in conn_active:
                report.warnings.append(
                    f"{event.source}:{event.line_no}: remove without active connection dev={event.dev}"
                )
            else:
                conn_active.remove(event.dev)

    for dev in sorted(conn_active):
        report.warnings.append(f"active connection remains dev={dev}")


def is_meter_trigger_activity(event: Event) -> bool:
    if event.role == "MTR" and event.kind == "SESSION_START":
        return True
    if event.role == "MTR" and event.kind == "TX" and event.fields.get("phase") == "FIRST_REQ":
        return True
    if event.role == "GW" and event.kind == "SESSION_START" and event.fields.get("reason") == "RX_FIRST_PACKET":
        return True
    return False


def is_meter_liveness_activity(event: Event) -> bool:
    if event.role == "MTR" and event.kind not in ("ERR", "WARN"):
        return True
    if event.role == "GW" and event.kind == "RX":
        return True
    if event.role == "GW" and event.kind == "SESSION_START" and event.fields.get("reason") == "RX_FIRST_PACKET":
        return True
    return False


def is_meter_session_start(event: Event) -> bool:
    if event.role == "MTR" and event.kind == "SESSION_START":
        return True
    if event.role == "MTR" and event.kind == "TX" and event.fields.get("phase") == "FIRST_REQ":
        return True
    if event.role == "GW" and event.kind == "SESSION_START" and event.fields.get("reason") == "RX_FIRST_PACKET":
        return True
    return False


def is_meter_session_end(event: Event) -> bool:
    return event.role in ("GW", "MTR") and event.kind == "SESSION_END"


def is_raw_wmbus_activity_line(line: str) -> bool:
    return any(
        pattern.search(line)
        for pattern in (
            RAW_GW_RX_RE,
            RAW_MTR_RX_RE,
            RAW_MTR_TX_DONE_RE,
            RAW_MTR_SESSION_END_RE,
        )
    )


def add_boundary_warning(report: SourceReport, line_no: int, boundary: str, message: str) -> None:
    report.counters[f"WATCHDOG.WARN.{boundary}"] += 1
    report.warnings.append(f"{report.name}:{line_no}: watchdog reset by {boundary} boundary {message}")


def add_meter_trigger_error(report: SourceReport, line_no: int, message: str) -> None:
    report.counters["MTR.ERR.TRIGGER_STOPPED"] += 1
    report.parser_diagnostics.append(
        ParserDiagnostic(
            source=report.name,
            line_no=line_no,
            level="ERR",
            code="TRIGGER_STOPPED",
            message=message,
            raw_line=report.lines[line_no - 1] if 1 <= line_no <= report.total_lines else None,
        )
    )


def add_meter_trigger_warning(report: SourceReport, line_no: int, message: str) -> None:
    report.counters["MTR.WARN.TRIGGER_DELAYED_BY_ACTIVE_SESSION"] += 1
    report.parser_diagnostics.append(
        ParserDiagnostic(
            source=report.name,
            line_no=line_no,
            level="WARN",
            code="TRIGGER_DELAYED_BY_ACTIVE_SESSION",
            message=message,
            raw_line=report.lines[line_no - 1] if 1 <= line_no <= report.total_lines else None,
        )
    )


def add_meter_session_stuck_error(report: SourceReport, line_no: int, message: str) -> None:
    report.counters["MTR.ERR.SESSION_STUCK"] += 1
    report.parser_diagnostics.append(
        ParserDiagnostic(
            source=report.name,
            line_no=line_no,
            level="ERR",
            code="SESSION_STUCK",
            message=message,
            raw_line=report.lines[line_no - 1] if 1 <= line_no <= report.total_lines else None,
        )
    )


def add_active_session_eof_warning(report: SourceReport, line_no: int, message: str) -> None:
    report.counters["MTR.WARN.ACTIVE_SESSION_AT_EOF"] += 1
    report.parser_diagnostics.append(
        ParserDiagnostic(
            source=report.name,
            line_no=line_no,
            level="WARN",
            code="ACTIVE_SESSION_AT_EOF",
            message=message,
            raw_line=report.lines[line_no - 1] if 1 <= line_no <= report.total_lines else None,
        )
    )


def add_meter_boot_warning(report: SourceReport, line_no: int, message: str) -> None:
    report.counters["MTR.WARN.BOOT_DURING_ACTIVE_SESSION"] += 1
    report.warnings.append(f"{report.name}:{line_no}: meter boot during active session {message}")


def check_meter_boot_during_active_session(report: SourceReport, events: Iterable[Event]) -> None:
    events_by_line: dict[int, list[Event]] = {}
    for event in events:
        events_by_line.setdefault(event.line_no, []).append(event)

    active_meter_sessions: set[str] = set()
    for line_no, line in enumerate(report.lines, start=1):
        if MANUAL_BOUNDARY_RE.search(line):
            active_meter_sessions.clear()

        for event in events_by_line.get(line_no, []):
            if is_meter_session_start(event):
                active_meter_sessions.add(event.dev)
            elif is_meter_session_end(event):
                active_meter_sessions.discard(event.dev)

        if active_meter_sessions and BOOT_MARKER_RE.search(line):
            devs = ",".join(sorted(active_meter_sessions))
            add_meter_boot_warning(report, line_no, f"dev={devs}")
            active_meter_sessions.clear()


def check_meter_trigger_watchdog(
    report: SourceReport,
    events: Iterable[Event],
    period_ms: int,
    tolerance_ms: int,
    session_idle_ms: int,
) -> None:
    deadline_ms = period_ms + tolerance_ms
    events_by_line: dict[int, list[Event]] = {}
    for event in events:
        events_by_line.setdefault(event.line_no, []).append(event)

    active_meter_sessions: set[str] = set()
    states: dict[str, MeterWatchdogState] = {}
    last_timestamp_ms: int | None = None
    source_dev = infer_source_dev(report.name)

    def state_for(dev: str) -> MeterWatchdogState:
        state = states.get(dev)
        if state is None:
            state = MeterWatchdogState()
            states[dev] = state
        return state

    def format_last_liveness(state: MeterWatchdogState) -> str:
        if state.last_liveness is None:
            return "last_liveness=none last_liveness_line=none"
        return f"last_liveness={state.last_liveness.kind} last_liveness_line={state.last_liveness.line_no}"

    def infer_current_dev() -> str | None:
        if source_dev is not None:
            return source_dev
        if len(active_meter_sessions) == 1:
            return next(iter(active_meter_sessions))
        if len(states) == 1:
            return next(iter(states))
        return None

    def synthetic_event(line_no: int, raw: str, role: str, kind: str, dev: str, timestamp_ms: int | None) -> Event:
        return Event(
            source=report.name,
            line_no=line_no,
            raw=raw,
            tags=[role, kind],
            fields={"dev": dev},
            timestamp_ms=timestamp_ms,
        )

    def report_trigger_gap(dev: str, state: MeterWatchdogState, event: Event, elapsed_ms: int) -> None:
        if state.last_trigger is None:
            return

        if dev not in active_meter_sessions:
            return

        active = 1 if dev in active_meter_sessions else 0
        liveness = format_last_liveness(state)
        previous = state.last_trigger
        if not state.liveness_seen_since_trigger:
            add_meter_trigger_error(
                report,
                event.line_no,
                (
                    f"dev={dev} elapsed_ms={elapsed_ms} limit_ms={deadline_ms} "
                    f"previous_trigger={previous.kind} previous_trigger_line={previous.line_no} "
                    f"{liveness} active_session={active}"
                ),
            )
            return

        if dev in active_meter_sessions and state.last_liveness is not None and state.last_liveness.timestamp_ms is not None:
            idle_ms = event.timestamp_ms - state.last_liveness.timestamp_ms if event.timestamp_ms is not None else 0
            if idle_ms > session_idle_ms:
                if not state.session_stuck_reported:
                    add_meter_session_stuck_error(
                        report,
                        event.line_no,
                        (
                            f"dev={dev} idle_ms={idle_ms} limit_ms={session_idle_ms} "
                            f"trigger_elapsed_ms={elapsed_ms} trigger_limit_ms={deadline_ms} "
                            f"last_liveness={state.last_liveness.kind} last_liveness_line={state.last_liveness.line_no} "
                            f"previous_trigger={previous.kind} previous_trigger_line={previous.line_no}"
                        ),
                    )
                    state.session_stuck_reported = True
                return

        add_meter_trigger_warning(
            report,
            event.line_no,
            (
                f"dev={dev} elapsed_ms={elapsed_ms} limit_ms={deadline_ms} "
                f"previous_trigger={previous.kind} previous_trigger_line={previous.line_no} "
                f"{liveness} active_session={active}"
            ),
        )

    def check_session_idle(line_no: int, timestamp_ms: int) -> None:
        for dev in sorted(active_meter_sessions):
            state = states.get(dev)
            if state is None or state.last_liveness is None or state.last_liveness.timestamp_ms is None:
                continue
            if state.session_stuck_reported:
                continue

            idle_ms = timestamp_ms - state.last_liveness.timestamp_ms
            if idle_ms > session_idle_ms:
                add_meter_session_stuck_error(
                    report,
                    line_no,
                    (
                        f"dev={dev} idle_ms={idle_ms} limit_ms={session_idle_ms} "
                        f"last_liveness={state.last_liveness.kind} last_liveness_line={state.last_liveness.line_no} "
                        f"active_session=1"
                    ),
                )
                state.session_stuck_reported = True

    def update_trigger(event: Event) -> None:
        state = state_for(event.dev)
        if event.timestamp_ms is not None and state.last_trigger is not None and state.last_trigger.timestamp_ms is not None:
            elapsed_ms = event.timestamp_ms - state.last_trigger.timestamp_ms
            if elapsed_ms > deadline_ms:
                report_trigger_gap(event.dev, state, event, elapsed_ms)

        state.last_trigger = event
        state.last_liveness = event
        state.liveness_seen_since_trigger = False
        state.session_stuck_reported = False

    def update_liveness(event: Event) -> None:
        state = state_for(event.dev)
        state.last_liveness = event
        if state.last_trigger is not None and event is not state.last_trigger:
            state.liveness_seen_since_trigger = True

    def update_raw_line_activity(line_no: int, line: str, timestamp_ms: int | None) -> None:
        gw_rx_match = RAW_GW_RX_RE.search(line)
        if gw_rx_match:
            dev = gw_rx_match.group(1).lower()
            update_liveness(synthetic_event(line_no, line, "GW", "RX", dev, timestamp_ms))
            return

        dev = infer_current_dev()
        if dev is None:
            return

        if RAW_MTR_RX_RE.search(line):
            update_liveness(synthetic_event(line_no, line, "MTR", "RX", dev, timestamp_ms))
        elif RAW_MTR_TX_DONE_RE.search(line):
            update_liveness(synthetic_event(line_no, line, "MTR", "TX_DONE", dev, timestamp_ms))

        if RAW_MTR_SESSION_END_RE.search(line):
            update_liveness(synthetic_event(line_no, line, "MTR", "SESSION_END", dev, timestamp_ms))
            active_meter_sessions.discard(dev)

    for line_no, line in enumerate(report.lines, start=1):
        timestamp_ms = parse_timestamp_ms(line)
        if timestamp_ms is not None:
            last_timestamp_ms = timestamp_ms

        manual_boundary = MANUAL_BOUNDARY_RE.search(line)
        boot_boundary = BOOT_MARKER_RE.search(line)
        if manual_boundary or boot_boundary:
            if active_meter_sessions:
                boundary = "BOOT" if boot_boundary else "MANUAL"
                devs = ",".join(sorted(active_meter_sessions))
                last_lines = ",".join(
                    f"{dev}:{state.last_trigger.line_no}"
                    for dev, state in sorted(states.items())
                    if state.last_trigger is not None
                )
                add_boundary_warning(report, line_no, boundary, f"active_devs={devs} last_trigger_lines={last_lines or 'none'}")
            active_meter_sessions.clear()
            states.clear()
            continue

        line_events = events_by_line.get(line_no, [])
        line_is_terminal = any(is_meter_session_end(event) for event in line_events) or RAW_MTR_SESSION_END_RE.search(line)
        line_is_checkpoint = bool(line_events) or is_raw_wmbus_activity_line(line)
        if timestamp_ms is not None and line_is_checkpoint and not line_is_terminal:
            check_session_idle(line_no, timestamp_ms)

        for event in line_events:
            trigger_activity = is_meter_trigger_activity(event)
            if trigger_activity:
                update_trigger(event)
                if is_meter_session_start(event):
                    active_meter_sessions.add(event.dev)
            elif is_meter_session_start(event):
                active_meter_sessions.add(event.dev)
            elif is_meter_session_end(event):
                active_meter_sessions.discard(event.dev)

            if is_meter_liveness_activity(event) and not trigger_activity:
                update_liveness(event)

        update_raw_line_activity(line_no, line, timestamp_ms)

    if last_timestamp_ms is None:
        return

    check_session_idle(report.total_lines, last_timestamp_ms)
    for dev, state in sorted(states.items()):
        if state.last_trigger is None or state.last_trigger.timestamp_ms is None:
            continue
        elapsed_ms = last_timestamp_ms - state.last_trigger.timestamp_ms
        if elapsed_ms > deadline_ms:
            eof_event = Event(source=report.name, line_no=state.last_trigger.line_no, raw="", tags=["MTR", "SESSION_START"], fields={"dev": dev}, timestamp_ms=last_timestamp_ms)
            report_trigger_gap(dev, state, eof_event, elapsed_ms)

    for dev in sorted(active_meter_sessions):
        state = states.get(dev)
        if state is None:
            add_active_session_eof_warning(report, report.total_lines, f"dev={dev} last_liveness=none last_liveness_line=none")
            continue
        if state.session_stuck_reported:
            continue
        last_liveness = format_last_liveness(state)
        if state.last_trigger is not None and state.last_trigger.timestamp_ms is not None:
            elapsed_ms = last_timestamp_ms - state.last_trigger.timestamp_ms
            add_active_session_eof_warning(
                report,
                report.total_lines,
                f"dev={dev} elapsed_ms={elapsed_ms} last_trigger={state.last_trigger.kind} "
                f"last_trigger_line={state.last_trigger.line_no} {last_liveness}",
            )
        else:
            add_active_session_eof_warning(report, report.total_lines, f"dev={dev} {last_liveness}")


def first_error(reports: Iterable[SourceReport]) -> Event | None:
    errors = [error for report in reports for error in report.errors]
    if not errors:
        return None
    return min(errors, key=lambda event: (event.source, event.line_no))


def error_context_failures(reports: Iterable[SourceReport]) -> list[tuple[str, int, str]]:
    failures: list[tuple[str, int, str]] = []
    for report in reports:
        failures.extend((error.source, error.line_no, "Firmware ERR") for error in report.errors)
        failures.extend(
            (diag.source, diag.line_no, f"{diag.level} {diag.code}")
            for diag in report.parser_diagnostics
            if diag.level == "ERR"
        )
    return sorted(failures, key=lambda item: (item[0], item[1], item[2]))


def context_lines(report: SourceReport, line_no: int, span: int) -> list[tuple[int, str]]:
    start = max(1, line_no - span)
    end = min(report.total_lines, line_no + span)
    return [(idx, report.lines[idx - 1]) for idx in range(start, end + 1)]


def print_report(reports: list[SourceReport], context: int) -> int:
    has_error = any(
        report.errors or any(diag.level == "ERR" for diag in report.parser_diagnostics)
        for report in reports
    )
    has_warning = any(
        report.warnings
        or report.warn_events
        or any(diag.level == "WARN" for diag in report.parser_diagnostics)
        for report in reports
    )

    print("=== WMBus Log Summary ===")
    for report in reports:
        print(f"\n{report.name}: {report.path}")
        print(f"  lines: {report.total_lines}")
        print(f"  parsed events: {report.parsed_events}")
        parser_error_count = sum(1 for diag in report.parser_diagnostics if diag.level == "ERR")
        parser_warning_count = sum(1 for diag in report.parser_diagnostics if diag.level == "WARN")
        print(f"  firmware errors: {len(report.errors)}")
        print(f"  warn events: {len(report.warn_events)}")
        print(f"  parser errors: {parser_error_count}")
        print(f"  parser diagnostic warnings: {parser_warning_count}")
        print(f"  parser warning details: {len(report.warnings)}")

        if report.counters:
            print("  events:")
            for key, count in sorted(report.counters.items()):
                print(f"    {key}: {count}")

        if report.warnings:
            print("  warning details:")
            for warning in report.warnings[:10]:
                print(f"    {warning}")
            if len(report.warnings) > 10:
                print(f"    ... {len(report.warnings) - 10} more")

    errors = sorted((error for report in reports for error in report.errors), key=lambda event: (event.source, event.line_no))
    if not errors:
        print("\nFirmware errors: none")
    else:
        print("\nFirmware errors:")
        for error in errors:
            print(f"  {error.source}:{error.line_no}: {error.raw}")

    diagnostics = sorted(
        (diag for report in reports for diag in report.parser_diagnostics),
        key=lambda diag: (diag.source, diag.line_no, diag.level, diag.code),
    )
    if not diagnostics:
        print("\nParser diagnostics: none")
    else:
        print("\nParser diagnostics:")
        for diag in diagnostics:
            print(f"  {diag.source}:{diag.line_no}: {diag.level} {diag.code} {diag.message}")
            if diag.raw_line is not None:
                print(f"    checked line: {diag.raw_line}")

    failures = error_context_failures(reports)
    if failures:
        max_context_failures = 5
        shown_failures = failures[:max_context_failures]
        print(f"\nContext around errors (+/- {context} lines):")
        for index, (source, line_no, label) in enumerate(shown_failures, start=1):
            if index > 1:
                print("")
            print(f"[{index}] {source}:{line_no}: {label}")
            source_report = next(report for report in reports if report.name == source)
            for context_line_no, line in context_lines(source_report, line_no, context):
                marker = "=>" if context_line_no == line_no else "  "
                print(f"{marker} {source}:{context_line_no}: {line}")

        remaining = len(failures) - len(shown_failures)
        if remaining > 0:
            print(f"\n... {remaining} more error contexts omitted")

    print(f"\nResult: {'FAIL' if has_error else 'WARN' if has_warning else 'PASS'}")
    return 1 if has_error else 0


def write_json(path: Path, reports: list[SourceReport]) -> None:
    payload = {
        "reports": [
            {
                "name": report.name,
                "path": str(report.path),
                "total_lines": report.total_lines,
                "parsed_events": report.parsed_events,
                "counters": dict(report.counters),
                "errors": [
                    {
                        "line_no": event.line_no,
                        "tags": event.tags,
                        "fields": event.fields,
                        "raw": event.raw,
                    }
                    for event in report.errors
                ],
                "warn_events": [
                    {
                        "line_no": event.line_no,
                        "tags": event.tags,
                        "fields": event.fields,
                        "raw": event.raw,
                    }
                    for event in report.warn_events
                ],
                "parser_diagnostics": [
                    {
                        "line_no": diag.line_no,
                        "level": diag.level,
                        "code": diag.code,
                        "message": diag.message,
                        "raw_line": diag.raw_line,
                    }
                    for diag in report.parser_diagnostics
                ],
                "warnings": report.warnings,
            }
            for report in reports
        ]
    }
    path.write_text(json.dumps(payload, indent=2), encoding="utf-8")


def parse_args(argv: list[str]) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Parse WMBus datalink UART logs")
    parser.add_argument(
        "--input",
        action="append",
        type=parse_input_arg,
        default=[],
        help="Input log as PREFIX=LOG_FILE, can be repeated",
    )
    parser.add_argument("--context", type=int, default=8, help="Lines printed around the first error")
    parser.add_argument(
        "--meter-trigger-period-ms",
        type=int,
        default=10000,
        help="Expected meter trigger period in milliseconds",
    )
    parser.add_argument(
        "--meter-trigger-tolerance-ms",
        type=int,
        default=3000,
        help="Allowed meter trigger timing tolerance in milliseconds",
    )
    parser.add_argument(
        "--meter-session-idle-timeout-ms",
        type=int,
        help="Maximum allowed silence during an active meter session in milliseconds, default is 2x meter trigger period",
    )
    parser.add_argument("--json", type=Path, help="Write machine-readable report")
    return parser.parse_args(argv)


def main(argv: list[str]) -> int:
    args = parse_args(argv)
    inputs: list[tuple[str, Path]] = []

    if args.meter_trigger_period_ms <= 0:
        print("error: --meter-trigger-period-ms must be greater than 0", file=sys.stderr)
        return 2
    if args.meter_trigger_tolerance_ms < 0:
        print("error: --meter-trigger-tolerance-ms must not be negative", file=sys.stderr)
        return 2
    if args.meter_session_idle_timeout_ms is None:
        args.meter_session_idle_timeout_ms = args.meter_trigger_period_ms * 2
    elif args.meter_session_idle_timeout_ms <= 0:
        print("error: --meter-session-idle-timeout-ms must be greater than 0", file=sys.stderr)
        return 2

    for prefix, pattern in args.input:
        for path in expand_paths([pattern]):
            inputs.append((source_name(prefix, path), path))

    if not inputs:
        print("error: provide --input PREFIX=LOG_FILE", file=sys.stderr)
        return 2

    reports: list[SourceReport] = []
    for name, path in inputs:
        if not path.is_file():
            print(f"error: log file not found: {path}", file=sys.stderr)
            return 2

        report, events = read_events(name, path)
        check_source(report, events)
        check_meter_boot_during_active_session(report, events)
        check_meter_trigger_watchdog(
            report,
            events,
            args.meter_trigger_period_ms,
            args.meter_trigger_tolerance_ms,
            args.meter_session_idle_timeout_ms,
        )
        reports.append(report)

    if args.json:
        write_json(args.json, reports)

    report_buffer = io.StringIO()
    with contextlib.redirect_stdout(report_buffer):
        result = print_report(reports, args.context)

    report_text = report_buffer.getvalue()
    print(report_text, end="")

    report_path = inputs[0][1].parent / "wmbus_log_parser_report.txt"
    report_path.write_text(report_text, encoding="utf-8")
    print(f"\nReport written: {report_path}")

    return result


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))
