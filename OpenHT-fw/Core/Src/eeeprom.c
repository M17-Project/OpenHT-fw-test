/*
 * Copyright (C) 2023 M17 Project and contributors
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */


#include "eeeprom.h"

#include "main.h"

#include "../shell/inc/sys_command_line.h"
#include "cmsis_os2.h"
#include "FreeRTOS.h"

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

/* Types definitions */
struct s_EEPROMInternalData{
	uint16_t	max_nb_entries;
	uint8_t 	active_page; 		/*< active page index. First page is thus at 0*/
	uint8_t 	actual_entry_size;
	uint32_t 	current_address;   /*< Contains the address of the next empty entry in the active page*/
};


/* variables */
extern RNG_HandleTypeDef hrng;


/* Functions definitions */
bool _move_active_page(EEEPROMHandle_t *handle);


/* Functions declarations */
bool EEEPROM_init(EEEPROMHandle_t *handle){
	/* Check the different lengths/sizes/...*/
	LOG(CLI_LOG_EEEPROM, "Start EEEPROM init.\r\n");
	if(handle->erase_page == NULL){
		return EXIT_FAILURE;
	}else if( (handle->data_size + handle->address_size) > handle->page_size){
		return EXIT_FAILURE;
	}

	// Check that both or none of the write/read functions are set
	if(!handle->read ^ !handle->write){
		return EXIT_FAILURE;
	}

	// Check if the erase_page function is set
	if(!handle->erase_page){
		return EXIT_FAILURE;
	}

	// Check if the start address is consistent with the page size
	if((uint32_t)(handle->start_address) % handle->page_size){
		return EXIT_FAILURE;
	}

	// Check if the number of pages is correct
	if(handle->number_pages <= 1){
		return EXIT_FAILURE;
	}

	// allocate the memory for the internal data
	handle->priv = (EEPROMInternalData_t *)pvPortMalloc(sizeof(EEPROMInternalData_t));
	if(!handle->priv){
		return EXIT_FAILURE;
	}

	// Round size with respect to alignment
	uint16_t entry_size = handle->address_size + handle->data_size;
	if(entry_size % handle->alignment){
		handle->priv->actual_entry_size = (1+entry_size/handle->alignment)*handle->alignment;
	}else{
		handle->priv->actual_entry_size = entry_size;
	}

	handle->priv->max_nb_entries = handle->page_size/handle->priv->actual_entry_size;

	// Determine which page is the active one
	handle->priv->active_page = handle->number_pages;
	for(unsigned int i = 0; i < handle->number_pages; i++){
		if(handle->read){
			uint32_t data = 0;
			handle->read((uint8_t *)(&data), handle->start_address + i*handle->page_size, 4);
			if(data == 0xFFFFFFFF){
				LOG(CLI_LOG_EEEPROM, "page %u is empty.\r\n", i);
				continue;
			}else{
				LOG(CLI_LOG_EEEPROM, "page %u set as active page.\r\n", i);
				handle->priv->active_page = i;
				break;
			}
		}else{
			uint32_t *data = (uint32_t *)(handle->start_address + i*handle->page_size);
			if(*data == 0xFFFFFFFF){
				continue;
			}else{
				handle->priv->active_page = i;
			}
		}
	}

	if(handle->priv->active_page >= handle->number_pages){
		//Randomly pick a page to get started
		LOG(CLI_LOG_EEEPROM, "No active page set. Picking one randomly.\r\n");
		handle->priv->active_page = HAL_RNG_GetRandomNumber(&hrng)%handle->number_pages;
	}

	// For resilience we can check that the previous page is empty.
	// If it is not, it means that the previous page move was interrupted and should be re-done
	bool prev_page_corrupted = false;
	if(handle->priv->active_page == 0){
		if(handle->read){
			uint32_t data = 0;
			handle->read((uint8_t *)(&data), handle->start_address + (handle->page_size * (handle->number_pages - 1)), 4);
			if(data != 0xFFFFFFFF){
				LOG(CLI_LOG_EEEPROM, "Previous page corrupted...\r\n");
				prev_page_corrupted = true;
				handle->priv->active_page = handle->number_pages-1;
			}
		}else{
			uint32_t *data = (uint32_t *)(handle->start_address + (handle->page_size * (handle->number_pages - 1)));
			if(*data != 0xFFFFFFFF){
				prev_page_corrupted = true;
				handle->priv->active_page = handle->number_pages-1;
			}
		}
	}


	// Find the last entry
	uint32_t mask = 0xFFFFFFFF;
	switch(handle->address_size){
	case EEEPROM_ADDRESS_1BYTE:
		mask = 0xFF;
		break;
	case EEEPROM_ADDRESS_2BYTES:
		mask = 0xFFFF;
		break;
	default:
		break;
	}
	handle->priv->current_address = handle->start_address + handle->page_size * handle->priv->active_page;
	uint32_t index = 0;
	while(index < handle->page_size){
		uint32_t temp = 0;
		if(handle->read){
			handle->read((uint8_t *)(&temp), handle->priv->current_address, handle->address_size);
		}else{
			temp = *(uint32_t *)(handle->priv->current_address);
			temp >>= 8*(4 - handle->address_size);
		}
		if(temp == mask){
			// This entry is empty
			break;
		}
		handle->priv->current_address += handle->priv->actual_entry_size;
		index += handle->priv->actual_entry_size;
	}
	LOG(CLI_LOG_EEEPROM, "Current address set at 0x%04lx.\r\n", handle->priv->current_address);

	// if the previous page (which is now the current page) is corrupted, we have to re-do the move
	if(prev_page_corrupted){
		_move_active_page(handle);
	}

	return EXIT_SUCCESS;
}

