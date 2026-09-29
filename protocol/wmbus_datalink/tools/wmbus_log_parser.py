#!/usr/bin/env python3
"""Parse WMBus datalink UART logs.

This tool parses the fixed tag format used by WMBUS_DBG_MSG_FLOW /
WMBUS_DBG_MSG_ERROR, monitors each source, and correlates meter triggers with
the gateway log.
"""

from __future__ import annotations

import argparse
import bisect
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
    failure_streak: int = 0
    failure_streak_reported: bool = False
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

def is_meter_liveness_activity(event: Event) -> bool:
    if event.role == "MTR":
        return True
    return False


def is_meter_session_start(event: Event) -> bool:
    return event.role == "MTR" and event.kind == "SESSION_START"


def is_meter_session_end(event: Event) -> bool:
    return event.role == "MTR" and event.kind == "SESSION_END"


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
    failure_streak_limit: int,
) -> None:
    deadline_ms = period_ms + tolerance_ms
    events = list(events)
    if not any(is_meter_session_start(event) for event in events):
        return

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

    def event_dev(event: Event) -> str:
        return event.dev.lower() if event.dev != "unknown" else (source_dev or "unknown")

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

    for line_no, line in enumerate(report.lines, start=1):
        timestamp_ms = parse_timestamp_ms(line)
        if timestamp_ms is not None:
            last_timestamp_ms = timestamp_ms

        manual_boundary = MANUAL_BOUNDARY_RE.search(line)
        boot_boundary = BOOT_MARKER_RE.search(line)
        if manual_boundary or boot_boundary:
            active_meter_sessions.clear()
            states.clear()
            continue

        line_events = events_by_line.get(line_no, [])
        if timestamp_ms is not None and line_events:
            check_session_idle(line_no, timestamp_ms)

        for event in line_events:
            if event.role != "MTR":
                continue

            dev = event_dev(event)
            state = state_for(dev)
            if is_meter_session_start(event):
                if (state.last_trigger is not None and
                    state.last_trigger.timestamp_ms is not None and
                    event.timestamp_ms is not None):
                    elapsed_ms = event.timestamp_ms - state.last_trigger.timestamp_ms
                    if elapsed_ms > deadline_ms:
                        if dev in active_meter_sessions:
                            if not state.session_stuck_reported:
                                add_meter_session_stuck_error(
                                    report,
                                    event.line_no,
                                    f"dev={dev} trigger_elapsed_ms={elapsed_ms} limit_ms={deadline_ms} "
                                    f"previous_trigger_line={state.last_trigger.line_no} active_session=1",
                                )
                        else:
                            add_meter_trigger_error(
                                report,
                                event.line_no,
                                f"dev={dev} elapsed_ms={elapsed_ms} limit_ms={deadline_ms} "
                                f"previous_trigger_line={state.last_trigger.line_no} active_session=0",
                            )

                state.last_trigger = event
                state.last_liveness = event
                state.session_stuck_reported = False
                active_meter_sessions.add(dev)
                continue

            if is_meter_session_end(event):
                state.last_liveness = event
                active_meter_sessions.discard(dev)
                reason = event.fields.get("reason", "UNKNOWN")
                if reason in ("RX_NKE", "NO_REQUEST"):
                    state.failure_streak = 0
                    state.failure_streak_reported = False
                elif reason in ("FAC_TIMEOUT", "REQUEST_TIMEOUT", "RETRY_LIMIT", "DEC_STOP"):
                    state.failure_streak += 1
                    if (state.failure_streak >= failure_streak_limit and
                        not state.failure_streak_reported):
                        report.counters["MTR.ERR.SESSION_FAILURE_STREAK"] += 1
                        report.parser_diagnostics.append(
                            ParserDiagnostic(
                                source=report.name,
                                line_no=event.line_no,
                                level="ERR",
                                code="SESSION_FAILURE_STREAK",
                                message=f"dev={dev} failures={state.failure_streak} limit={failure_streak_limit} last_reason={reason}",
                                raw_line=report.lines[event.line_no - 1],
                            )
                        )
                        state.failure_streak_reported = True
                continue

            if is_meter_liveness_activity(event):
                state.last_liveness = event

    if last_timestamp_ms is None:
        return

    check_session_idle(report.total_lines, last_timestamp_ms)


