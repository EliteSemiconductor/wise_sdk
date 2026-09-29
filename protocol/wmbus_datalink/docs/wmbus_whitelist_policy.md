# WMBus Gateway Whitelist Policy and Operation Guide

## 1. Purpose

This document defines the WMBus gateway whitelist behavior for fixed and
dynamic modes. It covers admission policy, storage ownership, Security Mode
assignment, CLI operation, application APIs, and safe configuration rules.

This document is the reference for firmware integration, system operation,
engineering handover, and customer configuration guidance.

## 2. Whitelist Entry

Each gateway whitelist entry contains:

- Meter device ID.
- Manufacturer.
- Device version.
- Device type.
- Configured Security Mode.
- Per-Meter link-layer runtime state, including access numbers, session state,
  message counters, and GW TX state.

The current membership lookup key is the Meter device ID. Manufacturer,
version, and device type are stored as entry information but are not part of
the whitelist lookup comparison.

Supported configured Security Modes are:

```text
0, 5, 7
```

When a Security Mode is not supplied by an interface that supports an optional
mode, Mode 7 is used.

## 3. Fixed and Dynamic Modes

### 3.1 Summary

| Configuration | Storage | Administrative update | Unknown Meter behavior | Automatically assigned mode |
|---|---|---|---|---|
| APP fixed whitelist | APP device table plus writable runtime-state buffer | Not allowed | Reject | Not applicable |
| APP dynamic whitelist | APP-provided writable RAM buffer | Allowed before GW start | Learn while capacity is available | Mode 7 |
| Temporary fixed whitelist | Heap-based writable test buffer | Allowed before GW start | Reject | Not applicable |
| Temporary dynamic whitelist | Heap-based writable test buffer | Allowed before GW start | Learn while capacity is available | Mode 7 |

The terms `fixed` and `dynamic` primarily define admission behavior:

- Fixed mode is a closed admission policy. Unknown Meters are rejected.
- Dynamic mode is a learning admission policy. Unknown Meters may be added to
  writable whitelist storage while capacity is available.

Storage mutability is a separate property. A temporary fixed whitelist is
writable for CLI provisioning even though its unknown-Meter admission policy is
fixed.

## 4. Fixed Whitelist

### 4.1 APP fixed whitelist

The application configures a fixed whitelist using:

```c
wmbus_link_gw_whitelist_load_fixed_with_security(
    info_p,
    security_mode_p,
    count,
    state_buffer_p,
    state_buffer_size);
```

The application owns:

- The device information table referenced by `info_p`.
- The writable runtime-state buffer referenced by `state_buffer_p`.

The device information table may be stored in flash or RAM. The runtime-state
buffer must be writable RAM and must be 4-byte aligned.

After a successful load:

```text
mode            = fixed
count           = configured entry count
capacity        = configured entry count
allow_update    = 0
admission_ctrl  = 1
```

Unknown Meters are rejected. Runtime `add` and `del` operations are rejected
because the fixed device table is not writable whitelist storage.

The link layer initializes each entry's runtime state when the fixed table is
loaded.

### 4.2 Fixed whitelist without a Security Mode array

The compatibility API is:

```c
wmbus_link_gw_whitelist_load_fixed(
    info_p,
    count,
    state_buffer_p,
    state_buffer_size);
```

This API assigns Mode 7 to every entry. Applications requiring Mode 0 or Mode 5
must use `wmbus_link_gw_whitelist_load_fixed_with_security()`.

### 4.3 Temporary fixed whitelist

The CLI creates a temporary fixed whitelist using:

```text
wm wl init fix [capacity]
```

Example:

```text
wm wl init fix 2
wm wl add 80000001 ESM 1 7 5
wm wl add 40000001 ESM 1 7 7
wm wl list
```

The link layer allocates writable storage from the heap. The operator may add
or delete entries before the GW is started. Unknown Meters are rejected by
default.

The temporary whitelist is not persistent and is lost after reset.

## 5. Dynamic Whitelist

### 5.1 APP dynamic whitelist

The application configures dynamic storage using:

```c
wmbus_link_gw_whitelist_load_dynamic_buffer(
    buffer_p,
    buffer_size,
    max_count);
```

The application provides one 4-byte-aligned writable RAM buffer. The link layer
divides the buffer into:

- Writable Meter device information entries.
- Writable per-Meter runtime-state entries.
- Required alignment padding.

The required size is available from:

```c
wmbus_link_gw_whitelist_get_dynamic_buffer_size(max_count);
```

