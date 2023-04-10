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

#include "user_settings.h"
#include <stdlib.h>
#include <stdio.h>

void save_settings(const settings_t *settings)
{
	BSP_LED_Off(LED_ORANGE);
	BSP_LED_Off(LED_GREEN);

	if (BSP_QSPI_Erase_Block(WRITE_READ_ADDR) != QSPI_OK) {
		BSP_LED_On(LED_ORANGE);
	} else {
		if (BSP_QSPI_Write((uint8_t*) settings, WRITE_READ_ADDR,
				sizeof(settings_t)) != QSPI_OK) {
			BSP_LED_On(LED_ORANGE);
		} else {
			BSP_LED_On(LED_GREEN);
		}
	}
}

void get_settings(const settings_t *settings)
{
	BSP_LED_Off(LED_ORANGE);
	BSP_LED_Off(LED_GREEN);

	if (BSP_QSPI_Read((uint8_t*) settings, WRITE_READ_ADDR,
			sizeof(settings_t)) != QSPI_OK) {
		BSP_LED_On(LED_ORANGE);
	} else {
		BSP_LED_On(LED_GREEN);
	}
}

