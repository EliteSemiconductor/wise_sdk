#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "wmbus_helper.h"
#include "util.h"

static const char manuf[3] = {'E', 'S', 'M'};
static WMBUS_deviceType_t deviceType = WMBUS_DEVICE_WATER_METER;
static const uint8_t version = 0x02;
#ifdef WMBUS_DEMO_PHY_METER
static uint32_t devId = 0xb0000001; //BCD
#else
static uint32_t devId = 0xa0000001; //BCD
#endif

void _wmbus_frame_m_field_2_chars(const uint16_t mfield, char *manuf)
{
    if (manuf != NULL) {
        manuf[0] = ((mfield >> 10) & 0x1F) + 64;
        manuf[1] = ((mfield >> 5) & 0x1F) + 64;
        manuf[2] = ((mfield >> 0) & 0x1F) + 64;
    }
}

uint16_t _wmbus_frame_chars_2_m_field(const char *manuf)
{
    uint16_t code = 0;

    if (manuf != NULL) {
        code  = (uint16_t)((manuf[0] - 64) << 10);
        code |= (uint16_t)((manuf[1] - 64) << 5);
        code |= (uint16_t)(manuf[2] - 64);
    }
    return code;
}

uint8_t _wmbus_frame_add_payload_i8(uint8_t *buffer, WMBUSframe_dibFunction_t dibFunction, uint8_t vib, int8_t value)
{
    if (buffer == NULL) {
        return 0;
    }

    buffer[0] = (uint8_t)((dibFunction << 4) + 1); //DIF=1 is int8
    buffer[1] = vib;
    memcpy(buffer + 2, &value, 1);
    return 3;
}

uint8_t _wmbus_frame_add_payload_i16(uint8_t *buffer, WMBUSframe_dibFunction_t dibFunction, uint8_t vib, int16_t value)
{
    if (buffer == NULL) {
        return 0;
    }

    buffer[0] = (uint8_t)((dibFunction << 4) + 2); //DIF=2 is int16
    buffer[1] = vib;
    memcpy(buffer + 2, &value, 2);
    return 4;
}

uint8_t _wmbus_frame_add_payload_i32(uint8_t *buffer, WMBUSframe_dibFunction_t dibFunction, uint8_t vib, int32_t value)
{
    if (buffer == NULL) {
        return 0;
    }

    buffer[0] = (uint8_t)((dibFunction << 4) + 4); //DIF=4 is int32
    buffer[1] = vib;
    memcpy(buffer + 2, &value, 4);

    return 6;
}

static uint16_t setupSTLHeader(void *buffer, uint8_t mode, uint8_t ciField, uint8_t accessNumber, WMBUS_accessibility_t accessibility,
                               bool synchronized, void *payload, uint16_t payloadLength)
{
    (void)ciField;
    WMBUS_stl_header_t *stlHeader = (WMBUS_stl_header_t *)buffer;

    memset(stlHeader, 0, sizeof(WMBUS_stl_header_t)); //clear the header

    stlHeader->ciField                         = WMBUS_CI_EN13757_3_APPLICATION_SHORT;
    stlHeader->accessNumber                    = accessNumber;
    stlHeader->confWord.mode_0_5.mode          = mode;
    stlHeader->confWord.mode_0_5.accessibility = accessibility;
    stlHeader->confWord.mode_0_5.synchronized  = synchronized;

    if (mode == 5) {
        uint8_t iv[16];
        uint16_t mf = _wmbus_frame_chars_2_m_field(manuf);

        //with long transport layer header, the address from the header should be used
        memcpy(iv, &mf, 2);
        memcpy(iv + 2, &devId, 4);
        memcpy(iv + 6, &version, 1);
        memcpy(iv + 7, &deviceType, 1);
        memset(iv + 8, accessNumber, 8);
        stlHeader->confWord.mode_0_5.numOfEncBlocks = wise_wmbus_crypto_encrypt(payload, payload, iv, payloadLength, 0);

        return 16 * stlHeader->confWord.mode_0_5.numOfEncBlocks; //payloadlength probably changed due to padding
    } else {
        return payloadLength;
    }
}

static void setupDLLHeader(void *buffer, bool isFrameB, uint16_t frameLength, WMBUS_functionCode_t functionCode, bool primary, bool fcvDcf,
                           bool fcbAcd)
{
    WMBUS_dll_header_t *dllHeader = (WMBUS_dll_header_t *)buffer;

    memset(dllHeader, 0, sizeof(WMBUS_dll_header_t)); //clear the header

    dllHeader->cField.detailed.primary       = primary;
    dllHeader->cField.detailed.fcbAcd        = fcbAcd;
    dllHeader->cField.detailed.fcvDcf        = fcvDcf;
    dllHeader->cField.detailed.functionCode  = functionCode;
    dllHeader->address.detailed.manufacturer = _wmbus_frame_chars_2_m_field(manuf);
    dllHeader->address.detailed.id           = devId;
    dllHeader->address.detailed.version      = version;
    dllHeader->address.detailed.deviceType   = deviceType;

    frameLength -= 1; //l field shouldn't be counted

    if (isFrameB) {
        if (frameLength < 125) {
            dllHeader->lField = (uint8_t)(frameLength + 2); //2block, 1 crc
        } else {
            dllHeader->lField = (uint8_t)(frameLength + 4); //3block, 2 crc
        }
    } else {
        dllHeader->lField = (uint8_t)frameLength;
    }
}

