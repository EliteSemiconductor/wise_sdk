# WMBus Security Mode Configuration and Verification Guide

## 1. Purpose

This document defines how WMBus Security Modes 0, 5, and 7 are provisioned,
enforced, and verified by the Meter and Gateway in
`protocol/wmbus_datalink`. Security Mode is configured locally on the Meter and
per device in the Gateway whitelist. It is not negotiated or learned over the
air.

It is the reference for firmware development, integration testing, engineering
handover, and customer-facing configuration explanations.

The rules in this document replace the conclusions in older command tables
created before the GW pre-encryption/pre-packed architecture was completed.

## 2. Scope

This document covers:

- Security Mode selection for Meter and GW.
- Per-Meter GW whitelist configuration.
- TX and RX security-mode enforcement.
- GW TX pre-encryption/pre-construction and RF pre-packing.
- Normal CLI configuration and verification procedures.

The `WMBUS_GW_PREENCRYPTION` implementation is a GW TX turnaround optimization.
It does not mean that the Meter TX path uses the same packed-buffer pipeline.

## 3. Normative Security Mode Policy

### 3.1 Supported modes

The implementation supports Security Mode 0, 5, and 7.

No other security-mode value is accepted by the configuration API or RX
validation path.

### 3.2 Provisioned mode, not negotiated mode

Security Mode is provisioned per device; it is not negotiated over the air.

- A Meter uses its locally configured Security Mode for TX application payloads.
- A Meter accepts RX application payloads only when the observed mode matches
  its locally configured mode.
- A GW stores a configured Security Mode in every whitelist entry.
- A GW uses the target Meter's whitelist mode for GW-to-Meter application
  payload creation.
- A GW accepts a Meter application payload only when the observed RX mode
  matches that Meter's whitelist mode.
- Neither side automatically changes its configured mode after receiving a
  frame.
- A mismatched frame does not update the whitelist or security configuration.

### 3.3 GW whitelist default

The GW whitelist command accepts an optional Security Mode:

```text
wm wl add [id] [manufacturer] [version] [device_type] [security_mode]
```

Valid values are `0`, `5`, and `7`.

When `security_mode` is omitted, the GW assigns Mode 7:

```text
wm wl add 80000001 ESM 1 7
```

is equivalent to:

```text
wm wl add 80000001 ESM 1 7 7
```

Mode 0 and Mode 5 must be specified explicitly. For production provisioning,
explicitly writing `7` is also recommended because it makes the intended policy
visible in the configuration record.

### 3.4 Application payload enforcement

For a frame containing application payload:

| Receiver | Configured mode source | Observed mode source | Mismatch action |
|---|---|---|---|
| GW | Meter whitelist entry | Received frame security field | Drop the frame and return to standby without changing the configured mode |
| Meter | Meter local configuration | Received frame security field | Drop the frame and end the affected session without changing the configured mode |

The implementation reports a mismatch using messages such as:

```text
[SEC][RX_MISMATCH] ... expected=<mode> observed=<mode> action=drop
[MTR][SEC_MISMATCH] ... action=end_session
```

The mismatch path must not be treated as a request to retry using another
Security Mode.

### 3.5 Empty control frames

Empty ACK, SND-NKE, and other frames without application payload are link-control
frames. They do not participate in the application-payload Security Mode
matching decision.

The packet builder uses Mode 0 semantics for an empty control frame even when
the associated Meter application policy is Mode 5 or Mode 7. This is not a
security downgrade because no application payload is carried.

### 3.6 Mode 0 restriction

Mode 0 provides neither application-payload encryption nor cryptographic
authentication.

For that reason:

- Mode 0 must be explicitly configured in the GW whitelist.
- A Mode 0 Meter application payload is accepted only when its whitelist entry
  is explicitly configured as Mode 0.
- Omitting the whitelist mode does not enable Mode 0; omission selects Mode 7.
- A received Mode 0 application frame must never cause a Mode 5 or Mode 7
  whitelist entry to change to Mode 0.

## 4. GW Pre-encryption and Pre-packed Architecture

