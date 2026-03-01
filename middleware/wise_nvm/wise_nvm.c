/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#include "wise_nvm.h"
#include "wise_flash_api.h"

static nvm_instance_t nvm_instances[MAX_NVM_INSTANCES] = {0};
static int instance_count = 0;
WISE_FLASH_INFO_T flash_info;

uint8_t cal_check_sum(uint8_t *data, int size)
{
	uint8_t checksum = 0;
	int i;

	for(i = 0; i < size; i++)
	{
		checksum += data[i];
	}

	return checksum;
}

int nvm_get_data(nvm_handle_t handle, uint32_t id, uint8_t *data)
{
	object_info_t obj_data;

	node_t* found = nvm_find_node_by_id(handle->id_list, id);

    if(found)
    {
    	//read obj_data header
    	wise_flash_read(found->offset, (uint8_t *)&obj_data, sizeof(obj_data));
    	//read data
    	wise_flash_read(found->offset + sizeof(object_info_t), data, obj_data.data_len);
		if(obj_data.check_sum == cal_check_sum(data, obj_data.data_len))
		{
			return obj_data.data_len;
		}
		else
		{
			return NVM_ACTION_FAIL;
		}
    }
    else
    {
    	return NVM_ACTION_FAIL;
    }
}

void nvm_push_data(uint32_t addr, uint32_t id, uint8_t *data, int size)
{
	object_info_t obj_data;

	obj_data.id = id;
	obj_data.data_len = size;
	obj_data.check_sum = cal_check_sum(data, size);
	obj_data.status = NVM_DATA_ACTIVED;

	//write obj_data header
	wise_flash_write(addr, (uint8_t *)&obj_data, sizeof(object_info_t));
	//write data data
	wise_flash_write(addr + sizeof(object_info_t), data, size);
}

int nvm_set_data(nvm_handle_t handle, uint16_t id, uint8_t *data, int size)
{
	object_info_t obj_data;

    node_t* found = nvm_find_node_by_id(handle->id_list, id);

    if((handle->free_offset + sizeof(obj_data) + size) >= (handle->start_addr + handle->length))
    {
    	//remove old id first
    	if(found && found->size >= size)
    	{
    		nvm_remove_node_by_id(handle, id);
    	}

    	if(nvm_gc_block(handle) == NVM_ACTION_OK)
    	{
    		nvm_free_all_nodes(&handle->id_list);
    		handle->list_size = 0;
    		nvm_create_nodes(handle);
    		if((handle->free_offset + sizeof(obj_data) + size) >= (handle->start_addr + handle->length))
    		{
    			return NVM_ACTION_FAIL;
    		}
    	}
    	else
    	{
    		return NVM_ACTION_FAIL;
    	}
    }
    else
    {
        if(found)
        {
        	wise_flash_read(found->offset, (uint8_t *)&obj_data, sizeof(obj_data));
        	obj_data.status = NVM_DATA_BACKUP;
        	wise_flash_write(found->offset, (uint8_t *)&obj_data, sizeof(obj_data));
        }
    }

    handle->list_size = nvm_append_or_update_node(&handle->id_list, id, size, handle->free_offset);
    nvm_push_data(handle->free_offset, id, data, size);
    handle->free_offset += sizeof(obj_data) + size;

    return size;
}

int nvm_write(nvm_handle_t handle, uint16_t id, uint8_t *data, int size)
{
	if(handle == NULL || data == NULL)
	{
		return NVM_ACTION_FAIL;
	}

	if(id == 0 || id >= 0xfff)
	{
		return NVM_ACTION_FAIL;
	}

	if(size <= 0 || size > 1023)
	{
		return NVM_ACTION_FAIL;
	}

	return nvm_set_data(handle, id, data, size);
}

int nvm_read(nvm_handle_t handle, uint16_t id, uint8_t *data)
{
	return nvm_get_data(handle, id, data);
}

int nvm_delete(nvm_handle_t handle, uint16_t id)
{
	object_info_t obj_data;

	node_t* found = nvm_find_node_by_id(handle->id_list, id);

    if(found)
    {
    	wise_flash_read(found->offset, (uint8_t *)&obj_data, sizeof(obj_data));
    	obj_data.status = NVM_DATA_BACKUP;
    	wise_flash_write(found->offset, (uint8_t *)&obj_data, sizeof(obj_data));

    	nvm_remove_node_by_id(handle, id);
    }
    else
    {
    	return NVM_ACTION_FAIL;
    }

    return NVM_ACTION_OK;
}

