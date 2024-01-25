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

#include "audio_out_driver.h"
#include "cs43l22.h"

#include "main.h"

#include <cmsis_os2.h>

extern I2C_HandleTypeDef hi2c2;
extern SAI_HandleTypeDef hsai_BlockA1;
extern DMA_HandleTypeDef hdma_sai1_a;


/**
 * Writes a register value to the codec via the I2C bus (in blocking mode)
 *
 * @param	address	Register address to write to
 * @param	value	Value to write to the register
 */
void codec_set_register(uint8_t address, uint8_t value);

/**
 * Read a register from the codec via the I2C bus (in blocking mode)
 *
 * @param	address	Register address to read from
 * @param	*value	Pointer to the location where the value read will be stored
 */
void codec_get_register(uint8_t address, uint8_t *value);

void audio_out_init(){

	// Frame Sync Active Length => Half the frame for LR
	hsai_BlockA1.Instance = SAI1_Block_A;
	hsai_BlockA1.Init.AudioFrequency = AUDIO_SAMPLE_RATE; // Will actually resolve to 24k
	hsai_BlockA1.Init.AudioMode = SAI_MODEMASTER_TX;
	hsai_BlockA1.Init.ClockSource = SAI_CLKSOURCE_PLLI2S;
	hsai_BlockA1.Init.ClockStrobing = SAI_CLOCKSTROBING_FALLINGEDGE;
	hsai_BlockA1.Init.CompandingMode = SAI_NOCOMPANDING;
	hsai_BlockA1.Init.DataSize = SAI_DATASIZE_16;
	hsai_BlockA1.Init.FIFOThreshold = SAI_FIFOTHRESHOLD_HF;
	hsai_BlockA1.Init.FirstBit = SAI_FIRSTBIT_MSB;
	//hsai_BlockA1.Init.Mckdiv = // Computed automatically
	hsai_BlockA1.Init.MonoStereoMode = SAI_MONOMODE;
	hsai_BlockA1.Init.NoDivider = SAI_MASTERDIVIDER_ENABLE;
	hsai_BlockA1.Init.OutputDrive = SAI_OUTPUTDRIVE_ENABLE;
	hsai_BlockA1.Init.Protocol = SAI_FREE_PROTOCOL;
	hsai_BlockA1.Init.Synchro = SAI_ASYNCHRONOUS;
	hsai_BlockA1.Init.SynchroExt = SAI_SYNCEXT_DISABLE;
	hsai_BlockA1.Init.TriState = SAI_OUTPUT_NOTRELEASED;
	hsai_BlockA1.FrameInit.ActiveFrameLength = 32;
	hsai_BlockA1.FrameInit.FSDefinition = SAI_FS_CHANNEL_IDENTIFICATION;
	hsai_BlockA1.FrameInit.FSOffset = SAI_FS_FIRSTBIT;
	hsai_BlockA1.FrameInit.FSPolarity = SAI_FS_ACTIVE_LOW;
	hsai_BlockA1.FrameInit.FrameLength = 64;
	hsai_BlockA1.SlotInit.FirstBitOffset = 0;
	hsai_BlockA1.SlotInit.SlotSize = SAI_SLOTSIZE_32B;
	hsai_BlockA1.SlotInit.SlotNumber = 2;
	hsai_BlockA1.SlotInit.SlotActive = (1 << 0) | (1 << 1);

	// Release codec reset pin
	HAL_GPIO_WritePin(AUDIO_RST_GPIO_Port, AUDIO_RST_Pin, GPIO_PIN_SET);

	// Codec Config
	codec_set_register(PWR_CTRL_1, POWER_DOWN);
	codec_set_register(PWR_CTRL_2, PDN_HPB_INV | PDN_HPA_INV | PDN_SPKB_OFF | PDN_SPKA_NORM); // HP when PE3 is low, SPK when high

	// Audo-detection disabled 						(0b0)
	// Single Speed									(0b01)
	// not in 32k group								(0b0)
	// no video clock								(0b0)
	// Ratio MCLK/LRCLK of 128, SCLK/LRCLK of 64	(0b00)
	// MCLK not divided by 2						(0b0)
	codec_set_register(CLK_CTRL, CLK_SPEED_SS | CLK_M_LR_RATIO_128);

	// Slave							(0b0)
	// SCLK polarity not inverted		(0b0)
	// Reserved							(0b0)
	// DSP mode disabled				(0b0)
	// DAC format left justified		(0b00)
	// Audio word length 16 bits		(0b11)
	codec_set_register(IFC_CTRL_1, IFC_SLAVE | IFC_DAC_FMT_LEFT| IFC_AUDIO_WORD_16_16);

	// Set volume of PCM channels
	codec_set_register(PCMA_VOLUME, (127-24)&PCMx_VOL_MASK); // -12 dB because Master can go up to +12
	codec_set_register(PCMB_VOLUME, (127-24)&PCMx_VOL_MASK); // -12 dB

	// Set headphones volume
	codec_set_register(HPA_VOLUME, 0); // -6 dB
	codec_set_register(HPB_VOLUME, 0); // -6 dB

	// Set speaker volume
	codec_set_register(SPKA_VOLUME, 0); //0dB, max volume
	codec_set_register(SPKB_VOLUME, 1); // SPK B Muted

	// Set master volume (A and B)
	codec_set_register(MSTRA_VOLUME, -12); // 0dB
	codec_set_register(MSTRB_VOLUME, -12); // 0dB

	codec_set_register(LIMIT_CTRL_2, PEAK_DET_LIM_EN);
	codec_set_register(LIMIT_ATK_RATE, 0b00111111);

	// Continue power-up sequence (those registers are undocumented)
	codec_set_register(0x00, 0x99);
	codec_set_register(0x47, 0x80);
	uint8_t tmp = 0;
	codec_get_register(0x32, &tmp);
	tmp |= (1<<7);
	codec_set_register(0x32, tmp);
	codec_get_register(0x32, &tmp);
	tmp &= ~(1<<7);
	codec_set_register(0x32, tmp);
	codec_set_register(0x00, 0x00);

	// Initialize SAI, starts the clocks
	HAL_SAI_Init(&hsai_BlockA1);
	__HAL_SAI_ENABLE(&hsai_BlockA1);
	hsai_BlockA1.Instance->DR = 0x0000;
	osDelay(10);

	// Enable the codec
    codec_set_register(PWR_CTRL_1, POWER_UP);
}

