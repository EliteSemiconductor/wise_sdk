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

void wmbus_link_init_dbg_msg_buf(WMBUS_dbg_msg_buf_t *strBuf);
uint8_t wmbus_link_add_dbg_msg(WMBUS_dbg_msg_buf_t *strBuf, const char *str);
void wmbus_link_dump_dbg_msg(const WMBUS_dbg_msg_buf_t *strBuf);
void wmbus_link_clear_dbg_msg(WMBUS_dbg_msg_buf_t *strBuf);

void dbg_dump_byte(uint8_t* tmp_buf, uint8_t* p_dump, int length);

extern WMBUS_dbg_msg_buf_t wmbus_dbg_msg_buffer;

//#ifdef WMBUS_DEMO
#if 0
	#define WMBUS_DBG_MSG_PRINTF(msg, ...)  do { } while (0)
	#define WMBUS_DBG_DUMP_MSG_PRINTF( ) do { }while(0)
#else

#ifdef USER_PRINTF
    #define WMBUS_DBG_MSG_PRINTF(msg, ...)                              \
    do {                                                                \
        if(1){  \
        uint8_t tmp_dbg_msg_buf[WMBUS_DEBUG_MAX_STRING_LEN];   \
        snprintf((char *)tmp_dbg_msg_buf, WMBUS_DEBUG_MAX_STRING_LEN, msg, ##__VA_ARGS__); \
        wmbus_link_add_dbg_msg(&wmbus_dbg_msg_buffer, (char *)tmp_dbg_msg_buf); \
        }   \
    } while(0)

    #define WMBUS_DBG_DUMP_MSG_PRINTF(p_dump, length)         \
    do{     \
        if(1){  \
        uint8_t tmp_dbg_msg_buf[WMBUS_DEBUG_MAX_STRING_LEN];   \
        dbg_dump_byte(tmp_dbg_msg_buf, p_dump, length);     \
	    wmbus_link_add_dbg_msg(&wmbus_dbg_msg_buffer, (char *)tmp_dbg_msg_buf); \
	    }   \
    }while(0)
#else
    //#define WMBUS_DBG_MSG_PRINTF(msg, ...) printf(msg, ##__VA_ARGS__)
    #define WMBUS_DBG_MSG_PRINTF(msg, ...) do { } while (0)
    #define WMBUS_DBG_DUMP_MSG_PRINTF(p_dump, length) do { }while(0)
#endif //USER_PRINTF

#endif //WMBUS_DEMO

#endif // WMBUS_DBG_MSG_BUF_H