void nvm_erase_instance_section(nvm_handle_t handle)
{
	if(handle)
	{
		uint32_t start_addr = handle->start_addr;
		uint32_t length = handle->length;

		while(length)
		{
			wise_flash_sector_erase(start_addr);
			length -= flash_info.minEraseSize;
			start_addr += flash_info.minEraseSize;
		}
		nvm_free_all_nodes(&handle->id_list);
        handle->list_size = 0;
	}
}

void nvm_init_flash(void)
{
	wise_flash_get_info(&flash_info);
}

void nvm_init_instance(nvm_handle_t handle)
{
	instance_hdr_t hdr;

	wise_flash_read(handle->start_addr, (uint8_t *)&hdr, sizeof(hdr));

	if((hdr.sig != NVM_SIGNATURE) || (hdr.length != handle->length))
	{
		nvm_erase_instance_section(handle);

		hdr.sig = NVM_SIGNATURE;
		hdr.length = handle->length;
		wise_flash_write(handle->start_addr, (uint8_t *)&hdr, sizeof(hdr));
	}
}

void nvm_delete_instance(nvm_handle_t handle)
{
    if(handle && handle->status == NVM_INSTANCE_ACTIVE)
    {
    	nvm_erase_instance_section(handle);

        handle->status = NVM_INSTANCE_INVALID;
    }
}

nvm_handle_t nvm_create_instance(uint32_t start_addr, uint32_t length)
{
	uint32_t new_start = start_addr;
	uint32_t new_end = start_addr + length - 1;
	int i;

    if(length == 0 || instance_count >= MAX_NVM_INSTANCES ||
    		(start_addr + length) > (flash_info.flashSize - flash_info.minEraseSize * 2))
    {
        return NULL;
    }

    if((start_addr % flash_info.minEraseSize != 0) || (length % flash_info.minEraseSize != 0))
    {
    	return NULL;
    }

    for(int i = 0; i < MAX_NVM_INSTANCES; i++)
    {
    	if(nvm_instances[i].status == NVM_INSTANCE_ACTIVE)
    	{
    		if(nvm_instances[i].start_addr == start_addr && nvm_instances[i].length == length)
    		{
    			return NULL;
    		}

    		uint32_t exist_start = nvm_instances[i].start_addr;
    		uint32_t exist_end = exist_start + nvm_instances[i].length - 1;

        	if(!(new_end < exist_start || new_start > exist_end))
        	{
        		return NULL;
        	}
        }
    }

    for(i = 0; i < MAX_NVM_INSTANCES; i++)
    {
    	if(nvm_instances[i].status == NVM_INSTANCE_INVALID)
    	{
    		break;
    	}
    }

   	nvm_instances[i].status = NVM_INSTANCE_ACTIVE;
   	nvm_instances[i].start_addr = start_addr;
   	nvm_instances[i].length = length;
   	nvm_instances[i].free_offset = start_addr + sizeof(instance_hdr_t);
   	nvm_handle_t handle = &nvm_instances[i];

   	nvm_init_instance(handle);

   	nvm_create_nodes(handle);

   	instance_count++;

   	return handle;
}

void nvm_create_nodes(nvm_handle_t handle)
{
	object_info_t obj_data;
	uint32_t addr;

	if(handle == NULL)
	{
		return;
	}

	addr = handle->start_addr + sizeof(instance_hdr_t);
	handle->list_size = 0;

	while (1)
	{
		wise_flash_read(addr, (uint8_t *)&obj_data, sizeof(obj_data));

		if(obj_data.id == 0xFFF)
		{
		    break;
		}

		if(obj_data.status == NVM_DATA_ACTIVED)
		{
		    handle->list_size = nvm_append_or_update_node(
		        &handle->id_list,
		        obj_data.id,
		        obj_data.data_len,
		        addr
		    );
		}

        addr += sizeof(obj_data) + obj_data.data_len;
	}
	handle->free_offset = addr;
}