`max_count` must be in the supported range and must not exceed
`WMBUS_WHITELIST_MAX_NUM`.

Loading a dynamic buffer clears the required buffer region and starts with an
empty table:

```text
mode            = dynamic
count           = 0
capacity        = max_count
allow_update    = 1
admission_ctrl  = 0
```

### 5.2 Temporary dynamic whitelist

The CLI creates a heap-based temporary dynamic whitelist using:

```text
wm wl init dyn [capacity]
```

Example:

```text
wm wl init dyn 8
wm wl list
wm start
```

The temporary dynamic table is writable and supports automatic learning. It is
not persistent and is lost after reset.

### 5.3 Dynamic learning

When a frame is received from a Meter whose device ID is not present in the
whitelist, the gateway applies the following admission sequence:

```text
Unknown Meter received
  -> Admission control enabled?
       Yes: reject as unknown
       No: continue
  -> Writable whitelist storage available?
       No: reject because no writable storage is available
       Yes: continue
  -> Capacity available?
       No: reject because the table is full
       Yes: add the Meter and continue processing
```

A dynamically learned entry is assigned Security Mode 7. The device
information is copied from the received WMBus address fields.

Application payload processing then applies the normal Security Mode policy.
The received application mode must match the newly configured Mode 7 entry.
Therefore, automatic dynamic learning supports Mode 7 Meters. Mode 0 and Mode 5
Meters must be provisioned explicitly before the GW is started.

If later frame validation fails, the learned entry is not automatically
removed.

### 5.4 Dynamic whitelist persistence

Dynamic learning is RAM-based. Learned entries are not automatically written to
nonvolatile storage.

When persistence is required, the application is responsible for saving the
approved Meter information and provisioning the required entries again after
reset.

## 6. Admission Control

Admission control determines how an unknown Meter is handled:

```text
admission_ctrl = 1: reject an unknown Meter
admission_ctrl = 0: attempt to learn an unknown Meter into writable storage
```

Whitelist initialization sets the default:

| Initialization | Default admission control |
|---|---:|
| Fixed | 1 |
| Dynamic | 0 |

The CLI can display or set the admission control value:

```text
wm adm
wm adm 0
wm adm 1
```

The effective behavior also depends on writable storage:

| Whitelist storage | Admission control | Unknown Meter result |
|---|---:|---|
| Temporary fixed, writable | 1 | Reject |
| Temporary fixed, writable | 0 | Learn as Mode 7 while capacity is available |
| Dynamic, writable | 0 | Learn as Mode 7 while capacity is available |
| Dynamic, writable | 1 | Reject |
| APP fixed, not writable | 0 or 1 | Reject |

Disabling admission control does not make an APP fixed table writable.

## 7. Administrative Operations

### 7.1 Add

The CLI syntax is:

```text
wm wl add [id] [manufacturer] [version] [device_type] [security_mode]
```

Examples:

```text
wm wl add 80000001 ESM 1 7 0
wm wl add 80000001 ESM 1 7 5
wm wl add 80000001 ESM 1 7 7
```

If `security_mode` is omitted, Mode 7 is assigned.

Add succeeds only when:

- Writable whitelist storage is configured.
- The device ID does not already exist.
- Capacity remains available.
- The Security Mode is 0, 5, or 7.

### 7.2 Delete

The CLI syntax is:

```text
wm wl del [id]
```

Delete requires writable whitelist storage. When an entry is deleted, the last
entry is moved into the deleted slot and the entry count is reduced. Entry
indices are therefore not persistent identifiers.

### 7.3 Clear

The CLI syntax is:

```text
wm wl clear
```

For a temporary whitelist, `clear` removes all entries but retains the allocated
buffer, capacity, mode, ownership, and admission policy.

For an APP-owned whitelist, `clear` detaches the whitelist from the link layer.
It does not free APP-owned memory.

### 7.4 Free

The CLI syntax is:

```text
wm wl free
```

`free` applies only to a temporary heap-based whitelist. It releases the
temporary allocation and resets the link-layer whitelist references.

APP-owned buffers are never freed by this command.

### 7.5 List

The CLI syntax is:

```text
wm wl list
```

List is read-only and reports:

- Mode.
- Owner.
- Entry count.
- Capacity.
- Update capability.
- Admission-control status.
- Per-entry identity, Security Mode, and runtime information.

List may be used before or after the GW is started.

## 8. Safe Operation Rule

Whitelist structural configuration must be completed before the GW is started.

