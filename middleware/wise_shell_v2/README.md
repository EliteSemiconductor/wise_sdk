# Shell Middleware

A lightweight embedded shell for command-line interaction.

## Features

- **Line Editing**: Cursor movement, insert/delete at any position
- **Command History**: Navigate previous commands with arrow keys
- **Tab Completion**: Auto-complete commands with menu selection
- **Command Groups**: Organize commands with subcommands
- **Auto-Registration**: Commands collected via linker sections

---

## Quick Start

### 1. Initialize Shell

```c
#include "shell.h"

static bool my_read_char(char *ch)
{
    // Return true and set *ch if a character is available
    // Return false if no character available
}

static void my_write_str(const char *s)
{
    // Output string to terminal
}

void app_init(void)
{
    shell_config_t cfg = {
        .read_char = my_read_char,
        .write_str = my_write_str,
        .prompt    = "MyShell> ",
    };

    shell_init(&cfg);
}
```

### 2. Run Shell Task

**Polling Mode:**
```c
void main_loop(void)
{
    while (1) {
        shell_task();
    }
}
```

**Event-Driven Mode:**
```c
void uart_rx_callback(char c)
{
    shell_process_char(c);
}
```

---

## Adding Commands

### Simple Command

```c
#include "shell.h"

static int cmd_hello(int argc, char **argv)
{
    PRINTF("Hello, World!\r\n");
    return 0;
}

SHELL_CMD_AUTO(hello, cmd_hello, "Say hello");
```

### Command Group

```c
#include "shell.h"

static int cmd_led_on(int argc, char **argv)
{
    PRINTF("LED ON\r\n");
    return 0;
}

static int cmd_led_off(int argc, char **argv)
{
    PRINTF("LED OFF\r\n");
    return 0;
}

static const shell_command_t led_subcmds[] = {
    SHELL_CMD(on,  cmd_led_on,  "Turn LED on"),
    SHELL_CMD(off, cmd_led_off, "Turn LED off"),
};

SHELL_CMD_GROUP_AUTO(led, led_subcmds, "LED control");
```

**Usage:**
```
MyShell> led on
LED ON
MyShell> led off
LED OFF
```

---

## Understanding argc/argv

The shell follows the Unix convention where `argv[0]` is always the **current command name**. However, the argc/argv values differ between simple commands and command group subcommands.

### Simple Command (`SHELL_CMD_AUTO`)

The handler receives the original `argc` and `argv` directly.

```
Input: kermit fs 0
       ↓
Handler receives:
  argc    = 3
  argv[0] = "kermit"   ← command name
  argv[1] = "fs"       ← first argument
  argv[2] = "0"        ← second argument
```

**Example:**
```c
static int cmd_kermit(int argc, char **argv)
{
    if (argc != 3) {
        printf("Usage: %s <target> <addr>\n", argv[0]);
        return 0;
    }
    const char *target = argv[1];
    uint32_t addr = strtoul(argv[2], NULL, 16);
    // ...
}

SHELL_CMD_AUTO(kermit, cmd_kermit, "Kermit file transfer");
```

### Command Group Subcommand (`SHELL_CMD_GROUP_AUTO`)

Subcommand handlers receive `argc - 1` and `argv + 1` (the parent command is stripped).

```
Input: gpio read 5
       ↓
Shell matches "gpio", then dispatches to "read" subcommand
       ↓
Subcommand handler receives:
  argc    = 2
  argv[0] = "read"     ← subcommand name
  argv[1] = "5"        ← first argument
```

**Example:**
```c
static int cmd_gpio_read(int argc, char **argv)
{
    if (argc < 2) {
        printf("Usage: %s <pin>\n", argv[0]);  // prints "read"
        return 0;
    }
    uint8_t pin = atoi(argv[1]);
    // ...
}

static const shell_command_t gpio_subcmds[] = {
    SHELL_CMD(read, cmd_gpio_read, "Read GPIO pin"),
};

SHELL_CMD_GROUP_AUTO(gpio, gpio_subcmds, "GPIO commands");
```

### Summary Table

| Type | Handler Receives | argv[0] Contains |
|------|------------------|------------------|
| `SHELL_CMD_AUTO` | `argc, argv` | Main command name |
| `SHELL_CMD_GROUP_AUTO` subcommand | `argc-1, argv+1` | Subcommand name |

### Design Rationale

This design follows the Unix/POSIX convention:

