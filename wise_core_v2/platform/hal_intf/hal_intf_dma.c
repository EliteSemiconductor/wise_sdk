/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "hal_intf_dma.h"
#ifdef CHIP_DMA_SUPPORT_PERIPHERAL
#include <stdint.h>
#include "util.h"
#include "types.h"
#include "util_debug_log.h"
/* ========================================================================== */
/*  Globals & Tables                                                          */
/* ========================================================================== */

static HAL_DMA_CHANNEL_STATE_T dma_channels[CHIP_DMA_CHANNEL_NUM];
static const HAL_DMA_SRC_REQ_SEL_T *dma_channel_func_map = NULL;

HAL_DMA_FUNC_MAP_T hal_dma_func_map;

static const char *dma_func_name_table[HAL_DMA_FUNC_MAX] = {
    [HAL_DMA_UNSED] = "UNUSED",      [HAL_DMA_MAC_TX] = "MAC_TX",     [HAL_DMA_MAC_RX] = "MAC_RX",
    [HAL_DMA_AES_IN] = "AES_IN",     [HAL_DMA_AES_OUT] = "AES_OUT",   [HAL_DMA_AES_AUTH_IN] = "AES_AUTH_IN",
    [HAL_DMA_SHA] = "SHA",           [HAL_DMA_SPI0_TX] = "SPI0_TX",   [HAL_DMA_SPI0_RX] = "SPI0_RX",
    [HAL_DMA_SPI1_TX] = "SPI1_TX",   [HAL_DMA_SPI1_RX] = "SPI1_RX",   [HAL_DMA_UART0_TX] = "UART0_TX",
    [HAL_DMA_UART0_RX] = "UART0_RX", [HAL_DMA_UART1_TX] = "UART1_TX", [HAL_DMA_UART1_RX] = "UART1_RX",
    [HAL_DMA_UART2_TX] = "UART2_TX", [HAL_DMA_UART2_RX] = "UART2_RX", [HAL_DMA_I2C0_TX] = "I2C0_TX",
    [HAL_DMA_I2C0_RX] = "I2C0_RX",   [HAL_DMA_I2C1_TX] = "I2C1_TX",   [HAL_DMA_I2C1_RX] = "I2C1_RX",
    [HAL_DMA_USER] = "USER"};

/* ========================================================================== */
/*  Static Helpers                                                            */
/* ========================================================================== */

static void get_dma_direction_and_address(HAL_DMA_SRC_REQ_SEL_T func, const void *src_addr, void *dst_addr, HAL_DMA_EXT_DIR_T *direction,
                                          uint32_t *addr_out)
{
    if (IS_DMA_EXT_RX_FUNC(func)) {
        *direction = HAL_DMA_EXT_RX;
        *addr_out  = (uint32_t)dst_addr;
    } else {
        *direction = HAL_DMA_EXT_TX;
        *addr_out  = (uint32_t)src_addr;
    }
}

static int32_t get_func_select_value(HAL_DMA_SRC_REQ_SEL_T func)
{
    if (func >= HAL_DMA_MAC_TX && func <= HAL_DMA_SHA) {
        return (int32_t)(func - HAL_DMA_MAC_TX); // 0 ~ 5
    }
    if (func >= HAL_DMA_SPI0_TX && func <= HAL_DMA_UART2_RX) {
        return (int32_t)((func - HAL_DMA_SPI0_TX) + 1); // 1 ~ 10
    }
    if (func >= HAL_DMA_I2C0_TX && func <= HAL_DMA_I2C0_RX) {
        return 11;
    }
    if (func >= HAL_DMA_I2C1_TX && func <= HAL_DMA_I2C1_RX) {
        return 12;
    }
    return HAL_ERR;
}

typedef enum {
    DMA_BUS_SPI  = 0,
    DMA_BUS_I2C  = 1,
    DMA_BUS_UART = 2,
} DMA_BUS_T;

