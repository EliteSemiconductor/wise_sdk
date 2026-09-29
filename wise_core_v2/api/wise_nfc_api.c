/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "api/wise_nfc_api.h"

static void _clear_data_and_dynamic_lock_bits(void);

void wise_nfc_init(void)
{
    hal_intf_module_clk_enable(NFC_MODULE);
    hal_intf_module_clk_enable(EFUSE_MODULE);
}

void wise_nfc_deinit(void)
{
    hal_intf_module_clk_disable(NFC_MODULE);
    hal_intf_module_clk_disable(EFUSE_MODULE);
}

void wise_nfc_config(WISE_NFC_CFG_T *cfg)
{
	//set nfc power source to MCU mode
	hal_intf_nfc_switch_pwr_src(NFC_PWR_MODE_ACTIVE);

    //set nfc sram work mode: default/full
    hal_intf_nfc_set_mem_work_mode(cfg->work_mode);

    //set sram lock memory mechanism : mode 1 / 2
    hal_intf_nfc_set_mem_lock_mode(cfg->lock_mode);

    //set interrupt idx 0/1
    hal_intf_nfc_set_interrupt(cfg->int_idx, ENABLE);

    _clear_data_and_dynamic_lock_bits();
}

uint8_t wise_nfc_get_mem_work_mode(void)
{
    return hal_intf_nfc_get_mem_work_mode();
}

void wise_nfc_set_mem_work_mode(NFC_WORK_MODE_E mode)
{
    hal_intf_nfc_set_mem_work_mode(mode);
}

uint8_t wise_nfc_get_mem_lock_mode(void)
{
    return hal_intf_nfc_get_mem_lock_mode();
}

void wise_nfc_set_mem_lock_mode(NFC_MEM_LOCK_MODE_E mode)
{
    hal_intf_nfc_set_mem_lock_mode(mode);
}

uint8_t wise_nfc_is_mem_locked(void)
{
    return hal_intf_nfc_is_mem_locked();
}

void wise_nfc_block_read_data(uint8_t block_idx, uint8_t block_len, uint32_t *rx_data_buff)
{
    hal_intf_nfc_block_read_data(block_idx, block_len, rx_data_buff);
}

void wise_nfc_block_write_data(uint8_t block_idx, uint8_t block_len, uint32_t *tx_data_buff)
{
    hal_intf_nfc_block_write_data(block_idx, block_len, tx_data_buff);
}

uint8_t wise_nfc_get_dpe_status_info(void)
{
    return hal_intf_nfc_get_dpe_status_info();
}

uint8_t wise_nfc_get_dpe_ctrl_info(void)
{
    return hal_intf_nfc_get_dpe_ctrl_info();
}

void wise_nfc_register_int_callback(NFC_INT_IDX_E int_idx, CALLBACK_T cb,            void *context)
{
    hal_intf_nfc_register_int_callback(int_idx, cb, context);
}

void wise_nfc_unregister_int_callback(NFC_INT_IDX_E int_idx)
{
    hal_intf_nfc_unregister_int_callback(int_idx);
}

void wise_nfc_set_host_locked(uint8_t enable)
{
    hal_intf_nfc_set_host_locked(enable);
}

uint8_t wise_nfc_get_interrupt_idx(void)
{
    return hal_intf_nfc_get_interrupt_idx();
}

void wise_nfc_set_interrupt_idx(NFC_INT_IDX_E int_idx)
{
    hal_intf_nfc_set_interrupt(int_idx, ENABLE);
}

void wise_nfc_switch_pwr_src(NFC_PWR_MODE_T src)
{
    hal_intf_nfc_switch_pwr_src((uint8_t)src);
}

NFC_PWR_MODE_T wise_nfc_get_pwr_src(void)
{
    return (NFC_PWR_MODE_T)hal_intf_nfc_get_pwr_src_idx();
}

void wise_nfc_set_wakeup_config(uint8_t pwr_mode)
{
    hal_intf_nfc_set_wakeup_config(pwr_mode);
}

void (*nfc_ctrl_cb)(void);

static void _nfc_isr_callback(void *context, uint8_t idx)
{
    (void)context;
    (void)idx;

    if (nfc_ctrl_cb) {
        nfc_ctrl_cb();
    }
}

static void _clear_data_and_dynamic_lock_bits(void)
{
    uint32_t zero_block = 0;

    for (int i = 0; i < NFC_TRIG_BLOCK_IDX; i++) {
        wise_nfc_block_write_data(i, 1, &zero_block);
    }

    uint32_t block255_val = 0;
    wise_nfc_block_read_data(NFC_TRIG_BLOCK_IDX, 1, &block255_val);

    block255_val &= 0xFFFF0000;

    wise_nfc_block_write_data(NFC_TRIG_BLOCK_IDX, 1, &block255_val);
}

static int _append_text_info(uint8_t *dest_buf, int start_idx, int max_len, const char *text)
{
    uint8_t rec[64] = {0};
    int rLen = 0;

    rec[rLen++] = 0xD1; // Header (TNF=1, SR=1)
    rec[rLen++] = 0x01; // Type Length
    int pLenIdx = rLen++;
    rec[rLen++] = 'T';  // Type: 'T'
    rec[rLen++] = 2;    // Status (UTF-8, 2-byte lang)

    int strLen = snprintf((char *)&rec[rLen], sizeof(rec) - rLen, "en%s", text);
    if (strLen < 0 || (rLen + strLen) > sizeof(rec)) {
        return -1; // Buffer overflow prevention
    }
    rLen += strLen;
    rec[pLenIdx] = (uint8_t)(rLen - 3);

    int total_len = 2 + rLen;
    if (start_idx + total_len > max_len) {
        return -1;
    }

    dest_buf[start_idx]     = 0x03; // NDEF Message TLV / Record start
    dest_buf[start_idx + 1] = (uint8_t)rLen;
    memcpy(&dest_buf[start_idx + 2], rec, rLen);

    return total_len;
}

