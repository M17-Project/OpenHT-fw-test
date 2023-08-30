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

#ifndef INC_USER_SETTINGS_H_
#define INC_USER_SETTINGS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "cps.h"
#include "openht_types.h"

#include "stm32469i_discovery.h" /* DISCOVERY includes component */
#include "stm32469i_discovery_qspi.h"

#define N0CALL "N0CALL"

typedef struct
{
	char callsign[10];
	uint8_t audio_vol;
	uint8_t mic_gain;
	bool use_freq_offset;  // using eeeprom bits for settings
	bool split_mode;
	bool show_callsign_boot;
	bool use_soft_ptt;
} __attribute__((packed)) user_settings_t;

void user_settings_reset();
void user_settings_init();
void user_settings_save();//const user_settings_t *settings);
//void user_settings_get(user_settings_t *settings);

typedef const char * (*user_callsign_func_t)();
void user_settings_set_callsign(const char *);
const char * user_settings_get_callsign(void);

void user_settings_set_audio_vol(uint8_t audio_vol);
uint8_t user_settings_get_audio_vol(void);

void user_settings_set_mic_gain(uint8_t mic_gain);
uint8_t user_settings_get_mic_gain(void);

void user_settings_set_use_freq_offset(bool use_freq_offset);
bool user_settings_get_use_freq_offset(void);

void user_settings_set_split_mode(bool split_mode);
bool user_settings_get_split_mode(void);

void user_settings_set_show_callsign_boot(bool show_callsign_boot);
bool user_settings_get_show_callsign_boot(void);

void user_settings_set_use_soft_ptt(bool use_soft_ptt);
bool user_settings_get_use_soft_ptt(void);


#ifdef __cplusplus
}
#endif

#endif /* INC_USER_SETTINGS_H_ */
