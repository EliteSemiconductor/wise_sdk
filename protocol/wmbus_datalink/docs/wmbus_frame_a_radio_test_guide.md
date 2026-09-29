# WMBus Frame-A Radio Test Mode User Guide

## 1. Purpose

This document defines the WMBus Frame-A Radio Test Mode implementation for
SOC_9006. It is the reference for RF-driver verification, manufacturing or
laboratory testing, integration testing, engineering handover, and customer
support.

The frame test provides a controlled two-device WMBus transmit/receive test
using the same WMBus RF driver used by the datalink product. It isolates the
RF-driver and WMBus Frame A path from the normal WMBus datalink protocol.

## 2. Scope and Operating Model

The frame test is built as a dedicated firmware configuration named
`SOC_9006_FRAME_TRX` in the `wmbus_dll` Eclipse project.

When this configuration is selected:

- The application starts the shell and the Frame test event handler.
- The normal WMBus datalink FSM is not started.
- Security Mode, whitelist, admission control, connection tables, sessions,
  application payload queues, pre-encryption, pre-packed datalink buffers, and
  Early-RX are not used by Frame test.
- The WMBus RF driver is initialized with the selected WMBus role and PHY mode.
- The RF driver performs its normal WMBus physical framing operations,
  including Frame A CRC and the applicable line coding.

The frame test therefore tests the RF-driver transmit and receive path without
requiring a normal datalink connection to be established.

### 2.1 Test boundary

The current test boundary starts with an application-generated WMBus test frame
and ends after the received frame has passed RF-driver decoding, Format A CRC
validation, and frame-test payload validation:

```text
Frame test frame
  -> Format A CRC packing
  -> Manchester / 3-out-of-6 coding
  -> RF TX
  -> RF RX
  -> coding decode
  -> Format A CRC validation
  -> frame header / pattern / sequence validation
```

This boundary simultaneously verifies:

- Format A CRC generation and checking.
- WMBus line coding and decoding for the selected PHY mode.
- RF-driver TX and RX operation.
- Frame-test header, repeated-pattern, and sequence handling.

It is not a raw RF PHY test mode. Arbitrary short packets that bypass WMBus
Format A framing and coding are outside this test boundary. The minimum frame
length is therefore 10 bytes, matching the first Format A block accepted by the
WMBus RF driver.

## 3. Build Configuration

In Eclipse, select the `SOC_9006_FRAME_TRX` build configuration under the existing
`wmbus_dll` project.

The configuration has the following relevant properties:

| Property | Value |
|---|---|
| RF platform | `subg_soc_9006` |
| Linker script | `ldscripts/default_9006.ld` |
| Compile symbol | `WMBUS_FRAME_TEST` |
| Datalink include path | `protocol/wmbus_datalink` |
| Platform interface include path | `protocol/wmbus_datalink/platform_intf` |

Build and program the same `SOC_9006_FRAME_TRX` image into both test devices. The
device role and PHY mode are selected at run time with `wm frame_test init`.

The startup banner identifies the dedicated image:

```text
WISE_CORE_V2 WMBUS FRAME-A RADIO TEST
```

## 4. Roles and PHY Modes

### 4.1 Role values

| Role value | RF-driver role | Typical device |
|---:|---|---|
| `0` | Other | GW or concentrator side |
| `1` | Meter | Meter side |

The value is passed directly to the WMBus RF driver as its WMBus role. It is
not a normal datalink GW/Meter configuration and does not create a session.

### 4.2 Mode values

| Mode value | WMBus mode | Direction class |
|---:|---|---|
| `0` | S1 | Unidirectional |
| `1` | S2 | Bidirectional |
| `2` | T1 | Unidirectional |
| `3` | T2 | Bidirectional |
| `4` | C1 | Unidirectional |
| `5` | C2 | Bidirectional |

R2 is intentionally not exposed because it is not supported by the current RF
driver.

### 4.3 Direction rules

For unidirectional modes S1, T1, and C1:

| Role | Permitted operation |
|---|---|
| Meter (`1`) | TX |
| Other/GW (`0`) | RX |

For bidirectional modes S2, T2, and C2, both roles may start TX or RX.

For a conventional two-device test, use Meter TX and Other/GW RX for all
modes. This gives one consistent command sequence and matches normal WMBus
traffic direction.

Both devices must use the same PHY mode.

## 5. Frame Test Shell Commands

