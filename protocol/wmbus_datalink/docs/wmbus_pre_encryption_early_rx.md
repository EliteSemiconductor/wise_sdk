# WMBus GW Pre-encryption, Pre-packed TX, and Early-RX Architecture

## 1. Purpose

This document defines the current gateway transmit-preparation and Early-RX
architecture implemented by `protocol/wmbus_datalink`. It explains how the
gateway reduces RX-to-TX turnaround time by moving packet construction,
security processing, and RF frame packing outside the time-critical response
path.

This document is the reference for firmware integration, timing verification,
engineering handover, and customer-facing architecture explanations.

## 2. Scope

This document covers:

- GW application-payload preparation.
- Security Mode 0, 5, and 7 preparation behavior.
- RF frame pre-packing.
- Per-connection next, in-flight, and SND-NKE buffer ownership.
- Late DLL first-block finalization.
- Early-RX processing before the complete Meter frame is available.
- Candidate validation, commit, fallback, retry, and queue behavior.
- Direct and staged RF TX paths.
- Timing diagnostics and expected verification behavior.

The architecture described here applies to the GW TX path. The Meter TX path
does not use the same per-connection packed-buffer pipeline.

## 3. Design Objective

The GW must respond after receiving a Meter frame while meeting the applicable
WMBus RX-to-TX turnaround requirement. The complete response preparation path
can include:

- Connection and Meter context selection.
- DLL, ELL, AFL, and TPL construction.
- Security Mode selection.
- AES encryption and MAC generation.
- Frame A CRC insertion.
- Manchester or 3-of-6 line coding.
- Session-dependent DLL C-field finalization.
- RF RX-to-TX reconfiguration.

The current architecture separates this work into two groups:

```text
Work completed before the response is required
  -> Application payload construction
  -> Security processing
  -> RF frame packing
  -> FCB=0 and FCB=1 first-block preparation

Work completed after the Meter frame is validated
  -> Select or promote the prepared frame
  -> Apply the current DLL C-field and FCB
  -> Stage the packed RF frame
  -> Configure and start RF TX
```

Early-RX additionally uses the Meter DLL header while the remainder of the
Meter frame is still arriving. This allows the GW to identify a candidate
response and, for a first session without an existing connection, prepare an
empty request during RX airtime.

## 4. Terminology

### 4.1 Prepared frame

A prepared frame has completed the packet-construction work that does not
depend on the final RX result.

### 4.2 Pre-encryption

The source code historically uses `pre-encryption` for the GW preparation
pipeline. Its precise meaning depends on the Security Mode:

| Security Mode | Processing completed in advance | Description |
|---|---|---|
| Mode 0 | DLL/TPL construction with plaintext application payload | Pre-constructed |
| Mode 5 | DLL/TPL construction and Mode 5 AES encryption | Pre-encrypted |
| Mode 7 | DLL/ELL/AFL/TPL construction, key derivation, encryption, and MAC generation | Pre-encrypted and authenticated |

Mode 0 uses the same low-latency pipeline but is not encrypted.

### 4.3 Pre-packed frame

A pre-packed frame has passed through the WMBus RF packing operation. The
packed bytes already contain the required Frame A CRC and line coding for the
selected physical mode.

Packed RF bytes are sent through the staged packed-frame TX path. They are not
passed through the raw DLL encoder a second time.

### 4.4 Candidate

An Early-RX candidate is a possible GW response selected before RX_DONE. A
candidate is not transmitted until the complete received frame has passed the
normal parser, admission, state, and security checks.

### 4.5 Commit and fallback

- `Commit` means that the candidate still matches the fully received frame and
  current connection state and can be selected for TX.
- `Fallback` means that the GW uses the normal RX_DONE response-selection path.

Fallback preserves normal link behavior when no Early-RX candidate exists or
when the candidate no longer matches the completed RX transaction.

## 5. Architecture Overview

The GW response pipeline is:

```text
APP write or previous TX_DONE
  -> Select target Meter whitelist and connection context
  -> Queue application payload
  -> Construct security and transport content
  -> Construct DLL frame
  -> Pack complete RF frame
  -> Store as tx_next

Meter SYNC received
  -> Sample DLL header during RX airtime
  -> Select a prepared candidate, or prepare a first-session empty request

Meter RX_DONE
  -> Parse and validate the complete Meter frame
  -> Commit Early-RX candidate or use normal prepared-frame fallback
  -> Promote tx_next to tx_in_flight when required
  -> Finalize DLL first block using the current FCB
  -> Stage packed RF frame
  -> Start RF TX

GW TX_DONE
  -> Preserve in-flight ownership until the Meter result is known
  -> Prepare the following queued payload or empty request
  -> Prepare the dedicated SND-NKE frame
  -> Return the radio to RX
```

## 6. Per-Connection TX Ownership

Each GW connection owns independent prepared TX storage:

| Buffer | Purpose |
|---|---|
| `tx_next_packed_buffer` | Next prepared packed frame that has not yet been selected for TX |
| `tx_in_flight_packed_buffer` | Selected frame whose transmission result is still pending |
| `tx_nke_packed_buffer` | Dedicated prepared SND-NKE control frame |

The next and in-flight records also contain:

- Packed-frame length.
- Frame type.
- DLL header template.
- FCB transmission state.
- Encoded first-block variant for FCB 0.
- Encoded first-block variant for FCB 1.
- Snapshot-valid state.

Promotion from `tx_next` to `tx_in_flight` swaps buffer ownership and metadata.
It does not copy and repack the complete frame.

The in-flight frame remains owned by the current transaction until its result
is known. This preserves the same payload, ciphertext, MAC, access information,
and DLL state when a retry is required.

## 7. GW Application-Payload Preparation

### 7.1 Application write

The GW application write identifies the target Meter and performs the following
sequence:

```text
GW application write
  -> Validate Meter ID and payload length
  -> Find the Meter whitelist entry
  -> Read the entry's configured Security Mode
  -> Select or create the Meter connection
  -> Add the payload to the Meter TX queue
  -> Prepare the payload immediately or defer it according to session state
```

Security Mode selection follows the target Meter's whitelist entry, as defined
in `wmbus_security_mode_guide.md`.

### 7.2 Standby preparation

When the target Meter is in GW standby and its TX queue was previously empty,
the first queued payload is prepared immediately:

```text
Payload
  -> Mode-specific security processing
  -> DLL/TPL/ELL/AFL construction as applicable
  -> Complete RF packing
  -> tx_next_packed_buffer
```

This allows the first downlink application payload to be ready before the Meter
starts the session.

### 7.3 Additional queued payloads

Additional writes are stored in the target Meter queue. After the current GW
frame is transmitted, the following queued payload is constructed and packed
before it is required by the next Meter response.

If no application payload remains, the GW prepares the applicable empty request
frame. The dedicated SND-NKE buffer is also prepared for session termination.

### 7.4 Write during an active session

When an application write occurs while the target Meter session is active, the
payload is retained in the Meter queue. Preparation is performed after the
current session returns to standby, using the connection and access state that
applies to the next session.

This keeps preparation associated with the correct Meter and session context.

## 8. Security Mode Processing

### 8.1 Application payloads

| Mode | Prepared security content | Common packed-frame path |
|---|---|---|
| 0 | Plaintext application payload and headers | Yes |
| 5 | AES-CBC encrypted application payload and Mode 5 transport fields | Yes |
| 7 | Derived Mode 7 keys, encrypted application payload, AFL, and MAC | Yes |

The mode-specific processing occurs before the common RF packing step.
Consequently, Mode 0, Mode 5, and Mode 7 application frames all use the same
next/in-flight packed-buffer ownership model.

### 8.2 Empty control frames

An empty ACK, REQ-UD2, SND-NKE, or other frame without application payload is a
link-control frame. The packet builder uses Mode 0 semantics for this frame
because there is no application payload to encrypt or authenticate.

An empty control frame can still be pre-constructed and pre-packed. Its use of
Mode 0 semantics is independent of the target Meter's configured application
Security Mode.

## 9. Late DLL First-Block Finalization

The complete prepared frame contains security and application content that must
remain unchanged for the selected transaction. The final DLL C-field may depend
on the latest session state.

The GW therefore prebuilds two encoded first-block variants:

```text
Variant 0: FCB = 0
Variant 1: FCB = 1
```

Immediately before TX, the GW applies the current link-layer semantics:

- Application data uses primary `SND_UD` with FCV enabled.
- An empty general request uses primary `REQ_UD2` with FCV enabled.
- A retry retains the FCB already used by the in-flight frame.
- A new transaction uses the current connection FCB.

Only the packed first block is replaced. The encrypted payload, MAC, remaining
CRC blocks, and remaining line-coded frame do not need to be rebuilt.

The term `late header decision` in this architecture refers specifically to
DLL first-block and C-field finalization. Security Mode, transport content,
payload type, access information, encrypted payload, AFL, and MAC are already
fixed in the prepared frame.

## 10. Early-RX Event Flow

### 10.1 SYNC detection

When the radio detects a WMBus sync word in GW RX operation:

1. The radio callback starts a new Early-RX generation.
2. Early-RX state becomes header-pending.
3. The callback posts `WMBUS_RX_SYNC_WORD` to the link event path.

The radio interrupt path does not perform packet construction, encryption, RF
packing, or application logging.

### 10.2 Early sample scheduling

The GW FSM receives `WMBUS_RX_SYNC_WORD`, resets the previous candidate, and
arms the Early-RX sample timer.

The default sample delay is 1800 microseconds. The CLI accepts values from 100
to 10000 microseconds in 100-microsecond steps:

```text
wm earlyrx delay [us]
```

The configured delay determines when the GW samples the active radio RX buffer.

### 10.3 DLL header snapshot

When the sample timer expires, the platform layer:

1. Verifies the active Early-RX generation and state.
2. Obtains the active radio RX buffer address.
3. Copies the first 10 bytes as a DLL header snapshot.
4. Marks the snapshot ready.
5. Posts `WMBUS_EARLY_RX_PREP`.

The snapshot is used only for candidate preparation. The complete received
frame remains subject to normal RX_DONE validation.

### 10.4 Candidate selection

Early-RX operates for the GW T2 receive flow. Using the Meter ID in the DLL
snapshot, the GW selects candidates in this order:

1. A valid in-flight application-data frame for the Meter.
2. A valid next prepared frame for the Meter.
3. A first-session empty REQ-UD2 candidate for a known Meter without an
   existing connection.

For an existing connection, candidate selection records the packed-buffer
identity, length, type, connection, and connection sequence. The already
prepared encrypted or plaintext RF frame is not rebuilt.

For a known Meter without an existing connection, Early-RX may construct and
pack the empty REQ-UD2 candidate during the remaining Meter RX airtime. The
Meter must already have a usable whitelist context.

An unknown Meter handled through dynamic admission is admitted by the complete
RX path. Its normal response is prepared after admission and full frame
validation.

### 10.5 RX_DONE validation

RX_DONE continues through the normal receive path:

```text
Complete RF frame received
  -> RF/frame validation
  -> DLL and transport parsing
  -> Whitelist admission
  -> Connection selection or creation
  -> Security Mode policy validation
  -> Decryption and authentication as applicable
  -> GW FSM response decision
```

No Early-RX candidate is transmitted based only on the sampled DLL header.

### 10.6 Candidate commit

Before an Early-RX candidate is committed, the GW verifies:

- The Early-RX generation is still current.
- The sampled DLL header matches the DLL header in the complete frame.
- The Meter identity matches the current connection.
- The connection object and connection sequence still match.
- The packed-buffer pointer, length, and frame type are unchanged.
- The required next or in-flight snapshot remains valid.
- A first-session candidate matches the connection created by the validated
  received frame.

After these checks, the GW promotes the next frame when required, applies the
current DLL first block, stages the packed frame, and marks the candidate
committed.

### 10.7 Fallback

If no candidate is available or any commit condition is not satisfied, the GW
uses the normal prepared-frame response path after RX_DONE.

The fallback path performs normal response selection, promotion, DLL
first-block finalization, staging, and TX. Encryption and full RF packing are
still avoided when a valid prepared frame is available.

## 11. Behavior by Session Scenario

