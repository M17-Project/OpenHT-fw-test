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

#include "radio_settings.h"

#include "nor_map.h"
#include "eeeprom.h"
#include "eeeprom_hal.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

static const uint8_t RX_FREQ_EEEPROM_ADDR =     	0x00;	// Rx frequency (in Hz)
static const uint8_t TX_FREQ_EEEPROM_ADDR =	    	0x01;	// Tx frequency (in Hz)
static const uint8_t AGC_PWR_MODE_EEEPROM_ADDR =   	0x02;	// AGC, Power level, OpMode
static const uint8_t M17_DST1_EEEPROM_ADDR =   		0x03;	// Contains CS letters [1:4]
static const uint8_t M17_DST2_EEEPROM_ADDR =   		0x04;	// Contains CS letters [5:8]
static const uint8_t M17_DST3_EEEPROM_ADDR =   		0x05;	// Contains CS letters [9:10]
static const uint8_t M17_INFO_EEEPROM_ADDR =		0x06;	// Contains M17 info struct

static const uint8_t FM_SETTINGS_EEEPROM_ADDR =    	0x07;	// Contains FM settings struct


EEEPROMHandle_t radio_settings_eeeprom = {
		.address_size = EEEPROM_ADDRESS_1BYTE,
		.alignment = 1,
		.data_size = 4,
		.read = EEEPROM_HAL_qspi_read,
		.write = EEEPROM_HAL_qspi_write,
		.erase_page = EEEPROM_HAL_erase_subsector,
		.number_pages = RADIO_SETTINGS_NB_SECTORS,
		.page_offset = RADIO_SETTINGS_START_SECTOR,
		.page_size = SUBSECTOR_SIZE,
		.start_address = RADIO_SETTINGS_START_ADDRESS,
};

static bool init_done = false;
static radio_settings_t cached_settings;
static radio_settings_t saved_settings;
static user_callsign_func_t callsign_cb = NULL; // function which provides user callsign
static radio_setting_changed_func_t radio_settings_rx_changed_cb = NULL; // function which is called when rx has changed

void radio_settings_reset(){
	bool res = EEEPROM_erase(&radio_settings_eeeprom);
	if( res != EXIT_SUCCESS){
		printf("Error erasing EEPROM content for user settings.\r\n");
		return;
	}
	printf("Successfully erased user settings. Rebooting\r\n");
	NVIC_SystemReset();
}

void radio_settings_init()
{
	if (init_done) {
		return;
	}

	if(EEEPROM_init(&radio_settings_eeeprom) != EXIT_SUCCESS){
		printf("Error initializing EEEPROM for user settings.\r\n");
	}
	uint32_t buffer;

	// RX freq
	if(EEEPROM_read_data(&radio_settings_eeeprom, RX_FREQ_EEEPROM_ADDR, &buffer) == EXIT_SUCCESS){
		saved_settings.rx_freq = buffer;
	}else{
		saved_settings.rx_freq = 0;
	}

	// TX freq
	if(EEEPROM_read_data(&radio_settings_eeeprom, TX_FREQ_EEEPROM_ADDR, &buffer) == EXIT_SUCCESS){
		saved_settings.tx_freq = buffer;
	}else{
		saved_settings.tx_freq = 0;
	}

	// AGC and Power output
	if(EEEPROM_read_data(&radio_settings_eeeprom, AGC_PWR_MODE_EEEPROM_ADDR, &buffer) == EXIT_SUCCESS){
		saved_settings.agc_target = buffer & 0xFF;
		saved_settings.output_pwr = (buffer>>8) & 0xFF;
		saved_settings.mode = (buffer>>16) & 0xFF;
	}else{
		saved_settings.agc_target = 0;
		saved_settings.output_pwr = 0;
		saved_settings.mode = 0;
	}

	// M17 Callsign
	// set func ptr to get user callsign
	callsign_cb = user_settings_callsign;

	// DST letters 1 to 4
	if(EEEPROM_read_data(&radio_settings_eeeprom, M17_DST1_EEEPROM_ADDR, &buffer) == EXIT_SUCCESS){
		*(uint32_t *)(saved_settings.m17_dst) = buffer;
	}else{
		saved_settings.m17_dst[0] = '\0';
	}
	// DST letters 5 to 8
	if(EEEPROM_read_data(&radio_settings_eeeprom, M17_DST2_EEEPROM_ADDR, &buffer) == EXIT_SUCCESS){
		*(uint32_t *)(saved_settings.m17_dst+4) = buffer;
	}else{
		saved_settings.m17_dst[4] = '\0';
	}
	// DST letters 9-10
	if(EEEPROM_read_data(&radio_settings_eeeprom, M17_DST3_EEEPROM_ADDR, &buffer) == EXIT_SUCCESS){
		*(uint16_t *)(saved_settings.m17_dst+8) = (uint16_t)buffer;
	}else{
		saved_settings.m17_dst[8] = '\0';
	}

	// M17 Info
	if(EEEPROM_read_data(&radio_settings_eeeprom, M17_INFO_EEEPROM_ADDR, &buffer) == EXIT_SUCCESS){
		*(uint32_t *)(&saved_settings.m17_info) = buffer;
	}else{
		*(uint32_t *)(&saved_settings.m17_info) = 0;
	}

	// FM Settings
	if(EEEPROM_read_data(&radio_settings_eeeprom, FM_SETTINGS_EEEPROM_ADDR, &buffer) == EXIT_SUCCESS){
		saved_settings.fm_settings.raw = buffer;
	}else{
		saved_settings.fm_settings.raw = 0;
	}

	// set the saved_settings (which represents the contents of the EEEPROM) and the
	// cached_settings (which is the volatile settings) to be equal
	memcpy(&cached_settings, &saved_settings, sizeof(radio_settings_t));

	init_done = true;
}

