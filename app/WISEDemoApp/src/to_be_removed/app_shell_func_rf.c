/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "app_shell_func_rf.h"

#include "api/wise_gptmr_api.h"
#include "radio_lib/wise_radio_api.h"
#include "api/wise_sys_api.h"

#define PER_TEST_DATA_LEN 24

#ifndef M2C_RADIO_CONFIG
#define M2C_RADIO_CONFIG 0
#endif

#define RADIO_TEST_BUF_LEN 256
#define RADIO_BUFFER_LEN 2048

const char *modString[] = {"BPSK", "QPSK", "OQPSK", "FSK", "GFSK", "BPSK RAMP"};
const char *drString[]  = {"12.5", "50",  "100",  "125", "200",
                           "250",  "500", "1000", "2000"};
const char *onOffString[] = {"off", "on"};

const int8_t defRadioIntf = 0;

uint8_t actChannel       = 20;
uint8_t radioInited      = 0;
uint8_t radioMode        = 0;
uint8_t radioConfigured  = 0;
uint8_t txPwrLevel       = 127;
uint32_t rxErrCount      = 0;
uint32_t rxGoodCount     = 0;
uint8_t rxStarted        = 0;
uint8_t rxLog            = 0;
uint16_t radioPANID      = 0xABCD;
uint16_t radioShortAddr  = 0xFFFF;
uint8_t radioLonfAddr[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

uint8_t radioTestBuf[RADIO_TEST_BUF_LEN];

int8_t periodicTxSchId;
int8_t rxSchId = -1;

uint8_t radioBufferPool[RADIO_BUFFER_LEN];
WISE_RADIO_BUFFER_T radioBuffer = {RADIO_BUFFER_LEN, (uint32_t)radioBufferPool};

#if M2C_RADIO_CONFIG
WISE_RADIO_CFG_T radioCfg = {
    .modulation = E_MOD_TYPE_GFSK,

    //channel configuration
    .ch_freq_min = 902750000, //Hz
    .ch_freq_max = 934500000, //Hz
    .ch_spacing  = 250000,    //Hz

    //modem configuration
    .deviation    = 180000, //Hz
    .data_rate    = E_DATA_RATE_500K,
    .preamble     = 0xAAAAAAAA, //pattern
    .preamble_len = 4,          //bytes

    .sync_word_len = 4,
    .syncword1     = 0x5475C52A, //will be send out MSB first
    .syncword2     = 0,

    .phy_mode = E_PHY_TRANSPARENT,
};

WISE_RADIO_PKT_FMT_T radioPktFmt = {
    .pktType = PKT_VARIABLE_LENGTH,
    .frameFormat =
        (FRAME_FMT_PREAMBLE_EN | FRAME_FMT_SYNCWORD_EN | FRAME_FMT_HEADER_EN |
         FRAME_FMT_CRC_EN | FRAME_FMT_MSB_FIRST),

    .maxPktLength = 256,

    //frame header configure
    .hdrBytes        = 2, //
    .hdrConfig       = (PHR_BYTE_ENDIAN_MSB_FIRST),
    .lengthBitSize   = 8, //8~12 bits
    .lengthBitOffset = 0, //bit offset in header

    //crc configure
    .crcConfig =
        (CRC_INPUT_BIT_ENDIAN_MSB_FIRST | CRC_OUTPUT_BYTE_ENDIAN_MSB_FIRST |
         CRC_OUTPUT_BIT_ENDIAN_MSB_FIRST | CRC_INCLUDE_HEADER_ON),
    .crcPolySel = CRC_POLYNOMIAL_CCITT_16,
    .crcSeed    = 0x1D0F,
};
#else
WISE_RADIO_CFG_T radioCfg = {
    .modulation = E_MOD_TYPE_GFSK,

    //channel configuration
    .ch_freq_min = 915500000, //Hz
    .ch_freq_max = 922800000, //Hz
    .ch_spacing  = 200000,    //Hz

    //modem configuration
    .deviation    = 100000, //Hz
    .data_rate    = E_DATA_RATE_500K,
    .preamble     = 0xAAAAAAAA, //pattern
    .preamble_len = 4,          //bytes

    .sync_word_len = 4,
    .syncword1     = 0xD8EC6B99, //will be send out MSB first
    .syncword2     = 0,

    .phy_mode = E_PHY_TRANSPARENT,
};

WISE_RADIO_PKT_FMT_T radioPktFmt = {
    .pkt_type          = PKT_VARIABLE_LENGTH,
    .hdr_bytes         = 1, //
    .max_pkt_length    = RADIO_MAX_FRAME_LEN,
    .length_bit_size   = 8, // 8~12 bits
    .length_bit_offset = 0, // bit offset in header
    .frame_format      = (FRAME_FMT_PREAMBLE_EN |
                          FRAME_FMT_SYNCWORD_EN |
                          FRAME_FMT_HEADER_EN |
                          /* FRAME_FMT_WHITENING_EN |*/
                          FRAME_FMT_CRC_EN),

    .hdr_config        = 0,
    .crc_config        = CRC_INPUT_BIT_ENDIAN_MSB_FIRST |
                         CRC_OUTPUT_BIT_ENDIAN_MSB_FIRST |
                         CRC_OUTPUT_BYTE_ENDIAN_MSB_FIRST |
                         CRC_INVERT,
    .crc_poly_sel      = CRC_POLYNOMIAL_CCITT_16,
    .crc_seed          = 0,
};
#endif

static void radioEventCb(WISE_RADIO_EVT_T evt);

void radioDebug(void)
{
    uint32_t freq = 0;

    wise_radio_get_channel_freq(defRadioIntf, actChannel, &freq);

    debug_print("Radio config:\n");
    if (radioMode == 0) {
        debug_print("   modulation: %s\n", modString[radioCfg.modulation]);
        debug_print("   base freq: %lu\n", radioCfg.ch_freq_min);
        debug_print("   channel number: %d\n",
                    wise_radio_get_channel_num(defRadioIntf));
        debug_print("   ch spacing: %lu\n", radioCfg.ch_spacing);
        debug_print("   deviation: %lu\n", radioCfg.deviation);
        debug_print("   data rate: %sK\n", drString[radioCfg.data_rate]);
        debug_print("   sync length: %d\n", radioCfg.sync_word_len);
        debug_print("   sync1: %08lx\n", radioCfg.syncword1);
        debug_print("   sync2: %08lx\n", radioCfg.syncword2);
        debug_print("   preamble len: %d\n", radioCfg.preamble_len);
        debug_print("   preamble: %02lx\n", radioCfg.preamble);
        debug_print("   max frame len: %d\n", radioPktFmt.max_pkt_length);
        debug_print(
            "   hw preamble: %s\n",
            onOffString[(radioPktFmt.frame_format & FRAME_FMT_PREAMBLE_EN ? 1
                                                                       : 0)]);
        debug_print(
            "   hw syncword: %s\n",
            onOffString[(radioPktFmt.frame_format & FRAME_FMT_SYNCWORD_EN ? 1
                                                                       : 0)]);
        debug_print(
            "   hw crc: %s\n",
            onOffString[(radioPktFmt.frame_format & FRAME_FMT_CRC_EN ? 1 : 0)]);
        debug_print(
            "   hw whitning: %s\n",
            onOffString[(radioPktFmt.frame_format & FRAME_FMT_WHITENING_EN ? 1
                                                                        : 0)]);
        debug_print("   current ch: %d %luhz\n", actChannel, freq);
        debug_print("   recv %s\n",
                    rxStarted ? onOffString[1] : onOffString[0]);
    } else if (radioMode == 1) {
        debug_print("   IEEE 802.15.4 mode\n");
        debug_print("   channel number: %d\n",
                    wise_radio_get_channel_num(defRadioIntf));
        debug_print("   current ch: %p %luhz\n", (void *)actChannel, freq);
        debug_print("   recv %s\n",
                    rxStarted ? onOffString[1] : onOffString[0]);
    }
}

void radioInit(void)
{
    if (!radioInited) {
        init_rand(wise_tick_get_counter());

        wise_radio_init(defRadioIntf);
        wise_radio_set_evt_callback(defRadioIntf, radioEventCb);
        wise_radio_set_buffer(defRadioIntf, &radioBuffer);
        wise_radio_config(defRadioIntf, &radioCfg, &radioPktFmt);
        wise_radio_set_tx_pwr(defRadioIntf, txPwrLevel);
        wise_radio_set_tx_io_mode(defRadioIntf, CORE_IO_BLOCKING);

        radioInited = 1;
    }

    radioMode       = 0;
    radioConfigured = 1;

    radioDebug();
}

void radioInit802154(void)
{
    if (!radioInited) {
        init_rand(wise_tick_get_counter());

        wise_radio_ieee802154_init(defRadioIntf);
        wise_radio_set_evt_callback(defRadioIntf, radioEventCb);

        wise_radio_set_tx_pwr(defRadioIntf, txPwrLevel);
        wise_radio_set_tx_io_mode(defRadioIntf, CORE_IO_BLOCKING);

        radioInited = 1;
    }

    radioMode       = 1;
    radioConfigured = 1;

    radioDebug();
}

void radioEventCb(WISE_RADIO_EVT_T evt)
{
#if 0
	if (evt & WISE_RADIO_EVT_RX_FRAME) {
		WISE_RX_META_T meta;
		uint32_t bufAddr = 0;

		if (CORE_STATUS_SUCCESS == wise_radio_get_rx_frame_info(defRadioIntf, &bufAddr, &meta)) {
			rxGoodCount++;

			debug_print("Rx len=%d rssi=%d, Good/Err CNT = %ld/%ld\n", meta.dataLen, meta.rssi, rxGoodCount, rxErrCount);
		} 
		else
		{
			debug_print("rx read buffer fail\n");
        }
        
		wise_radio_release_rx_frame(defRadioIntf);
		
		if (rxLog == 1) {
			debug_print("timestamp=%ld\n", meta.timestamp);
			dump_buffer((unsigned char *)bufAddr, meta.dataLen);
		}
	}

	if (evt & WISE_RADIO_EVT_RX_ERR) {
		WISE_RX_META_T meta;
		uint32_t bufAddr = 0;
	
		if (CORE_STATUS_SUCCESS != wise_radio_get_rx_frame_info(defRadioIntf, &bufAddr, &meta)) {
			debug_print("rx read buffer fail\n");
		}

		rxErrCount++;
		wise_radio_release_rx_frame(defRadioIntf);
		debug_print("Good/Err CNT = %ld/%ld\n", rxGoodCount, rxErrCount);
		debug_print("E F\n");
		if (rxLog == 1) {
			debug_print("timestamp=%ld\n", meta.timestamp);
			dump_buffer((unsigned char *)bufAddr, meta.dataLen);
		}
	}
#else
    if ((evt & WISE_RADIO_EVT_RX_FRAME) || (evt & WISE_RADIO_EVT_RX_ERR)) {
        WISE_RX_META_T meta;
        uint32_t bufAddr = 0;
        uint8_t validRx  = 0;

        if (WISE_SUCCESS ==
            wise_radio_get_rx_frame_info(defRadioIntf, &bufAddr, &meta)) {
            if ((evt & WISE_RADIO_EVT_RX_FRAME) && meta.valid) {
                validRx = 1;
            } else {
                validRx = 0;
            }
        } else {
            debug_print("rx read buffer fail\n");
            validRx = 0;
        }

        wise_radio_release_rx_frame(defRadioIntf);

        if (validRx) {
            rxGoodCount++;
            debug_print("Rx len=%d rssi=%d, Good/Err CNT = %ld/%ld\n",
                        meta.data_len, meta.rssi, rxGoodCount, rxErrCount);
        } else {
            rxErrCount++;
            debug_print("Invalid rx, Good/Err CNT = %ld/%ld\n", rxGoodCount,
                        rxErrCount);
        }

        if (rxLog == 1) {
            debug_print("timestamp=%ld\n", meta.timestamp);
            dump_buffer((unsigned char *)bufAddr, meta.data_len);
        }
    }
#endif
}

void radioRxOn(void)
{
    rxGoodCount = 0;
    rxErrCount  = 0;
    wise_radio_rx_start(defRadioIntf, actChannel, RADIO_RX_CONTINUOUS);
    rxStarted = 1;
}

void radioRxOff(void)
{
    wise_radio_rx_stop(defRadioIntf);
    rxStarted = 0;
}

uint8_t isRadioConfigured(void)
{
    return radioConfigured;
}

uint8_t isRxOn(void)
{
    return rxStarted;
}

void periodicTxProc(uint32_t _testCount)
{
    uint32_t testNum           = _testCount;
    static uint32_t testCount  = 0;
    int testLen                = 0;
    static uint8_t testPattern = 0;
    uint8_t maxLen             = 240;

    if (radioMode == 1) {
        maxLen = 120;
    }

    while (testCount != testNum) {
        if (testCount == 0) {
            testPattern = 0;
        }

        debug_print("test cnt=%ld \n", testCount);

        do {
            testLen = rand_cmwc() % maxLen;
        } while (testLen == 0);

        radioTestTxFrame(testLen, testPattern);
        wise_tick_delay_ms(10);
        testPattern += testLen;
        testCount++;
    }
    testCount = 0;
}

uint8_t isTxProcEnd(void)
{
    return periodicTxSchId < 0 ? 1 : 0;
}

void radioTestSend(int testCount)
{
    if (testCount <= 1) {
        radioTestTxFrame(25, 0);
    } else {
        periodicTxProc(testCount);
    }
}

void radioTestTxFrame(int testTxLen, uint8_t patternStart)
{
    int i;

#if M2C_RADIO_CONFIG
    radioTestBuf[0] = bit_reverse(testTxLen & 0xff, 1);
    radioTestBuf[1] = testTxLen & 0xff;

    for (i = 0; i < testTxLen; i++) {
        radioTestBuf[2 + i] = patternStart++ & 0xff;
    }

    testTxLen += 2;
#else
    if (radioMode == 0) {
        radioTestBuf[0] = testTxLen; //bit_reverse(testTxLen & 0xff, 1);
        for (i = 0; i < testTxLen; i++) {
            radioTestBuf[i + 1] = patternStart++ & 0xff;
        }

        testTxLen++;
    } else {
        radioTestBuf[0] = 0;
        radioTestBuf[1] = 0;

        for (i = 2; i < testTxLen; i++) {
            radioTestBuf[i] = patternStart++ & 0xff;
        }
    }
#endif

    debug_print("ch=%d Tx len = %d\n", actChannel, testTxLen);
    if (rxLog) {
        dump_buffer(&radioTestBuf[0], testTxLen);
    }

    wise_radio_tx_frame(defRadioIntf, actChannel, &radioTestBuf[0], testTxLen);
}

void radioSetChannel(uint8_t channelIdx)
{
    uint32_t freq = 0;
    actChannel    = channelIdx;
    wise_radio_get_channel_freq(defRadioIntf, actChannel, &freq);

    if (isRxOn()) {
        wise_radio_rx_stop(defRadioIntf);
        wise_radio_rx_start(defRadioIntf, actChannel, RADIO_RX_CONTINUOUS);
    }
}

uint32_t radioGetFreq(void)
{
    uint32_t freq = 0;
    wise_radio_get_channel_freq(defRadioIntf, actChannel, &freq);
    return freq;
}

uint8_t radioGetChannel(void)
{
    uint32_t freq = 0;

    wise_radio_get_channel_freq(defRadioIntf, actChannel, &freq);
    return actChannel;
}

uint8_t radioGetValidChannelNumber(void)
{
    uint8_t chNum = wise_radio_get_channel_num(defRadioIntf);
    return chNum;
}

uint8_t isInputPwrVaild(uint8_t _inputpwr)
{
    return (_inputpwr <= MAX_TX_PWR_LEVEL) ? 1 : 0;
}

void radioSetTxPwr(uint8_t _inputpwr)
{
    txPwrLevel = _inputpwr;
    wise_radio_set_tx_pwr(defRadioIntf, txPwrLevel);
}

uint8_t radioGetTxPwr(void)
{
    return txPwrLevel;
}

void radioSetRxLog(uint8_t _val)
{
    rxLog = _val;
}

int8_t radioGetRssi()
{
    return wise_radio_get_rssi(defRadioIntf);
}

void radioSingleToneOn(uint8_t pwrLevel)
{
    wise_radio_enable_singletone(defRadioIntf, pwrLevel, ENABLE);
}

void radioSingleToneOff()
{
    wise_radio_enable_singletone(defRadioIntf, 0, DISABLE);
}

void setPanId(uint16_t _panid)
{
    radioPANID = _panid;
    wise_radio_ieee802154_set_pan_id(defRadioIntf, _panid);
}

void setShortAddress(uint16_t _saddr)
{
    radioShortAddr = _saddr;
    wise_radio_ieee802154_set_short_address(defRadioIntf, _saddr);
}

void setLongAddress(uint8_t *_laddr)
{
    memcpy(radioLonfAddr, _laddr, sizeof(radioLonfAddr));
    wise_radio_ieee802154_set_long_address(defRadioIntf, radioLonfAddr);
}

uint8_t perRxExpect   = 0;
int32_t rssiAggregate = 0;
void radioPEREventCb(WISE_RADIO_EVT_T evt)
{
    if ((evt & WISE_RADIO_EVT_RX_FRAME) || (evt & WISE_RADIO_EVT_RX_ERR)) {
        WISE_RX_META_T meta;
        uint32_t bufAddr = 0;

        if (WISE_SUCCESS ==
            wise_radio_get_rx_frame_info(defRadioIntf, &bufAddr, &meta)) {
            uint8_t *pkt = (uint8_t *)bufAddr;

            if ((evt & WISE_RADIO_EVT_RX_FRAME) && meta.valid) {
                rxGoodCount++;
                rssiAggregate += meta.rssi;

                if (pkt[3] == perRxExpect) {
                    debug_print("<");
                } else {
                    debug_print("X");
                }

                perRxExpect = pkt[3] + 1;
            } else {
                rxErrCount++;
            }
        } else {
            debug_print("rx read buffer fail\n");
        }

        wise_radio_release_rx_frame(defRadioIntf);
    }
}

uint32_t perCalc(uint32_t rxCount, uint32_t testCount)
{
    uint32_t lost = testCount - rxCount;

    return ((uint32_t)(lost * 10000) / testCount);
}

void startPERTest(uint8_t role, uint32_t testCount)
{
    int i, j;

    if (!radioConfigured) {
        debug_print("radio function not inited\n");
        return;
    }

    if (role == 0) {
        uint32_t prevGoodCount   = 0;
        uint32_t lastRxTimestamp = 0;
        uint32_t per             = 0;

        //receiver
        rxGoodCount   = 0;
        rxErrCount    = 0;
        perRxExpect   = 0;
        rssiAggregate = 0;

        wise_radio_set_evt_callback(defRadioIntf, radioPEREventCb);
        wise_radio_rx_start(defRadioIntf, actChannel, RADIO_RX_CONTINUOUS);

        debug_print("Press any key to terminate\n");
        while (1) {
            uint8_t inputC;

            if (WISE_SUCCESS == wise_uart_read_char(STDIO_UART_PORT, &inputC)) {
                break;
            }

            if (rxGoodCount == testCount) {
                break;
            }

            if (rxGoodCount > 0) {
                if (prevGoodCount == rxGoodCount) {
                    uint32_t blankTime = wise_get_tu() - lastRxTimestamp;

                    if (blankTime >= MS_TO_SCHLR_UNIT(2000)) {
                        debug_print("\nrx time out\n");
                        break;
                    }
                } else {
                    prevGoodCount   = rxGoodCount;
                    lastRxTimestamp = wise_get_tu();
                }
            }
        }

        wise_radio_rx_stop(defRadioIntf);
        wise_radio_set_evt_callback(defRadioIntf, radioEventCb);

        per = perCalc(rxGoodCount, testCount);

        debug_print("\nPER test result:\n");
        debug_print("    %d/%d packets received\n", rxGoodCount, testCount);
        debug_print("    Average rssi: %d\n",
                    ((int)rssiAggregate / (int)rxGoodCount));
        debug_print("    PER: %d.%02d%%\n", per / 100, per % 100);
    } else if (role == 1) {
        uint8_t *pPayload = NULL;
        uint16_t txLen    = 0;

#if M2C_RADIO_CONFIG
        radioTestBuf[0] = bit_reverse(PER_TEST_DATA_LEN & 0xff, 1);
        radioTestBuf[1] = PER_TEST_DATA_LEN & 0xff;
        pPayload        = &radioTestBuf[2];

        txLen = PER_TEST_DATA_LEN + 2;
#else
        radioTestBuf[0] = PER_TEST_DATA_LEN & 0xff;
        pPayload        = &radioTestBuf[1];

        txLen = PER_TEST_DATA_LEN + 1;
#endif

        //sender
        for (i = 0; i < testCount; i++) {
            for (j = 0; j < PER_TEST_DATA_LEN; j++) {
                pPayload[j] = i & 0xff;
            }

            wise_radio_tx_frame(defRadioIntf, actChannel, &radioTestBuf[0],
                                txLen);
            debug_print(">");
            wise_tick_delay_ms(5);
        }
        debug_print("\nfinished\n");
    }
}
