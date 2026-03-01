#include <stdint.h>
#include "fmt_print.h"
#include "wise_sys_api.h"
#include "wise_radio_api.h"
#include "wise_gpio_api.h"
const char *const modString[]     = {"BPSK", "QPSK", "OQPSK", "FSK", "GFSK", "BPSK RAMP"};
const char *const drString[]      = {"4.8 Kbps", "12.5 Kbps", "32.768 Kbps", "50 Kbps", "100 Kbps", "125 Kbps", "200 Kbps", "250 Kbps", "500 Kbps", "1 Mbps", "2 Mbps"};
const char *const onOffString[]   = {"OFF", "ON"};
const char *const paString[]      = {"Low voltage(10dBm)", "High voltage(14dBm)"};
const char *const matchString[]   = {"915MHz", "868MHz", "490MHz"};
const char *const operateString[] = {"Transparent", "IEEE 802.15.4", "BLE", "WMBUS"};
const char *const fmtLenTypeString[] = {"Fixed", "Variable"};
const char *const wmbus_modeString[] = {"S", "T", "C", "R", "F"};
const char *const wmbus_roleString[] = {"Other", "Meter"};
const char *const wmbus_actString[] = {"RX", "TX"};
const char *const symbolCodingString[] = {"None", "NRZ", "MANCHESTER", "3of6"};

#define dbg_out(...) debug_print(__VA_ARGS__)
#define DBG_TOTAL 54 // Max Length
#define CORNER_COLS 1
#define CORNER "+"
#define H "-" // horizontal
#define PAD_L "| "
#define PAD_R " |"
#define PAD_L_COLS 2
#define PAD_R_COLS 2
#define INNER_COLS (DBG_TOTAL - PAD_L_COLS - PAD_R_COLS)

static char fbuf[32];

extern uint16_t util_crc16_get_poly(uint8_t crcType);

static void putn(const char *unit, int n)
{
    for (int i = 0; i < n; ++i) {
        dbg_out("%s", unit);
    }
}

static void dbg_line(const char *content)
{
    char fmt[16], buf[256];
    snprintf(fmt, sizeof(fmt), "%%-%d.%ds", INNER_COLS, INNER_COLS);
    snprintf(buf, sizeof(buf), fmt, content);
    dbg_out("%s%s%s\n", PAD_L, buf, PAD_R);
}

void dbg_box_begin(const char *title)
{
    int inner      = DBG_TOTAL - 2;
    int title_cols = (int)strlen(title) + 2;
    if (title_cols > inner) {
        title_cols = inner;
    }

    int left  = (inner - title_cols) / 2;
    int right = inner - title_cols - left;

    dbg_out("%s", CORNER);
    putn(H, left);
    dbg_out(" %.*s ", title_cols >= 2 ? title_cols - 2 : 0, title);
    putn(H, right);
    dbg_out("%s\n", CORNER);
}

void dbg_box_end(void)
{
    dbg_out("%s", CORNER);
    putn(H, DBG_TOTAL - 2 * CORNER_COLS);
    dbg_out("%s\n", CORNER);
}

void dbg_section(const char *title)
{
    char buf[200];
    snprintf(buf, sizeof(buf), "[ %s ]", title);
    dbg_line(buf);
}

void dbg_field(const char *label, const char *value)
{
    char line[256];
    snprintf(line, sizeof(line), ". %-22s: %s", label, value);
    dbg_line(line);
}

const char *fmt_int(int v)
{
    snprintf(fbuf, sizeof(fbuf), "%d", v);
    return fbuf;
}

const char *fmt_hz_u32(uint32_t hz)
{
    if (hz >= 1000000U) {
        uint32_t mhz  = hz / 1000000U;
        uint32_t frac = (hz % 1000000U) / 1000U;
        snprintf(fbuf, sizeof(fbuf), "%lu.%03lu MHz", mhz, frac);
    } else if (hz >= 1000U) {
        uint32_t khz  = hz / 1000U;
        uint32_t frac = hz % 1000U;
        snprintf(fbuf, sizeof(fbuf), "%lu.%03lu kHz", khz, frac);
    } else {
        snprintf(fbuf, sizeof(fbuf), "%lu Hz", hz);
    }
    return fbuf;
}