| Scenario | Work completed before RX_DONE | Response behavior |
|---|---|---|
| Known Meter with a queued GW application payload | Security processing and complete RF packing were completed at application-write time | Validate RX, select/promote prepared data, finalize first block, stage, and transmit |
| Known Meter without an existing connection or queued payload | Early-RX constructs and packs an empty first-session REQ-UD2 during RX airtime | Validate RX, create connection, commit candidate, finalize first block, stage, and transmit |
| Dynamically admitted unknown Meter | No whitelist-dependent Early-RX candidate is created | Complete admission and validation, then prepare the normal response |
| Existing session with a next prepared frame | Next or in-flight frame is available before RX_DONE | Commit the matching candidate or use prepared-frame fallback |
| Retry or negative response | In-flight frame remains owned by the current transaction | Reuse the same in-flight packed frame and its sent FCB |
| Application write during active session | Payload remains in the Meter queue | Prepare it after the connection returns to standby for the next session |

## 12. RF TX Dispatch

The GW TX dispatcher distinguishes raw DLL frames from packed RF frames.

### 12.1 Generated raw DLL frame

A generated raw DLL frame uses the normal radio TX API. The radio layer performs
Frame A CRC insertion and line coding before TX.

### 12.2 Prepared packed frame

A prepared in-flight or SND-NKE frame uses:

```text
Stage packed frame
  -> Configure RF TX
  -> Start staged TX
```

### 12.3 Early-RX committed frame

An Early-RX committed frame is already staged. The dispatcher starts staged TX
without sending the packed bytes through the raw DLL packing path.

### 12.4 Current direct-TX behavior

The current GW source enables `TEST_TX_DIRECTLY`. Response branches controlled
by this build flag start the selected RF TX path immediately after the RX
handling and candidate-selection work is complete. The alternative build path
uses the scheduler and `tRO` delay.

This build-time behavior is separate from pre-encryption and pre-packing:

- Pre-encryption/pre-packing determines how the TX frame is prepared and stored.
- Early-RX determines which preparation work is performed during RX airtime.
- Direct or scheduled TX determines when the prepared frame is started.

## 13. TX_DONE, Acknowledgement, and Retry Flow

### 13.1 After GW TX_DONE

After a normal GW transmission:

1. The in-flight frame remains associated with the current transaction.
2. The following queued application payload is prepared as `tx_next`, if one
   exists.
3. Otherwise, the next empty request is prepared.
4. The dedicated SND-NKE frame is prepared.
5. The GW starts RX and waits for the Meter response.

This moves the next frame's security and packing work into the Meter response
period rather than the following RX_DONE-to-TX interval.

### 13.2 Confirmed response

When the expected Meter acknowledgement is accepted:

- The transmitted application payload is removed from the queue.
- The link FCB advances when required.
- The confirmed in-flight ownership is released.
- The prepared next frame remains available for the following GW response.

### 13.3 Retry

When the same transaction must be retried:

- The current in-flight frame is retained.
- The frame is not reconstructed or re-encrypted.
- The FCB already associated with the transmitted frame is retained.
- The packed frame is staged and transmitted again through the packed TX path.

## 14. Integrity and Isolation Rules

The architecture preserves correctness through the following rules:

- A complete Meter frame is parsed and validated before GW TX starts.
- Early-RX header sampling does not replace CRC, admission, security, or state
  validation.
- Every connection owns independent TX queues and packed buffers.
- A connection sequence identifies the current lifetime of a connection object.
- A prepared next frame cannot overwrite an active in-flight frame.
- Queue payload is consumed only after the corresponding acknowledgement is
  accepted.
- Retry reuses the same in-flight prepared frame.
- DLL late finalization changes only the first packed block.
- Packed RF bytes are never processed as a raw DLL frame.
- A stale or mismatched Early-RX candidate is discarded and the normal response
  path is used.

## 15. Memory Model

The maximum packed-frame buffer size is 580 bytes. Each connection allocates
three packed-frame buffers:

```text
tx_next       580 bytes
tx_in_flight  580 bytes
tx_nke        580 bytes
```

Each connection additionally owns:

- A 256-byte RX buffer.
- Application TX queue payload buffers.
- Queue pointer and length arrays.
- TX snapshots and link state.

The Early-RX path also owns one global candidate context containing a raw DLL
buffer, a packed candidate buffer, DLL header metadata, and first-block
variants. Only one radio RX transaction is active at a time, so one global
Early-RX candidate context is used.

Connection count and queue depth therefore determine the heap requirement for
the GW configuration.

## 16. Diagnostics and Verification

### 16.1 Early-RX CLI

The Early-RX diagnostic command is:

```text
wm earlyrx [dump|reset|delay us]
```

Commands:

