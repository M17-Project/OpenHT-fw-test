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

#ifndef INC_OPENHT_HWCONFIG_H_
#define INC_OPENHT_HWCONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "openht_types.h"

#define NUM_BANDS 3  // number of bands supported by hardware

//typedef enum
//{
//	OPENHT_OK = 0, /* (0) Succeeded */
//	OPENHT_ERR, /* (1) A error occurred */
//} openht_res_t;

typedef struct
{
	freq_t start_freq;
	freq_t end_freq;
} openht_bandrange_t;

typedef struct
{
	void (*set_frequency_cb)(freq_t);
	void (*tx_start_cb)(void);
	void (*tx_end_cb)(void);

	uint8_t num_bands;
	openht_bandrange_t * bands;

} openht_hwconfig_t;



void init_openht_hwconfig(void);
openht_hwconfig_t get_openht_hwconfig();

#ifdef __cplusplus
}
#endif

#endif /* INC_OPENHT_HWCONFIG_H_ */