The following operations MUST be executed only while the GW has not yet
executed `wm start` or `wmbus_link_start()`:

```text
whitelist initialization or load
wm wl add
wm wl del
wm wl clear
wm wl free
```

After the GW is started:

- The operator and application MUST NOT initialize, reload, add, delete, clear,
  or free whitelist storage.
- `wm wl list` remains allowed because it is read-only.
- Internal dynamic learning remains allowed because it is part of the dynamic
  admission flow and appends a new entry through the controlled RX path.

This rule prevents active connections, scheduled timers, queued payloads, and
per-Meter runtime state from referencing entries that have been removed, moved,
cleared, replaced, or freed.

To change whitelist structure after the GW has started:

1. Reset the device.
2. Initialize or load the whitelist.
3. Complete all required manual add or delete operations.
4. Verify the result with `wm wl list`.
5. Execute `wm start` only after the configuration is complete.

`wm stop` does not authorize whitelist reconfiguration. A reset is required
before changing whitelist structure.

## 9. Recommended Operating Profiles

### 9.1 Fixed production profile

Use fixed mode when the approved Meter population and Security Modes are known
before operation.

Recommended sequence:

```text
Load fixed whitelist
  -> Verify every Meter identity and Security Mode
  -> Register admission-result callback if required
  -> Start the GW
  -> Reject all unknown Meters during operation
```

### 9.2 Dynamic enrollment profile

Use dynamic mode for a controlled enrollment period in which unknown Mode 7
Meters may be learned.

Recommended sequence:

```text
Provide writable dynamic storage
  -> Set the required capacity
  -> Start the GW
  -> Learn Mode 7 Meters while capacity remains
  -> Read and approve the learned Meter population
  -> Persist approved identities in the application when required
```

Mode 0 and Mode 5 Meters must be provisioned explicitly before start.

### 9.3 Temporary CLI verification profile

Use `wm wl init fix` or `wm wl init dyn` for command-line testing. Temporary
whitelist storage is heap-based and nonpersistent.

Example fixed verification sequence:

```text
wm wl init fix 2
wm wl add 80000001 ESM 1 7 5
wm wl add 40000001 ESM 1 7 7
wm wl list
wm start
```

No whitelist structural command is issued after `wm start`.

## 10. Admission Result Reporting

The application may register a gateway admission callback. The callback reports
one of the following results:

| Result | Meaning |
|---|---|
| `WMBUS_LINK_GW_ADMISSION_ACCEPTED_ADDED` | The Meter was added to writable whitelist storage |
| `WMBUS_LINK_GW_ADMISSION_REJECTED_UNKNOWN` | Fixed or admission-controlled policy rejected the unknown Meter |
| `WMBUS_LINK_GW_ADMISSION_REJECTED_TABLE_FULL` | No whitelist capacity remained |
| `WMBUS_LINK_GW_ADMISSION_REJECTED_NO_WRITABLE_STORAGE` | Automatic learning was requested without writable storage |

The callback's Meter information is valid only during the callback. The
application must copy the information if it needs to retain it.

## 11. Customer-facing Summary

The WMBus gateway supports fixed and dynamic whitelist policies.

> Fixed mode accepts only pre-provisioned Meters. Dynamic mode can learn unknown
> Mode 7 Meters while writable capacity is available. Mode 0 and Mode 5 Meters
> must be provisioned explicitly. Whitelist initialization, loading, manual
> addition, deletion, clearing, and freeing must be completed before the gateway
> is started. After start, whitelist inspection is allowed, and internal dynamic
> learning may continue according to the configured admission policy.

## 12. Implementation Reference

The principal implementation points are:

- `wmbus_datalink_api.c`
  - `wmbus_link_gw_admit_meter()`
  - `wmbus_link_gw_learn_whitelist_entry()`
  - `wmbus_link_gw_whitelist_load_fixed_with_security()`
  - `wmbus_link_gw_whitelist_load_dynamic_buffer()`
  - `wmbus_link_gw_whitelist_init_temporary()`
  - `wmbus_link_gw_whitelist_add_device()`
  - `wmbus_link_gw_whitelist_del_device()`
  - `wmbus_link_gw_whitelist_clear()`
  - `wmbus_link_gw_whitelist_free_temporary()`
- `wmbus_datalink_cmd.c`
  - `wm wl` command parsing and validation.
  - `wm adm` admission-control command.
- `wmbus_datalink_api.h`
  - Fixed and dynamic whitelist application APIs.
  - Admission result definitions and callback contract.