static bool resolve_bus_dma(DMA_BUS_T bus, uint8_t index, bool is_tx, uint8_t *out_channel, HAL_DMA_SRC_REQ_SEL_T *out_func)
{
    if (!out_channel || !out_func) {
        return false;
    }

    switch (bus) {
    case DMA_BUS_SPI:
        if (index == 0) {
            *out_channel = (uint8_t)(is_tx ? hal_dma_func_map.spi0_tx : hal_dma_func_map.spi0_rx);
            *out_func    = (is_tx ? HAL_DMA_SPI0_TX : HAL_DMA_SPI0_RX);
        } else {
            *out_channel = (uint8_t)(is_tx ? hal_dma_func_map.spi1_tx : hal_dma_func_map.spi1_rx);
            *out_func    = (is_tx ? HAL_DMA_SPI1_TX : HAL_DMA_SPI1_RX);
        }
        break;

    case DMA_BUS_I2C:
        if (index == 0) {
            *out_channel = (uint8_t)(is_tx ? hal_dma_func_map.i2c0_tx : hal_dma_func_map.i2c0_rx);
            *out_func    = (is_tx ? HAL_DMA_I2C0_TX : HAL_DMA_I2C0_RX);
        } else {
            *out_channel = (uint8_t)(is_tx ? hal_dma_func_map.i2c1_tx : hal_dma_func_map.i2c1_rx);
            *out_func    = (is_tx ? HAL_DMA_I2C1_TX : HAL_DMA_I2C1_RX);
        }
        break;

    case DMA_BUS_UART:
        switch (index) {
        case 0:
            *out_channel = (uint8_t)(is_tx ? hal_dma_func_map.uart0_tx : hal_dma_func_map.uart0_rx);
            *out_func    = (is_tx ? HAL_DMA_UART0_TX : HAL_DMA_UART0_RX);
            break;
        case 1:
            *out_channel = (uint8_t)(is_tx ? hal_dma_func_map.uart1_tx : hal_dma_func_map.uart1_rx);
            *out_func    = (is_tx ? HAL_DMA_UART1_TX : HAL_DMA_UART1_RX);
            break;
        default:
            *out_channel = (uint8_t)(is_tx ? hal_dma_func_map.uart2_tx : hal_dma_func_map.uart2_rx);
            *out_func    = (is_tx ? HAL_DMA_UART2_TX : HAL_DMA_UART2_RX);
            break;
        }
        break;

    default:
        return false;
    }
    return (*out_channel != (uint8_t)HAL_ERR);
}

/* ========================================================================== */
/*  Mapping / Lookup                                                          */
/* ========================================================================== */

HAL_DMA_SRC_REQ_SEL_T hal_drv_dma_get_func_by_channel(uint8_t ch)
{
    if (ch >= CHIP_DMA_CHANNEL_NUM) {
        return HAL_DMA_UNSED;
    }
    return dma_channel_func_map[ch];
}

int hal_drv_dma_find_channel_by_func(HAL_DMA_SRC_REQ_SEL_T func)
{
    /* API exposes channels CH0–CH5; driver maps them to HW DMA channels 2–7. */
    for (int i = 0; i < CHIP_DMA_CHANNEL_NUM - 2; ++i) {
        if (dma_channel_func_map[i] == func) {
            return i + 2;
        }
    }
    return HAL_ERR; // not found
}

