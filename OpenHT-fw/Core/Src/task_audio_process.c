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
	/* 16 samples = 32 bytes. Total size = 32 samples */
	audio_in_buffer = xStreamBufferGenericCreate(64, 32, pdFALSE);
}

void audio_process_set_mode(openht_mode_t mode){
	DBG("Not implemented yet.\r\n");
}

uint32_t write_voice_samples(uint16_t *samples, uint32_t number){
	uint32_t nb_samples_free = xStreamBufferSpacesAvailable(audio_in_buffer)/2;
	uint32_t written;
	if(nb_samples_free >= number){
		xStreamBufferSend(audio_in_buffer, samples, number*2, 0);
		written = number;
	}else{
		xStreamBufferSend(audio_in_buffer, samples, nb_samples_free*2, 0);
		written = nb_samples_free;
	}

	return written;
}

uint32_t read_voice_samples(uint16_t *samples, uint32_t number){
	uint32_t nb_samples_available = xStreamBufferBytesAvailable(audio_in_buffer)/2;
	uint32_t read;
	if(nb_samples_available < number){
		xStreamBufferReceive(audio_in_buffer, samples, nb_samples_available, 0);
		for(size_t i = nb_samples_available; i < number; i++){
			samples[i] = samples[nb_samples_available-1];
		}
		read = nb_samples_available;
	}else{
		xStreamBufferReceive(audio_in_buffer, samples, number, 0);
		read = number;
	}

	return read;
}
