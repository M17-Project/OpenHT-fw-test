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


#include "task_radio.h"

#include "main.h"
#include "eeeprom.h"
#include "eeeprom_hal.h"
#include "nor_map.h"
#include "ui/ui_mode_change_panel.h"
#include "ui/ui_fpga_status_panel.h"
#include "openht_types.h"
#include "fpga_reg_defs.h"

#include "../shell/inc/sys_command_line.h"
#include <fatfs.h>
#include <cmsis_os2.h>
#include <stm32f4xx_ll_gpio.h>

#include <stdlib.h>

#ifdef DEBUG
#define WAIT_TIMEOUT osWaitForever
#else
#define WAIT_TIMEOUT 1500
#endif

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
uint32_t 		_fpga_check_status_register(uint8_t *reg);
void 			_fpga_spi_complete_callback	(SPI_HandleTypeDef *hspi);
void 			_fpga_wait_busy();
uint32_t 		_fpga_sspi_classA(uint32_t length, uint8_t *tx, uint8_t *rx);
uint32_t 		_fpga_sspi_classB(uint32_t length, uint8_t *tx);
uint32_t 		_fpga_sspi_classC(uint8_t cmd);
void 			_select_chip();
void 			_release_chip();

/* Event flags */
#define FPGA_SEND_SAMPLES	(1 << 0)
#define FPGA_FETCH_IQ		(1 << 1)
#define FPGA_UPLOAD_BIN		(1 << 2)
#define FPGA_DOWNLOAD_BIN	(1 << 3)
#define FPGA_RESET			(1 << 4)
#define FPGA_ERASE_STORAGE	(1 << 5)
#define RADIO_INITN_CHANGED (1 << 6)
#define XCVR_INIT			(1 << 7)

#define RADIO_ALL_FLAGS		(FPGA_SEND_SAMPLES | FPGA_FETCH_IQ | FPGA_UPLOAD_BIN | FPGA_DOWNLOAD_BIN | FPGA_RESET |\
							 FPGA_ERASE_STORAGE | RADIO_INITN_CHANGED | XCVR_INIT)


#define SPI_PORT_ACTIVATION_KEY	0x8AF4C6A4 // Swapped for endianness