static int _append_esmt_info(uint8_t *dest_buf, int start_idx, int max_len, const uint8_t *payload, int payload_len)
{
    uint8_t rec[64] = {0};
    int rLen = 0;

    rec[rLen++] = 0xD4; // Header (TNF=4 External, SR=1)
    rec[rLen++] = 0x04; // Type Length ("ESMT")
    int pLenIdx = rLen++;

    rec[rLen++] = 'E';
    rec[rLen++] = 'S';
    rec[rLen++] = 'M';
    rec[rLen++] = 'T';

    if (rLen + payload_len > sizeof(rec)) {
        return -1; // Buffer overflow prevention
    }
    memcpy(&rec[rLen], payload, payload_len);
    rLen += payload_len;

    rec[pLenIdx] = (uint8_t)(rLen - 3);

    int total_len = 2 + rLen;
    if (start_idx + total_len > max_len) {
        return -1;
    }

    dest_buf[start_idx]     = 0x03;
    dest_buf[start_idx + 1] = (uint8_t)rLen;
    memcpy(&dest_buf[start_idx + 2], rec, rLen);

    return total_len;
}

void wise_nfc_crtl_tag_config(uint8_t *str)
{
    int idx = 0;
    int totalDataLen = 0;
    uint32_t tag_cnt_buf[NFC_DATA_BLOCK_IDX] = {0};
    uint8_t ndefBytes[NFC_DATA_BLOCK_IDX * 4] = {0};
    int max_ndef_len = sizeof(ndefBytes);

    WISE_NFC_CFG_T nfc_cfg = {
        .work_mode = 1,
        .lock_mode = 1,
        .int_idx = NFC_INT_IDX_1,
    };

    wise_nfc_config(&nfc_cfg);

    // Append Text Record
    int written = _append_text_info(ndefBytes, idx, max_ndef_len, (char *)str);
    if (written < 0) return; // Error handling
    idx += written;

    // Custom Payload Configuration
    uint8_t custom_payload[] = {
        NFC_DATA_BLOCK_IDX,
        NFC_DATA_BLOCK_LEN,
        NFC_ACK_BLOCK_IDX,
        NFC_ACK_BLOCK_LEN,
        NFC_TRIG_BLOCK_IDX,
        (uint8_t)(NFC_TRIG_BLOCK_VAL >> 0),
        (uint8_t)(NFC_TRIG_BLOCK_VAL >> 8),
        (uint8_t)(NFC_TRIG_BLOCK_VAL >> 16),
        (uint8_t)(NFC_TRIG_BLOCK_VAL >> 24),
    };

    // Append ESMT Record
    written = _append_esmt_info(ndefBytes, idx, max_ndef_len, custom_payload, sizeof(custom_payload));
    if (written < 0) return; // Error handling
    idx += written;

    // Terminating Terminator TLV
    if (idx < max_ndef_len) {
        ndefBytes[idx++] = 0xFE;
    }
    totalDataLen = idx;

    // Preset Header Blocks
    tag_cnt_buf[0] = 0x48743480;
    tag_cnt_buf[1] = 0x801CE3E1;
    tag_cnt_buf[2] = 0x0000009E;
    tag_cnt_buf[3] = 0x007C10E1;

    // Pack bytes into 32-bit words (Little-Endian assumption based on original code)
    for (int i = 0; i < totalDataLen; i += 4) {
        int blockIdx = 4 + (i / 4);
        if (blockIdx >= NFC_DATA_BLOCK_IDX) break;

        uint32_t word = 0;
        int remaining = totalDataLen - i;
        int copy_len = (remaining < 4) ? remaining : 4;

        for (int b = 0; b < copy_len; b++) {
            word |= ((uint32_t)ndefBytes[i + b] << (b * 8));
        }
        tag_cnt_buf[blockIdx] = word;
    }

    wise_nfc_block_write_data(0, NFC_DATA_BLOCK_IDX, tag_cnt_buf);
}

int wise_nfc_ctrl_read_data(uint8_t *buf)
{
    uint32_t header_block = 0;
    wise_nfc_block_read_data(NFC_DATA_BLOCK_IDX, 1, &header_block);

    uint8_t *p_bytes = (uint8_t *)&header_block;

    uint16_t len = (p_bytes[0] << 8) | p_bytes[1];

    int total_bytes_to_read = len + 2;
    int blocks_to_read = (total_bytes_to_read + 3) / 4;

    int dest_idx = 0;
    for (int i = 2; i < 4 && i < total_bytes_to_read; i++) {
        buf[dest_idx++] = p_bytes[i];
    }

    if (blocks_to_read > 1) {
        wise_nfc_block_read_data(NFC_DATA_BLOCK_IDX + 1, blocks_to_read - 1, (uint32_t *)&buf[dest_idx]);
    }

    return len;
}

void wise_nfc_ctrl_write_data(uint8_t *buf, uint8_t len)
{
    wise_nfc_block_write_data(NFC_ACK_BLOCK_IDX, (len >> 2) + 1, (uint32_t *)buf);
}

void wise_nfc_ctrl_register_int_cb(void (*cb_ptr)(void))
{
    wise_nfc_register_int_callback(NFC_INT_IDX_1, _nfc_isr_callback, NULL);

    nfc_ctrl_cb = cb_ptr;
}