void hal_dma_func_map_export(void)
{
    // hal_dma_func_map.mac_tx      = hal_drv_dma_find_channel_by_func(HAL_DMA_MAC_TX);
    // hal_dma_func_map.mac_rx      = hal_drv_dma_find_channel_by_func(HAL_DMA_MAC_RX);
    hal_dma_func_map.mac_tx      = 0;
    hal_dma_func_map.mac_rx      = 1;
    hal_dma_func_map.aes_in      = hal_drv_dma_find_channel_by_func(HAL_DMA_AES_IN);
    hal_dma_func_map.aes_out     = hal_drv_dma_find_channel_by_func(HAL_DMA_AES_OUT);
    hal_dma_func_map.aes_auth_in = hal_drv_dma_find_channel_by_func(HAL_DMA_AES_AUTH_IN);
    hal_dma_func_map.sha         = hal_drv_dma_find_channel_by_func(HAL_DMA_SHA);
    hal_dma_func_map.spi0_tx     = hal_drv_dma_find_channel_by_func(HAL_DMA_SPI0_TX);
    hal_dma_func_map.spi0_rx     = hal_drv_dma_find_channel_by_func(HAL_DMA_SPI0_RX);
    hal_dma_func_map.spi1_tx     = hal_drv_dma_find_channel_by_func(HAL_DMA_SPI1_TX);
    hal_dma_func_map.spi1_rx     = hal_drv_dma_find_channel_by_func(HAL_DMA_SPI1_RX);
    hal_dma_func_map.uart0_tx    = hal_drv_dma_find_channel_by_func(HAL_DMA_UART0_TX);
    hal_dma_func_map.uart0_rx    = hal_drv_dma_find_channel_by_func(HAL_DMA_UART0_RX);
    hal_dma_func_map.uart1_tx    = hal_drv_dma_find_channel_by_func(HAL_DMA_UART1_TX);
    hal_dma_func_map.uart1_rx    = hal_drv_dma_find_channel_by_func(HAL_DMA_UART1_RX);
    hal_dma_func_map.uart2_tx    = hal_drv_dma_find_channel_by_func(HAL_DMA_UART2_TX);
    hal_dma_func_map.uart2_rx    = hal_drv_dma_find_channel_by_func(HAL_DMA_UART2_RX);
    hal_dma_func_map.i2c0_tx     = hal_drv_dma_find_channel_by_func(HAL_DMA_I2C0_TX);
    hal_dma_func_map.i2c0_rx     = hal_drv_dma_find_channel_by_func(HAL_DMA_I2C0_RX);
    hal_dma_func_map.i2c1_tx     = hal_drv_dma_find_channel_by_func(HAL_DMA_I2C1_TX);
    hal_dma_func_map.i2c1_rx     = hal_drv_dma_find_channel_by_func(HAL_DMA_I2C1_RX);
    hal_dma_func_map.user        = hal_drv_dma_find_channel_by_func(HAL_DMA_USER);
}

void hal_intf_dma_func_map_export_printf(void)
{
    debug_printf("\n");
    /* API exposes channels CH0–CH5; driver maps them to HW DMA channels 2–7. */
    for (uint8_t ch = 0; ch < CHIP_DMA_CHANNEL_NUM - 2; ++ch) {
        HAL_DMA_SRC_REQ_SEL_T func = hal_drv_dma_get_func_by_channel(ch);

        const char *func_name = "UNKNOWN";
        (void)func_name;
        if (func < HAL_DMA_FUNC_MAX) {
            func_name = dma_func_name_table[func];
        }

        debug_printf("DMA Channel %u -> %s\n", ch, func_name);
    }
#if 0
    for (uint8_t i = 0; i < CHIP_DMA_CHANNEL_NUM; i++) { 
        debug_printf("dma_channel[%d], function = %d\n", i, dma_channels[i].func);
    }
#endif
}

/* ========================================================================== */
/*  DMA Core: Channel Init / Setup / Trigger / Interrupts                     */
/* ========================================================================== */

void hal_intf_dma_init_channels(const uint8_t *ch_func)
{
    uint32_t func_select = 0;
    uint8_t adj_ch       = 0;
    dma_channel_func_map = ch_func;
    hal_intf_pmu_module_clk_enable(DMA_MODULE);

    dma_channels[0].func      = HAL_DMA_MAC_TX;
    dma_channels[0].allocated = true;
    dma_channels[1].func      = HAL_DMA_MAC_RX;
    dma_channels[1].allocated = true;

    /* API exposes channels CH0–CH5; driver maps them to HW DMA channels 2–7. */
    for (uint8_t i = 0; i < CHIP_DMA_CHANNEL_NUM - 2; i++) {
        adj_ch      = i + 2;
        func_select = hal_drv_dma_get_func_by_channel(i);
        if (func_select == HAL_DMA_UNSED) {
            dma_channels[adj_ch].allocated = false;
            continue;
        }
        dma_channels[adj_ch].func      = func_select;
        dma_channels[adj_ch].allocated = true;
    }

    hal_dma_func_map_export();
}