def check_cross_source_health(
    reports: list[SourceReport],
    events_by_source: dict[str, list[Event]],
    match_window_ms: int,
    missed_trigger_limit: int,
    source_silence_ms: int,
) -> None:
    latest_timestamp_ms: int | None = None
    gw_starts_by_dev: dict[str, list[int]] = {}
    gw_timestamps: list[int] = []
    for events in events_by_source.values():
        for event in events:
            if event.timestamp_ms is None:
                continue
            latest_timestamp_ms = (
                event.timestamp_ms if latest_timestamp_ms is None
                else max(latest_timestamp_ms, event.timestamp_ms)
            )
            if event.role == "GW":
                gw_timestamps.append(event.timestamp_ms)
                if (event.kind == "SESSION_START" and
                    event.fields.get("reason") == "RX_FIRST_PACKET"):
                    gw_starts_by_dev.setdefault(event.dev.lower(), []).append(event.timestamp_ms)

    if latest_timestamp_ms is None:
        return

    for timestamps in gw_starts_by_dev.values():
        timestamps.sort()

    if gw_timestamps:
        gw_first_ms = min(gw_timestamps)
        gw_last_ms = max(gw_timestamps)
        for report in reports:
            meter_starts = [
                event for event in events_by_source[report.name]
                if is_meter_session_start(event) and event.timestamp_ms is not None
            ]
            if not meter_starts:
                continue

            missed_streak = 0
            reported = False
            previous_line_no = 0
            for event in meter_starts:
                if any(
                    MANUAL_BOUNDARY_RE.search(line) or BOOT_MARKER_RE.search(line)
                    for line in report.lines[previous_line_no:event.line_no - 1]
                ):
                    missed_streak = 0
                    reported = False
                previous_line_no = event.line_no
                if event.timestamp_ms is None or not (gw_first_ms <= event.timestamp_ms <= gw_last_ms):
                    continue
                dev = event.dev.lower()
                timestamps = gw_starts_by_dev.get(dev, [])
                index = bisect.bisect_left(timestamps, event.timestamp_ms)
                candidates = timestamps[max(0, index - 1):index + 1]
                matched = any(abs(timestamp - event.timestamp_ms) <= match_window_ms for timestamp in candidates)
                if matched:
                    missed_streak = 0
                    reported = False
                    continue

                missed_streak += 1
                if missed_streak >= missed_trigger_limit and not reported:
                    report.counters["SYSTEM.ERR.GW_MISSED_TRIGGER_STREAK"] += 1
                    report.parser_diagnostics.append(
                        ParserDiagnostic(
                            source=report.name,
                            line_no=event.line_no,
                            level="ERR",
                            code="GW_MISSED_TRIGGER_STREAK",
                            message=f"dev={dev} misses={missed_streak} limit={missed_trigger_limit} match_window_ms={match_window_ms}",
                            raw_line=report.lines[event.line_no - 1],
                        )
                    )
                    reported = True

    for report in reports:
        source_events = [event for event in events_by_source[report.name] if event.timestamp_ms is not None]
        if source_events:
            last_event = max(source_events, key=lambda event: event.timestamp_ms or 0)
            last_line_no = last_event.line_no
            last_source_timestamp_ms = last_event.timestamp_ms or latest_timestamp_ms
            last_event_name = last_event.kind
        else:
            timestamped_lines = [
                (line_no, parse_timestamp_ms(line))
                for line_no, line in enumerate(report.lines, start=1)
                if parse_timestamp_ms(line) is not None
            ]
            if not timestamped_lines:
                continue
            last_line_no, parsed_timestamp_ms = timestamped_lines[-1]
            last_source_timestamp_ms = parsed_timestamp_ms or latest_timestamp_ms
            last_event_name = "NO_TAGGED_EVENT"

        silent_ms = latest_timestamp_ms - last_source_timestamp_ms
        if silent_ms <= source_silence_ms:
            continue
        if any(
            MANUAL_BOUNDARY_RE.search(line) or BOOT_MARKER_RE.search(line)
            for line in report.lines[last_line_no:]
        ):
            continue

        report.counters["SYSTEM.ERR.SOURCE_SILENT"] += 1
        report.parser_diagnostics.append(
            ParserDiagnostic(
                source=report.name,
                line_no=last_line_no,
                level="ERR",
                code="SOURCE_SILENT",
                message=f"silent_ms={silent_ms} limit_ms={source_silence_ms} last_event={last_event_name}",
                raw_line=report.lines[last_line_no - 1],
            )
        )


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
    parser.add_argument("--context", type=int, default=8, help="Lines printed around each reported error")
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
    parser.add_argument(
        "--session-failure-streak-limit",
        type=int,
        default=3,
        help="Consecutive failed meter sessions reported as an error",
    )
    parser.add_argument(
        "--gw-rx-match-window-ms",
        type=int,
        default=1000,
        help="Timestamp window used to match a meter trigger to GW SESSION_START",
    )
    parser.add_argument(
        "--gw-missed-trigger-limit",
        type=int,
        default=3,
        help="Consecutive meter triggers absent from the GW log reported as an error",
    )
    parser.add_argument(
        "--source-silence-timeout-ms",
        type=int,
        help="Maximum source timestamp lag, default is 3x meter trigger period",
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
    if args.session_failure_streak_limit <= 0:
        print("error: --session-failure-streak-limit must be greater than 0", file=sys.stderr)
        return 2
    if args.gw_rx_match_window_ms < 0:
        print("error: --gw-rx-match-window-ms must not be negative", file=sys.stderr)
        return 2
    if args.gw_missed_trigger_limit <= 0:
        print("error: --gw-missed-trigger-limit must be greater than 0", file=sys.stderr)
        return 2
    if args.source_silence_timeout_ms is None:
        args.source_silence_timeout_ms = args.meter_trigger_period_ms * 3
    elif args.source_silence_timeout_ms <= 0:
        print("error: --source-silence-timeout-ms must be greater than 0", file=sys.stderr)
        return 2

    for prefix, pattern in args.input:
        for path in expand_paths([pattern]):
            inputs.append((source_name(prefix, path), path))

    if not inputs:
        print("error: provide --input PREFIX=LOG_FILE", file=sys.stderr)
        return 2

    reports: list[SourceReport] = []
    events_by_source: dict[str, list[Event]] = {}
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
            args.session_failure_streak_limit,
        )
        reports.append(report)
        events_by_source[name] = events

    check_cross_source_health(
        reports,
        events_by_source,
        args.gw_rx_match_window_ms,
        args.gw_missed_trigger_limit,
        args.source_silence_timeout_ms,
    )

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
