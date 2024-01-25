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

#include "task_audio_out.h"

#include "task_audio_process.h"
#include "cli_commands.h"

#include "../../Drivers/audio/audio_out_driver.h"
#include "main.h"
#include "../../Drivers/audio/cs43l22.h"
#include "../shell/inc/sys_command_line.h"

#include <cmsis_os2.h>
#include <FreeRTOS.h>
#include <math.h>
#include <string.h>

#define SAI_DMA_BUFFER_SAMPLES 	(480)
#define SAI_DMA_BUFFER_BYTES 	(SAI_DMA_BUFFER_SAMPLES*2)

#define DMA_INTERRUPT_FLAG 		(1 << 0)

#define ALL_FLAGS				(DMA_INTERRUPT_FLAG)

extern SAI_HandleTypeDef hsai_BlockA1;

/* DMA buffer variables */
int16_t sai_tx_buffer[SAI_DMA_BUFFER_SAMPLES*2]; // Two times 480 samples, 40ms of audio
int16_t *buffer_wr_pointer;

osThreadId_t audio_thread_id;

/* Beeps variables */
volatile bool 	beep_enabled 		= false;
float 			beep_2_pi_f 		= 0;
float 			beep_t 				= 0;
int16_t			beep_amplitude 		= 0;
size_t 			beep_samples_left 	= 0;

/* dma checks */
volatile bool dma_done = true;
volatile bool dma_underrun = false;

/* private functions */

/**
 * Gets the samples from the beeps channel
 *
 * @param	samples	Number of samples to read
 * @param	*buffer	place where to store the samples
 *
 * @return	number of samples written to the buffer (between 0 and samples)
 */
size_t audio_output_beeps_run(size_t samples, int16_t *buffer);

void StartTaskAudioOut(void *argument){
	audio_thread_id = osThreadGetId();
	static uint32_t status_tick = 0;
	bool speaker = true;
	CLI_ADD_CMD("audio", audio_help, cli_audio_cmd);

	MX_I2C2_Init();

	audio_output_speaker(speaker);

	audio_out_init();

	audio_output_start();

	audio_output_volume(100);

	for(;;){
		uint32_t flags = osThreadFlagsWait(ALL_FLAGS, osFlagsNoClear, osWaitForever);

		if(flags & DMA_INTERRUPT_FLAG){
			osThreadFlagsClear(DMA_INTERRUPT_FLAG);
			/* Read the audio to send to the speaker */
			uint32_t read = read_rx_audio_samples(buffer_wr_pointer, SAI_DMA_BUFFER_SAMPLES, 2);
			if(read < SAI_DMA_BUFFER_SAMPLES){
				memset(buffer_wr_pointer + read, 0, (SAI_DMA_BUFFER_SAMPLES - read)*sizeof(buffer_wr_pointer[0]));
			}

			/* If the beeps channel is enabled, process it and add it to the audio samples */
			if(beep_enabled){
				int16_t beep_samples[SAI_DMA_BUFFER_SAMPLES];
				// Read samples from beeps channel
				size_t beep_samples_n = audio_output_beeps_run(SAI_DMA_BUFFER_SAMPLES, beep_samples);

				// Add beeps samples
				for(size_t i = 0; i < beep_samples_n; i++){
					buffer_wr_pointer[i] = (buffer_wr_pointer[i]/2) + (beep_samples[i]/2);
				}
			} /* end beep_enabled */
			dma_done = true;
			if(dma_underrun){
				LOG(CLI_LOG_GENERAL, "Audio out DMA underrun.\r\n");
				dma_underrun = false;
			}
		} /* end flags tests */


		if( (osKernelGetTickCount()-status_tick) > 10000){
			uint8_t tmp;
			codec_get_register(STATUS, &tmp);
			LOG(CLI_LOG_AUDIO_OUT, "Status reg = 0x%02x.\r\n", tmp);
			codec_get_register(SPK_STATUS, &tmp);
			LOG(CLI_LOG_AUDIO_OUT, "SPK Status reg = 0x%02x.\r\n", tmp);
			status_tick = osKernelGetTickCount();
			speaker = !speaker;
			//audio_output_speaker(speaker);
		}

	} /* end for(;;) */
}

void audio_output_start(){

	uint32_t read = read_rx_audio_samples(sai_tx_buffer, 2*SAI_DMA_BUFFER_SAMPLES, 7);
	if(read < 2*SAI_DMA_BUFFER_SAMPLES){
		memset(sai_tx_buffer + read, 0, (2*SAI_DMA_BUFFER_SAMPLES-read)*2);
	}
	HAL_SAI_Transmit_DMA(&hsai_BlockA1, (uint8_t *)sai_tx_buffer, 2*SAI_DMA_BUFFER_SAMPLES);
}

void audio_output_stop(){
	HAL_SAI_DMAStop(&hsai_BlockA1);
}

void audio_output_mute(){
	audio_set_mute(true);
}

void audio_output_unmute(){
	audio_set_mute(false);
}

void audio_output_volume(uint8_t volume){
	if(volume > 100){
		volume = 100;
	}

	volume = volume*(24+204)/100 - 204;
	audio_set_volume(volume);
}

void audio_output_speaker(bool enable){
	if(enable){
		audio_set_output(SPEAKER);
	}else{
		audio_set_output(HEADPHONES);
	}
}

void audio_output_beep(float frequency, size_t duration, uint8_t volume){
	if(beep_enabled){
		return;
	}
	beep_samples_left = duration*AUDIO_SAMPLE_RATE/1000;
	beep_amplitude = INT16_MAX*((float)volume/UINT8_MAX);
	beep_t = 0;
	beep_2_pi_f = 2.0*M_PI*frequency;
	beep_enabled = true;
}

size_t audio_output_beeps_run(size_t samples, int16_t *buffer){

	if(!beep_enabled){
		return 0;
	}

	if(beep_samples_left < samples){
		samples = beep_samples_left;
	}

	for(size_t n = 0; n < samples; n++){
		buffer[n] = beep_amplitude*sinf(beep_2_pi_f*beep_t);
		beep_t += 1.0/AUDIO_SAMPLE_RATE;
	}

	beep_samples_left -= samples;
	if(beep_samples_left <= 0){
		beep_enabled = false;
	}

	return samples;
}

void HAL_SAI_TxCpltCallback(SAI_HandleTypeDef *hsai){
	buffer_wr_pointer = sai_tx_buffer + SAI_DMA_BUFFER_SAMPLES;
	if(!dma_done){
		dma_underrun = true;
	}
	dma_done = false;
	osThreadFlagsSet(audio_thread_id, DMA_INTERRUPT_FLAG);
}

void HAL_SAI_TxHalfCpltCallback(SAI_HandleTypeDef *hsai){
	buffer_wr_pointer = sai_tx_buffer;
	if(!dma_done){
		dma_underrun = true;
	}
	dma_done = false;
	osThreadFlagsSet(audio_thread_id, DMA_INTERRUPT_FLAG);
}
