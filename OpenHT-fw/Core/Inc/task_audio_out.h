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

#ifndef INC_TASK_AUDIO_OUT_H_
#define INC_TASK_AUDIO_OUT_H_

#include "main.h"

#include <stdbool.h>
#include <stdint.h>

void StartTaskAudioOut(void *argument);

/**
 * Start the output audio stream
 */
void audio_output_start();

/**
 * Stop the output audio stream
 */
void audio_output_stop();

/**
 * Mutes the audio output
 */
void audio_output_mute();

/**
 * Un-mutes the audio output
 */
void audio_output_unmute();

/**
 * Sets the audio output volume
 *
 * @param	volume	Volume setting, an integer between 0 and 40
 */
void audio_output_volume(uint8_t volume);

/**
 * Selects the audio output device
 *
 * @param 	enable	If true, selects the speaker, if false, select the headphones.
 */
void audio_output_speaker(bool enable);

/**
 * Generates a beep sound effect on the beeps channel
 *
 * @param	frequency	Sets the beep frequency (in Hz)
 * @param	duration	Beep duration (in ms)
 * @param	volume		Beep volume (range from 0 to 255)
 */
void audio_output_beep(float frequency, size_t duration, uint8_t volume);

#endif /* INC_TASK_AUDIO_OUT_H_ */