```text
wm earlyrx dump
wm earlyrx reset
wm earlyrx delay 1800
```

- `dump` displays Early-RX validation and timing counters.
- `reset` clears the Early-RX validation statistics.
- `delay` sets the DLL snapshot time in microseconds.

### 16.2 Verification scenarios

Verification covers the following flows:

1. Known Meter, first session, no queued GW payload.
2. Known Meter with a GW payload prepared before the session.
3. Two or more queued GW payloads.
4. Existing session with normal acknowledgement.
5. Retry using the in-flight packed frame.
6. Session termination using SND-NKE.
7. Dynamic admission of an unknown Meter.
8. Security Mode 0, 5, and 7 application payloads.
9. Empty control frames with a Mode 5 or Mode 7 application policy.
10. Minimum and maximum supported application payload sizes.
11. Multiple Meter connections and independent queues.
12. Timeout followed by standby recovery or retry.

### 16.3 Expected evidence

The verification record identifies:

- Meter ID and configured Security Mode.
- Whether the response source was generated, next, in-flight, SND-NKE, or an
  Early-RX candidate.
- Whether Early-RX committed or used fallback.
- Prepared and packed-frame lengths.
- FCB used for the transmitted frame.
- Queue position before and after acknowledgement.
- RX_DONE-to-TX timing markers.
- Successful Meter payload reception or expected empty control response.

## 17. Customer-facing Summary

The following text may be used in customer documentation:

> The WMBus gateway reduces response turnaround time by preparing downlink
> frames before they are required. For application data, the gateway constructs
> the protocol headers, performs the configured security processing, and packs
> the RF frame in advance. Security Mode 0 frames are pre-constructed and
> pre-packed; Mode 5 frames are pre-encrypted and pre-packed; and Mode 7 frames
> are pre-encrypted, authenticated, and pre-packed.
>
> During reception of a Meter frame, Early-RX identifies the Meter from the DLL
> header and selects a prepared response candidate. For a known Meter starting
> its first session without queued downlink data, the gateway can construct and
> pack the empty request while the Meter frame is still being received. The
> gateway transmits only after the complete Meter frame passes normal frame,
> whitelist, session, and security validation.
>
> Session-dependent DLL information, including FCB, is finalized immediately
> before transmission by replacing only the first packed RF block. The
> encrypted payload and the remainder of the packed frame remain unchanged.
> Retries reuse the same in-flight frame, while queued data for each Meter is
> maintained in independent connection storage.

## 18. Implementation Reference

The principal implementation points are:

- `wmbus_datalink_dll.h`
  - `WMBUS_GW_PREENCRYPTION`.
  - Prepared-frame APIs and GW TX source definitions.
- `wmbus_datalink_connection.h` and `wmbus_datalink_connection.c`
  - Per-connection next, in-flight, and SND-NKE packed buffers.
  - Connection sequence and queue ownership.
- `wmbus_datalink_api.c`
  - `wmbus_link_gw_enqueue_payload()`.
  - `wmbus_link_create_packet_internal()`.
  - `wmbus_link_gw_pack_next_frame()`.
  - `wmbus_link_gw_promote_next()`.
  - `wmbus_link_gw_finalize_in_flight_tx_frame()`.
  - `wmbus_link_GW_next_pre_encryption()`.
  - `wmbus_link_gw_prepare_early_null()`.
- `wmbus_datalink_gw.c`
  - Early-RX candidate preparation and commit.
  - Prepared-frame fallback.
  - Packed-frame staging and direct TX dispatch.
  - TX_DONE, acknowledgement, retry, and next-frame preparation.
- `wmbus_datalink_fsm.c`
  - `WMBUS_RX_SYNC_WORD`, `WMBUS_EARLY_RX_PREP`, and `WMBUS_RX_DONE` event
    dispatch.
- `platform_intf/platform_intf_rf.c`
  - Radio sync notification.
  - Early-RX generation, timer, DLL snapshot, and diagnostic counters.
  - Raw and staged packed-frame TX interfaces.
- `wise_core_v2/radio_lib/wise_radio_wmbus_api.c`
  - Complete WMBus RF frame packing.
  - Packed first-block generation and finalization.
  - Packed-frame staging and RF TX start.
- `wmbus_datalink_cmd.c`
  - `wm earlyrx` diagnostic command.