HAL_STATUS hal_intf_dma_channel_setup(HAL_DMA_SRC_REQ_SEL_T func, const void *src_addr, void *dst_addr, uint32_t data_len, uint32_t data_size,
                                      bool int_en)
{
    int8_t dma_channel = hal_drv_dma_find_channel_by_func(func);
    if (dma_channel == HAL_ERR) {
        debug_printf("!!dma func not setup\n");
        return HAL_ERR;
    }

    uint32_t enable_module_clock = 0;
    if (!hal_intf_pmu_module_clk_is_enabled(DMA_MODULE)) {
        enable_module_clock |= DMA_MODULE;
    }
    hal_intf_pmu_module_clk_enable(enable_module_clock);
    hal_drv_dma_interrupt_enable((uint8_t)dma_channel, int_en);

    dma_channels[dma_channel].src       = src_addr;
    dma_channels[dma_channel].dst       = dst_addr;
    dma_channels[dma_channel].data_size = data_size;
    dma_channels[dma_channel].data_len  = data_len;
    dma_channels[dma_channel].int_en    = int_en;

    if (func >= HAL_DMA_MAC_TX && func <= HAL_DMA_SHA) {
        int32_t func_select = get_func_select_value(func);
        uint32_t addr       = (func == HAL_DMA_MAC_RX || func == HAL_DMA_AES_OUT) ? (uint32_t)dst_addr : (uint32_t)src_addr;
        hal_drv_dma_p2m2p_config_and_trigger((uint8_t)dma_channel, func_select, addr, data_len);

    } else if (func == HAL_DMA_USER) {
        hal_drv_dma_m2m_config((uint8_t)dma_channel, (uint32_t)src_addr, (uint32_t)dst_addr, data_size, data_len);

    } else {
        int32_t func_select = get_func_select_value(func);
        if (func_select == HAL_ERR) {
            return HAL_ERR;
        }
        HAL_DMA_EXT_DIR_T direction;
        uint32_t addr;
        get_dma_direction_and_address(func, src_addr, dst_addr, &direction, &addr);
        hal_drv_dma_extsrc_m2m_config((uint8_t)dma_channel, func_select, direction, addr, data_size, data_len);
    }

    return HAL_NO_ERR;
}

HAL_STATUS hal_intf_dma_channel_trigger(HAL_DMA_SRC_REQ_SEL_T func)
{
    int8_t dma_channel = hal_drv_dma_find_channel_by_func(func);
    if (dma_channel == HAL_ERR) {
        debug_printf("!!dma func not setup\n");
        return HAL_ERR;
    }

    uint32_t enable_module_clock = 0;
    if (!hal_intf_pmu_module_clk_is_enabled(DMA_MODULE)) {
        enable_module_clock |= DMA_MODULE;
    }
    hal_intf_pmu_module_clk_enable(enable_module_clock);

    if (func != HAL_DMA_USER) {
        int32_t func_select = get_func_select_value(func);
        if (func_select == HAL_ERR) {
            return HAL_ERR;
        }
        hal_drv_dma_extsrc_m2m_trigger((uint8_t)dma_channel, func_select);
    } else {
        hal_drv_dma_src_m2m_trigger((uint8_t)dma_channel);
    }
    return HAL_NO_ERR;
}

HAL_STATUS hal_intf_dma_interrupt_enable(uint8_t dma_channel, bool enable)
{
    return hal_drv_dma_interrupt_enable(dma_channel, enable);
}

HAL_STATUS hal_intf_dma_register_event_callback(uint8_t dma_channel, EVT_CALLBACK_T cb, void *context)
{
    return hal_drv_dma_register_callback(dma_channel, cb, context);
}

HAL_STATUS hal_intf_dma_unregister_event_callback(uint8_t dma_channel)
{
    return hal_drv_dma_unregister_callback(dma_channel);
}

/* ========================================================================== */
/*  DMA External Source (ExtSrc) Utilities                                    */
/* ========================================================================== */

HAL_STATUS hal_intf_dma_extsrc_config(uint8_t ch, HAL_DMA_SRC_REQ_SEL_T func)
{
    return hal_drv_dma_extsrc_config(ch, func);
}

HAL_STATUS hal_intf_dma_extsrc_update(uint8_t ch, HAL_DMA_SRC_REQ_SEL_T func, void *data, uint32_t len)
{
    hal_drv_dma_extsrc_update(ch, func, (uint32_t)(uintptr_t)data, len);
    return HAL_NO_ERR;
}

HAL_STATUS hal_intf_dma_extsrc_setup(uint8_t ch, HAL_DMA_SRC_REQ_SEL_T func, void *data, uint32_t len)
{
    hal_intf_dma_extsrc_config(ch, func);
    hal_intf_dma_extsrc_update(ch, func, data, len);
    return HAL_NO_ERR;
}