bool EEEPROM_delete(EEEPROMHandle_t *handle){
	vPortFree(handle->priv);

	return EXIT_SUCCESS;
}

bool EEEPROM_write_data(EEEPROMHandle_t *handle, uint32_t address, const void *data){
	/* Check if this entry would go beyond the page limit */
	if(handle->priv->current_address + handle->priv->actual_entry_size
			> handle->start_address + (handle->priv->active_page+1)*handle->page_size){
		_move_active_page(handle);
	}

	// Write the address
	if(handle->write){
		handle->write((uint8_t *)(&address), handle->priv->current_address, (uint32_t)handle->address_size);
	}else{
		switch(handle->address_size){
		case EEEPROM_ADDRESS_1BYTE:
			*((uint8_t *)(handle->priv->current_address)) = (uint8_t) address;
			break;
		case EEEPROM_ADDRESS_2BYTES:
			*((uint16_t *)(handle->priv->current_address)) = (uint16_t) address;
			break;
		case EEEPROM_ADDRESS_4BYTES:
			*((uint32_t *)(handle->priv->current_address)) = (uint32_t) address;
			break;
		}
	}

	// Write the data
	uint32_t write_address = handle->priv->current_address + handle->address_size;
	if(handle->write){
		handle->write((uint8_t *)data, write_address, handle->data_size);
	}else{
		memcpy((void *)write_address, data, handle->data_size);
	}
	handle->priv->current_address += handle->priv->actual_entry_size;

	return EXIT_SUCCESS;
}

bool EEEPROM_read_data(EEEPROMHandle_t *handle, uint32_t address, void *data){
	int32_t index = handle->priv->current_address - handle->priv->actual_entry_size;

	// Go backward in the active page until we find the requested address
	while( (index > 0) && (index >= handle->start_address + (handle->priv->active_page * handle->page_size) ) ){
		uint32_t temp = 0;
		if(handle->read){
			handle->read((uint8_t *)&temp, index, handle->address_size);
		}else{
			temp = *(uint32_t *)(handle->priv->current_address);
			temp >>= 8*(4 - handle->address_size);
		}

		if(temp == address){
			LOG(CLI_LOG_EEEPROM, "Found address %ld at address 0x%04lx.\r\n", address, index);
			if(handle->read){
				handle->read(data, index + handle->address_size, handle->data_size);
			}else{
				memcpy(data, (void *)(index + handle->address_size), handle->data_size);
			}
			return EXIT_SUCCESS;
		}

		index -= handle->priv->actual_entry_size;
	}

	// no entry found for this address
	return EXIT_FAILURE;
}

