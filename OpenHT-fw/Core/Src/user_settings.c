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

#include "nor_map.h"
#include "eeeprom.h"
#include "eeeprom_hal.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

static const uint8_t CALLSIGN1_EEEPROM_ADDR =		0x00;	// Contains CS letters [1:4]
static const uint8_t CALLSIGN2_EEEPROM_ADDR =		0x01;	// Contains CS letters [5:8]
static const uint8_t CALLSIGN3_EEEPROM_ADDR = 		0x02;	// Contains CS letters [9:10]
static const uint8_t AV_MODE_EEEPROM_ADDR =			0x03;	// Contains Audio volume and reserved
static const uint8_t CONFIG_BITS_EEEPROM_ADDR =		0x04;	// Configuration bool values

EEEPROMHandle_t user_settings_eeeprom = {
		.address_size = EEEPROM_ADDRESS_1BYTE,
		.alignment = 1,
		.data_size = 4,
		.read = EEEPROM_HAL_qspi_read,
		.write = EEEPROM_HAL_qspi_write,
		.erase_page = EEEPROM_HAL_erase_subsector,
		.number_pages = USER_SETTINGS_NB_SECTORS,
		.page_offset = USER_SETTINGS_START_SECTOR,
		.page_size = SUBSECTOR_SIZE,
		.start_address = USER_SETTINGS_START_ADDRESS,
};

static bool init_done = false;
static user_settings_t cached_settings;
static user_settings_t saved_settings;

void user_settings_reset(){
	bool res = EEEPROM_erase(&user_settings_eeeprom);
	if( res != EXIT_SUCCESS){
		printf("Error erasing EEPROM content for user settings.\r\n");
		return;
	}
	printf("Successfully erased user settings. Rebooting\r\n");
	NVIC_SystemReset();
}

void user_settings_init()
{
	if (init_done) {
		return;
	}

	if(EEEPROM_init(&user_settings_eeeprom) != EXIT_SUCCESS){
		printf("Error initializing EEEPROM for user settings.\r\n");
	}

	uint32_t buffer;

	// set the default callsign...
	strcpy(saved_settings.callsign, N0CALL);

	// CS letters 1 to 4
	if(EEEPROM_read_data(&user_settings_eeeprom, CALLSIGN1_EEEPROM_ADDR, &buffer) == EXIT_SUCCESS){
		*(uint32_t *)(saved_settings.callsign) = buffer;
	}

	// CS letters 5 to 8
	if(EEEPROM_read_data(&user_settings_eeeprom, CALLSIGN2_EEEPROM_ADDR, &buffer) == EXIT_SUCCESS){
		*(uint32_t *)(saved_settings.callsign+4) = buffer;
	}

	// CS letters 9-10
	if(EEEPROM_read_data(&user_settings_eeeprom, CALLSIGN3_EEEPROM_ADDR, &buffer) == EXIT_SUCCESS){
		*(uint16_t *)(saved_settings.callsign+8) = (uint16_t)buffer;
	}

	// Audio volume
	if(EEEPROM_read_data(&user_settings_eeeprom, AV_MODE_EEEPROM_ADDR, &buffer) == EXIT_SUCCESS){
		saved_settings.audio_vol = buffer & 0xFF;
		saved_settings.mic_gain = (buffer>>8) & 0xFF;
	}else{
		saved_settings.audio_vol = 0;
		saved_settings.mic_gain = 22;  // 17-26 range
	}

	// config bits
	if(EEEPROM_read_data(&user_settings_eeeprom, CONFIG_BITS_EEEPROM_ADDR, &buffer) == EXIT_SUCCESS){
		saved_settings.use_freq_offset = buffer >> 0 & 0x01;
		saved_settings.split_mode = buffer >> 1 & 0x01;
		saved_settings.use_soft_ptt = buffer >> 2 & 0x01;
	}else{
		saved_settings.use_freq_offset = false;
		saved_settings.split_mode = false;
		saved_settings.use_soft_ptt = false;
	}

	// set the saved_settings (which represents the contents of the EEEPROM) and the
	// cached_settings (which is the volatile settings) to be equal
	memcpy(&cached_settings, &saved_settings, sizeof(user_settings_t));

	init_done = true;
}

