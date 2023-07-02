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
#include "ui/openht_ui.h"
#include "openht_types.h"
#include "../radio/fpga_reg_defs.h"
#include "../radio/at86rf215.h"
#include "radio_settings.h"
#include "task_audio_process.h"
#include "task_microphone.h"
#include "../radio/radio_hal.h"

#include "../shell/inc/sys_command_line.h"
#include <fatfs.h>
#include <cmsis_os2.h>
#include <stm32f4xx_ll_gpio.h>

#include <stdlib.h>
#include <math.h>

typedef struct __attribute((__packed__)){
	uint32_t start_sector:24;
	uint32_t size;
}fpga_bin_entry_t;

extern SPI_HandleTypeDef 	hspi1;
extern osMutexId_t 			SPI1AccessHandle;
extern osMutexId_t 			NORAccessHandle;

volatile bool				radio_enabled = false;
volatile bool				tx_nRx = false; 						// 0 when RX, 1 when TX

osThreadId_t 				FPGA_thread_id 			= NULL;
uint32_t 					bitstream_load_address 	= 0x80000000;
uint32_t 					bitstream_load_offset 	= 0;
osTimerId_t					ptt_debounce_timer = NULL;
const osTimerAttr_t			ptt_debounce_timer_attr = {
		.name = "ptt_debounce"
};

UINT 			fatfs_bitstream_stream		(const BYTE *p, UINT btf);
void 			_ptt_timer_expired(void *argument);

/* Event flags */
#define FPGA_SEND_SAMPLES		(1 << 0)
#define FPGA_READ_SAMPLES		(1 << 1)
#define FPGA_FETCH_IQ			(1 << 2)
#define FPGA_UPLOAD_BIN			(1 << 3)
#define FPGA_DOWNLOAD_BIN		(1 << 4)
#define FPGA_RESET				(1 << 5)
#define FPGA_ERASE_STORAGE		(1 << 6)
#define RADIO_INITN_CHANGED 	(1 << 7)
#define XCVR_INIT				(1 << 8)
#define XCVR_CONFIG				(1 << 9)
#define RADIO_PTT_START_TIMER	(1 << 10)
#define RADIO_START_TX			(1 << 11)
#define RADIO_START_RX			(1 << 12)

#define RADIO_ALL_FLAGS		(FPGA_SEND_SAMPLES | FPGA_READ_SAMPLES | FPGA_FETCH_IQ | FPGA_UPLOAD_BIN |\
							 FPGA_DOWNLOAD_BIN | FPGA_RESET | FPGA_ERASE_STORAGE | RADIO_INITN_CHANGED |\
							 XCVR_INIT | XCVR_CONFIG | RADIO_PTT_START_TIMER | RADIO_START_TX | RADIO_START_RX)

void rx_changed_cb()
{
	LOG(CLI_LOG_RADIO, "RX settings changed. Updating them.\r\n");
	task_radio_event(CONFIG);
}