HAL_STATUS hal_intf_dma_extsrc_send(uint8_t ch, HAL_DMA_SRC_REQ_SEL_T func, void *data, uint32_t len)
{
    hal_intf_dma_extsrc_update(ch, func, data, len);
    hal_intf_dma_extsrc_trigger(ch, func);
    return HAL_NO_ERR;
}

HAL_STATUS hal_intf_dma_extsrc_trigger(uint8_t channel, HAL_DMA_SRC_REQ_SEL_T func)
{
    hal_drv_dma_extsrc_trigger(channel, func);
    return HAL_NO_ERR;
}

/* ========================================================================== */
/*  SPI DMA Helpers                                                           */
/* ========================================================================== */

HAL_STATUS hal_intf_spi_dma_init(void)
{
    if (hal_dma_func_map.spi0_tx != HAL_ERR) {
        hal_intf_dma_extsrc_config((uint8_t)hal_dma_func_map.spi0_tx, HAL_DMA_SPI0_TX);
    }
    if (hal_dma_func_map.spi0_rx != HAL_ERR) {
        hal_intf_dma_extsrc_config((uint8_t)hal_dma_func_map.spi0_rx, HAL_DMA_SPI0_RX);
    }
    if (hal_dma_func_map.spi1_tx != HAL_ERR) {
        hal_intf_dma_extsrc_config((uint8_t)hal_dma_func_map.spi1_tx, HAL_DMA_SPI1_TX);
    }
    if (hal_dma_func_map.spi1_rx != HAL_ERR) {
        hal_intf_dma_extsrc_config((uint8_t)hal_dma_func_map.spi1_rx, HAL_DMA_SPI1_RX);
    }
    return HAL_NO_ERR;
}

HAL_STATUS hal_intf_spi_dma_update(uint8_t spi_index, void *tx_fifo_ptr, uint32_t tx_count, void *rx_fifo_ptr, uint32_t rx_count)
{
    uint8_t ch;
    HAL_DMA_SRC_REQ_SEL_T func;

    if (tx_count != 0 && tx_fifo_ptr != NULL && resolve_bus_dma(DMA_BUS_SPI, spi_index, true, &ch, &func)) {
        hal_intf_dma_extsrc_update(ch, func, tx_fifo_ptr, tx_count);
    }

    if (rx_count != 0 && rx_fifo_ptr != NULL && resolve_bus_dma(DMA_BUS_SPI, spi_index, false, &ch, &func)) {
        hal_intf_dma_extsrc_update(ch, func, rx_fifo_ptr, rx_count);
    }

    return HAL_NO_ERR;
}

HAL_STATUS hal_intf_spi_dma_trigger(uint8_t spi_index, void *tx_fifo_ptr, uint32_t tx_count, void *rx_fifo_ptr, uint32_t rx_count)
{
    uint8_t ch;
    HAL_DMA_SRC_REQ_SEL_T func;

    if (tx_count != 0 && tx_fifo_ptr != NULL && resolve_bus_dma(DMA_BUS_SPI, spi_index, true, &ch, &func)) {
        hal_intf_dma_extsrc_trigger(ch, func);
    }

    if (rx_count != 0 && rx_fifo_ptr != NULL && resolve_bus_dma(DMA_BUS_SPI, spi_index, false, &ch, &func)) {
        hal_intf_dma_extsrc_trigger(ch, func);
    }

    return HAL_NO_ERR;
}

