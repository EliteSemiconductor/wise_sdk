# WISE SDK V2

This SDK is used to developing FW to run on ESMT Radio SoC family.

## Features

 - Provide driver APIs for all functions of ESMT SoC for application development.
 - Various software middleware are implemented based on driver APIs to reduce developing effort.
 - Simplify process for application porting on ESMT Radio SoCs.
 - Full set of function demonstration are implemented in shell command.
 - Supported platforms
    ESMT Sub 1GHz SoC ER8130 series


## Dirctory structures

| Path                                  | Description                                         |
|---------------------------------------|-----------------------------------------------------|
| wise_sdk/                             | (top-level directory)                               |
| ├── app/                              | Projects                                            |
| │   ├── AppLoader/                    | Application loader                                  |
| │   ├── m2c_beacon                    | Sample code of syncing with M2C AP beacon          |
| │   ├── project_template              | Simple Hello World project with UART enabled       |
| │   └── WISEDemoApp/                  | Integrated demo application                         |
| ├── boards/                           | Configurations for various dev boards              |
| ├── docs/                             | Documents                                          |
| ├── middleware/                       | Software functions                                  |
| │   ├── wise_ctrl_cmd/                | ESMT control commands                              |
| │   ├── wise_flash_filesystem/        | Flash partition management                          |
| │   ├── wise_shell/                   | Console command line interface                      |
| │   │   ├── commands/                 | Shell command table                                |
| │   │   └── functions/                | Command implementation, API demonstration          |
| │   ├── wise_system/                  | RTOS-like APIs for bare-metal implementation        |
| │   ├── wise_kermit/                  | Kermit implementation                              |
| │   └── retarget/                     | Wrapper of stdio                                    |
| └── wise_core/                        | Core layer for ESMT SoC ER family                  |
|     ├── api/                          | Common WISE APIs for application development       |
|     └── platform/                     | Platform HAL driver for ESMT SoCs                  |


## Getting Started

	1. Import both projects, AppLoader and WISEDemoApp, into Eclipse workspace, and build

	2. Once the firmwares are successfully built, the output binaries will be created in the following path of project folder
		<project_name>/eclipse/ER8130_TC04/<project_name>.bin
		
	3. Download Apploader, AppLoader.bin, to TC04 EVB by esmt_flash_programmer. 
	   Before downloading, please make sure JLink or DAPLink is well connected.


## Using CLI in AppLoader

	1. Connect to PC through UART and launch your terminal application on PC
		UART Rx --> IO 0
		UART Tx --> IO 1
		Configuration: 115200 8N1
		
	2. Press reset button on EVB to trigger board reset. If everything is setup well, text messages should be displayed on terminal

		ESMT SBL v2.00 running @00000701
		Press c or to start console mode
		..c

	3. To interrupt the boot sequence, press 'c' in 3 seconds after powered-on to enter console mode

	4. CLI commands in AppLoader
		help						# list all supported commands
		reset						# trigger software reset to CPU
		fs							# file system related commands
			fs info					# list partition info if file system is valid
			fs format				# create default partition table
		kermit						# download application to flash through kermit file transfer
			kermit [fs/flash/ram] [partition/flashAddr/ramAddr]

	5. Download APP firmware
		For demo set, AppLoader supports only boot to application at offset 0 of partition 1. Please make sure to create file system
		using the following command before downloading.
			ESMT> fs format 
		
		To do firmware downloading, start kermit receiver first.
			ESMT> kermit fs 1
		
		And then, start kermit file transfer from your terminal softare. For example, if you are using teraterm:
		File ->Transfer ->Kermit ->Send
		Choose WISEDemoApp.bin which is built from WISEDemoApp project.

		After downloading is finished, reset the system to start normal boot.
		
		
## Start function demostration

	1. If demo application is luanched properly, the following text messages should be displayed on terminal.
		========================================
		   ESMT WISE Demo Application V2.00
		   Built@ Jan 23 2025 16:02:24
		========================================

	2. Command: <help> - list all commands supported in demo application
		ESMT> help

    	Usage: help [subcommand]
           help         help
           reset        reboot system
           rf           <rf help> to see all rf commands
           rtc          <rtc help>
           wdt          <wdt help>
           timer        <timer help>
           spi          <spi help>
           i2c          <i2c help>
           pwm          <pwm help>
           pwmslow      <pwmslow help>
           kermit       start kermit receiver
           <C-g>        abort timer

	3. Command: <rf help> - List commands for rf operations
		ESMT> rf help

        Usage: rf [subcommand]
           help         radio function
           init         init radio system
           conf         radio configure
           recv         radio rx on/off
           send         radio tx pkt number
           ch           radio channel number
           txpwr        radio tx power
           rxlog        show rx data
           cca          enable rx cca mode
           sing         enable tx single-tone
           filt         radio rx filter enable
           per          radio PER test