### 4.1 Terminology

The source code historically uses the term `pre-encryption` for the complete GW
packet-preparation pipeline. The precise meaning depends on the selected mode:

| Mode | Security processing performed in advance | RF frame packed in advance | Preferred description |
|---|---|---|---|
| 0 | No encryption; headers and plaintext payload are constructed | Yes | Pre-constructed and pre-packed |
| 5 | Mode 5 AES encryption is completed | Yes | Pre-encrypted and pre-packed |
| 7 | Mode 7 key derivation, encryption, and MAC generation are completed | Yes | Pre-encrypted, authenticated, and pre-packed |

Therefore, Mode 0 is supported by the same low-latency architecture, but it
must not be described to customers as encrypted.

### 4.2 Compile-time enablement

The current project enables the GW pipeline through:

```c
#define WMBUS_GW_PREENCRYPTION
```

in `wmbus_datalink_dll.h`.

If this compile-time option is removed, the timing and buffer behavior described
in this section no longer applies.

### 4.3 Preparation flow

For a GW application write, the current flow is:

```text
GW application/CLI write
  -> Find the target Meter whitelist entry
  -> Read configured_security_mode from that entry
  -> Add the application payload to the target Meter TX queue
  -> Construct DLL and TPL content
       Mode 0: copy plaintext application payload
       Mode 5: encrypt the application payload using the Mode 5 path
       Mode 7: derive keys, encrypt, and generate the MAC
  -> Pack the complete RF frame using platform_rf_pack_frame()
  -> Store it in the target connection's tx_next_packed_buffer
  -> Promote it to tx_in_flight_packed_buffer when selected for TX
  -> Finalize the small link-layer field that depends on current session state
  -> Stage the packed frame and start RF TX
```

The common packing step occurs after the mode-specific security operation.
Consequently, Mode 0, Mode 5, and Mode 7 all reach the same packed-buffer TX
path.

### 4.4 First and subsequent queued payloads

When the GW is in standby and the target queue is empty, the first application
payload is constructed and packed immediately. Diagnostic output reports:

```text
[GW][PREENC_APP] ... action=prepared
```

Additional application payloads are first stored in the target Meter queue:

```text
[GW][PREENC_APP] ... action=queued
```

After the current frame is transmitted, the next queued payload is constructed
and packed before it is needed for the following GW response:

```text
[GW][PREENC_NEXT] ... result=DATA
```

If no application payload remains, the GW prepares the required empty control
frame instead:

```text
[GW][PREENC_NEXT] ... result=NULL_PACKET
```

SND-NKE also has a dedicated packed buffer so that session termination does not
require rebuilding the complete RF frame in the RX-to-TX critical path.

### 4.5 RX-to-TX timing behavior

The purpose of the packed-buffer architecture is to remove packet construction,
encryption, MAC generation, and full RF frame packing from the time-critical GW
RX-to-TX turnaround path.

At response time, the GW normally performs only the remaining session-dependent
work:

1. Select the prepared frame for the current Meter.
2. Finalize the first RF block using the correct FCB variant.
3. Stage the already packed frame.
4. Start RF TX.

The packed RF bytes must not be passed through the DLL/RF encoder a second time.

### 4.6 Retry and queue ownership

- The selected frame remains in the in-flight packed buffer until its result is
  known.
- A retry reuses the in-flight packed frame rather than recreating and
  re-encrypting the application payload.
- Queue data is removed only after the expected Meter acknowledgement is
  accepted.
- Preparing the next frame must not overwrite the in-flight frame.
- Each Meter connection owns independent next, in-flight, and SND-NKE packed
  buffers and independent security state.

These rules are required to preserve ciphertext, MAC, access-number, message
counter, and FCB consistency across the session.

## 5. Configuration Rules by Role

### 5.1 GW

The GW does not use one global Security Mode for all Meters. Its application
policy is configured per whitelist entry.

Examples:

```text
# Meter 0x80000001 uses Mode 0
wm wl add 80000001 ESM 1 7 0

# Meter 0x80000001 uses Mode 5
wm wl add 80000001 ESM 1 7 5

# Meter 0x80000001 uses Mode 7
wm wl add 80000001 ESM 1 7 7
```

