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

#ifndef AUDIO_AUDIO_OUT_DRIVER_H_
#define AUDIO_AUDIO_OUT_DRIVER_H_

#include "main.h"

#define CS43L22_ADDR		0x94 	//8 bits address
#define AUDIO_SAMPLE_RATE 	8000

typedef enum {
	HEADPHONES,
	SPEAKER
} AudioOutputDevice;

/**
 * Initialize the audio output
 *
 * Power on the codec, initializes and start the SAI peripheral. Initial output volume is -60.
 * Takes about 1ms.
 */
void audio_out_init();

/**
 * De-initialize the audio output
 *
 * Power off the codec, de-init the SAI peripheral.
 * Takes about 1ms.
 */
void audio_out_disable();

/**
 * Sets the volume of the audio output
 *
 * @param	volume	Volume value as an attenuation (value between 0 and -255, -255 mutes the output)
 */
void audio_set_volume(uint8_t volume);

/**
 * Sets the audio output device
 *
 * @param	output	Output device
 */
void audio_set_output(AudioOutputDevice output);

/**
 * Mutes the audio output
 *
 * @param	muted	If true, mutes the output. If false, output is normal.
 */
void audio_set_mute(bool muted);


void codec_set_register(uint8_t address, uint8_t value);
void codec_get_register(uint8_t address, uint8_t *value);

#endif /* AUDIO_AUDIO_OUT_DRIVER_H_ */