## Simply Tx/Rx test
	Prepare 2 demo sets with WISEDemoApp running.
	
	for Rx side:
	1. Command: <rf init 0> - initialize radio interface 0
		ESMT> rf init 0

		PA type : high
		Radio config:
		   modulation: GFSK
		   base freq: 915500000
		   channel number: 37
		   ch spacing: 200000
		   deviation: 100000
		   data rate: 500K
		   sync length: 4
		   sync1: 667df05a
		   sync2: 00000000
		   preamble len: 4
		   preamble: aaaaaaaa
		   max frame len: 256
		   hw preamble: on
		   hw syncword: on
		   hw crc: on
		   hw whitning: off
		   current ch: 0 915500000hz
		   recv off

	2. Command: <rf ch 10> - select a predefined RF channel, example: channel 10
		ESMT> rf ch 10

		Set channel 10 freq=917500000

	3. Command: <rf recv on> - Start Rx
		ESMT> rf recv on

		radio rx on
	

	for Tx side:
	1. Command: <rf init 0> - initialize radio interface 0
		ESMT> rf init 0

		PA type : high
		Radio config:
		   modulation: GFSK
		   base freq: 915500000
		   channel number: 37
		   ch spacing: 200000
		   deviation: 100000
		   data rate: 500K
		   sync length: 4
		   sync1: 667df05a
		   sync2: 00000000
		   preamble len: 4
		   preamble: aaaaaaaa
		   max frame len: 256
		   hw preamble: on
		   hw syncword: on
		   hw crc: on
		   hw whitning: off
		   current ch: 0 915500000hz
		   recv off

	2. Command: <rf ch 10> - select a predefined RF channel, should be the same as Tx side
		ESMT> rf ch 10

		Set channel 10 freq=917500000

	3. Command: <rf send 5> - Start tx 5 packets.
		ESMT> rf send 5

		test cnt=0
		ch=10 Tx len = 25
		0000: 41 c8 00 cd ab ff ff 02    00 00 ab aa 00 00 00 01
		0010: 02 03 04 05 06 07 08 09    0a

		test cnt=1
		ch=10 Tx len = 25
		0000: 41 c8 00 cd ab ff ff 02    00 00 ab aa 00 00 00 01
		0010: 02 03 04 05 06 07 08 09    0a

		test cnt=2
		ch=10 Tx len = 25
		0000: 41 c8 00 cd ab ff ff 02    00 00 ab aa 00 00 00 01
		0010: 02 03 04 05 06 07 08 09    0a

		test cnt=3
		ch=10 Tx len = 25
		0000: 41 c8 00 cd ab ff ff 02    00 00 ab aa 00 00 00 01
		0010: 02 03 04 05 06 07 08 09    0a

		test cnt=4
		ch=10 Tx len = 25
		0000: 41 c8 00 cd ab ff ff 02    00 00 ab aa 00 00 00 01
		0010: 02 03 04 05 06 07 08 09    0a

		tx pkt 5 end

	4. Radio packets should be received in Rx side
		Good/Err CNT = 1/0
		recv len 27 buf=20001651 rssi=-71
		Good/Err CNT = 2/0
		recv len 27 buf=20001771 rssi=-71
		Good/Err CNT = 3/0
		recv len 27 buf=20001891 rssi=-71
		Good/Err CNT = 4/0
		recv len 27 buf=200019b1 rssi=-71
		Good/Err CNT = 5/0
		recv len 27 buf=20001ad1 rssi=-72

	5. for more information about radio configuration, please refer to func_rf.c

## Radio Packet Error Rate (PER) test
    Prepare 2 demo sets with WISEDemoApp running.
	
	for Rx side:
	1. Command: <rf init 0> - initialize radio interface 0
		ESMT> rf init 0

		PA type : high
		Radio config:
		   modulation: GFSK
		   base freq: 915500000
		   channel number: 37
		   ch spacing: 200000
		   deviation: 100000
		   data rate: 500K
		   sync length: 4
		   sync1: 667df05a
		   sync2: 00000000
		   preamble len: 4
		   preamble: aaaaaaaa
		   max frame len: 256
		   hw preamble: on
		   hw syncword: on
		   hw crc: on
		   hw whitning: off
		   current ch: 0 915500000hz
		   recv off
    
    2. Command: <rf per rx 1000> - start PER test receiver for 1000 packets
        ESMT> rf per rx 1000

        Start PER test receiver testCount=1000
        Press any key to terminate
    
    for Tx side:
	1. Command: <rf init 0> - initialize radio interface 0
		ESMT> rf init 0

		PA type : high
		Radio config:
		   modulation: GFSK
		   base freq: 915500000
		   channel number: 37
		   ch spacing: 200000
		   deviation: 100000
		   data rate: 500K
		   sync length: 4
		   sync1: 667df05a
		   sync2: 00000000
		   preamble len: 4
		   preamble: aaaaaaaa
		   max frame len: 256
		   hw preamble: on
		   hw syncword: on    
		   hw crc: on
		   hw whitning: off
		   current ch: 0 915500000hz
		   recv off
    
    2. Command: <rf per tx 1000> - start PER test sender for 1000 packets
        ESMT> rf per tx 1000

        Start PER test sender testCount=1000
        
    test result:
        Once the test is finished, the result should be printed in terminal of 
        receiver side.
        
        PER test result:
        1000/1000 packets received
        Average rssi: -71
        PER: 0.00%
        
## Autobuild script in DOS command line
    1. To use auto_build.bat in DOS command line, copy the file auto_build.bat 
        from wise_sdk to your working folder first.
    2. Modify the following variables to the corresponding path in your working
        environment.
        ESMT_TOOL_PATH - root directory of eclipse-embedcpp-esmt
        ECLIPSE_PATH - root directory of Eclipse
        TOOL_CHAIN_PATH - path of bin in ARM toolchain
        BUILD_TOOL_PATH - path of bin in build tools
    3. Run auto_build.bat in DOS command line.
    
