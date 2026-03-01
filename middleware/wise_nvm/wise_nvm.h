#ifndef __WISE_NVM_H
#define __WISE_NVM_H

#include "types.h"

#include <stdint.h>

#define MAX_NVM_INSTANCES 8

typedef struct node {
    uint16_t id;
    uint16_t size;
    uint32_t offset;
    struct node* next;
} node_t;

typedef struct {
	uint16_t status;
    uint32_t start_addr;
    uint32_t length;
    node_t* id_list;
    uint16_t list_size;
    uint32_t free_offset;
} nvm_instance_t;

typedef struct {
    uint16_t id         : 12;  // [11:0]
    uint16_t status     : 2;   // [13:12]
    uint16_t data_len   : 10;  // [23:14]
    uint8_t  check_sum;        // [31:24]
} __attribute__((packed)) object_info_t;

typedef struct {
	uint32_t sig;
	uint32_t length;
} instance_hdr_t;

typedef nvm_instance_t* nvm_handle_t;

#define NVM_SIGNATURE		0x564E5345	//"ESNV"

#define NVM_INSTANCE_ACTIVE		1
#define NVM_INSTANCE_INVALID	0

#define NVM_DATA_INVALID	0b11
#define NVM_DATA_ACTIVED	0b10
#define NVM_DATA_BACKUP		0x00

#define NVM_ACTION_OK		(0)
#define NVM_ACTION_FAIL		(-1)

uint8_t cal_check_sum(uint8_t *data, int size);
int nvm_get_data(nvm_handle_t handle, uint32_t id, uint8_t *data);
void nvm_push_data(uint32_t addr, uint32_t id, uint8_t *data, int size);
int nvm_set_data(nvm_handle_t handle, uint16_t id, uint8_t *data, int size);
int nvm_write(nvm_handle_t handle, uint16_t id, uint8_t *data, int size);
int nvm_read(nvm_handle_t handle, uint16_t id, uint8_t *data);
int nvm_delete(nvm_handle_t handle, uint16_t id);
void nvm_delete_instance(nvm_handle_t handle);
void nvm_erase_instance_section(nvm_handle_t handle);
void nvm_init_flash(void);
void nvm_init_instance(nvm_handle_t handle);
nvm_handle_t nvm_create_instance(uint32_t start_addr, uint32_t length);
uint16_t nvm_append_or_update_node(node_t** head, uint16_t id, uint16_t size, uint32_t offset);
node_t* nvm_find_node_by_id(node_t* head, uint16_t id);
void nvm_free_all_nodes(node_t** head);
void nvm_create_nodes(nvm_handle_t handle);
void nvm_remove_node_by_id(nvm_handle_t handle, uint16_t id);
void nvm_list_all_nodes(nvm_handle_t handle);
int nvm_gc_block(nvm_handle_t handle);
#endif