void mode_changed_cb()
{
	LOG(CLI_LOG_RADIO, "Mode settings changed. Updating them.\r\n");
	task_radio_event(CONFIG);
}

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

	// Radio settings

	// init
	radio_settings_init();
	radio_settings_sub_rx_freq_cb(rx_changed_cb);
	radio_settings_sub_mode_cb(mode_changed_cb);

	openht_mode_t 		mode 		= radio_settings_get_mode();
	freq_t 				rx_freq 	= radio_settings_get_rx_freq();
	freq_t 				tx_freq 	= radio_settings_get_tx_freq();
	fmInfo_t 			fm_info 	= radio_settings_get_fm_settings();
	openht_radio_agc	agc 		= radio_settings_get_radio_agc();
	uint8_t				tx_power	= radio_settings_get_output_pwr();
	tx_power = 0x1F;

	float			ppm 		= 0;

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

	if(HAL_GPIO_ReadPin(FPGA_INITN_GPIO_Port, FPGA_INITN_Pin) == GPIO_PIN_SET){
		task_radio_event(INITN_IRQ);
	}

	ptt_debounce_timer = osTimerNew(_ptt_timer_expired, osTimerOnce, NULL, &ptt_debounce_timer_attr);

	// For now we init this here. It will be in it's own thread later on
	audio_process_init();

	for(;;){
		uint32_t flag = osThreadFlagsWait(RADIO_ALL_FLAGS, osFlagsNoClear, osWaitForever);

		if(flag & FPGA_SEND_SAMPLES){
			osThreadFlagsClear(FPGA_SEND_SAMPLES);
			uint8_t samples[34];
			*(uint16_t *)samples = MOD_IN & REG_WR;
			read_voice_samples((int16_t *)(samples+2), 16, 0);
			FPGA_chip_select(true);
			HAL_SPI_Transmit_IT(&hspi1, samples, sizeof(samples));
			printf("(x)\n");
			wait_spi_xfer_done(WAIT_TIMEOUT);
			FPGA_chip_select(false);
		}else if(flag & FPGA_READ_SAMPLES){
			osThreadFlagsClear(FPGA_READ_SAMPLES);

		}else if(flag & FPGA_FETCH_IQ){
			osThreadFlagsClear(FPGA_FETCH_IQ);

		}else if(flag & XCVR_CONFIG){
			osThreadFlagsClear(XCVR_CONFIG);
			LOG(CLI_LOG_RADIO, "Configuring XCVR.\r\n");
			ui_log_add("[RADIO]: Configuring XCVR.\n");


			// TODO: M17 mode support. This will get user callsign:
			//const char * test = radio_settings_get_m17_callsign();

			mode 		= radio_settings_get_mode();
			rx_freq 	= radio_settings_get_rx_freq();
			tx_freq 	= radio_settings_get_tx_freq();
			fm_info 	= radio_settings_get_fm_settings();
			agc			= radio_settings_get_radio_agc();
			tx_power	= radio_settings_get_output_pwr();

			tx_power = 0x1F;

			// Simulate a PTT press to re-send all settings
			_ptt_timer_expired(NULL);

		}else if(flag & RADIO_PTT_START_TIMER){
			osThreadFlagsClear(RADIO_PTT_START_TIMER);
			osTimerStart(ptt_debounce_timer, 5);
		}else if(flag & RADIO_START_RX){
			osThreadFlagsClear(RADIO_START_RX);

			// Disable IO3 IRQ
			HAL_NVIC_ClearPendingIRQ(EXTI15_10_IRQn);
			HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);

			stop_microphone_acquisition();
			BSP_LED_Off(LED_RED);
			tx_nRx = false;
			radio_configure_rx(rx_freq, ppm, mode, fm_info, agc);
		}else if(flag & RADIO_START_TX){
			osThreadFlagsClear(RADIO_START_TX);

			// Disable IO3 IRQ
			HAL_NVIC_ClearPendingIRQ(EXTI15_10_IRQn);
			HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);

			start_microphone_acquisition();
			BSP_LED_On(LED_RED);
			radio_configure_tx(tx_freq, ppm, mode, fm_info, tx_power);
			tx_nRx = true;
			uint8_t voice[66];
			read_voice_samples((int16_t *)(voice+2), 32, 10);
			//memset(voice+2, 0x00, 64);
			*(uint16_t *)(voice) = MOD_IN | REG_WR;

			FPGA_chip_select(true);
			// Enable IO3 IRQ
			HAL_NVIC_ClearPendingIRQ(EXTI15_10_IRQn);
			HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

			HAL_SPI_Transmit_IT(&hspi1, voice, sizeof(voice));
			wait_spi_xfer_done(WAIT_TIMEOUT);
			FPGA_chip_select(false);


		}else if(flag & RADIO_INITN_CHANGED){
			osThreadFlagsClear(RADIO_INITN_CHANGED);
			GPIO_PinState initn 	= HAL_GPIO_ReadPin(FPGA_INITN_GPIO_Port, FPGA_INITN_Pin);
			GPIO_PinState done		= HAL_GPIO_ReadPin(FPGA_DONE_GPIO_Port, FPGA_DONE_Pin);
			if( (initn == GPIO_PIN_RESET) \
				&& (done == GPIO_PIN_RESET))
			{
				// Looks like everything is off...
				radio_off();
				set_fpga_status(FPGA_Offline);
				LOG(CLI_LOG_RADIO, "PoC powered off.\r\n");
				ui_log_add("[RADIO]: PoC powered off.\n");

				HAL_NVIC_ClearPendingIRQ(EXTI15_10_IRQn);
				HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);

				radio_enabled = false;
			}else if( (initn == GPIO_PIN_SET) \
					  && (done == GPIO_PIN_RESET))
			{
				// Looks like the PoC was powered on!
				radio_on();
				set_fpga_status(FPGA_Online);
				task_radio_event(INIT);
				LOG(CLI_LOG_RADIO, "PoC powered on.\r\n");
				ui_log_add("[RADIO]: PoC powered on.\n");
				radio_enabled = true;
			}else if( (initn == GPIO_PIN_RESET) \
					  && (done == GPIO_PIN_SET) ){
				LOG(CLI_LOG_RADIO, "INITN and DONE pins are in an inconsistent state...\r\n");
				ui_log_add("[RADIO]: NITN and DONE pins are in an inconsistent state...\n");

				set_fpga_status(FPGA_Error);
			}

		}else if(flag & XCVR_INIT){
			osThreadFlagsClear(XCVR_INIT);
			if(!radio_enabled){
				continue;
			}
			HAL_GPIO_WritePin(RF_RST_GPIO_Port, RF_RST_Pin, GPIO_PIN_SET);
			osDelay(5);
			task_radio_event(UPLOAD_BITSTREAM);
		}else if(flag & FPGA_UPLOAD_BIN){
			osThreadFlagsClear(FPGA_UPLOAD_BIN);				// Clear the flag
			if(!radio_enabled){
				continue;
			}
			osPriority_t prev_prio = osThreadGetPriority(NULL); // Lower task's priority
			osThreadSetPriority(NULL, osPriorityBelowNormal);
			set_fpga_status(FPGA_Loading);
			LOG(CLI_LOG_FPGA, "Started FPGA bitstream upload\r\n");
			ui_log_add("[FPGA]: Started FPGA bitstream upload\n");

			uint8_t bufferTX[8] = {0};	// Used for data to TX
			uint8_t bufferRX[8] = {0};

			osMutexAcquire(SPI1AccessHandle, WAIT_TIMEOUT);						// Exclusive access to SPI1
			FPGA_config_enable_sspi();

			// Clock in READ_ID cmd and read 32 bits IDCODE
			bufferTX[0] = 0xE0;
			FPGA_config_classA(8, bufferTX, bufferRX);
			LOG(CLI_LOG_FPGA, "FPGA IDCODE is 0x%02x%02x%02x%02x.\r\n", bufferRX[4], bufferRX[5], bufferRX[6], bufferRX[7]);
			ui_log_add("[FPGA]: FPGA IDCODE is 0x%02x%02x%02x%02x.\n", bufferRX[4], bufferRX[5], bufferRX[6], bufferRX[7]);

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
			ui_log_add("[FPGA]: Sending ISC_ENABLE.\n");
			FPGA_config_classC(0xC6);

			// ISC-Erase
			LOG(CLI_LOG_FPGA, "Sending ISC_ERASE.\r\n");
			ui_log_add("[FPGA]: Sending ISC_ERASE.\n");
			FPGA_config_classC(0x0E);
			FPGA_wait_busy();

			// send bitstream
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
			osMutexAcquire(NORAccessHandle, WAIT_TIMEOUT);
			BSP_QSPI_EnableMemoryMappedMode();	// Enable memory mapped mode to read the bitstream
			FPGA_send_bitstream((uint32_t)bitstream, e.size);
			BSP_QSPI_Init();					// set the NOR flash back in indirect access mode
			osMutexRelease(NORAccessHandle);

			// Check status register
			uint8_t status[8];
			bool error = false;
			FPGA_check_status_register(status, true);
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
			FPGA_config_classC(0x26);

			if(error){
				ERR("Error during FPGA upload...\r\n");
				set_fpga_status(FPGA_Error);
			}else{
				LOG(CLI_LOG_FPGA, "FPGA upload done.\r\n");
				ui_log_add("[FPGA]: FPGA upload done.\n");
				set_fpga_status(FPGA_Running);
				HAL_GPIO_WritePin(FPGA_RST_GPIO_Port, FPGA_RST_Pin, GPIO_PIN_SET);
			}

			// Wait for DONE pin to be set
			while(HAL_GPIO_ReadPin(FPGA_DONE_GPIO_Port, FPGA_DONE_Pin) == GPIO_PIN_RESET){
				osDelay(5);
			}
			LOG(CLI_LOG_FPGA, "FPGA configuration finished.\r\n");
			ui_log_add("[FPGA]: FPGA configuration finished.\n");

			// Wait for IO3 to signal that the internal PLLs are
			// locked and the device is ready for operation
			while(HAL_GPIO_ReadPin(IO3_GPIO_Port, IO3_Pin) == GPIO_PIN_RESET){
				osDelay(5);
			}
			LOG(CLI_LOG_FPGA, "FPGA PLL is locked. Querying revision number.\r\n");
			ui_log_add("[FPGA]: FPGA PLL is locked. Querying revision number.\n");

			FPGA_read_reg(SR_1, (uint16_t *)bufferRX);
			DBG("FPGA revision is %u.%u.\r\n", bufferRX[1], bufferRX[0]);
			maj_min_rev_t fpga_revision;
			fpga_revision.maj_rev = bufferRX[1];
			fpga_revision.min_rev = bufferRX[0];
			radio_settings_set_fpga_rev(fpga_revision);
			ui_log_add("[FPGA]: FPGA revision is %u.%u.\n", bufferRX[1], bufferRX[0]);

			uint8_t read = 0;
			XCVR_read_reg(RF_PN, &read);
			LOG(CLI_LOG_RADIO, "XCVR PN is 0x%02x.\r\n", read);
			ui_log_add("[RADIO]: XCVR PN is 0x%02x.\n", read);
			osDelay(1);
			XCVR_read_reg(RF_VN, &read);
			LOG(CLI_LOG_RADIO, "XCVR VN is 0x%02x.\r\n", read);
			ui_log_add("[RADIO]: XCVR VN is 0x%02x.\n", read);

			// Set XCVR to IEEE lvds mode
			XCVR_write_reg(RF_IQIFC0, RF_IQIFC0_CMV1V2 | RF_IQIFC0_DRV3);

			// Tell the XCVR that we will be using the IQ interface
			XCVR_write_reg(RF_IQIFC1, RF_IQIFC1_RF | RF_IQIFC1_SKEWDRV39);

			task_radio_event(CONFIG);

			// Release SPI mutex and restore thread priority
			osMutexRelease(SPI1AccessHandle);
			LOG(CLI_LOG_FPGA, "Done!\r\n");
			ui_log_add("[FPGA]: Done!\n");
			osThreadSetPriority(FPGA_thread_id, prev_prio);
		}else if(flag & FPGA_DOWNLOAD_BIN){
			osThreadFlagsClear(FPGA_DOWNLOAD_BIN);				// Clear the flag
			osPriority_t prev_prio = osThreadGetPriority(NULL); // Lower task's priority
			osThreadSetPriority(NULL, osPriorityBelowNormal);
			LOG(CLI_LOG_FPGA, "Started FPGA bitstream download...\r\n");
			ui_log_add("[FPGA]: Started FPGA bitstream download...\n");

			// Download BIN image from SD card to NOR flash
			// Open bitstream file
			FIL bin_file;
			fpga_bin_entry_t bin_entry;
			FSIZE_t file_size;
			UINT btf;
			DIR dir;
			FILINFO file_info;

			FRESULT fr = f_findfirst(&dir, &file_info, "", "*.bit");
			if(fr == FR_OK){
				if(f_open(&bin_file, file_info.fname, FA_OPEN_EXISTING | FA_READ) != FR_OK){
					ERR("Error opening file %s.\r\n", file_info.fname);
					osThreadSetPriority(NULL, prev_prio);
					f_closedir(&dir);
					continue;
				}
			}else{
				ERR("No bitstream found on SD card.\r\n");
				osThreadSetPriority(NULL, prev_prio);
				f_closedir(&dir);
				continue;
			}
			f_closedir(&dir);

			LOG(CLI_LOG_FPGA, "Reading bitstream from file %s.\r\n", file_info.fname);

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
				for(size_t i = 0; i < nb_sectors; i+=16){
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
			LOG(CLI_LOG_FPGA, "Done!\r\n");
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
			for(size_t i = FPGA_BIN_STORAGE_START_SECTOR; i < FPGA_BIN_STORAGE_END_SECTOR; i+=16){
				//CUSTOM_QSPI_Erase_Sector(i*SUBSECTOR_SIZE);
				CUSTOM_QSPI_Erase_Sector(i*SUBSECTOR_SIZE);
			}

			LOG(CLI_LOG_FPGA, "Done!\r\n");

			osThreadSetPriority(FPGA_thread_id, prev_prio);	// Restore the task's previous priority
		}
	}

}

