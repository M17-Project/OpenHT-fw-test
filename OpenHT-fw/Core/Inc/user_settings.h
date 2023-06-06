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

#include "openht_types.h"
#include "stm32469i_discovery.h" /* DISCOVERY includes component */
#include "stm32469i_discovery_qspi.h"


typedef struct
{
	uint32_t tx_freq;
	uint32_t rx_freq;
	char callsign[10];
	openht_mode_t mode;
	uint8_t audio_vol;
	bool use_freq_offset;
} settings_t;

void user_settings_reset();
void user_settings_save(const settings_t *settings);
void user_settings_get(settings_t *settings);

#ifdef __cplusplus
}
#endif

#endif /* INC_USER_SETTINGS_H_ */