/* ========================================================================== */
/*  I2C DMA Helpers                                                           */
/* ========================================================================== */
#if 0 //!TODO
HAL_STATUS hal_intf_i2c_check_dma_ch_setup(uint8_t i2c_index) 
{
    if (i2c_index == 0) {
        if (hal_dma_func_map.i2c0_tx == HAL_ERR) {
            return HAL_ERR;
        }
        if (hal_dma_func_map.i2c0_rx == HAL_ERR) {
            return HAL_ERR;
        }
    } else {
        if (hal_dma_func_map.i2c1_tx == HAL_ERR) {
            return HAL_ERR;
        }
        if (hal_dma_func_map.i2c1_rx == HAL_ERR) {
            return HAL_ERR;
        }
    }
    return HAL_NO_ERR;
}
#endif
HAL_STATUS hal_intf_i2c_dma_init(void)
{
    /* I2C0 */
    if (hal_dma_func_map.i2c0_tx != HAL_ERR) {
        hal_intf_dma_extsrc_config((uint8_t)hal_dma_func_map.i2c0_tx, HAL_DMA_I2C0_TX);
    }
    if (hal_dma_func_map.i2c0_rx != HAL_ERR) {
        hal_intf_dma_extsrc_config((uint8_t)hal_dma_func_map.i2c0_rx, HAL_DMA_I2C0_RX);
    }

    /* I2C1 */
    if (hal_dma_func_map.i2c1_tx != HAL_ERR) {
        hal_intf_dma_extsrc_config((uint8_t)hal_dma_func_map.i2c1_tx, HAL_DMA_I2C1_TX);
    }
    if (hal_dma_func_map.i2c1_rx != HAL_ERR) {
        hal_intf_dma_extsrc_config((uint8_t)hal_dma_func_map.i2c1_rx, HAL_DMA_I2C1_RX);
    }

    return HAL_NO_ERR;
}

HAL_STATUS hal_intf_i2c_dma_update(uint8_t i2c_index, void *tx_fifo_ptr, uint32_t tx_count, void *rx_fifo_ptr, uint32_t rx_count)
{
    uint8_t ch;
    HAL_DMA_SRC_REQ_SEL_T f;

    /* TX: memory -> I2C TX FIFO */
    if (tx_count != 0 && tx_fifo_ptr != NULL && resolve_bus_dma(DMA_BUS_I2C, i2c_index, true, &ch, &f)) {
        hal_intf_dma_extsrc_update(ch, f, tx_fifo_ptr, tx_count);
    }

    /* RX: I2C RX FIFO -> memory */
    if (rx_count != 0 && rx_fifo_ptr != NULL && resolve_bus_dma(DMA_BUS_I2C, i2c_index, false, &ch, &f)) {
        hal_intf_dma_extsrc_update(ch, f, rx_fifo_ptr, rx_count);
    }

    return HAL_NO_ERR;
}

HAL_STATUS hal_intf_i2c_dma_trigger(uint8_t i2c_index, void *tx_fifo_ptr, uint32_t tx_count, void *rx_fifo_ptr, uint32_t rx_count)
{
    uint8_t ch;
    HAL_DMA_SRC_REQ_SEL_T f;

    if (rx_count != 0 && rx_fifo_ptr != NULL && resolve_bus_dma(DMA_BUS_I2C, i2c_index, false, &ch, &f)) {
        hal_intf_dma_extsrc_trigger(ch, f);
    }

    if (tx_count != 0 && tx_fifo_ptr != NULL && resolve_bus_dma(DMA_BUS_I2C, i2c_index, true, &ch, &f)) {
        hal_intf_dma_extsrc_trigger(ch, f);
    }

    return HAL_NO_ERR;
}

/* ========================================================================== */
/*  UART DMA Helpers                                                          */
/* ========================================================================== */

HAL_STATUS hal_intf_uart_dma_init(void)
{
    /* UART0 */
    if (hal_dma_func_map.uart0_tx != HAL_ERR) {
        hal_intf_dma_extsrc_config((uint8_t)hal_dma_func_map.uart0_tx, HAL_DMA_UART0_TX);
    }
    if (hal_dma_func_map.uart0_rx != HAL_ERR) {
        hal_intf_dma_extsrc_config((uint8_t)hal_dma_func_map.uart0_rx, HAL_DMA_UART0_RX);
    }

    /* UART1 */
    if (hal_dma_func_map.uart1_tx != HAL_ERR) {
        hal_intf_dma_extsrc_config((uint8_t)hal_dma_func_map.uart1_tx, HAL_DMA_UART1_TX);
    }
    if (hal_dma_func_map.uart1_rx != HAL_ERR) {
        hal_intf_dma_extsrc_config((uint8_t)hal_dma_func_map.uart1_rx, HAL_DMA_UART1_RX);
    }

    /* UART2 */
    if (hal_dma_func_map.uart2_tx != HAL_ERR) {
        hal_intf_dma_extsrc_config((uint8_t)hal_dma_func_map.uart2_tx, HAL_DMA_UART2_TX);
    }
    if (hal_dma_func_map.uart2_rx != HAL_ERR) {
        hal_intf_dma_extsrc_config((uint8_t)hal_dma_func_map.uart2_rx, HAL_DMA_UART2_RX);
    }

    return HAL_NO_ERR;
}