The frame-test firmware intentionally exposes only the `frame_test` command
group under `wm`:

```text
wm frame_test help
```

The complete command set is:

```text
wm frame_test init [role] [mode]
wm frame_test tx [frame_length] [pattern] [count] [interval_ms]
wm frame_test rx [on|off]
wm frame_test status
wm frame_test reset
wm frame_test stop
wm frame_test deinit
```

### 5.1 `wm frame_test init [role] [mode]`

Initializes the WMBus RF driver and the Frame test state.

```text
wm frame_test init 0 3
```

The example configures an Other/GW receiver in T2 mode.

```text
wm frame_test init 1 3
```

The example configures a Meter transmitter in T2 mode.

`init` is valid only after startup or after a successful `deinit`. To change
role or mode, stop the active operation, deinitialize the current Frame test instance,
and initialize it again with the new values.

After a board reset, Frame test has not yet been initialized. In that state,
`wm frame_test deinit` reports `Frame test deinit failed`; this is expected and does not
prevent a subsequent `wm frame_test init`.

### 5.2 `wm frame_test tx [frame_length] [pattern] [count] [interval_ms]`

Starts a finite, non-blocking transmit test.

| Argument | Valid range or values | Description |
|---|---|---|
| `frame_length` | `10` to `256` bytes | Total frame-test frame length before RF packing |
| `pattern` | `aa`, `55`, `0f`, `f0`, `ff`, `00`, or `0` to `5` | Repeated test-data pattern |
| `count` | `1` to `4294967295` | Number of frames to request |
| `interval_ms` | `0` to `268435455` ms | Delay after local TX completion before the next frame |

Example:

```text
wm frame_test tx 32 aa 100 100
```

This requests 100 frames of 32 bytes, using the `0xAA` test pattern and a
100 ms interval after each TX completion.

`tx` succeeds only when Frame test is in `READY` state and the selected role/mode
permits transmission. It returns immediately after the first RF submission;
the remaining frames are sent from the Frame test event and scheduler path.

`interval_ms=0` starts the next frame after the preceding TX completion event,
without inserting an additional scheduler delay.

### 5.3 `wm frame_test rx on` and `wm frame_test rx off`

Starts or stops one-shot receive operation.

```text
wm frame_test rx on
```

After every receive completion or receive error, Frame test restarts one-shot RX from
the main event context. This avoids printing, packet parsing, or shell work in
the RF interrupt callback.

`wm frame_test rx off` is valid only while the current state is `RX`. If Frame test is
already `READY`, the command reports `Frame test RX off failed`; no RF operation is
active in that case.

### 5.4 `wm frame_test status`

Prints the current state, selected role/mode, TX settings, and accumulated
statistics. This is the primary pass/fail observation command.

Example output:

```text
Frame test state=RX role=Other/GW(0) mode=T2(3)
TX requested/accepted/done/error/rejected=0/0/0/0/0
TX len=0 pattern=AA interval=0 ms
RX good/error/format/pattern/lost/sequence_error=100/0/0/0/0/0 rssi=-61
```

### 5.5 `wm frame_test reset`

Clears Frame test statistics and resets TX/RX sequence tracking. It is valid in
`READY` and `RX` state. It is intentionally rejected during `TX`, because the
TX counters control the active finite transmit operation.

For a new two-device measurement, reset both devices before the transmitter
starts the test.

### 5.6 `wm frame_test stop`

Stops an active TX scheduler operation or active RX operation and returns Frame test
to `READY` state. If a frame is being transmitted, the RF TX stop API is used.

### 5.7 `wm frame_test deinit`

Stops Frame test, deinitializes the RF driver, and returns the Frame test state to
`UNINITIALIZED`. Use this command before a new `init` with different role or
mode values.

## 6. Test Frame Format

The frame test creates an application-level test frame before handing it to the
WMBus RF driver.

| Byte offset | Field | Description |
|---:|---|---|
| `0` | L field | `frame_length - 1` |
| `1` | Marker | Fixed value `0xD7` |
| `2` | Version | Fixed value `0x01` |
| `3` | Pattern ID | `0` to `5` |
| `4..7` | Sequence | Little-endian 32-bit sequence number |
| `8` | Source role | Frame-test role used by the sender |
| `9` | PHY mode | WMBus PHY mode used by the sender |
| `10..N-1` | Test data | Repeated selected pattern byte |

The RF driver subsequently applies its WMBus physical frame processing. Frame test
does not generate or consume normal datalink, ELL, AFL, TPL, security, or
application-payload fields.