uint16_t nvm_append_or_update_node(node_t** head, uint16_t id, uint16_t size, uint32_t offset)
{
    node_t* current = *head;
    node_t* last = NULL;
    bool found = false;
    uint16_t count = 0;

    while(current != NULL)
    {
        count++;

        if(current->id == id)
        {
            current->size = size;
            current->offset = offset;
            found = true;
        }

        last = current;
        current = current->next;
    }

    if(!found)
    {
        node_t* new_node = (node_t*)malloc(sizeof(node_t));
        if(!new_node)
        {
            return count;
        }

        new_node->id = id;
        new_node->size = size;
        new_node->offset = offset;
        new_node->next = NULL;

        if(*head == NULL)
        {
            *head = new_node;
        }
        else
        {
            last->next = new_node;
        }

        count++;
    }

    return count;
}

node_t* nvm_find_node_by_id(node_t* head, uint16_t id)
{
    while(head != NULL)
    {
        if(head->id == id)
        {
            return head;
        }
        head = head->next;
    }

    return NULL;
}

void nvm_free_all_nodes(node_t** head)
{
    if(head == NULL || *head == NULL)
    {
        return;
    }

    node_t* current = *head;
    while(current)
    {
        node_t* temp = current;
        current = current->next;
        free(temp);
    }

    *head = NULL;
}

void nvm_remove_node_by_id(nvm_handle_t handle, uint16_t id)
{
    if(handle == NULL || handle->id_list == NULL)
    {
        return;
    }

    node_t* current = handle->id_list;
    node_t* prev = NULL;

    while(current != NULL)
    {
        if(current->id == id)
        {
            if(prev == NULL)
            {
                handle->id_list = current->next;
            }
            else
            {
                prev->next = current->next;
            }

            free(current);

            if(handle->list_size > 0)
            {
                handle->list_size--;
            }

            return;
        }

        prev = current;
        current = current->next;
    }
}

void nvm_list_all_nodes(nvm_handle_t handle)
{
	node_t* curr = handle->id_list;

	if(handle->status == NVM_INSTANCE_ACTIVE)
	{
		printf("\nInstance :\n");
		printf("  Start address : %08x - %08x\n", (unsigned int)handle->start_addr, (unsigned int)handle->length);
		while(curr)
		{
			printf("  ID: %u, Size: %u\n", curr->id, curr->size);
			curr = curr->next;
		}
	}
}

int nvm_gc_block(nvm_handle_t handle)
{
    uint32_t block_size = flash_info.minEraseSize;
    uint32_t gc_addr = flash_info.flashSize - (flash_info.minEraseSize * 2);
    uint32_t block_start = handle->start_addr;
    uint32_t goal_addr = block_start;
    node_t* current = handle->id_list;

    while(block_start < (handle->start_addr + handle->length))
    {
        uint32_t block_end = block_start + block_size;
        uint32_t write_ptr = gc_addr;
        uint32_t used_in_block = 0;
        wise_flash_sector_erase(gc_addr);

        while(current)
        {
            if((current->offset >= block_start) && (current->offset < block_end))
            {
                uint32_t obj_size = sizeof(object_info_t) + current->size;
                used_in_block += obj_size;
                if(used_in_block > block_size)
                {
                	return NVM_ACTION_FAIL;
                }

                uint8_t* buf = malloc(obj_size);
                if(!buf)
                {
                	return NVM_ACTION_FAIL;
                }

                wise_flash_read(current->offset, buf, obj_size);
                wise_flash_write(write_ptr, buf, obj_size);
                write_ptr += obj_size;
                free(buf);
                current = current->next;
            }
            else
            {
            	break;
            }
        }

        wise_flash_sector_erase(block_start);

        if(block_start == handle->start_addr)
        {
            instance_hdr_t hdr = {
                .sig = NVM_SIGNATURE,
                .length = handle->length
            };
            wise_flash_write(goal_addr, (uint8_t *)&hdr, sizeof(hdr));
            goal_addr += sizeof(hdr);
        }

        uint32_t src = gc_addr;
        while(src < write_ptr)
        {
            object_info_t hdr;
            wise_flash_read(src, (uint8_t*)&hdr, sizeof(hdr));
            uint32_t obj_size = sizeof(hdr) + hdr.data_len;

            uint8_t* buf = malloc(obj_size);
            if(!buf)
            {
            	return NVM_ACTION_FAIL;
            }

            wise_flash_read(src, buf, obj_size);
            wise_flash_write(goal_addr, buf, obj_size);
            src += obj_size;
            goal_addr += obj_size;
            free(buf);
        }

        block_start += block_size;
    }

    return NVM_ACTION_OK;
}
