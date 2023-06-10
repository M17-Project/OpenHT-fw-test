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

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <openht_hwconfig.h>
#include <openht_hwinterface.h>

// This file contains the unique hardware configuration of the OpenHT.

static openht_hwconfig_t openht_hwconfig;

void init_openht_hwconfig(void)
{
	// setup callbacks
	openht_hwconfig.set_frequency_cb = &openht_hw_set_freq;
	openht_hwconfig.tx_start_cb = &openht_hw_tx_start;
	openht_hwconfig.tx_end_cb = &openht_hw_tx_end;

	// PoC bands supported:
	// 389.5-510 MHz <-- LPF to about 450 MHz
	// 779-1020 MHz <-- LPF'd away
	// 2400- 2483.5 MHz

	openht_hwconfig.num_bands = 2;

	openht_hwconfig.bands = malloc(sizeof(openht_bandrange_t) * openht_hwconfig.num_bands);

	openht_hwconfig.bands[0].start_freq = 389500000;
	openht_hwconfig.bands[0].end_freq =   450000000;

//	openht_hwconfig.bands[1].start_freq = 779000000;
//	openht_hwconfig.bands[1].end_freq =  1020000000;

	openht_hwconfig.bands[1].start_freq =2400000000;
	openht_hwconfig.bands[1].end_freq =  2483500000;
}

openht_hwconfig_t get_openht_hwconfig()
{
	return openht_hwconfig;
}