The minimum length is 10 bytes because the RF driver requires a valid minimum
WMBus frame length. A 10-byte test frame contains only the Frame test control header;
use a length greater than 10 when pattern-byte validation is required.

## 7. TX and RX Processing

### 7.1 TX processing

```text
Shell command
  -> Validate Frame test state, role, mode, length, pattern, count, and interval
  -> Build application-level test frame
  -> Submit frame to WMBus RF driver
  -> RF TX_DONE or TX_ERROR interrupt callback
  -> Post Frame test event
  -> Main event handler updates counters
  -> Schedule or submit the next frame
```

The interval begins after local RF completion. Frame test does not wait for a remote
acknowledgement because it is a physical-link test and does not implement a
datalink acknowledgement protocol.

### 7.2 RX processing

```text
RF RX completion or error interrupt callback
  -> Copy the bounded received frame into frame-test-owned static storage
  -> Release RF driver RX storage
  -> Post Frame test RX event
  -> Main event handler validates Frame test header, pattern, role, mode, and sequence
  -> Restart one-shot RX
```

The interrupt callback does not print messages and does not run datalink
parsing. This keeps the ISR work bounded and avoids altering RF timing while
the test is running.

## 8. Statistics and Interpretation

### 8.1 TX statistics

| Field | Meaning |
|---|---|
| `requested` | Frames requested by the `tx` command |
| `accepted` | Frames accepted by the RF TX API |
| `done` | RF TX completion events received |
| `error` | RF TX error events received |
| `rejected` | RF API submission or scheduler rejection |

A completed local transmit test normally has:

```text
requested = accepted = done
error = rejected = 0
state = READY
```

TX completion proves that the local RF driver completed the transmit operation.
It does not itself prove that a remote receiver decoded the frame.

### 8.2 RX statistics

| Field | Meaning |
|---|---|
| `good` | Frames accepted as valid by the RF driver and passed to Frame test |
| `error` | RF RX errors, missing Frame test snapshot, or failure to restart RX |
| `format` | Invalid length, Frame test marker/version, role, or PHY-mode field |
| `pattern` | Frame-test data byte does not match the declared pattern |
| `lost` | Forward sequence-number gap detected |
| `sequence_error` | Repeated, old, or reverse-order sequence number |
| `rssi` | RSSI of the most recently accepted frame |

For an isolated two-device test, a successful receive result normally has:

```text
good = transmitter done count
error = format = pattern = lost = sequence_error = 0
```

`good` is a count of RF-driver accepted frames. A Frame test format or pattern error
is reported separately so that RF reception and Frame test payload validation remain
distinguishable.

## 9. Standard Two-Device Test Procedure

This procedure uses T2, a 32-byte frame, the `0xAA` pattern, 100 frames, and a
100 ms post-TX interval.

### 9.1 Prepare the Other/GW receiver

On the device used as the receiver:

```text
wm frame_test init 0 3
wm frame_test reset
wm frame_test rx on
wm frame_test status
```

Expected initial status:

```text
Frame test state=RX role=Other/GW(0) mode=T2(3)
RX good/error/format/pattern/lost/sequence_error=0/0/0/0/0/0 rssi=0
```

### 9.2 Start the Meter transmitter

On the device used as the transmitter:

```text
wm frame_test init 1 3
wm frame_test reset
wm frame_test tx 32 aa 100 100
```

Allow the test to finish. The nominal interval portion is approximately ten
seconds, plus RF airtime and shell processing.

### 9.3 Verify both sides

On the transmitter:

```text
wm frame_test status
```

Expected result:

```text
Frame test state=READY role=Meter(1) mode=T2(3)
TX requested/accepted/done/error/rejected=100/100/100/0/0
```

On the receiver:

```text
wm frame_test status
```

Expected result:

```text
Frame test state=RX role=Other/GW(0) mode=T2(3)
RX good/error/format/pattern/lost/sequence_error=100/0/0/0/0/0
```

The following observed result is a successful reference measurement:

```text
Meter TX requested/accepted/done/error/rejected=100/100/100/0/0
Other/GW RX good/error/format/pattern/lost/sequence_error=100/0/0/0/0/0
RSSI=-61
```

### 9.4 Stop the receiver

After recording the receiver statistics:

```text
wm frame_test rx off
```

The receiver should then report `state=READY`.

## 10. Additional Test Cases