const char *fmt_bps_u32(uint32_t bps)
{
    if (bps >= 1000000U) {
        uint32_t mbps = bps / 1000000U;
        uint32_t frac = (bps % 1000000U) / 1000U;
        snprintf(fbuf, sizeof(fbuf), "%lu.%03lu Mbps", mbps, frac);
    } else if (bps >= 1000U) {
        uint32_t kbps = bps / 1000U;
        uint32_t frac = bps % 1000U;
        snprintf(fbuf, sizeof(fbuf), "%lu.%03lu kbps", kbps, frac);
    } else {
        snprintf(fbuf, sizeof(fbuf), "%lu bps", bps);
    }
    return fbuf;
}

const char *fmt_bytes_u32(uint32_t n)
{
    snprintf(fbuf, sizeof(fbuf), "%lu bytes", n);
    return fbuf;
}

const char *fmt_hex8(uint8_t v)
{
    snprintf(fbuf, sizeof(fbuf), "0x%02X", v);
    return fbuf;
}

const char *fmt_hex32(uint32_t v)
{
    snprintf(fbuf, sizeof(fbuf), "0x%08lX", v);
    return fbuf;
}

const char *fmt_hex16(uint16_t v)
{
    snprintf(fbuf, sizeof(fbuf), "0x%04X", v);
    return fbuf;
}

void fmt_radio_debug(int8_t radioIntf, uint8_t actChIdx, uint8_t rxOn, const WISE_RADIO_CFG_T *cfg, const WISE_RADIO_PKT_FMT_T *pktFmt)
{
    uint32_t freq = 0;

    wise_radio_get_channel_freq(radioIntf, actChIdx, &freq);

    /* Board */
    dbg_box_begin("Board INFO");
    dbg_field("Platform", CHIP_SOC_UNIQUE);
    dbg_field("CHIP ID", fmt_hex32(wise_sys_get_chip_id()));
    dbg_field("PA type", paString[wise_sys_get_pa_type()]);
    dbg_field("Matching type", matchString[wise_sys_get_board_match_type()]);
    dbg_box_end(); // Board

    /* RADIO */
    dbg_box_begin("RADIO Configurations");
    /* RF-PHY */
    dbg_section("PHY Setting");
    //dbg_field("Operate Mode", operateString[cfg->phy_mode]);
    dbg_field("Modulation", modString[cfg->modulation]);
    dbg_field("Base Freq", fmt_hz_u32(cfg->ch_freq_min));
    dbg_field("Channel Num", fmt_int(wise_radio_get_channel_num(radioIntf)));
    dbg_field("Chan Spacing", fmt_hz_u32(cfg->ch_spacing));
    dbg_field("Deviation", fmt_hz_u32(cfg->deviation));
    dbg_field("Data Rate", drString[cfg->data_rate]);
    /* RF-SYNC */
    dbg_section("Sync Format");
    dbg_field("Preamble Len", fmt_bytes_u32(cfg->preamble_len));
    dbg_field("Preamble Pat", fmt_hex8(cfg->preamble));
    dbg_field("Sync Len", fmt_bytes_u32(cfg->sync_word_len));
    dbg_field("Sync Word", fmt_hex32(cfg->syncword));

    /* RF-PACKET */
    if(pktFmt)
    {
        dbg_section("Packet Format");
        dbg_field("Length Config", fmtLenTypeString[pktFmt->pkt_type]);
        dbg_field("MAX Frame Len", fmt_bytes_u32(pktFmt->max_pkt_length));
        dbg_field("HW Preamble", onOffString[!!(pktFmt->frame_format & FRAME_FMT_PREAMBLE_EN)]);
        dbg_field("HW Sync", onOffString[!!(pktFmt->frame_format & FRAME_FMT_SYNCWORD_EN)]);
        dbg_field("Whitening", onOffString[!!(pktFmt->frame_format & FRAME_FMT_WHITENING_EN)]);
        dbg_field("FEC", onOffString[!!(pktFmt->frame_format & FRAME_FMT_FEC_EN)]);
        //dbg_field("Manchester", onOffString[!!(pktFmt->frame_format & FRAME_FMT_MANCH_EN)]);
        dbg_field("Symbol Coding", symbolCodingString[pktFmt->frame_codec]);
        dbg_field("Bit order", (pktFmt->frame_format & FRAME_FMT_MSB_FIRST) ? "MSB first" : "LSB first");
        dbg_field("HW CRC", onOffString[!!(pktFmt->frame_format & FRAME_FMT_CRC_EN)]);
        if(!!(pktFmt->frame_format & FRAME_FMT_CRC_EN)) {
            dbg_section("CRC SETTING");
            uint32_t cfg = pktFmt->crc.crc_config;
            dbg_field("Input bit order",(cfg  & CRC_INPUT_BIT_ENDIAN_MSB_FIRST) ? "MSB_FIRST" : "LSB_FIRST");
            dbg_field("Output bit order",(cfg  & CRC_OUTPUT_BIT_ENDIAN_MSB_FIRST) ? "MSB_FIRST" : "LSB_FIRST");
            dbg_field("Output byte order",(cfg  & CRC_OUTPUT_BYTE_ENDIAN_MSB_FIRST) ? "MSB_FIRST" : "LSB_FIRST");
            dbg_field("CRC includes header",onOffString[!!(cfg  & CRC_INCLUDE_HEADER_ON)]);
            dbg_field("Invert",onOffString[!!(cfg & CRC_INVERT_ON) ]);
            
            dbg_field("poly",fmt_hex16(util_crc16_get_poly(pktFmt->crc.crc_poly_sel)));
            dbg_field("seed",fmt_hex16(pktFmt->crc.crc_seed));
        }
    }
    
    /* RF-RUNTIME */
    dbg_section("Runtime State");
    dbg_field("Current Ch", fmt_int(actChIdx));
    dbg_field("Current Freq", fmt_hz_u32(freq));
    dbg_field("RX RECV", onOffString[rxOn]);
    dbg_box_end(); // RADIO
}

