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
#include "../shell/inc/sys_command_line.h"

typedef struct __attribute((__packed__)){
	uint32_t start_sector:24;
	uint32_t size;
}fpga_bin_entry_t;

osThreadId_t 				FPGA_thread_id 			= NULL;
uint32_t 					bitstream_load_address 	= 0x80000000;
uint32_t 					bitstream_load_offset 	= 0;
extern SPI_HandleTypeDef 	hspi1;
extern osMutexId_t 			SPI1AccessHandle;
extern osMutexId_t 			NORAccessHandle;


UINT 			fatfs_bitstream_stream		(const BYTE *p, UINT btf);
void 			_fpga_spi_complete_callback	(SPI_HandleTypeDef *hspi);

/* Event flags */
#define FPGA_SEND_SAMPLES	(1 << 0)
#define FPGA_FETCH_IQ		(1 << 1)
#define FPGA_UPLOAD_BIN		(1 << 2)
#define FPGA_DOWNLOAD_BIN	(1 << 3)
#define FPGA_RESET			(1 << 4)
#define FPGA_ERASE_STORAGE	(1 << 5)
#define FPGA_ALL_FLAGS		(FPGA_SEND_SAMPLES | FPGA_FETCH_IQ | FPGA_UPLOAD_BIN | FPGA_DOWNLOAD_BIN | FPGA_RESET | FPGA_ERASE_STORAGE)

#define FPGA_SPI_ISR_DONE	(1 << 30)

#define SPI_PORT_ACTIVATION_KEY	0x8AF4C6A4 // Swapped for endianness