1. **argv[0] is always the current command** - Handlers can use `argv[0]` in usage messages without knowing their parent
2. **Subcommand independence** - The same handler can be reused across different command groups
3. **Consistent argument indexing** - In both cases, actual arguments start from `argv[1]`

### Common Mistakes to Avoid

```c
// WRONG: Missing argc check before accessing argv
static int cmd_test(int argc, char **argv)
{
    uint8_t pin = atoi(argv[1]);  // Crash if no argument provided!
    // ...
}

// CORRECT: Always validate argc first
static int cmd_test(int argc, char **argv)
{
    if (argc < 2) {
        printf("Usage: %s <pin>\n", argv[0]);
        return 0;
    }
    uint8_t pin = atoi(argv[1]);
    // ...
}
```

---

## Keyboard Shortcuts

| Shortcut | Description |
|----------|-------------|
| `Tab` | Auto-complete command (menu selection) |
| `↑ / ↓` | Navigate history / menu |
| `← / →` | Move cursor |
| `Ctrl+A` | Jump to start of line |
| `Ctrl+E` | Jump to end of line |
| `Ctrl+U` | Delete to start of line |
| `Ctrl+K` | Delete to end of line |
| `Ctrl+C` | Cancel current input |
| `Ctrl+L` | Clear screen |

---

## Configuration

Edit `shell_config.h` - only 3 options:

```c
#define SHELL_FEATURE_HISTORY     1    // Command history (↑↓)
#define SHELL_FEATURE_COMPLETION  1    // Tab completion
#define SHELL_INPUT_BUF_SIZE    128    // Input buffer size (32-1024)
```

### Configuration Profiles

**Minimal** (~170 bytes RAM)
```c
#define SHELL_FEATURE_HISTORY     0
#define SHELL_FEATURE_COMPLETION  0
#define SHELL_INPUT_BUF_SIZE     64
```

**Full Featured** (~900 bytes RAM)
```c
#define SHELL_FEATURE_HISTORY     1
#define SHELL_FEATURE_COMPLETION  1
#define SHELL_INPUT_BUF_SIZE    128
```

---

## Memory Footprint

### RAM Usage

| Component | Size | Condition |
|-----------|------|-----------|
| Input buffer | `SHELL_INPUT_BUF_SIZE` | Always |
| History | ~640 bytes | `HISTORY=1` |
| Completion | ~140 bytes | `COMPLETION=1` |
| State | ~20 bytes | Always |

### Derived Values (Internal)

| Value | Formula |
|-------|---------|
| History entries | 10 |
| Max history length | `INPUT_BUF_SIZE / 2` |
| Max completion matches | 16 |

---

## Linker Script

Add to your `.ld` file:

```ld
.shell_cmds :
{
    . = ALIGN(4);
    PROVIDE(__shell_cmds_start = .);
    KEEP(*(.shell_cmds))
    PROVIDE(__shell_cmds_end = .);
    . = ALIGN(4);
} > FLASH
```

---

## API Reference

### Core API

| Function | Description |
|----------|-------------|
| `shell_init(cfg)` | Initialize shell |
| `shell_task()` | Process input (polling mode) |
| `shell_process_char(c)` | Process single character (event-driven) |

### Command Break API

For long-running commands that need to respond to Ctrl+C:

| Function | Description |
|----------|-------------|
| `shell_poll_break()` | Poll for Ctrl+C (use in tight loops) |
| `shell_is_break_requested()` | Check if Ctrl+C was pressed |

```c
static int cmd_monitor(int argc, char **argv)
{
    PRINTF("Press Ctrl+C to stop\r\n");

    while (1) {
        do_work();
        if (shell_poll_break()) {
            PRINTF("Stopped\r\n");
            break;
        }
    }
    return 0;
}
```

### Helper API

| Function | Description |
|----------|-------------|
| `shell_get_commands(count)` | Get registered commands |

### Registration Macros

| Macro | Description |
|-------|-------------|
| `SHELL_CMD_AUTO(name, handler, help)` | Register simple command |
| `SHELL_CMD_GROUP_AUTO(name, subcmds, help)` | Register command group |
| `SHELL_CMD(name, handler, help)` | Define subcommand |

---

## File Structure

```
middleware/shell/
├── shell.h              # Main API
├── shell.c              # Core implementation
├── shell_config.h       # Configuration (3 options)
├── shell_cmd.h/c        # Command execution
├── shell_complete.h/c   # Tab completion
├── shell_input.h/c      # Line editing
├── shell_render.h/c     # Terminal output
├── cmdhist.h/c          # Command history

```