const char *gpio_pin_func_to_str(uint8_t func)
{
    switch (func) {
    case MODE_PIO_FUNC_GPIO:      return "GPIO";
    case MODE_PIO_FUNC_UART0_TX:  return "UART0_TX";
    case MODE_PIO_FUNC_UART0_RX:  return "UART0_RX";
    case MODE_PIO_FUNC_UART0_RTS: return "UART0_RTS";
    case MODE_PIO_FUNC_UART0_CTS: return "UART0_CTS";
    case MODE_PIO_FUNC_UART1_TX:  return "UART1_TX";
    case MODE_PIO_FUNC_UART1_RX:  return "UART1_RX";
    case MODE_PIO_FUNC_UART1_RTS: return "UART1_RTS";
    case MODE_PIO_FUNC_UART1_CTS: return "UART1_CTS";
    case MODE_PIO_FUNC_UART2_TX:  return "UART2_TX";
    case MODE_PIO_FUNC_UART2_RX:  return "UART2_RX";
    case MODE_PIO_FUNC_UART2_RTS: return "UART2_RTS";
    case MODE_PIO_FUNC_UART2_CTS: return "UART2_CTS";
    case MODE_PIO_FUNC_SPI0_CLK:  return "SPI0_CLK";
    case MODE_PIO_FUNC_SPI0_CS:   return "SPI0_CS";
    case MODE_PIO_FUNC_SPI0_MOSI: return "SPI0_MOSI";
    case MODE_PIO_FUNC_SPI0_MISO: return "SPI0_MISO";
    case MODE_PIO_FUNC_SPI0_HOLD: return "SPI0_HOLD";
    case MODE_PIO_FUNC_SPI0_WP:   return "SPI0_WP";
    case MODE_PIO_FUNC_SPI1_CLK:  return "SPI1_CLK";
    case MODE_PIO_FUNC_SPI1_CS:   return "SPI1_CS";
    case MODE_PIO_FUNC_SPI1_MOSI: return "SPI1_MOSI";
    case MODE_PIO_FUNC_SPI1_MISO: return "SPI1_MISO";
    case MODE_PIO_FUNC_SPI1_HOLD: return "SPI1_HOLD";
    case MODE_PIO_FUNC_SPI1_WP:   return "SPI1_WP";
    case MODE_PIO_FUNC_I2C0_SCL:  return "I2C0_SCL";
    case MODE_PIO_FUNC_I2C0_SDA:  return "I2C0_SDA";
    case MODE_PIO_FUNC_I2C1_SCL:  return "I2C1_SCL";
    case MODE_PIO_FUNC_I2C1_SDA:  return "I2C1_SDA";
    default:                      return "UNKNOWN";
    }
}

