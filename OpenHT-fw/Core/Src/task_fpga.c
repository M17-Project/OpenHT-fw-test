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

#include "task_fpga.h"
#include <stdlib.h>
#include <fatfs.h>
#include <cmsis_os2.h>
#include "eeeprom.h"
#include "eeeprom_hal.h"
#include "nor_map.h"


typedef struct __attribute((__packed__)){
	uint32_t start_sector:24;
	uint32_t size;
}fpga_bin_entry_t;

void _erase_subsector(uint32_t subsector);
void _qspi_read(uint8_t *data, uint32_t addr, uint32_t len);
void _qspi_write(uint8_t *data, uint32_t addr, uint32_t len);

void StartTaskFPGA(void *argument) {

	BSP_QSPI_Init(); // Ensure the QSPI is initialized and that it is not in mapped memory mode

	// Init EEEPROM
	EEEPROMHandle_t eeeprom = {
			.start_address = FPGA_BIN_TABLE_START_ADDR,
			.erase_page = EEEPROM_HAL_erase_subsector,
			.read = EEEPROM_HAL_qspi_read,
			.write = EEEPROM_HAL_qspi_write,
			.page_offset = FPGA_BIN_TABLE_START_SECTOR,
			.page_size = SUBSECTOR_SIZE,
			.number_pages = 2,
			.data_size = 7,
			.address_size = 1,
			.alignment = 4,
	};

	if(EEEPROM_init(&eeeprom) == EXIT_FAILURE){
		printf("Error initializing fpga binaries EEEPROM.\r\n");
	}

	/*fpga_bin_entry_t e;
	for(size_t i = 1; i <= 600; i++){
		e.start_sector = FPGA_BIN_STORAGE_START_SECTOR + i*10;
		e.size = i + i * i;

		EEEPROM_write_data(&eeeprom, (i%5), (void *)(&e));
		if(!(i%16)){
			BSP_QSPI_EnableMemoryMappedMode();
			__NOP();
			BSP_QSPI_Init();
		}
	}

	EEEPROM_read_data(&eeeprom, 3, &e);
	BSP_QSPI_MemoryMappedMode();*/

	for(;;){

		osDelay(100);
	}

}

bool load_binary_file(const char *filename, uint16_t index){
	FIL bin_file;
	FRESULT f_result;

	f_result = f_open(&bin_file, filename, FA_READ | FA_OPEN_EXISTING);
	if(f_result != FR_OK){
		printf("[FATFS] Error opening file %s.\n", filename);
		return EXIT_FAILURE;
	}

	// TODO

	return EXIT_SUCCESS;
}

