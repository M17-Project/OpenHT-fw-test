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

#ifndef INC_TASK_RADIO_H_
#define INC_TASK_RADIO_H_

#include "main.h"

#include <stdlib.h>

typedef enum{
	SAMPLES_IRQ,
	UPLOAD_BITSTREAM,
	DOWNLOAD_BITSTREAM,
	FPGA_SOFT_RESET,
	ERASE_BITSTREAM_STORAGE,
	INITN_IRQ,
	CONFIG,
	PTT_IRQ,
	START_RX,
	START_TX
} task_radio_event_t;

void StartTaskRadio(void *argument);

uint32_t task_radio_event(task_radio_event_t event);

#endif /* INC_TASK_RADIO_H_ */