void StartTaskFPGA(void *argument) {

	// Init EEEPROM
	//BSP_QSPI_EnableMemoryMappedMode();
	//BSP_QSPI_Init();
	EEEPROMHandle_t eeeprom = {
			.start_address = FPGA_BIN_TABLE_START_ADDR,
			.erase_page = EEEPROM_HAL_erase_subsector,
			.read = EEEPROM_HAL_qspi_read,
			.write = EEEPROM_HAL_qspi_write,
			.page_offset = FPGA_BIN_TABLE_START_SECTOR,
			.page_size = SUBSECTOR_SIZE,
			.number_pages = FPGA_BIN_TABLE_NB_SECTORS,
			.data_size = 7,
			.address_size = EEEPROM_ADDRESS_1BYTE,
			.alignment = 4,
	};

	if(EEEPROM_init(&eeeprom) == EXIT_FAILURE){
		ERR("Error initializing fpga binaries EEEPROM.\r\n");
	}

	FPGA_thread_id = osThreadGetId();

	// Init GPIOs
	HAL_GPIO_WritePin(FPGA_PROGRAMN_GPIO_Port, 	FPGA_PROGRAMN_Pin, 	GPIO_PIN_RESET);
	HAL_GPIO_WritePin(FPGA_NSS_GPIO_Port, 		FPGA_NSS_Pin, 		GPIO_PIN_SET);
	HAL_GPIO_WritePin(FPGA_RST_GPIO_Port, 		FPGA_RST_Pin, 		GPIO_PIN_SET);

	for(;;){
		uint32_t flag = osThreadFlagsWait(FPGA_ALL_FLAGS, osFlagsNoClear, osWaitForever);

		if(flag & FPGA_SEND_SAMPLES){

		}else if(flag & FPGA_FETCH_IQ){

		}else if(flag & FPGA_UPLOAD_BIN){
			osThreadFlagsClear(FPGA_UPLOAD_BIN);				// Clear the flag
			osPriority_t prev_prio = osThreadGetPriority(NULL); // Lower task's priority
			osThreadSetPriority(NULL, osPriorityBelowNormal);
			LOG(CLI_LOG_FPGA, "Started FPGA bitstream upload\r\n");

			uint8_t bufferTX[16];	// Used for data to TX
			uint8_t bufferRX[16];

			// First set PROGRAMN low
			HAL_GPIO_WritePin(FPGA_PROGRAMN_GPIO_Port, FPGA_PROGRAMN_Pin, GPIO_PIN_RESET);
			osDelay(1); // Must wait for at least 50ns

			// Drive SCSN low then clock in the key
			osMutexAcquire(SPI1AccessHandle, osWaitForever);						// Exclusive access to SPI1
			hspi1.TxCpltCallback 	= _fpga_spi_complete_callback;					// Set SPI callbacks
			hspi1.RxCpltCallback 	= _fpga_spi_complete_callback;
			hspi1.TxRxCpltCallback 	= _fpga_spi_complete_callback;
			HAL_GPIO_WritePin(FPGA_NSS_GPIO_Port, FPGA_NSS_Pin, GPIO_PIN_RESET);	// Slave select lines
			HAL_GPIO_WritePin(XCVR_NSS_GPIO_Port, XCVR_NSS_Pin, GPIO_PIN_SET);
			bufferTX[0] = 0;
			*(uint32_t *)(bufferTX+1) = SPI_PORT_ACTIVATION_KEY;
			HAL_SPI_Transmit_IT(&hspi1, bufferTX, 5);
			osThreadFlagsWait(FPGA_SPI_ISR_DONE, 0, osWaitForever);					// Wait for TX to be done
			HAL_GPIO_WritePin(FPGA_NSS_GPIO_Port, FPGA_NSS_Pin, GPIO_PIN_SET);

			osDelay(50);

			// Clock in READ-ID cmd and read 32 bits IDCODE
			memset(bufferTX, 0, 8);
			bufferTX[0] = 0xE0;
			LOG(CLI_LOG_FPGA, "Reading IDCODE.\r\n");
			HAL_GPIO_WritePin(FPGA_NSS_GPIO_Port, FPGA_NSS_Pin, GPIO_PIN_RESET);
			HAL_SPI_TransmitReceive_IT(&hspi1, bufferTX, bufferRX, 8);
			osThreadFlagsWait(FPGA_SPI_ISR_DONE, 0, osWaitForever);
			HAL_GPIO_WritePin(FPGA_NSS_GPIO_Port, FPGA_NSS_Pin, GPIO_PIN_SET);
			LOG(CLI_LOG_FPGA, "FPGA IDCODE is 0x%08lx.\r\n", *(uint32_t *)(bufferRX + 4));

			// ISC-Enable
			bufferTX[0] = 0xC6;
			LOG(CLI_LOG_FPGA, "Sending ISC_ENABLE.\r\n");
			HAL_GPIO_WritePin(FPGA_NSS_GPIO_Port, FPGA_NSS_Pin, GPIO_PIN_RESET);
			HAL_SPI_Transmit_IT(&hspi1, bufferTX, 4);
			osThreadFlagsWait(FPGA_SPI_ISR_DONE, 0, osWaitForever);
			HAL_GPIO_WritePin(FPGA_NSS_GPIO_Port, FPGA_NSS_Pin, GPIO_PIN_SET);

			// ISC-Erase
			bufferTX[0] = 0x0E;
			LOG(CLI_LOG_FPGA, "Sending ISC_ERASE.\r\n");
			HAL_GPIO_WritePin(FPGA_NSS_GPIO_Port, FPGA_NSS_Pin, GPIO_PIN_RESET);
			HAL_SPI_Transmit_IT(&hspi1, bufferTX, 4);
			osThreadFlagsWait(FPGA_SPI_ISR_DONE, 0, osWaitForever);
			HAL_GPIO_WritePin(FPGA_NSS_GPIO_Port, FPGA_NSS_Pin, GPIO_PIN_SET);


			// LSC-CHECK-BUSY (5 bits)
			bufferTX[0] = 0xF0;
			LOG(CLI_LOG_FPGA, "Waiting until FPGA is not busy.\r\n");
			while(true){ // Wait for device to not be busy anymore
				HAL_GPIO_WritePin(FPGA_NSS_GPIO_Port, FPGA_NSS_Pin, GPIO_PIN_RESET);
				HAL_SPI_TransmitReceive_IT(&hspi1, bufferTX, bufferRX, 5);
				osThreadFlagsWait(FPGA_SPI_ISR_DONE, 0, osWaitForever);
				HAL_GPIO_WritePin(FPGA_NSS_GPIO_Port, FPGA_NSS_Pin, GPIO_PIN_SET);
				break;
				if(bufferRX[4] == 1){
					LOG(CLI_LOG_FPGA, "FPGA not busy anymore.\r\n");
					break;
				}else{
					osDelay(1);
				}
			}

			// LSC-Bitstream-burst followed by bitstream
			fpga_bin_entry_t e;
			EEEPROM_read_data(&eeeprom, 0, &e);
			uint8_t *bitstream = (uint8_t *)(e.start_sector*SUBSECTOR_SIZE);
			bufferTX[0] = 0x7A;
			LOG(CLI_LOG_FPGA, "Sending LSC_BITSTREAM_BURST.\r\n");
			HAL_GPIO_WritePin(FPGA_NSS_GPIO_Port, FPGA_NSS_Pin, GPIO_PIN_RESET);
			HAL_SPI_Transmit_IT(&hspi1, bufferTX, 4);
			osThreadFlagsWait(FPGA_SPI_ISR_DONE, 0, osWaitForever);

			osMutexAcquire(NORAccessHandle, 150);
			BSP_QSPI_EnableMemoryMappedMode();	// Enable memory mapped mode to read the bitstream

			uint32_t sent = 0;
			while(sent < e.size){
				uint32_t bytes_left = e.size - sent;
				uint16_t to_send = (UINT16_MAX < bytes_left)?UINT16_MAX:bytes_left;
				HAL_SPI_Transmit_DMA(&hspi1, bitstream + sent, to_send);
				LOG(CLI_LOG_FPGA, "Sent %lu bitstream bytes.\r\n", sent);
				osThreadFlagsWait(FPGA_SPI_ISR_DONE, 0, osWaitForever);
				sent += to_send;
			}
			HAL_GPIO_WritePin(FPGA_NSS_GPIO_Port, FPGA_NSS_Pin, GPIO_PIN_SET);
			BSP_QSPI_Init();					// set the NOR flash back in indirect access mode
			osMutexRelease(NORAccessHandle);

			// Check status register
			bufferTX[0] = 0x3C;
			HAL_GPIO_WritePin(FPGA_NSS_GPIO_Port, FPGA_NSS_Pin, GPIO_PIN_RESET);
			HAL_SPI_TransmitReceive_IT(&hspi1, bufferTX, bufferRX, 12);
			osThreadFlagsWait(FPGA_SPI_ISR_DONE, 0, osWaitForever);
			HAL_GPIO_WritePin(FPGA_NSS_GPIO_Port, FPGA_NSS_Pin, GPIO_PIN_SET);
			LOG(CLI_LOG_FPGA, "Status register is 0x%08lx%08lx.\r\n",
					*(uint32_t *)bufferRX + 4, *(uint32_t *)bufferRX + 8);

			// ISC-Disable
			bufferTX[0] = 0x26;
			LOG(CLI_LOG_FPGA, "Sending ISC_DISABLE.\r\n");
			HAL_GPIO_WritePin(FPGA_NSS_GPIO_Port, FPGA_NSS_Pin, GPIO_PIN_RESET);
			HAL_SPI_Transmit_IT(&hspi1, bufferTX, 4);
			osThreadFlagsWait(FPGA_SPI_ISR_DONE, 0, osWaitForever);
			HAL_GPIO_WritePin(FPGA_NSS_GPIO_Port, FPGA_NSS_Pin, GPIO_PIN_SET);

			LOG(CLI_LOG_FPGA, "FPGA upload done.\r\n");

			// Release SPI mutex and restore thread priority
			hspi1.TxCpltCallback 	= NULL;					// Set SPI callbacks
			hspi1.RxCpltCallback 	= NULL;
			hspi1.TxRxCpltCallback 	= NULL;
			osMutexRelease(SPI1AccessHandle);
			osThreadSetPriority(FPGA_thread_id, prev_prio);
		}else if(flag & FPGA_DOWNLOAD_BIN){
			osThreadFlagsClear(FPGA_DOWNLOAD_BIN);				// Clear the flag
			osPriority_t prev_prio = osThreadGetPriority(NULL); // Lower task's priority
			osThreadSetPriority(NULL, osPriorityBelowNormal);
			LOG(CLI_LOG_FPGA, "Started FPGA bitstream download...\r\n");

			// Download BIN image from SD card to NOR flash
			// Open bitstream file
			FIL bin_file;
			fpga_bin_entry_t bin_entry;
			FSIZE_t file_size;
			UINT btf;

			if(f_open(&bin_file, "/fpga_00.bit", FA_OPEN_EXISTING | FA_READ) != FR_OK){
				ERR("Error opening file fpga_00.bit.\r\n");
				osThreadSetPriority(NULL, prev_prio);
				continue;
			}

			// Check that the bitstream size fits in the allocated space
			file_size = f_size(&bin_file);
			if(file_size > FPGA_BIN_MAX_SIZE){
				ERR("FPGA bitstream file is too large to fit in the allocated space.\r\n");
				f_close(&bin_file);
				osThreadSetPriority(NULL, prev_prio);
				continue;
			}

			// Check if there already was a bin image
			if(EEEPROM_read_data(&eeeprom, 0, &bin_entry) == EXIT_SUCCESS){
				uint32_t prev_bin_size = bin_entry.size;
				uint32_t nb_sectors = (prev_bin_size/SUBSECTOR_SIZE) + 1*(prev_bin_size%SUBSECTOR_SIZE);
				LOG(CLI_LOG_FPGA, "An image is already stored in memory. Erasing %lu sectors.\r\n", nb_sectors);
				bool error = false;
				for(size_t i = 0; i < nb_sectors; i += 16){
					if(CUSTOM_QSPI_Erase_Sector(FPGA_BIN_STORAGE_START_SECTOR + i*SUBSECTOR_SIZE) != QSPI_OK){
						ERR("Could not erase NOR flash sector %d.\r\n", FPGA_BIN_STORAGE_START_SECTOR + i*SUBSECTOR_SIZE);
						error = true;
						break;
					}
				}
				if(error){
					f_close(&bin_file);
					osThreadSetPriority(NULL, prev_prio);
					continue;
				}
			}else{
				LOG(CLI_LOG_FPGA, "No previous image found.\r\n");
			}

			// Write the entry to the table
			bin_entry.size = file_size;
			bin_entry.start_sector = FPGA_BIN_STORAGE_START_SECTOR;
			if(EEEPROM_write_data(&eeeprom, 0, (void *)(&bin_entry)) == EXIT_FAILURE){
				ERR("Error writing bin file entry to EEEPROM.\r\n");
				f_close(&bin_file);
				osThreadSetPriority(NULL, prev_prio);
				continue;
			}

			// write the file to NOR flash
			bitstream_load_address = FPGA_BIN_STORAGE_START_ADDR;
			bitstream_load_offset = 0;
			f_forward(&bin_file, fatfs_bitstream_stream, file_size, &btf);
			LOG(CLI_LOG_FPGA, "Written %u/%lu bytes of the FPGA bitstream.\r\n", btf, file_size);

			f_close(&bin_file);						// Close file
			osThreadSetPriority(NULL, prev_prio);	// Restore previous priority
		}else if(flag & FPGA_RESET){
			osThreadFlagsClear(FPGA_RESET);
			LOG(CLI_LOG_FPGA, "Soft-reset FPGA.\r\n");

			// Set the FPGA RST pin low for 1ms then high
			HAL_GPIO_WritePin(FPGA_RST_GPIO_Port, FPGA_RST_Pin, GPIO_PIN_RESET);
			osDelay(1);
			HAL_GPIO_WritePin(FPGA_RST_GPIO_Port, FPGA_RST_Pin, GPIO_PIN_SET);
		}else if(flag & FPGA_ERASE_STORAGE){
			osThreadFlagsClear(FPGA_ERASE_STORAGE);				// Clear the flag
			osPriority_t prev_prio = osThreadGetPriority(NULL); // Lower task's priority
			osThreadSetPriority(NULL, osPriorityBelowNormal);

			// Erase the EEEPROM and the binary file storage
			LOG(CLI_LOG_FPGA, "Erasing FPGA storage...\r\n");
			LOG(CLI_LOG_FPGA, "Erasing EEEPROM...\r\n");
			EEEPROM_erase(&eeeprom);
			LOG(CLI_LOG_FPGA, "Erasing bin storage...\r\n");
			for(size_t i = FPGA_BIN_STORAGE_START_SECTOR; i < FPGA_BIN_STORAGE_END_SECTOR; i += 16){
				CUSTOM_QSPI_Erase_Sector(i*SUBSECTOR_SIZE);
			}
			LOG(CLI_LOG_FPGA, "Done!\r\n");

			osThreadSetPriority(FPGA_thread_id, prev_prio);	// Restore the task's previous priority
		}
	}

}

