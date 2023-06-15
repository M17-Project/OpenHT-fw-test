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

#ifndef INC_RADIO_SETTINGS_H_
#define INC_RADIO_SETTINGS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "cps.h"
#include "openht_types.h"

#include "stm32469i_discovery.h" /* DISCOVERY includes component */
#include "stm32469i_discovery_qspi.h"


typedef struct
{
	freq_t rx_freq;			// RX_FREQ_EEEPROM_ADDR
	freq_t tx_freq;			// TX_FREQ_EEEPROM_ADDR

	uint8_t agc_target;		// AGC_PWR_MODE_EEEPROM_ADDR
	uint8_t output_pwr; 	// AGC_PWR_MODE_EEEPROM_ADDR

	openht_mode_t mode; 	// AGC_PWR_MODE_EEEPROM_ADDR
	//uint8_t reserved; 	// AGC_PWR_MODE_EEEPROM_ADDR

	char m17_callsign[10];	// M17_CALLSIGN[1][2][3]_EEEPROM_ADDR
	char m17_dst[10];	 	// M17_DST[1][2][3]_EEEPROM_ADDR
	m17Info_t m17_info;		// M17_INFO_EEEPROM_ADDR

	fmInfo_t fm_settings;	// FM_SETTINGS_EEEPROM_ADDR

} __attribute__((packed)) radio_settings_t; // pack the struct into smallest data size

void radio_settings_reset();
void radio_settings_save(const radio_settings_t *settings);
void radio_settings_get(radio_settings_t *settings);

#ifdef __cplusplus
}
#endif

#endif /* INC_RADIO_SETTINGS_H_ */