void audio_out_disable(){

	// Mute DAC (HP)
	codec_set_register(HPA_VOLUME, HPx_VOLUME_MUTED);
	codec_set_register(HPB_VOLUME, HPx_VOLUME_MUTED);

	// Mute PWM (Speaker)
	codec_set_register(SPKA_VOLUME, SPKx_VOLUME_MUTED);
	codec_set_register(SPKB_VOLUME, SPKx_VOLUME_MUTED);

	// Disable soft ramp and Zero cross volume transitions
	codec_set_register(MISC_CTRL, 0);

	// Power CTL 1
	codec_set_register(PWR_CTRL_1, POWER_DOWN);

	// Wait 100us
	osDelay(1);

	// Disable SAI
	HAL_SAI_DeInit(&hsai_BlockA1);

	// Set reset pin low
	HAL_GPIO_WritePin(AUDIO_RST_GPIO_Port, AUDIO_RST_Pin, GPIO_PIN_RESET);
}

void audio_set_volume(uint8_t volume){
	// Set master volume (A and B)
	if(volume > MSTRx_VOLUME_MAX && volume < MSTRx_VOLUME_MIN){
		volume = MSTRx_VOLUME_MIN;
	}
	codec_set_register(MSTRA_VOLUME, volume);
	codec_set_register(MSTRB_VOLUME, volume);
}

void audio_set_output(AudioOutputDevice output){
	if(output == HEADPHONES){
		HAL_GPIO_WritePin(SPKR_HP_GPIO_Port, SPKR_HP_Pin, GPIO_PIN_RESET);
	}else{
		HAL_GPIO_WritePin(SPKR_HP_GPIO_Port, SPKR_HP_Pin, GPIO_PIN_SET);
	}
}

void audio_set_mute(bool muted){
	uint8_t reg = 0;
	codec_get_register(PLAYBACK_CTRL_2, &reg);
	if(muted){
		reg |= HPA_MUTE | HPB_MUTE | SPKA_MUTE;
	}else{
		reg &= ~HPA_MUTE & ~HPB_MUTE & ~SPKA_MUTE;
	}

	codec_set_register(PLAYBACK_CTRL_2, reg);
}

void codec_set_register(uint8_t address, uint8_t value){
	uint8_t buffer[2] = {address, value};
	HAL_I2C_Master_Transmit(&hi2c2, CS43L22_ADDR, buffer, 2, 10);
}
void codec_get_register(uint8_t address, uint8_t *value){
	HAL_I2C_Master_Transmit(&hi2c2, CS43L22_ADDR, &address, 1, 10);
	HAL_I2C_Master_Receive(&hi2c2, CS43L22_ADDR, value, 1, 10);
}