bool EEEPROM_erase(EEEPROMHandle_t *handle){
	for(uint8_t i = 0; i < handle->number_pages; i++){
		handle->erase_page(handle->page_offset + i);
	}

	return EXIT_SUCCESS;
}

bool _move_active_page(EEEPROMHandle_t *handle){
	uint8_t dest_page = (handle->priv->active_page + 1)%handle->number_pages;

	//uint8_t *destination = (uint8_t *)(handle->start_address + handle->page_size * dest_page);
	uint8_t *source = (uint8_t *)(handle->priv->current_address - handle->priv->actual_entry_size);
	uint32_t found_addresses_len = 0;
	uint8_t *found_addresses = pvPortMalloc(handle->address_size * (handle->page_size / handle->priv->actual_entry_size));
	if(!found_addresses){
		return EXIT_FAILURE;
	}
	uint8_t *buffer = pvPortMalloc(handle->address_size + handle->data_size);
	if(!buffer){
		vPortFree(found_addresses);
		return EXIT_FAILURE;
	}

	handle->priv->current_address = handle->start_address + handle->page_size*dest_page;

	/* Scroll through the source page */
	for(uint32_t i = 0; i < handle->priv->max_nb_entries; i++){
		uint32_t temp = 0; // address read
		if(handle->read){
			handle->read((uint8_t *)(&temp), (uint32_t)source, handle->address_size);
		}else{
			temp = *(uint32_t *)(source);
			temp >>= 8*(4 - handle->address_size);
		}

		// Check if the address read have been read before
		bool found = false;
		for(uint32_t j = 0; j < found_addresses_len; j += handle->address_size){
			switch(handle->address_size){
			case EEEPROM_ADDRESS_1BYTE:
				if(temp == found_addresses[j]){
					found = true;
				}
				break;
			case EEEPROM_ADDRESS_2BYTES:
				if(temp == *(uint16_t *)(found_addresses + j)){
					found = true;
				}
				break;
			case EEEPROM_ADDRESS_4BYTES:
				if(temp == *(uint32_t *)(found_addresses + j)){
					found = true;
				}
				break;
			}
			if(found){
				break;
			}
		}

		/* Append found address to the list then copy it to the next page */
		if(!found){
			switch(handle->address_size){
			case EEEPROM_ADDRESS_1BYTE:
				found_addresses[found_addresses_len] = (uint8_t)(temp);
				break;
			case EEEPROM_ADDRESS_2BYTES:
				*((uint16_t *)(found_addresses + found_addresses_len)) = (uint16_t)(temp);
				break;
			case EEEPROM_ADDRESS_4BYTES:
				*((uint32_t *)(found_addresses + found_addresses_len)) = temp;
				break;
			}
			found_addresses_len += handle->address_size;

			if(handle->read){
				handle->read(buffer, (uint32_t)source, handle->address_size + handle->data_size);
				handle->write(buffer, handle->priv->current_address, handle->address_size + handle->data_size);
			}else{
				memcpy((void *)handle->priv->current_address, source, handle->address_size + handle->data_size);
			}

			handle->priv->current_address += handle->priv->actual_entry_size;
		}
		source -= handle->priv->actual_entry_size;
	}

	handle->erase_page(handle->priv->active_page + handle->page_offset);
	handle->priv->active_page = dest_page;
	vPortFree(buffer);
	vPortFree(found_addresses);
	return EXIT_SUCCESS;
}