bool upload_fpga_binary(){
	if(FPGA_thread_id){
		uint32_t result = osThreadFlagsSet(FPGA_thread_id, FPGA_UPLOAD_BIN);
		if(result < 0x80000000){
			return EXIT_SUCCESS;
		}else{
			ERR("Could not upload FPGA binary: osThreadFlagsSet returned 0x%08lx.\r\n", result);
		}
	}else{
		ERR("Could not upload FPGA binary file: Thread ID not set.\r\n");
	}

	return EXIT_FAILURE;
}

bool download_fpga_binary_file(){
	if(FPGA_thread_id){
		uint32_t result = osThreadFlagsSet(FPGA_thread_id, FPGA_DOWNLOAD_BIN);
		if(result < 0x80000000){
			return EXIT_SUCCESS;
		}else{
			ERR("Could not download FPGA binary: osThreadFlagsSet returned 0x%08lx.\r\n", result);
		}
	}else{
		ERR("Could not download FPGA binary file: Thread ID not set.\r\n");
	}

	return EXIT_FAILURE;
}

bool erase_fpga_storage(){
	if(FPGA_thread_id){
		uint32_t result = osThreadFlagsSet(FPGA_thread_id, FPGA_ERASE_STORAGE);
		if(result < 0x80000000){
			return EXIT_SUCCESS;
		}else{
			ERR("Could not erase FPGA storage: osThreadFlagsSet returned 0x%08lx.\r\n", result);
		}
	}else{
		ERR("Could not erase FPGA storage: Thread ID not set.\r\n");
	}

	return EXIT_FAILURE;
}