void radio_settings_save()
{
	uint32_t buffer;
	// RX freq
	if(saved_settings.rx_freq != cached_settings.rx_freq){
		EEEPROM_write_data(&radio_settings_eeeprom, RX_FREQ_EEEPROM_ADDR, &(cached_settings.rx_freq) );
	}

	// TX freq
	if(saved_settings.tx_freq != cached_settings.tx_freq){
		EEEPROM_write_data(&radio_settings_eeeprom, TX_FREQ_EEEPROM_ADDR, (void *)(&(cached_settings.tx_freq)));
	}

	// AGC, Power output, OpMode
	if( (saved_settings.agc_target != cached_settings.agc_target) ||
		(saved_settings.output_pwr != cached_settings.output_pwr) ||
		(saved_settings.mode != cached_settings.mode) ){
		buffer = cached_settings.agc_target +
				( ( (uint16_t)cached_settings.output_pwr ) << 8 ) +
				( ( (uint32_t)cached_settings.mode ) << 16 );
		EEEPROM_write_data(&radio_settings_eeeprom, AGC_PWR_MODE_EEEPROM_ADDR, (void *)(&buffer));
	}

	// M17 Callsign
	// using a func ptr from the user_settings

	// M17 DST
	if(strcmp(cached_settings.m17_dst, saved_settings.m17_dst) != 0){
		// Compare first four letters
		if(*(uint32_t *)(cached_settings.m17_dst) != *(uint32_t *)(saved_settings.m17_dst)){
			EEEPROM_write_data(&radio_settings_eeeprom, M17_DST1_EEEPROM_ADDR, (void *)cached_settings.m17_dst);
		}
		// Compare letters 5 to 8
		if(*(uint32_t *)(cached_settings.m17_dst+4) != *(uint32_t *)(saved_settings.m17_dst+4)){
			EEEPROM_write_data(&radio_settings_eeeprom, M17_DST2_EEEPROM_ADDR, (void *)(cached_settings.m17_dst+4));
		}
		// Compare letters 9-10
		if(*(uint16_t *)(cached_settings.m17_dst+8) != *(uint16_t *)(saved_settings.m17_dst+8)){
			EEEPROM_write_data(&radio_settings_eeeprom, M17_DST3_EEEPROM_ADDR, (void *)(cached_settings.m17_dst+8));
		}
	}

	// M17 info
	if(*(uint32_t *)(&saved_settings.m17_info) != *(uint32_t *)(&cached_settings.m17_info)){
		EEEPROM_write_data(&radio_settings_eeeprom, M17_INFO_EEEPROM_ADDR, (void *)(&(cached_settings.m17_info)));
	}

	// FM settings
	if(saved_settings.fm_settings.raw != cached_settings.fm_settings.raw){
		EEEPROM_write_data(&radio_settings_eeeprom, FM_SETTINGS_EEEPROM_ADDR, (void *)(&(cached_settings.fm_settings)));
	}

	// update the saved_settings to be equal to cached_settings now that the EEEPROM has been updated.
	memcpy(&saved_settings, &cached_settings, sizeof(radio_settings_t));
}

void radio_settings_subscribe_freq_changed(radio_setting_changed_func_t cb)
{
	radio_settings_rx_changed_cb = cb;
}

void radio_settings_set_rx_freq (freq_t freq)
{
	cached_settings.rx_freq = freq;

	if (radio_settings_rx_changed_cb != NULL) {
		radio_settings_rx_changed_cb();
	}
}

freq_t radio_settings_get_rx_freq (void)
{
	return cached_settings.rx_freq;
}

void radio_settings_set_tx_freq (freq_t freq)
{
	cached_settings.tx_freq = freq;
}

freq_t radio_settings_get_tx_freq (void)
{
	return cached_settings.tx_freq;
}

void radio_settings_set_agc_target (uint8_t agc_target)
{
	cached_settings.agc_target = agc_target;
}

uint8_t radio_settings_get_agc_target (void)
{
	return cached_settings.agc_target;
}

void radio_settings_set_output_pwr (uint8_t output_pwr)
{
	cached_settings.output_pwr = output_pwr;
}

uint8_t radio_settings_get_output_pwr (void)
{
	return cached_settings.output_pwr;
}

void radio_settings_set_mode (openht_mode_t mode)
{
	cached_settings.mode = mode;
}

openht_mode_t radio_settings_get_mode (void)
{
	return cached_settings.mode;
}

const char * radio_settings_get_m17_callsign(void)
{
	if (callsign_cb != NULL) {
		return callsign_cb();
	}

	return NULL;
}

void radio_settings_set_m17_dst (const char * m17_dst)
{
	strcpy(cached_settings.m17_dst, m17_dst);
}

const char * radio_settings_get_m17_dst (void)
{
	return cached_settings.m17_dst;
}

void radio_settings_set_m17_info (m17Info_t m17_info)
{
	cached_settings.m17_info = m17_info;
}

m17Info_t radio_settings_get_m17_info (void)
{
	return cached_settings.m17_info;
}

void radio_settings_set_fm_settings (fmInfo_t fm_settings)
{
	cached_settings.fm_settings = fm_settings;
}

fmInfo_t radio_settings_get_fm_settings (void)
{
	return cached_settings.fm_settings;
}