uint16_t wmbus_setup_tx_frame(uint8_t *buffer, uint8_t accessNumber, WMBUS_accessibility_t accessibility, bool periodic, bool encrypt)
{
    uint8_t *payloadPtr = buffer + sizeof(WMBUS_dll_header_t) + sizeof(WMBUS_stl_header_t);
    uint8_t *stlPtr     = buffer + sizeof(WMBUS_dll_header_t);
    uint8_t *dllPtr     = buffer;
    //setup the payload before the headers, so we'll know the length
    uint16_t frameLength = 0;

    deviceType = WMBUS_DEVICE_WATER_METER;
    
    if (encrypt) { //2B AES check
        frameLength += _add_idle_filler(payloadPtr + frameLength, 2);
    }

    //VIF 0x13 is volume in m^3
    frameLength += _wmbus_frame_add_payload_i32(payloadPtr + frameLength, WMBUS_DIB_FUNCTION_INSTANTANEOUS, 0x13, 0x1000);

    //VIF 0x3B is volume flow in m^3
    frameLength += _wmbus_frame_add_payload_i16(payloadPtr + frameLength, WMBUS_DIB_FUNCTION_INSTANTANEOUS, 0x3B, 0x10);

    //continue with STL header, since encryption could change the length
    frameLength = setupSTLHeader(stlPtr, encrypt ? 5 : 0, WMBUS_CI_EN13757_3_APPLICATION_SHORT, accessNumber, accessibility, periodic, payloadPtr, frameLength);

    //add the header lengths to frameLnegth
    frameLength += sizeof(WMBUS_dll_header_t) + sizeof(WMBUS_stl_header_t);
    //finally, set up the DLL header
    setupDLLHeader(dllPtr, false, frameLength, WMBUS_FUNCTION_SND_NR, true, false, false);

    return frameLength;
}

uint16_t wmbus_setup_null_frame(uint8_t *buffer, uint8_t accessNumber, WMBUS_accessibility_t accessibility, bool periodic, bool encrypt)
{
    uint8_t *payloadPtr = buffer + sizeof(WMBUS_dll_header_t) + sizeof(WMBUS_stl_header_t);
    uint8_t *stlPtr     = buffer + sizeof(WMBUS_dll_header_t);
    uint8_t *dllPtr     = buffer;
    //setup the payload before the headers, so we'll know the length
    uint16_t frameLength = 0;

    deviceType = WMBUS_DEVICE_OTHER;
    
    if (encrypt) { //2B AES check
        frameLength += _add_idle_filler(payloadPtr + frameLength, 2);
    }
    
    //continue with STL header, since encryption could change the length
    frameLength = setupSTLHeader(stlPtr, encrypt ? 5 : 0, WMBUS_CI_EN13757_3_APPLICATION_SHORT, accessNumber, accessibility, periodic, payloadPtr, frameLength);

    //add the header lengths to frameLnegth
    frameLength += sizeof(WMBUS_dll_header_t) + sizeof(WMBUS_stl_header_t);
    //finally, set up the DLL header
    setupDLLHeader(dllPtr, false, frameLength, WMBUS_FUNCTION_SND_NR, true, false, false);

    return frameLength;
}


static void print_blocks(const uint8_t *buffer, uint16_t length)
{
    for (uint8_t i = 0; i < length; i++) {
        if (i % 16 == 0) {
            debug_print("[0x%02X ", buffer[i]);
        } else if (i % 16 == 8) {
            debug_print("| 0x%02X ", buffer[i]);
        } else if (i % 16 == 15 || i == length - 1) {
            debug_print("0x%02X]\n", buffer[i]);
        } else {
            debug_print("0x%02X ", buffer[i]);
        }
    }
}

void wmbus_dump_frame_info(uint8_t *pframe, uint16_t len)
{
    const WMBUS_dll_header_t *dllHeader = (WMBUS_dll_header_t *)pframe;
    const WMBUS_stl_header_t *stlHeader = (WMBUS_stl_header_t *)(pframe + sizeof(WMBUS_dll_header_t));
    char mField[3];

    _wmbus_frame_m_field_2_chars(dllHeader->address.detailed.manufacturer, mField);
    debug_print("Block-1:[L:%d,C:0x%02X,M:%c%c%c,ID:%08X,Version:0x%02X,devType:0x%02X]\n", dllHeader->lField, dllHeader->cField.raw, mField[0],
                mField[1], mField[2], (unsigned int)dllHeader->address.detailed.id, (unsigned int)dllHeader->address.detailed.version,
                (unsigned int)dllHeader->address.detailed.deviceType);

    if (stlHeader->ciField == WMBUS_CI_EN13757_3_APPLICATION_SHORT) {
        uint8_t *payload_start = pframe + sizeof(WMBUS_dll_header_t) + sizeof(WMBUS_stl_header_t);
        uint16_t payload_len   = dllHeader->lField - sizeof(WMBUS_dll_header_t) - sizeof(WMBUS_stl_header_t) + 1;

        debug_print("AppHeader:[CI:0x%02X,AccessNr:%d,Status:0x%02X,encMode:%d,Accessibility:%02X,encBlocks:%d,sync:%d]\n", stlHeader->ciField,
                    stlHeader->accessNumber, stlHeader->status, stlHeader->confWord.mode_0_5.mode, stlHeader->confWord.mode_0_5.accessibility,
                    stlHeader->confWord.mode_0_5.numOfEncBlocks, stlHeader->confWord.mode_0_5.synchronized);

        if (stlHeader->confWord.mode_0_5.mode == 5) {
            uint8_t iv[16];

            //with long transport layer header, the address from the header should be used
            memcpy(iv, &(dllHeader->address.raw), 8);
            memset(iv + 8, stlHeader->accessNumber, 8);

            wise_wmbus_crypto_decrypt(payload_start, payload_start, iv, payload_len);
        }

        print_blocks(payload_start, payload_len);
    } else {
        print_blocks(pframe + sizeof(WMBUS_dll_header_t), dllHeader->lField - sizeof(WMBUS_dll_header_t) + 1);
    }

    debug_print("\n");
}

