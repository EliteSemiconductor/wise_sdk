# WISE SDK V2

This SDK is used to develop firmware that runs on the ESMT Radio SoC family.

## Features

- Provide driver APIs for all functions of ESMT SoC for application development.
- Various software middleware are implemented based on driver APIs to reduce development effort.
- Simplify the process for application porting on ESMT Radio SoCs.
- A full set of function demonstrations are implemented with shell commands.
- Supported platforms
  - ESMT Sub-1GHz SoC ER8130 series

## Directory structures

| Path                                  | Description                                         |
|---------------------------------------|-----------------------------------------------------|
| wise_sdk/                             | (top-level directory)                               |
| ├── app/                              | Projects                                            |
| │   ├── project_template              | Simple Hello World project with UART enabled        |
| │   └── WISEDemoApp/                  | Integrated demo application                         |
| ├── boards/                           | Configurations for various dev boards               |
| ├── documents/                        | SDK Documents                                       |
| ├── middleware/                       | Software functions                                  |
| │   ├── retarget/                     | Implementation of wrappers for standard libs        |
| │   ├── wise_ctrl_cmd/                | ESMT control commands                               |
| │   ├── wise_flash_filesystem/        | Flash partition management                          |
| │   ├── wise_kermit/                  | Kermit transmission protocol over UART              |
| │   ├── wise_nvm/                     | Functions for NVM access and management             |
| │   ├── wise_shellv2/                 | Console command line interface                      |
| │   ├── wise_system/                  | RTOS-like APIs for bare-metal implementation        |
| │   ├── wise_timer_hub/               | Multiple channel simulation over WUTMR              |
| │   └── wise_wmbus_crypto/            | Crypto helper for Wireless M-Bus                    |
| ├── third_party/                      | Open source software packages                       |
| │   ├── FreeRTOS-Kernel/              | Porting of FreeRTOS on ER8130                       |
| │   └── mbedtls/                      | Software implementation of crypto functions         |
| ├── tools/                            | Firmware download tools                             |
| └── wise_core_v2/                     | Core layer for ESMT SoC ER family                   |

## Getting Started

1. Import the demo project, `WISEDemoApp`, into the Eclipse workspace, and build all configurations  
   Right click on project name `WISEDemoApp`, select **Build Configurations -> Build All**

2. Once the firmware is successfully built, the output binaries will be created at:
   - `WISEDemoApp/eclipse/<config_name>/WISEDemoApp_<config_name>.bin`

3. Most simple demo functions can be found as a related build configuration in `WISEDemoApp`,
   which includes a simple C source file named `demo_<function>.c`.

## Example Applications (WISEDemoApp)

The `WISEDemoApp` project contains a set of example applications (Example APP group) that
demonstrate how to use the Core APIs and middleware modules. Most examples provide a shell
prompt over UART and expose commands for testing.

> Notes
> - Shell prompt string may differ per demo, e.g. `WDT>`, `WUTMR>`, `FLASH>`, `RADIO>`, etc.
> - Some examples are role/mode selectable via compile-time macros (e.g. master/slave, meter/collector).

### Core peripheral demos

| Demo                      | Brief 
|---------------------------|-----------------------------------------------------------------------------------------------------------------------------------|
| GPIO Demo                 | Configure GPIO as input/output, toggle outputs, and demonstrate GPIO interrupt callback.
| I2C Demo (Master/Slave)   | Basic I2C communication demo supporting master TX and slave RX with event callback. 
|                           | Includes IO pin mux setup and optional DMA flag. 
| SPI Demo (Master/Slave)   | Basic SPI communication demo in REGULAR mode with command phase and non-blocking transfer-done callback.
|                           | Supports IO pin mux setup for multiple modes.
| UART Shell Demo           | Minimal shell backend example using UART read/write callbacks and a prompt. Useful as a template for CLI based demos. 
| GPTMR Demo                | General-purpose timer demo: configure periodic and one-shot timers, register callbacks, and start timers.
| WUTMR Demo + Power CLI    | CLI to show WUTMR counter, start periodic/one-shot WUTMR interrupts, and enter CPU idle / system sleep 
|                           | (single or loop) for current measurement.
| Timer Hub Demo            | Demonstrate middleware timer hub that simulates multiple software timer channels on top of WUTMR, including periodic 
|                           | and one-shot behavior.
| WDT Demo                  | Configure watchdog timer, periodically refresh in main loop, and provide a `block` command to stop refreshing and trigger reset.
| RTC Demo                  | Set/get RTC time and configure an alarm via shell commands; prints alarm event in callback.
| PWM Demo                  | PWM output demo using frequency mode. Supports continuous and one-shot pulse generation with completion callback.
| PWMSLOW Demo              | Low-frequency PWM demo using LFOSC clock source. Demonstrates LFOSC configuration/calibration, continuous and one-shot modes.
| NFC Demo                  | Demonstrate NFC init/config, tag content write, and interrupt callback for NFC events.
| Flash CLI Demo            | Shell-based flash utilities: read, write (pattern), and sector erase. Prints flash info and UID.

### Radio & protocol demos

| Demo                      | Brief 
|---------------------------|-----------------------------------------------------------------------------------------------------------------------------------|
| Sub-1GHz Radio Demo       | Basic radio configuration and interactive CLI commands for RX on/off, TX packets, channel selection, and RX event handling.
| WMBus PHY Demo            | Wireless M-Bus PHY demo using WMBus mode/role configuration. Meter periodically reports and waits for response; Collector 
|                           | receives, delays ACK, and responds. Includes crypto helper key setup and frame helper parsing/dump.

### System / loader / RTOS demos
| Demo                      | Brief 
|---------------------------|-----------------------------------------------------------------------------------------------------------------------------------|
| Power Mode Demo           | Shell commands to enter CPU idle, system sleep, and shutdown mode. Demonstrates wake sources (WUTMR/NFC/GPIO) and 
|                           | wakeup IO interrupt configuration.
| Loader + Kermit Update    | A simple loader shell that supports booting to APP address and updating APP via UART kermit protocol into flash, 
|                           | then remapping/booting.
| FreeRTOS CLI + Queue Demo | Demonstrates FreeRTOS tasks, queue-based command dispatch, and shared resource management patterns. Provides 
|                           | CLI commands that enqueue messages to worker task and shows timing/latency.

### Where to find demo sources

In `WISEDemoApp`, each build configuration usually maps to a single demo C file (e.g. `demo_gpio_main.c`,
`demo_spi_main.c`, `demo_wdt_main.c`, `demo_wutmr_main.c`, etc.).  
Search for files with prefix `demo_` under the project source tree to locate all examples.

### Documentation
For detailed API references, please refer to documents/WISE API/html/index.html.


## 2-Stage Booting

A reference implementation of a 2-stage booting mechanism is provided in
`demo_boot_loader`.

To configure the application boot address:
1. Modify the macro `APP_BOOT_ADDR` to the desired boot address.
2. Rebuild the project.
3. Download the generated binary `WISEDemoApp_demo_boot_loader.bin` to the ER8130 EVK.

The default method for downloading application firmware is via Kermit transmission.
Once updated, the boot loader can remap and execute the downloaded application image.