### 10.1 Pattern coverage

Repeat the standard procedure with each pattern:

```text
wm frame_test tx 32 aa 100 100
wm frame_test tx 32 55 100 100
wm frame_test tx 32 0f 100 100
wm frame_test tx 32 f0 100 100
wm frame_test tx 32 ff 100 100
wm frame_test tx 32 00 100 100
```

Before each independent measurement, reset both devices so that sequence and
statistics start from a known state.

### 10.2 Frame-length coverage

Use a length greater than 10 when validating the repeated pattern bytes. Useful
boundary tests include:

```text
wm frame_test tx 11 aa 10 100
wm frame_test tx 32 aa 100 100
wm frame_test tx 256 aa 10 100
```

### 10.3 Interval coverage

The following tests exercise maximum throughput and paced operation:

```text
wm frame_test tx 32 aa 100 0
wm frame_test tx 32 aa 100 10
wm frame_test tx 32 aa 100 100
wm frame_test tx 32 aa 100 1000
```

### 10.4 Mode coverage

For each selected mode, both devices must use the same mode value. For
unidirectional modes, retain Meter TX and Other/GW RX. For bidirectional modes,
repeat the test with the roles reversed if bidirectional operation is being
verified.

## 11. Diagnostic Guide

| Observation | Meaning and action |
|---|---|
| `Frame test init failed` | Frame test may already be initialized, the role/mode is invalid, or RF initialization failed. Run `status`; if already initialized, use `deinit` before changing role/mode. |
| `Frame test TX start failed` | Frame test is not `READY`, TX direction is invalid for the selected unidirectional mode, or RF TX submission failed. Check `status`, role, mode, and direction rules. |
| `Frame test RX on failed` | Frame test is not `READY`, RX direction is invalid for the selected unidirectional mode, or RF RX start failed. |
| `Frame test RX off failed` | Frame test is not in `RX` state. This is expected after TX completion, after `stop`, or before `rx on`. |
| TX `done` is lower than `requested` | TX is still active or was stopped. Wait for `state=READY`, then check `error` and `rejected`. |
| TX `error` or `rejected` is nonzero | Local RF TX did not complete normally. Preserve both logs and report the selected role, mode, frame length, pattern, count, and interval. |
| RX `good=0`, TX succeeds | Confirm that the receiver was initialized as `role=0`, has `state=RX`, uses the same mode, and was started before TX. Then check antenna, distance, power, and RF environment. |
| RX `format` is nonzero | A radio-valid frame was received but does not match the expected frame-test format, role, or mode. Check that the peer is running the same frame-test image and selected mode. |
| RX `pattern` is nonzero | The Frame test header was valid but the repeated test bytes did not match its pattern ID. Preserve logs and repeat with a fixed pattern and frame length greater than 10. |
| RX `lost` is nonzero | One or more forward sequence numbers were absent. Compare transmitter `done` with receiver `good`, then repeat with a longer interval or shorter distance. |
| RX `sequence_error` is nonzero | A duplicate, old, or reverse-order frame was observed. Reset both devices before the next independent test. |

## 12. Operational Notes

- Record both device logs for every test. The command echo and `status` output
  identify the actual role of each physical device; do not infer role solely
  from a log filename.
- Run receiver setup first and verify `state=RX` before starting the
  transmitter.
- Read receiver status only after the transmitter has completed. A receiver
  status captured before TX normally reports zero received frames.
- Do not expect one console line per received frame. Per-frame printing is
  intentionally omitted to protect RF timing.
- Frame test is not an interoperability test for normal datalink sessions, security
  modes, or application payload exchange. Use the normal WMBus firmware and
  the security-mode test procedures for those functions.

## 13. Source Components

| Component | Responsibility |
|---|---|
| `wmbus_datalink_frame_test.c` | Frame test state, frame creation, statistics, TX/RX event processing |
| `wmbus_datalink_frame_test.h` | Frame test public types and APIs |
| `platform_intf_rf.c` | Frame test RF-driver initialization, bounded ISR callback, RX snapshot ownership |
| `platform_intf_rf.h` | Frame test platform event and RF interface declarations |
| `wmbus_datalink_cmd.c` | `wm frame_test` command parsing and status output |
| `app/wmbus_dll/src/main.c` | Frame-test-specific application startup path |
| `app/wmbus_dll/eclipse/.cproject` | `SOC_9006_FRAME_TRX` build configuration |