Always verify the resulting entry:

```text
wm wl list
```

The displayed `sec` value is the authoritative GW application policy for that
Meter.

### 5.2 Meter

The Meter uses:

```text
wm sec 0
wm sec 5
wm sec 7
```

The selected value controls Meter application TX and application RX acceptance.
It does not modify the GW whitelist.

### 5.3 Key configuration

Mode 5 and Mode 7 require the GW and Meter to use identical shared key material.

The current CLI test firmware initializes the following built-in test key at
startup:

```text
00 11 22 33 44 55 66 77 88 99 aa bb cc dd ee ff
```

Therefore, the `wm key` command is not required for the Mode 5 and Mode 7
procedures in this document when both devices are using the default test key
and neither side has overridden it after startup.

To test with a different key, execute the same command on both devices before
starting communication:

```text
wm key <16 hexadecimal bytes>
```

The built-in key is intended only for laboratory verification. Production
systems must provision application-specific key material.

Mode 0 does not use the key to protect its application payload.

## 6. CLI Verification Procedures

The following procedures use Mode T2 and one GW/Meter pair:

```text
GW ID:    0x80000002
Meter ID: 0x80000001
```

No manual function-code command is required for these normal test flows.
The Mode 5 and Mode 7 procedures use the built-in test key described in
Section 5.3, so the optional `wm key` override command is omitted.

### 6.1 Mode 0

GW:

```text
wm dbg 4
wm role 0
wm mode 3
wm setinfo 80000002 ESM 1 7
wm wl init fix 1
wm wl add 80000001 ESM 1 7 0
wm wl list
wm start
```

Meter:

```text
wm dbg 4
wm role 1
wm mode 3
wm sec 0
wm setinfo 80000001 ESM 1 7
wm gwonly 1 80000002
wm wr 5 1 2 3 4 5
wm start
```

Optional GW-to-Meter application payloads:

```text
wm wr 80000001 3 1 2 3
wm wr 80000001 4 1 2 3 4
```

Trigger the Meter session:

```text
wm fsm 0
```

Expected result:

- GW receives `01 02 03 04 05` as Mode 0 application data.
- Meter receives `01 02 03` and `01 02 03 04` as Mode 0 application data.
- No `[SEC][RX_MISMATCH]` is reported.
- The session ends normally after the queued data is completed.

### 6.2 Mode 5

GW:

```text
wm dbg 4
wm role 0
wm mode 3
wm setinfo 80000002 ESM 1 7
wm wl init fix 1
wm wl add 80000001 ESM 1 7 5
wm wl list
wm start
```

Meter:

```text
wm dbg 4
wm role 1
wm mode 3
wm sec 5
wm setinfo 80000001 ESM 1 7
wm gwonly 1 80000002
wm wr 5 1 2 3 4 5
wm start
```

Optional GW-to-Meter application payloads:

```text
wm wr 80000001 3 1 2 3
wm wr 80000001 4 1 2 3 4
```

Trigger the Meter session:

```text
wm fsm 0
```

Expected result:

- Both application directions use Mode 5.
- The GW prepares the first downlink payload before the Meter request.
- A second GW payload is queued and prepared before the following response.
- No `[SEC][RX_MISMATCH]` or encryption error is reported.

### 6.3 Mode 7

GW:

```text
wm dbg 4
wm role 0
wm mode 3
wm setinfo 80000002 ESM 1 7
wm wl init fix 1
wm wl add 80000001 ESM 1 7 7
wm wl list
wm start
```

The following whitelist command may be used to verify the Mode 7 default:

```text
wm wl add 80000001 ESM 1 7
```

Afterward, `wm wl list` must still display `sec=7`.

Meter:

```text
wm dbg 4
wm role 1
wm mode 3
wm sec 7
wm setinfo 80000001 ESM 1 7
wm gwonly 1 80000002
wm wr 5 1 2 3 4 5
wm start
```

Optional GW-to-Meter application payloads:

```text
wm wr 80000001 3 1 2 3
wm wr 80000001 4 1 2 3 4
```

Trigger the Meter session:

```text
wm fsm 0
```

Expected result:

- Both application directions use Mode 7.
- Encryption and MAC processing complete successfully.
- The first GW payload reports `action=prepared` and the next reports
  `action=queued`.
- Meter receives both GW payloads in order.
- No `[SEC][RX_MISMATCH]`, encryption error, or MAC error is reported.

## 7. Verification Status

The current implementation and available hardware logs support the following
status:

| Mode | Code-path review | Bidirectional application communication | GW packed multi-payload evidence |
|---|---|---|---|
| 0 | Confirmed | Confirmed | Confirmed: first payload prepared, second queued, both received |
| 5 | Confirmed | Confirmed | Dedicated two-downlink-payload evidence should still be recorded |
| 7 | Confirmed | Confirmed | Confirmed: first payload prepared, second queued, both received |

Mode 5 uses the same common GW packed-buffer path after its mode-specific AES
operation. The remaining Mode 5 item is an evidence-completeness task, not a
known implementation limitation.

## 8. Migration from the Previous Behavior

Older test tables were created before the current pre-encryption/pre-packed
architecture and per-whitelist security policy were completed. They must not be
used to infer the current Security Mode.

The current behavior differs as follows:

| Previous assumption | Current rule |
|---|---|
| Security Mode could appear to follow current RX or global GW state | GW application policy comes from the target Meter whitelist entry |
| A received frame could influence later GW TX mode | RX never changes the configured whitelist mode |
| Different Meter and GW settings might still produce application data | Application payload mode mismatch is rejected |
| Omitting a whitelist mode left the intended mode unclear | Omission has the defined value Mode 7 |
| Empty ACK/NKE mode could be used to infer application policy | Empty control frames are excluded from application-mode enforcement |

When migrating an existing deployment:

1. Assign every Meter an intended Security Mode.
2. Configure the same mode in the Meter and its GW whitelist entry.
3. Explicitly provision Mode 0 and Mode 5 entries.
4. Verify the GW entry using `wm wl list`.
5. Verify at least one Meter-to-GW and one GW-to-Meter application payload.
6. Treat any mismatch log as a provisioning error, not as a negotiation event.

## 9. Customer-facing Explanation

The following text may be used in customer documentation:

> The WMBus GW supports Security Modes 0, 5, and 7 on a per-device basis. Each
> Meter is provisioned with one Security Mode, and the corresponding GW
> whitelist entry must be configured with the same value. The Security Mode is
> not negotiated or learned over the air. If the GW whitelist mode is omitted,
> Mode 7 is used by default. Mode 0 must be explicitly selected and does not
> provide application-payload encryption or cryptographic authentication. A
> received application frame with a mode different from the configured device
> policy is discarded.

For timing-sensitive GW responses, the GW prepares security processing and RF
frame packing before the response is required. This optimization applies to all
three supported modes: Mode 0 frames are pre-constructed and pre-packed, while
Mode 5 and Mode 7 frames are pre-encrypted and pre-packed.

## 10. Implementation Reference

The principal implementation points are:

- `wmbus_datalink_dll.h`
  - `WMBUS_GW_PREENCRYPTION`
  - Per-connection packed-buffer and whitelist security state definitions.
- `wmbus_datalink_api.c`
  - `wmbus_link_prepare_tx_security_mode()`
  - `wmbus_link_create_packet_internal()`
  - `wmbus_link_gw_pack_next_frame()`
  - `wmbus_link_gw_enqueue_payload()`
  - `wmbus_link_GW_next_pre_encryption()`
  - Packed-buffer promotion, selection, and RX Security Mode enforcement.
- `wmbus_datalink_gw.c`
  - Selection, finalization, staging, and transmission of prepared packed
    frames.
- `wmbus_datalink_meter.c`
  - Meter RX mismatch handling and session termination.
- `wmbus_datalink_connection.c`
  - Per-connection next, in-flight, and SND-NKE packed-buffer ownership.