void StartTaskRadio(void *argument) {
	FPGA_thread_id = osThreadGetId();

	// Init EEEPROM
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

	// Init INITN pin (PC4)
	GPIO_InitTypeDef initn_pin = {
			.Mode = GPIO_MODE_IT_RISING_FALLING,
			.Pin = FPGA_INITN_Pin,
			.Speed = GPIO_SPEED_LOW,
			.Pull = GPIO_NOPULL,
	};
	HAL_GPIO_Init(FPGA_INITN_GPIO_Port, &initn_pin);
	HAL_NVIC_SetPriority(EXTI4_IRQn, 5, 0);
	HAL_NVIC_EnableIRQ(EXTI4_IRQn);

	for(;;){
		uint32_t flag = osThreadFlagsWait(RADIO_ALL_FLAGS, osFlagsNoClear, osWaitForever);

		if(flag & FPGA_SEND_SAMPLES){

		}else if(flag & FPGA_FETCH_IQ){

		}else if(flag & RADIO_INITN_CHANGED){
			osThreadFlagsClear(RADIO_INITN_CHANGED);
			GPIO_PinState initn 	= HAL_GPIO_ReadPin(FPGA_INITN_GPIO_Port, FPGA_INITN_Pin);
			GPIO_PinState done		= HAL_GPIO_ReadPin(FPGA_DONE_GPIO_Port, FPGA_DONE_Pin);
			if( (initn == GPIO_PIN_RESET) \
				&& (done == GPIO_PIN_RESET))
			{
				// Looks like everything is off...
				HAL_GPIO_WritePin(MAIN_KILL_GPIO_Port, MAIN_KILL_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(FPGA_PROGRAMN_GPIO_Port, FPGA_PROGRAMN_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(FPGA_NSS_GPIO_Port, FPGA_NSS_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(FPGA_RST_GPIO_Port, FPGA_RST_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(XCVR_NSS_GPIO_Port, XCVR_NSS_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(SW_CTL1_GPIO_Port, SW_CTL1_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(SW_CTL2_GPIO_Port, SW_CTL2_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(RF_RST_GPIO_Port, RF_RST_Pin, GPIO_PIN_RESET);
				set_fpga_status(FPGA_Offline);
				LOG(CLI_LOG_RADIO, "PoC powered off.\r\n");
			}else if( (initn == GPIO_PIN_SET) \
					  && (done == GPIO_PIN_RESET))
			{
				// Looks like the PoC was powered on!
				HAL_GPIO_WritePin(MAIN_KILL_GPIO_Port, MAIN_KILL_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(FPGA_PROGRAMN_GPIO_Port, FPGA_PROGRAMN_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(FPGA_NSS_GPIO_Port, FPGA_NSS_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(FPGA_RST_GPIO_Port, FPGA_RST_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(XCVR_NSS_GPIO_Port, XCVR_NSS_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(SW_CTL1_GPIO_Port, SW_CTL1_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(SW_CTL2_GPIO_Port, SW_CTL2_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(RF_RST_GPIO_Port, RF_RST_Pin, GPIO_PIN_RESET);
				set_fpga_status(FPGA_Online);
				xcvr_init();
				LOG(CLI_LOG_RADIO, "PoC powered on.\r\n");
			}else if( (initn == GPIO_PIN_RESET) \
					  && (done == GPIO_PIN_SET) ){
				LOG(CLI_LOG_RADIO, "INITN and DONE pins are in an inconsistent state...\r\n");
				set_fpga_status(FPGA_Error);
			}

		}else if(flag & XCVR_INIT){
			osThreadFlagsClear(XCVR_INIT);
			HAL_GPIO_WritePin(RF_RST_GPIO_Port, RF_RST_Pin, GPIO_PIN_SET);
			upload_fpga_binary();
		}else if(flag & FPGA_UPLOAD_BIN){
			osThreadFlagsClear(FPGA_UPLOAD_BIN);				// Clear the flag
			osPriority_t prev_prio = osThreadGetPriority(NULL); // Lower task's priority
			osThreadSetPriority(NULL, osPriorityBelowNormal);
			set_fpga_status(FPGA_Loading);
			LOG(CLI_LOG_FPGA, "Started FPGA bitstream upload\r\n");

			uint8_t bufferTX[16] = {0};	// Used for data to TX
			uint8_t bufferRX[16] = {0};

			// First set PROGRAMN low
			HAL_GPIO_WritePin(FPGA_PROGRAMN_GPIO_Port, FPGA_PROGRAMN_Pin, GPIO_PIN_RESET);
			osDelay(2); // Must wait for at least 50ns

			// Drive SCSN low then clock in the key
			osMutexAcquire(SPI1AccessHandle, WAIT_TIMEOUT);						// Exclusive access to SPI1
			HAL_GPIO_WritePin(FPGA_NSS_GPIO_Port, FPGA_NSS_Pin, GPIO_PIN_RESET);	// Slave select lines
			HAL_GPIO_WritePin(XCVR_NSS_GPIO_Port, XCVR_NSS_Pin, GPIO_PIN_SET);

			*(uint32_t *)(bufferTX+1) = SPI_PORT_ACTIVATION_KEY;
			_fpga_sspi_classB(5, bufferTX);
			osDelay(50);

			// Clock in READ_ID cmd and read 32 bits IDCODE
			memset(bufferTX, 0, sizeof(bufferTX));
			memset(bufferRX, 0, sizeof(bufferRX));
			bufferTX[0] = 0xE0;
			_fpga_sspi_classA(8, bufferTX, bufferRX);
			LOG(CLI_LOG_FPGA, "FPGA IDCODE is 0x%02x%02x%02x%02x.\r\n", bufferRX[4], bufferRX[5], bufferRX[6], bufferRX[7]);

			if(*(uint32_t *)(bufferRX+4) != 0x43100F11){
				ERR("Wrong FPGA IDCODE. Aborting upload.\r\n");
				// Release SPI mutex and restore thread priority
				osMutexRelease(SPI1AccessHandle);
				osThreadSetPriority(FPGA_thread_id, prev_prio);
				set_fpga_status(FPGA_Error);
				continue;
			}

			// ISC-Enable
			LOG(CLI_LOG_FPGA, "Sending ISC_ENABLE.\r\n");
			_fpga_sspi_classC(0xC6);

			// ISC-Erase
			memset(bufferTX, 0, sizeof(bufferTX));
			memset(bufferRX, 0, sizeof(bufferRX));
			LOG(CLI_LOG_FPGA, "Sending ISC_ERASE.\r\n");
			_fpga_sspi_classC(0x0E);
			_fpga_wait_busy();

			// Check status register
			_fpga_check_status_register(NULL);

			// LSC_Bitstream_burst followed by bitstream
			fpga_bin_entry_t e;
			bool eeeprom_result = EEEPROM_read_data(&eeeprom, 0, &e);
			if(eeeprom_result == EXIT_FAILURE){
				ERR("Could not find a valid FPGA image.\r\n");
				set_fpga_status(FPGA_Error);
				// Release SPI mutex and restore thread priority
				osMutexRelease(SPI1AccessHandle);
				osThreadSetPriority(FPGA_thread_id, prev_prio);
				continue;
			}
			uint8_t *bitstream = (uint8_t *)(e.start_sector*SUBSECTOR_SIZE) + START_MMP_NOR_ADDR;
			memset(bufferTX, 0, sizeof(bufferTX));
			bufferTX[0] = 0x7A;
			LOG(CLI_LOG_FPGA, "Sending LSC_BITSTREAM_BURST.\r\n");
			HAL_GPIO_WritePin(FPGA_NSS_GPIO_Port, FPGA_NSS_Pin, GPIO_PIN_RESET);
			HAL_SPI_Transmit_IT(&hspi1, bufferTX, 4);
			wait_spi_xfer_done(WAIT_TIMEOUT);

			osMutexAcquire(NORAccessHandle, WAIT_TIMEOUT);
			BSP_QSPI_EnableMemoryMappedMode();	// Enable memory mapped mode to read the bitstream

			uint32_t sent = 0;
			while(sent < e.size){
				uint32_t bytes_left = e.size - sent;
				uint16_t to_send = (UINT16_MAX < bytes_left)?UINT16_MAX:bytes_left;
				HAL_SPI_Transmit_DMA(&hspi1, bitstream + sent, to_send);
				LOG(CLI_LOG_FPGA, "Sent %lu bitstream bytes.\r\n", sent);
				wait_spi_xfer_done(WAIT_TIMEOUT);
				sent += to_send;
			}
			LOG(CLI_LOG_FPGA, "Sent %lu bitstream bytes.\r\n", sent);
			HAL_GPIO_WritePin(FPGA_NSS_GPIO_Port, FPGA_NSS_Pin, GPIO_PIN_SET);
			LOG(CLI_LOG_FPGA, "Bitstream sent, reconfiguring NOR flash.\r\n");
			BSP_QSPI_Init();					// set the NOR flash back in indirect access mode
			osMutexRelease(NORAccessHandle);

			// Check status register
			uint8_t status[8];
			bool error = false;
			_fpga_check_status_register(status);
			if( (status[2]&(1<<4)) == 0){
				ERR("INITN flag not set!\r\n");
				error = true;
			}
			if(status[4] & 0xF){
				ERR("BSE Error Code is %d!\r\n", status[4] & 0xF);
				error = true;
			}
			if( (status[6] & 1) == 0 ){
				ERR("DONE flag not set!\r\n");
				error = true;
			}
			if( (status[6] & (1<<5)) > 0){
				ERR("Fail flag set!\r\n");
				error = true;
			}

			// ISC_Disable
			bufferTX[0] = 0x26;
			_fpga_sspi_classC(0x26);

			if(error){
				ERR("Error during FPGA upload...\r\n");
				set_fpga_status(FPGA_Error);
			}else{
				LOG(CLI_LOG_FPGA, "FPGA upload done.\r\n");
				set_fpga_status(FPGA_Running);
				HAL_GPIO_WritePin(FPGA_RST_GPIO_Port, FPGA_RST_Pin, GPIO_PIN_SET);
			}

			while(HAL_GPIO_ReadPin(FPGA_DONE_GPIO_Port, FPGA_DONE_Pin) == GPIO_PIN_RESET){
				osDelay(5);
			}
			LOG(CLI_LOG_FPGA, "FPGA configuration finished.\r\n");

			while(HAL_GPIO_ReadPin(IO3_GPIO_Port, IO3_Pin) == GPIO_PIN_RESET){
				osDelay(5);
			}
			LOG(CLI_LOG_FPGA, "FPGA PLL is locked. Querying revision number.\r\n");

			*((uint32_t *)(bufferTX)) = 0;
			*((uint32_t *)(bufferRX)) = 0;
			bufferTX[1] = SR_1;
			_select_chip();
			HAL_SPI_TransmitReceive_IT(&hspi1, bufferTX, bufferRX, 4);
			wait_spi_xfer_done(WAIT_TIMEOUT);
			_release_chip();
			DBG("FPGA revision is %u.%u.\r\n", bufferRX[2], bufferRX[3]);

			// Release SPI mutex and restore thread priority
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
				for(size_t i = 0; i < nb_sectors; i ++){
					if(BSP_QSPI_Erase_Block(FPGA_BIN_STORAGE_START_SECTOR + i*SUBSECTOR_SIZE) != QSPI_OK){
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
			for(size_t i = FPGA_BIN_STORAGE_START_SECTOR; i < FPGA_BIN_STORAGE_END_SECTOR; i++){
				//CUSTOM_QSPI_Erase_Sector(i*SUBSECTOR_SIZE);
				BSP_QSPI_Erase_Block(i*SUBSECTOR_SIZE);
			}
			LOG(CLI_LOG_FPGA, "Done!\r\n");

			osThreadSetPriority(FPGA_thread_id, prev_prio);	// Restore the task's previous priority
		}
	}

}

bool upload_fpga_binary(){
	uint32_t result = osThreadFlagsSet(FPGA_thread_id, FPGA_UPLOAD_BIN);
	if(result < (1<<31)){
		return EXIT_SUCCESS;
	}else{
		ERR("Could not upload FPGA binary: osThreadFlagsSet returned 0x%08lx.\r\n", result);
	}


	return EXIT_FAILURE;
}

bool download_fpga_binary_file(){
	uint32_t result = osThreadFlagsSet(FPGA_thread_id, FPGA_DOWNLOAD_BIN);
	if(result < (1<<31)){
		return EXIT_SUCCESS;
	}else{
		ERR("Could not download FPGA binary: osThreadFlagsSet returned 0x%08lx.\r\n", result);
	}

	return EXIT_FAILURE;
}

bool erase_fpga_storage(){
	uint32_t result = osThreadFlagsSet(FPGA_thread_id, FPGA_ERASE_STORAGE);
	if(result < (1<<31)){
		return EXIT_SUCCESS;
	}else{
		ERR("Could not erase FPGA storage: osThreadFlagsSet returned 0x%08lx.\r\n", result);
	}
	return EXIT_FAILURE;
}

bool fpga_soft_reset(){
	uint32_t result = osThreadFlagsSet(FPGA_thread_id, FPGA_RESET);
	if(result < (1<<31)){
		return EXIT_SUCCESS;
	}else{
		ERR("Could not soft reset FPGA: osThreadFlagsSet returned 0x%08lx.\r\n", result);
	}

	return EXIT_FAILURE;
}

bool xcvr_init() {
	uint32_t result = osThreadFlagsSet(FPGA_thread_id, XCVR_INIT);
	if(result < (1<<31)){
		return EXIT_SUCCESS;
	}else{
		ERR("Could not start transceiver initialization: osThreadFlagsSet returned 0x%08lx.\r\n", result);
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

void _fpga_wait_busy(){
	// LSC-CHECK-BUSY (5 bits)
	uint8_t tx[5] = {0xF0, 0, 0, 0, 0};
	uint8_t rx[5] = {0};

	while(true){ // Wait for device to not be busy anymore
		_fpga_sspi_classA(5, tx, rx);
		if(rx[4] == 0){
			LOG(CLI_LOG_FPGA, "FPGA not busy anymore.\r\n");
			break;
		}else{
			printf(".");
			osDelay(2);
		}
	}
}

uint32_t _fpga_check_status_register(uint8_t *reg){
	uint8_t rx[12] = {0};
	uint8_t tx[12] = {0x3C, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	_fpga_sspi_classA(12, tx, rx);

	LOG(CLI_LOG_FPGA, "Status register is 0x%02x%02x%02x%02x%02x%02x%02x%02x.\r\n",
			rx[4], rx[5], rx[6], rx[7], rx[8], rx[9], rx[10], rx[11]);

	if(reg != NULL){
		memcpy(reg, rx+4, 8);
	}

	return EXIT_SUCCESS;
}

uint32_t _fpga_sspi_classA(uint32_t length, uint8_t *tx, uint8_t *rx){
	uint32_t res = 0;
	_select_chip();
	//reset_spi1_flag();
	res = HAL_SPI_TransmitReceive_IT(&hspi1, tx, rx, length);
	if(res != HAL_OK){
		LOG(CLI_LOG_FPGA, "HAL_SPI_TransmitReceive_IT returned %lu.\r\n", res);
	}
	wait_spi_xfer_done(WAIT_TIMEOUT);
	_release_chip();
	return EXIT_SUCCESS;
}

uint32_t _fpga_sspi_classB(uint32_t length, uint8_t *tx){
	uint32_t res = 0;
	_select_chip();
	//reset_spi1_flag();
	res = HAL_SPI_Transmit_IT(&hspi1, tx, length);
	if(res != HAL_OK){
		LOG(CLI_LOG_FPGA, "HAL_SPI_TransmitReceive_IT returned %lu.\r\n", res);
	}
	wait_spi_xfer_done(WAIT_TIMEOUT);
	_release_chip();
	return EXIT_SUCCESS;
}

uint32_t _fpga_sspi_classC(uint8_t cmd){
	uint32_t res = 0;
	uint8_t tx[4] = {cmd, 0, 0, 0};
	_select_chip();
	//reset_spi1_flag();
	res = HAL_SPI_Transmit_IT(&hspi1, tx, 4);
	if(res != HAL_OK){
		LOG(CLI_LOG_FPGA, "HAL_SPI_TransmitReceive_IT returned %lu.\r\n", res);
	}
	wait_spi_xfer_done(WAIT_TIMEOUT);
	_release_chip();
	return EXIT_SUCCESS;
}

void _select_chip(){
	HAL_GPIO_WritePin(FPGA_NSS_GPIO_Port, FPGA_NSS_Pin, GPIO_PIN_RESET);
}

void _release_chip(){
	HAL_GPIO_WritePin(FPGA_NSS_GPIO_Port, FPGA_NSS_Pin, GPIO_PIN_SET);
}

void radio_INITn_it(){
	uint32_t result = osThreadFlagsSet(FPGA_thread_id, RADIO_INITN_CHANGED);
	if(result >= (1<<31)){
		LOG(CLI_LOG_RADIO, "Could not process INITN changed: osThreadFlagSet returned %lu.\r\n", result);
	}
}