uint32_t task_radio_event(task_radio_event_t event){
	uint32_t result = 0;

	switch(event){
	case SAMPLES_IRQ:
		printf("(y)\n");
		if(tx_nRx && radio_enabled){
			result = osThreadFlagsSet(FPGA_thread_id, FPGA_SEND_SAMPLES);
		}else if(radio_enabled){
			result = osThreadFlagsSet(FPGA_thread_id, FPGA_READ_SAMPLES);
		}
		break;
	case UPLOAD_BITSTREAM:
		result = osThreadFlagsSet(FPGA_thread_id, FPGA_UPLOAD_BIN);
		break;
	case DOWNLOAD_BITSTREAM:
		result = osThreadFlagsSet(FPGA_thread_id, FPGA_DOWNLOAD_BIN);
		break;
	case FPGA_SOFT_RESET:
		result = osThreadFlagsSet(FPGA_thread_id, FPGA_RESET);
		break;
	case ERASE_BITSTREAM_STORAGE:
		result = osThreadFlagsSet(FPGA_thread_id, FPGA_ERASE_STORAGE);
		break;
	case INITN_IRQ:
		result = osThreadFlagsSet(FPGA_thread_id, RADIO_INITN_CHANGED);
		break;
	case INIT:
		result = osThreadFlagsSet(FPGA_thread_id, XCVR_INIT);
		break;
	case CONFIG:
		result = osThreadFlagsSet(FPGA_thread_id, XCVR_CONFIG);
		break;
	case PTT_IRQ:
		result = osThreadFlagsSet(FPGA_thread_id, RADIO_PTT_START_TIMER);
		break;
	case START_RX:
		result = osThreadFlagsSet(FPGA_thread_id, RADIO_START_RX);
		break;
	case START_TX:
		result = osThreadFlagsSet(FPGA_thread_id, RADIO_START_TX);
		break;
	default:
		LOG(CLI_LOG_RADIO, "Unknown event %d.\r\n", event);
		return EXIT_FAILURE;
	}

	if(result >= (1 << 31)){
		LOG(CLI_LOG_RADIO, "Error while processing event %d. osThreadFlagsSet returned 0x%02lx.\r\n", event, result);
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
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

void _ptt_timer_expired(void *argument){
	GPIO_PinState ptt = HAL_GPIO_ReadPin(PTT_GPIO_Port, PTT_Pin);

	// SET means released
	if(ptt == GPIO_PIN_RESET){
		task_radio_event(START_TX);
	}else{
		task_radio_event(START_RX);
	}
}