void user_settings_save()
{
	uint32_t buffer;
	if(strcmp(cached_settings.callsign, saved_settings.callsign) != 0){
		// Compare first four letters
		if(*(uint32_t *)(cached_settings.callsign) != *(uint32_t *)(saved_settings.callsign)){
			EEEPROM_write_data(&user_settings_eeeprom, CALLSIGN1_EEEPROM_ADDR, (void *)cached_settings.callsign);
		}
		// Compare letters 5 to 8
		if(*(uint32_t *)(cached_settings.callsign+4) != *(uint32_t *)(saved_settings.callsign+4)){
			EEEPROM_write_data(&user_settings_eeeprom, CALLSIGN2_EEEPROM_ADDR, (void *)(cached_settings.callsign+4));
		}
		// Compare letters 9-10
		if(*(uint16_t *)(cached_settings.callsign+8) != *(uint16_t *)(saved_settings.callsign+8)){
			EEEPROM_write_data(&user_settings_eeeprom, CALLSIGN3_EEEPROM_ADDR, (void *)(cached_settings.callsign+8));
		}
	}

	if( (cached_settings.audio_vol != saved_settings.audio_vol)
		|| (cached_settings.mic_gain != saved_settings.mic_gain) ){
		buffer = cached_settings.audio_vol + ( ( (uint16_t)cached_settings.mic_gain ) << 8 );
		EEEPROM_write_data(&user_settings_eeeprom, AV_MODE_EEEPROM_ADDR, (void *)(&buffer));
	}

	if( (cached_settings.use_freq_offset != saved_settings.use_freq_offset)
			|| (cached_settings.split_mode != saved_settings.split_mode)
			|| (cached_settings.use_soft_ptt != saved_settings.use_soft_ptt) ){
		buffer = (cached_settings.use_freq_offset << 0) +
				 (cached_settings.split_mode << 1) +
				 (cached_settings.use_soft_ptt << 2);
		EEEPROM_write_data(&user_settings_eeeprom, CONFIG_BITS_EEEPROM_ADDR, (void *)(&buffer));
	}

	// update the saved_settings to be equal to cached_settings now that the EEEPROM has been updated.
	memcpy(&saved_settings, &cached_settings, sizeof(user_settings_t));
}

void user_settings_set_callsign(const char * callsign)
{
	strcpy(cached_settings.callsign, callsign);
}

const char * user_settings_get_callsign()
{
	if(!init_done){
		user_settings_init();
	}

	return (const char *) cached_settings.callsign;
}

void user_settings_set_audio_vol(uint8_t audio_vol)
{
	cached_settings.audio_vol = audio_vol;
}

uint8_t user_settings_get_audio_vol(void)
{
	return cached_settings.audio_vol;
}

void user_settings_set_mic_gain(uint8_t mic_gain)
{
	cached_settings.mic_gain = mic_gain;
}

uint8_t user_settings_get_mic_gain(void)
{
	return cached_settings.mic_gain;
}

void user_settings_set_use_freq_offset(bool use_freq_offset)
{
	cached_settings.use_freq_offset = use_freq_offset;
}

bool user_settings_get_use_freq_offset(void)
{
	return cached_settings.use_freq_offset;
}

void user_settings_set_split_mode(bool split_mode)
{
	cached_settings.split_mode = split_mode;
}

bool user_settings_get_split_mode(void)
{
	return cached_settings.split_mode;
}

void user_settings_set_use_soft_ptt(bool use_soft_ptt)
{
	cached_settings.use_soft_ptt = use_soft_ptt;
}

bool user_settings_get_use_soft_ptt(void)
{
	return cached_settings.use_soft_ptt;
}


