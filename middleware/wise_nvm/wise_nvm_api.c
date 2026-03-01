/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "wise_nvm_api.h"

void wise_nvm_init(void)
{
	nvm_init_flash();
}

int32_t wise_nvm_read_data(nvm_handle_t handle, uint16_t id, uint8_t *data)
{
	return nvm_read(handle, id, data);
}

int32_t wise_nvm_write_data(nvm_handle_t handle, uint16_t id, uint8_t *data, int size)
{
	return nvm_write(handle, id, data, size);
}

int32_t wise_nvm_delete_data(nvm_handle_t handle, uint16_t id)
{
	return nvm_delete(handle, id);
}

void wise_nvm_delete_instance(nvm_handle_t handle)
{
	nvm_delete_instance(handle);
}

nvm_handle_t wise_nvm_create_instance(uint32_t start_addr, uint32_t length)
{
	return nvm_create_instance(start_addr, length);
}