bool fpga_soft_reset(){
	if(FPGA_thread_id){
		uint32_t result = osThreadFlagsSet(FPGA_thread_id, FPGA_RESET);
		if(result < 0x80000000){
			return EXIT_SUCCESS;
		}else{
			ERR("Could not soft reset FPGA: osThreadFlagsSet returned 0x%08lx.\r\n", result);
		}
	}else{
		ERR("Could not soft reset FPGA: Thread ID not set.\r\n");
	}

	return EXIT_FAILURE;
}

UINT fatfs_bitstream_stream(const BYTE *p, UINT btf){
	if(btf == 0){ /* sense call */
		if(bitstream_load_address < 0x80000000){
			return 1;
		}else{
			return 0;
		}
	}else{
		if(bitstream_load_address < 0x80000000){
			BSP_QSPI_Write((uint8_t *)p, bitstream_load_address + bitstream_load_offset, btf);
			bitstream_load_offset += btf;
			return btf;
		}
	}

	return 0;
}

void _fpga_spi_complete_callback(SPI_HandleTypeDef *hspi){
	if(FPGA_thread_id){
		uint32_t result = osThreadFlagsSet(FPGA_thread_id, FPGA_SPI_ISR_DONE);
		if(result > 0x80000000){
			ERR("Could not soft reset FPGA: osThreadFlagsSet returned 0x%08lx.\r\n", result);
		}
	}else{
		ERR("Could not soft reset FPGA: Thread ID not set.\r\n");
	}
}
