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

/*
 * The NOR flash memory is organized as 256 (64KB) main sectors
 * that are further divided into 16 subsectors each (4096 subsectors
 * in total). The memory can be erased one 4KB subsector at a time,
 * 64KB sectors at a time, or as a whole. Erasing the memory turns
 * bits on ('1'). Writing memory drives them off ('0').
 * Erasing flash causes wear.
 */

#include "user_settings.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "nor_map.h"

static uint32_t current_address = 0;
const static uint32_t END_PATTERN = 0xBEEFADDF;

// memory mapped

// is this hackish? need a zero buffer
const static uint32_t data_size = (sizeof(settings_t) + 4);
static uint8_t* zero_buf;

void initialize_settings()
{
//	if (BSP_QSPI_Erase_Chip() != QSPI_OK) {
//		BSP_LED_On(LED_ORANGE);
//	}

	// strategy is to zero data once discarded
	zero_buf = malloc(data_size);
	memset(zero_buf, 0, data_size);

	// memory mapped mode is relatively fast and can loop by
	// 4 bytes through the entire 16MB NOR flash in 5 seconds
	BSP_QSPI_EnableMemoryMappedMode();

	// find the end of zeros going forward...
	uint32_t data_start_addr = START_MMP_NOR_ADDR;
	while (data_start_addr < END_MMP_NOR_ADDR) {
		uint32_t data = *(uint32_t*) data_start_addr;

		// found the start of data
		if (data != 0) {
			break;
		} else {
			data_start_addr += 4;
		}
	}

	// find the last END_PATTERN going in reverse
	// use the zeros as a cheat so we don't need to traverse the entire
	// flash memory

	// uint32_t readaddr = END_MMP_NOR_ADDR - 4;
	uint32_t readaddr = data_start_addr + SUBSECTOR_SIZE;
	while (readaddr > START_MMP_NOR_ADDR) {
		uint32_t data = *(uint32_t*) readaddr;
		if (data == END_PATTERN) {
			BSP_LED_On(LED_BLUE);
			current_address = readaddr - START_MMP_NOR_ADDR;
			current_address -= sizeof(settings_t);
			break;
		} else {
			BSP_LED_On(LED_GREEN);
			readaddr -= 4;
		}
	}

	BSP_QSPI_Init();
}

void save_settings(const settings_t *settings)
{
	BSP_LED_Off(LED_ORANGE);
	BSP_LED_Off(LED_GREEN);

	uint32_t old_address = current_address;
	current_address += (sizeof(settings_t) + 4);

//	if (BSP_QSPI_Erase_Block(current_address) != QSPI_OK) {
//		BSP_LED_On(LED_ORANGE);
//	} else {
		if (BSP_QSPI_Write((uint8_t*) settings, current_address, sizeof(settings_t)) != QSPI_OK) {
			BSP_LED_On(LED_ORANGE);
		} else {
			if (BSP_QSPI_Write((uint8_t*) &END_PATTERN, current_address + sizeof(settings_t),
					sizeof(END_PATTERN)) != QSPI_OK) {
				BSP_LED_On(LED_ORANGE);
			} else {
				BSP_LED_On(LED_GREEN);
			}
		}

		// zero out previous flash data
		if (BSP_QSPI_Write((uint8_t*) zero_buf, old_address, data_size) != QSPI_OK) {
			BSP_LED_On(LED_ORANGE);
		}

		// test
		//uint8_t buff [4096];
		//if (BSP_QSPI_Read((uint8_t*) &buff, 0, sizeof(buff)) != QSPI_OK) {
		//	BSP_LED_On(LED_ORANGE);
		//}

//	}
}

void get_settings(settings_t *settings)
{
	BSP_LED_Off(LED_ORANGE);
	BSP_LED_Off(LED_GREEN);

	settings_t temp_settings;
	initialize_settings();

	if (BSP_QSPI_Read((uint8_t*) &temp_settings, current_address,
			sizeof(settings_t)) != QSPI_OK) {
		BSP_LED_On(LED_ORANGE);
	} else {
		uint32_t end;
		if (BSP_QSPI_Read((uint8_t*) &end, current_address + sizeof(settings_t),
				sizeof(end)) != QSPI_OK) {
			BSP_LED_On(LED_ORANGE);
		} else {
			if (end == END_PATTERN) {
				BSP_LED_On(LED_GREEN);
				memcpy(settings, &temp_settings, sizeof(settings_t));
			} else {
				BSP_LED_On(LED_ORANGE);
			}
		}
	}
}