HAL_STATUS hal_intf_uart_dma_update(uint8_t uart_index, void *tx_fifo_ptr, uint32_t tx_count, void *rx_fifo_ptr, uint32_t rx_count)
{
    uint8_t ch;
    HAL_DMA_SRC_REQ_SEL_T f;

    /* TX: memory -> UART TX FIFO */
    if (tx_count != 0 && tx_fifo_ptr != NULL && resolve_bus_dma(DMA_BUS_UART, uart_index, true, &ch, &f)) {
        hal_intf_dma_extsrc_update(ch, f, tx_fifo_ptr, tx_count);
    }

    /* RX: UART RX FIFO -> memory */
    if (rx_count != 0 && rx_fifo_ptr != NULL && resolve_bus_dma(DMA_BUS_UART, uart_index, false, &ch, &f)) {
        hal_intf_dma_extsrc_update(ch, f, rx_fifo_ptr, rx_count);
    }

    return HAL_NO_ERR;
}

HAL_STATUS hal_intf_uart_dma_trigger(uint8_t uart_index, void *tx_fifo_ptr, uint32_t tx_count, void *rx_fifo_ptr, uint32_t rx_count)
{
    uint8_t ch;
    HAL_DMA_SRC_REQ_SEL_T f;

    if (rx_count != 0 && rx_fifo_ptr != NULL && resolve_bus_dma(DMA_BUS_UART, uart_index, false, &ch, &f)) {
        hal_intf_dma_extsrc_trigger(ch, f);
    }

    if (tx_count != 0 && tx_fifo_ptr != NULL && resolve_bus_dma(DMA_BUS_UART, uart_index, true, &ch, &f)) {
        hal_intf_dma_extsrc_trigger(ch, f);
    }

    return HAL_NO_ERR;
}

/* ========================================================================== */
/*  MAC DMA: TX / RX (Init / Update_and_Trigger)                              */
/* ========================================================================== */

HAL_STATUS hal_intf_mac_dma_init(void)
{
    /* MAC TX*/
    if (hal_dma_func_map.mac_tx != HAL_ERR) {
        hal_drv_dma_p2m2p_config((uint8_t)hal_dma_func_map.mac_tx, HAL_DMA_P2M_MAC_TX);
    }
    /* MAC RX*/
    if (hal_dma_func_map.mac_rx != HAL_ERR) {
        hal_drv_dma_p2m2p_config((uint8_t)hal_dma_func_map.mac_rx, HAL_DMA_P2M_MAC_RX);
    }
    return HAL_NO_ERR;
}

HAL_STATUS hal_intf_mac_tx_dma_update_and_trigger(const void *input_ptr, uint32_t input_length)
{
    hal_drv_dma_p2m2p_update_transfer((uint8_t)hal_dma_func_map.mac_tx, (uint32_t)(uintptr_t)(input_ptr), input_length);

    return HAL_NO_ERR;
}

HAL_STATUS hal_intf_mac_rx_dma_update_and_trigger(void *output_ptr, uint32_t output_length)
{
    hal_drv_dma_p2m2p_update_transfer((uint8_t)hal_dma_func_map.mac_rx, (uint32_t)(uintptr_t)(output_ptr), output_length);

    return HAL_NO_ERR;
}

/* ========================================================================== */
/*  Crypto DMA: AES / SHA (Init / Setup / Update / Trigger)                   */
/* ========================================================================== */

HAL_STATUS hal_intf_aes_dma_init(void)
{
    /* AES input */
    if (hal_dma_func_map.aes_in != HAL_ERR) {
        hal_drv_dma_p2m2p_config((uint8_t)hal_dma_func_map.aes_in, HAL_DMA_P2M_AES_IN);
    }
    /* AES output */
    if (hal_dma_func_map.aes_out != HAL_ERR) {
        hal_drv_dma_p2m2p_config((uint8_t)hal_dma_func_map.aes_out, HAL_DMA_P2M_AES_OUT);
    }
    /* AES AUTH input (optional) */
    if (hal_dma_func_map.aes_auth_in != HAL_ERR) {
        hal_drv_dma_p2m2p_config((uint8_t)hal_dma_func_map.aes_auth_in, HAL_DMA_P2M_AES_AUTH_IN);
    }
    return HAL_NO_ERR;
}

