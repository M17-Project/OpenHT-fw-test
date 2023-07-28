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

#ifndef INC_TASK_AUDIO_PROCESS_H_
#define INC_TASK_AUDIO_PROCESS_H_

#include "main.h"
#include "openht_types.h"

/* This task will do the codecs computations but will also manage
 * the audio inputs/outputs for the different modes */

void audio_process_init();

void audio_process_set_mode(openht_mode_t mode);

uint32_t write_tx_voice_samples(int16_t *samples, uint32_t number, uint32_t timeout);

uint32_t read_tx_baseband_samples(int16_t *samples, uint32_t number, uint32_t timeout);


#endif /* INC_TASK_AUDIO_PROCESS_H_ */
