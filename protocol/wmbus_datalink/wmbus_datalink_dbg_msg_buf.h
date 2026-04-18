#ifndef WMBUS_DBG_MSG_BUF_H
#define WMBUS_DBG_MSG_BUF_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>


#define WMBUS_DEBUG_MAX_NUM 32
#define WMBUS_DEBUG_MAX_STRING_LEN 128

typedef struct {
    char buffer[WMBUS_DEBUG_MAX_NUM][WMBUS_DEBUG_MAX_STRING_LEN];
    int count;
} WMBUS_dbg_msg_buf_t;

typedef enum {
    WMBUS_DBG_MSG_ERROR = 1,
    WMBUS_DBG_MSG_INFO,
    WMBUS_DBG_MSG_DEBUG
} WMBUS_dbg_msg_level_t;

uint8_t wmbus_link_get_dbg_msg_level(void);
void wmbus_link_set_dbg_msg_level(uint8_t level);
void wmbus_link_init_dbg_msg_buf(WMBUS_dbg_msg_buf_t *strBuf);
uint8_t wmbus_link_add_dbg_msg(WMBUS_dbg_msg_buf_t *strBuf, const char *str);
void wmbus_link_dump_dbg_msg(const WMBUS_dbg_msg_buf_t *strBuf);
void wmbus_link_clear_dbg_msg(WMBUS_dbg_msg_buf_t *strBuf);

void dbg_dump_byte(uint8_t* tmp_buf, uint8_t* tag, uint8_t* p_dump, int length);

extern WMBUS_dbg_msg_buf_t wmbus_dbg_msg_buffer;
extern uint8_t g_wmbus_dbg_msg_level;

#define WMBUS_USER_PRINTF
#ifdef WMBUS_USER_PRINTF
    #define WMBUS_DBG_MSG_PRINTF(level, fmt, ...)                              \
    do {                                                                \
        if((level) <= g_wmbus_dbg_msg_level){  \
        uint8_t tmp_dbg_msg_buf[WMBUS_DEBUG_MAX_STRING_LEN];   \
        snprintf((char *)tmp_dbg_msg_buf, WMBUS_DEBUG_MAX_STRING_LEN, fmt, ##__VA_ARGS__); \
        wmbus_link_add_dbg_msg(&wmbus_dbg_msg_buffer, (char *)tmp_dbg_msg_buf); \
        }   \
    } while(0)

    #define WMBUS_DBG_DUMP_MSG_PRINTF(level, tag, p_dump, length)         \
    do{     \
        if((level) <= g_wmbus_dbg_msg_level){  \
        uint8_t tmp_dbg_msg_buf[WMBUS_DEBUG_MAX_STRING_LEN];   \
        dbg_dump_byte(tmp_dbg_msg_buf, tag, p_dump, length);     \
	    wmbus_link_add_dbg_msg(&wmbus_dbg_msg_buffer, (char *)tmp_dbg_msg_buf); \
        wmbus_link_dump_dbg_msg(&wmbus_dbg_msg_buffer); \
        wmbus_link_clear_dbg_msg(&wmbus_dbg_msg_buffer);    \
	    }   \
    }while(0)
#else
    //#define WMBUS_DBG_MSG_PRINTF(fmt, ...) printf(msg, ##__VA_ARGS__)
    #define WMBUS_DBG_MSG_PRINTF(fmt, ...) do { } while (0)
    #define WMBUS_DBG_DUMP_MSG_PRINTF(p_dump, length) do { }while(0)
#endif //WMBUS_USER_PRINTF

#endif // WMBUS_DBG_MSG_BUF_H

