# WISE SDK V2

Firmware SDK for the ESMT ER8130A/ER8131A Sub-1GHz radio SoC. C, bare-metal or
FreeRTOS.

The current version and change list are in [ReleaseNote.txt](ReleaseNote.txt).

## Supported platform

| Platform        | Platform name in the SDK |
|-----------------|--------------------------|
| ER8130A/ER8131A | `er8131p9`               |

## Features

- Driver APIs for every SoC function: GPIO, UART, SPI, I2C, PWM, PWM slow,
  timers (GPTMR, WUTMR, RTC, tick), WDT, flash, eFuse, crypto, TRNG, PMU and
  power modes, NFC, and cache control.
- Radio API for Sub-1GHz TX/RX, with W-MBus PHY support.
- W-MBus data link layer (`libWMbusDatalink`) with Security Mode 0/5/7.
- Middleware: shell (CLI), flash file system, NVM, Kermit download, timer hub,
  W-MBus crypto, control commands, EPD (e-paper display) panel drivers.
- Firmware update over UART (AppLoader + Kermit) and over NFC.
- FreeRTOS support.
- Eclipse projects for every project; `project_template` and `WISEDemoApp` also
  build with Keil MDK and CMake + Ninja.

## Directory structure

| Path                                | Description                                                 |
|-------------------------------------|-------------------------------------------------------------|
| `app/`                              | Projects (see [Projects](#projects))                        |
| `boards/`                           | Board configuration; `board_default_cfg.h` is the default board |
| `documents/`                        | User guide, programming guide, demo guide, API reference    |
| `middleware/`                       | Software components built on the core APIs                  |
| `├── retarget/`                     | stdio wrapper                                               |
| `├── wise_ctrl_cmd/`                | ESMT control commands                                       |
| `├── wise_epd/`                     | E-paper display panel drivers                               |
| `├── wise_flash_filesystem/`        | Flash partition management                                  |
| `├── wise_kermit/`                  | Kermit file transfer                                        |
| `├── wise_nvm/`                     | Non-volatile storage                                        |
| `├── wise_shell/`, `wise_shell_v2/` | Console command line interface                              |
| `├── wise_system/`                  | System init and RTOS-like services for bare-metal           |
| `├── wise_timer_hub/`               | Multi-channel software timers                               |
| `└── wise_wmbus_crypto/`            | W-MBus encryption                                           |
| `protocol/`                         | Protocol stacks                                             |
| `├── wmbus_datalink/`               | W-MBus data link layer API                                  |
| `└── prebuilt_libs/`                | Prebuilt W-MBus data link library                           |
| `third_party/`                      | Upstream code (mbedtls, FreeRTOS-Kernel); do not modify     |
| `wise_core_v2/`                     | Core layer                                                  |
| `├── api/`                          | Public WISE driver APIs                                     |
| `├── core_utils/`                   | Common utilities                                            |
| `├── platform/`                     | HAL interface and platform drivers                          |
| `├── radio_lib/`                    | Radio API                                                   |
| `└── prebuilt_libs/`                | Prebuilt radio library (`libWISERadioLib_er8131p9.a`)       |

## Projects

| Project            | Description                                                         |
|--------------------|---------------------------------------------------------------------|
| `AppLoader`        | Boot loader: boots the application partition, UART console, Kermit firmware download |
| `project_template` | Minimal "Hello World" project with UART enabled; start a new application from here |
| `WISEDemoApp`      | Demo application; each demo is a separate build configuration (see below) |

### WISEDemoApp demos

| Build configuration          | Demonstrates                                                |
|------------------------------|-------------------------------------------------------------|
| `demo_cli`                   | UART shell integration                                      |
| `demo_gpio`                  | GPIO                                                        |
| `demo_spi`                   | SPI master/slave transfer                                   |
| `demo_i2c`                   | I2C master/slave communication                              |
| `demo_pwm`                   | Continuous and one-shot PWM output                          |
| `demo_pwmslow`               | Low-frequency PWM output                                    |
| `demo_rtc`                   | RTC time read/set and alarm                                 |
| `demo_wutmr`                 | Wake-up timer and idle/sleep power modes                    |
| `demo_gptmr`                 | General-purpose timer configuration and callbacks           |
| `demo_wdt`                   | Watchdog configuration and refresh                          |
| `demo_crypto`                | Crypto engine                                               |
| `demo_trng`                  | True random number generator                                |
| `demo_flash`                 | Flash operations                                            |
| `demo_power_mode`            | Idle/sleep/shutdown power modes and wake-up sources         |
| `demo_timer_hub`             | Timer hub multi-channel scheduling                          |
| `demo_freeRTOS`              | FreeRTOS: UART CLI, inter-task message queue, LED heartbeat |
| `demo_radio_trx`             | Basic radio TX/RX control                                   |
| `demo_nfc`                   | NFC configuration and interrupt callbacks                   |
| `demo_nfc_ctrl`              | Firmware update and data exchange over the NFC control channel |
| `demo_boot_loader`           | Loader with UART shell, Kermit update and boot-to-app       |
| `demo_wmbus_phy_meter`       | W-MBus PHY TX/RX and PER test, meter side                   |
| `demo_wmbus_phy_meter_sleep` | W-MBus PHY meter that sleeps between periodic reports       |
| `demo_wmbus_phy_other`       | W-MBus PHY TX/RX and PER test, other (collector) side       |
| `demo_wmbus_link_meter`      | W-MBus data link, meter side                                |
| `demo_wmbus_link_other`      | W-MBus data link, other (gateway) side                      |
| `_demo_template`             | Template for a new demo                                     |

The W-MBus PHY demos are described in
[documents/WMBus_PHY_Demo_Guide.pdf](documents/WMBus_PHY_Demo_Guide.pdf).

## Building

A project has up to three build systems (`AppLoader` has Eclipse only); keep
them in sync when you change project settings.

| Build system  | Location               | Notes                                                    |
|---------------|------------------------|----------------------------------------------------------|
| Eclipse       | `<project>/eclipse/`   | Main development environment                             |
| Keil MDK      | `<project>/keil/`      | `WISEDemoApp/keil/WISEDemoApp.uvmpw` holds all demos     |
| CMake + Ninja | `<project>/cmake/`     | Commands in `<project>/cmake/setup_powershell.txt`       |

The GCC toolchain is expected in
`C:\esmt\Development_Suite\v1.0\tools\toolchain\gcc\bin`.

Eclipse:

1. Import the project from `app/<project>/eclipse` into an Eclipse workspace.
2. Select the build configuration: `er8131p9` for `AppLoader` and
   `project_template`, or one of the demo configurations for `WISEDemoApp`.
3. Build. The binary is written to `app/<project>/eclipse/<configuration>/`
   (`<project>.bin`, or `WISEDemoApp_<demo>.bin` for the demos).

CMake (PowerShell, from `app/<project>/cmake`):

```powershell
$env:Path += ";C:\esmt\Development_Suite\v1.0\tools\toolchain\gcc\bin\"
cmake -S . -B build -G Ninja --toolchain toolchains/arm-gcc.cmake
cmake --build build --target <target_name>
```

## Getting started with AppLoader and WISEDemoApp

1. Build `AppLoader` and a `WISEDemoApp` demo, for example `demo_cli`.
2. Program `AppLoader.bin` to the EVB with the ESMT flash programmer through
   J-Link or DAPLink.
3. Connect the EVB UART to the PC and open a terminal. With the default board
   configuration (`boards/board_default_cfg.h`):

   | Setting  | Value       |
   |----------|-------------|
   | UART TX  | IO 0        |
   | UART RX  | IO 1        |
   | Format   | 115200 8N1  |

4. Reset the EVB. AppLoader prints its banner and waits 3 seconds; type `c`
   three times within that time to enter the console:

   ```text
   ESMT Sphynx APP Loader V2.02
   Press 'cccc' to enter console...
   ..ccc
   ESMT>
   ```

   Without input, AppLoader boots the application partition.

5. AppLoader commands:

   | Command                    | Description                                      |
   |----------------------------|--------------------------------------------------|
   | `help`                     | List commands                                    |
   | `fs format`                | Create the default partition table               |
   | `fs info`                  | Show partition information                       |
   | `fs dump`                  | Dump flash contents                              |
   | `kermit fs <partition>`    | Receive a firmware image into a partition (not 0, which holds AppLoader) |
   | `kermit flash`             | Receive a firmware image to the fixed application address |
   | `reset`                    | Reset the chip                                   |
   | `dump`                     | Dump a buffer                                    |

6. Download the application. On a new board, create the partition table first:

   ```text
   ESMT> fs format
   ESMT> kermit fs 1
   ```

   Then send `WISEDemoApp_<demo>.bin` with Kermit from the terminal (Tera Term:
   *File > Transfer > Kermit > Send*). When the transfer finishes, reset the
   board; AppLoader boots the application. The demo prints a banner with the
   demo name, build time, SDK version and radio library version, then shows the
   `ESMT>` prompt. Type `help` to list the demo's commands.

## Documentation

| Document                                                       | Content                                   |
|----------------------------------------------------------------|-------------------------------------------|
| `documents/ESAP-ER8130-001-SC WISE SDK User Guide_*.pdf`       | SDK user guide                            |
| `documents/ESAP_ER813X-002-SC Programming Guide_*.pdf`         | Programming guide                         |
| `documents/WMBus_PHY_Demo_Guide.pdf`                           | W-MBus PHY demo and PER test              |
| [`documents/WISE API/index.html`](documents/WISE%20API/index.html) | API reference (Doxygen HTML)          |

## Version numbering

The SDK version is `major.minor.release`, for example `4.13.00`. Read it at run
time with `wise_core_get_version()`:

```c
WISE_SDK_VERSION_T v;
wise_core_get_version(&v);
printf("WISE SDK Version %d.%02d.%02d\n", v.verMajor, v.verMinor, v.verRelease);
```