HAL_STATUS hal_intf_sha_dma_init(void)
{
    if (hal_dma_func_map.sha != HAL_ERR) {
        hal_drv_dma_p2m2p_config((uint8_t)hal_dma_func_map.sha, HAL_DMA_P2M_SHA);
    }
    return HAL_NO_ERR;
}

HAL_STATUS hal_intf_crypto_dma_init(void)
{
    hal_intf_aes_dma_init();
    hal_intf_sha_dma_init();
    return HAL_NO_ERR;
}

HAL_STATUS hal_intf_aes_dma_setup(const HAL_AES_DMA_SETUP_T *cfg)
{
    HAL_STATUS ret;

    /* AES input */
    ret = hal_intf_dma_channel_setup(HAL_DMA_AES_IN, cfg->data_io.input, NULL, cfg->data_io.input_length, 0, 0);
    if (ret != HAL_NO_ERR) {
        debug_printf("!!dma setup fail aes in\n");
        return ret;
    }

    /* AES output */
    ret = hal_intf_dma_channel_setup(HAL_DMA_AES_OUT, NULL, cfg->data_io.output, cfg->data_io.output_length, 0, 0);
    if (ret != HAL_NO_ERR) {
        debug_printf("!!dma setup fail aes out\n");
        return ret;
    }

    /* AES AUTH input (optional) */
    if (cfg->auth_in) {
        ret = hal_intf_dma_channel_setup(HAL_DMA_AES_AUTH_IN, cfg->auth_in->input, NULL, cfg->auth_in->input_length, 0, 0);
        if (ret != HAL_NO_ERR) {
            debug_printf("!!dma setup fail aes auth in\n");
            return ret;
        }
    }

    return HAL_NO_ERR;
}

HAL_STATUS hal_intf_aes_dma_update(const HAL_AES_DMA_SETUP_T *cfg)
{
    /* AES input */
    hal_drv_dma_p2m2p_update_transfer((uint8_t)hal_dma_func_map.aes_in, (uint32_t)(uintptr_t)(cfg->data_io.input), cfg->data_io.input_length);

    /* AES output */
    hal_drv_dma_p2m2p_update_transfer((uint8_t)hal_dma_func_map.aes_out, (uint32_t)(uintptr_t)(cfg->data_io.output), cfg->data_io.output_length);

    /* AES AUTH input (optional) */
    if (cfg->auth_in) {
        hal_drv_dma_p2m2p_update_transfer((uint8_t)hal_dma_func_map.aes_auth_in, (uint32_t)(uintptr_t)(cfg->auth_in->input),
                                          cfg->auth_in->input_length);
    }

    return HAL_NO_ERR;
}

HAL_STATUS hal_intf_sha_dma_setup(const HAL_DMA_DATA_CFG_T *cfg)
{
    HAL_STATUS ret;

    /* SHA input */
    ret = hal_intf_dma_channel_setup(HAL_DMA_SHA, cfg->input, NULL, cfg->input_length, 0, 0);
    if (ret != HAL_NO_ERR) {
        debug_printf("!!dma setup fail sha in\n");
        return ret;
    }

    return HAL_NO_ERR;
}

HAL_STATUS hal_intf_sha_dma_update(const HAL_DMA_DATA_CFG_T *cfg)
{
    hal_drv_dma_p2m2p_update_transfer((uint8_t)hal_dma_func_map.sha, (uint32_t)(uintptr_t)(cfg->input), cfg->input_length);
    return HAL_NO_ERR;
}

HAL_STATUS hal_intf_dma_aes_in_channel_trigger(void)
{
    /* Buffer A trigger for AES_IN */
    hal_drv_dma_buffer_trigger((uint8_t)hal_dma_func_map.aes_in, 0); /* BUFF_A */
    return HAL_NO_ERR;
}
#endif /* CHIP_DMA_SUPPORT_PERIPHERAL */
