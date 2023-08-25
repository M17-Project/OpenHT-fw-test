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

#include "task_audio_process.h"

#include "main.h"
#include "openht_types.h"
#include "FreeRTOS.h"
#include "stream_buffer.h"

#include "../shell/inc/sys_command_line.h"


// Because CMSIS-OS2 does not provide anything resembling stream buffers, we use the ones from FreeRTOS
StreamBufferHandle_t audio_in_buffer = NULL;

void audio_process_init(){
	/* 64 samples = 128 bytes. Total size = 128 samples */
	/* Trigger level = 32 bytes = 16 samples */
	audio_in_buffer = xStreamBufferGenericCreate(256, 32, pdFALSE);
}

void audio_process_set_mode(openht_mode_t mode){
	DBG("Not implemented yet.\r\n");
}

uint32_t write_tx_voice_samples(int16_t *samples, uint32_t number, uint32_t timeout){
	size_t sent = xStreamBufferSend(audio_in_buffer, samples, number*2, timeout);

	return sent/2;
}

uint32_t read_tx_baseband_samples(int16_t *samples, uint32_t number, uint32_t timeout){
	size_t read = xStreamBufferReceive(audio_in_buffer, samples, number*2, timeout);

	return read/2;
}
