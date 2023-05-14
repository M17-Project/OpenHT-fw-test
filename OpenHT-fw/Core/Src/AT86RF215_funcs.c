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
#include "openht_types.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_spi.h"
#include <math.h>

// TODO: pins and SPI not configured yet!
#define FPGA_RST_Pin 0
#define FPGA_RST_GPIO_Port 1

static SPI_HandleTypeDef hspi1;

void AT86_Write(const uint16_t addr, const uint8_t val)
{
	uint8_t tx[3]={(addr>>8)|(1<<7), addr&0xFF, val}; //write enable

	GPIOA->BSRR=(1<<(4+16));
	HAL_SPI_Transmit(&hspi1, tx, 3, 1);
	GPIOA->BSRR=(1<<4);
	HAL_Delay(2);
}

void AT86_Read(uint8_t* val, uint16_t addr)
{
	uint8_t tx[3]={addr>>8, addr&0xFF, 0};
	uint8_t rx[3]={0, 0, 0};

	GPIOA->BSRR=(1<<(4+16));
	HAL_SPI_TransmitReceive(&hspi1, tx, rx, 3, 2);
	GPIOA->BSRR=(1<<4);
	*val=rx[2];
}

uint8_t AT86_SetFreq(const uint32_t freq)
{
	if(freq>=389500000 && freq<=510000000) //range check
	{
		uint32_t val=round((freq-377000000)/(203125.0/2048.0));

		//use the block write starting at 0x0105
		//Fine Resolution Channel Scheme CNM.CM=1, 389.5-510.0MHz with 99.182Hz channel stepping
		uint8_t regs[6]={0x01|(1<<7), 0x05, (val>>8)&0xFF, (val>>16)&0xFF, (val>>0)&0xFF, 1<<6};

		GPIOA->BSRR=(1<<(4+16));
		HAL_SPI_Transmit(&hspi1, regs, 6, 1);
		GPIOA->BSRR=(1<<4);

		return 0;
	}
	else if(freq>=779000000 && freq<=1020000000) //range check
	{
		uint32_t val=round((freq-754000000)/(203125.0/1024.0));

		//use the block write starting at 0x0105
		//Fine Resolution Channel Scheme CNM.CM=2, 779-1020MHz with 198.364Hz channel stepping
		uint8_t regs[6]={0x01|(1<<7), 0x05, (val>>8)&0xFF, (val>>16)&0xFF, (val>>0)&0xFF, 2<<6};

		GPIOA->BSRR=(1<<(4+16));
		HAL_SPI_Transmit(&hspi1, regs, 6, 1);
		GPIOA->BSRR=(1<<4);

		return 0;
	}
	else if(freq>=2400000000 && freq<=2483500000) //range check
	{
		uint32_t val=round((freq-2366000000)/(406250.0/1024.0));

		//use the block write starting at 0x0205
		//Fine Resolution Channel Scheme CNM.CM=3, 2400-2483.5MHz with 396.728Hz channel stepping
		uint8_t regs[6]={0x02|(1<<7), 0x05, (val>>8)&0xFF, (val>>16)&0xFF, (val>>0)&0xFF, 3<<6};

		GPIOA->BSRR=(1<<(4+16));
		HAL_SPI_Transmit(&hspi1, regs, 6, 1);
		GPIOA->BSRR=(1<<4);

		return 0;
	}

	return 1;
}

void AT86_ConfigCommon(void)
{
	//AT86_Write(0x0007, 0b1010);		//CLKO to 32 MHz (default: 26 MHz) 4mA drive

	AT86_Write(0x000B, 0b10010);		//IQIFC1.CHPM=1; IQ mode for both transceivers
	//AT86_Write(0x0301, 0b11010100);		//BBC0_PC.CTX=1
	
	//AT86_Write(0x0306, 0xFF);			//BB0: frame length to max (2047)
	//AT86_Write(0x0307, 0x07);
	//AT86_Write(0x0406, 0xFF);			//BB1: frame length to max (2047)
	//AT86_Write(0x0407, 0x07);
}

//agc_en: 1-on, 0-off
//agc_tgt: 0..7
void AT86_RX09(uint32_t freq, float ppm, uint8_t agc_en, uint8_t agc_tgt)
{
	AT86_Write(0x0103, 0x02);			//RF09: set mode to TRXOFF; CMD=TRXOFF
	
	AT86_Write(0x010B, agc_en);			//RF09: AGC register (LSB=EN)
	AT86_Write(0x010C, agc_tgt<<5);		//RF09: AGC register (TGT: 0 - max)
	
	AT86_SetFreq(freq*(1.0f+ppm*1e-6));
	
	AT86_Write(0x010A,(4<<5)| 0xA);		//RF09: sample rate 400kHz, f_cut at 1x f_s/2
	
	HAL_GPIO_WritePin(FPGA_RST_GPIO_Port, FPGA_RST_Pin, 0);
	HAL_Delay(5);
	AT86_Write(0x0103, 0x05);			//RF09: set mode to RX; CMD=RX
}

//pwr: 0..0x1F
void AT86_TX09(uint32_t freq, float ppm, uint8_t pwr)
{
	AT86_Write(0x0103, 0x02);			//RF09: set mode to TRXOFF; CMD=TRXOFF

	AT86_Write(0x0114, (3<<5)|pwr);	//RF09: power output - max. 0x7F - default setting. range 0..0x1F
	AT86_SetFreq(freq*(1.0f+ppm*1e-6));

	AT86_Write(0x0112, 0);				//RF09:_TXCUTC=0; PA ramp and lowpass set to the minimum (80kHz)
	AT86_Write(0x0113, 0xA);			//RF09: sample rate 400kHz, f_cut at 1/4x f_s/2

	AT86_Write(0x0103, 0x03);			//RF09: set mode to TXPREP; CMD=TXPREP
	HAL_GPIO_WritePin(FPGA_RST_GPIO_Port, FPGA_RST_Pin, 0);
	HAL_Delay(5);
	AT86_Write(0x0103, 0x04);			//RF09: issue TX command
}

//agc_en: 1-on, 0-off
//agc_tgt: 0..7
void AT86_RX24(uint32_t freq, float ppm, uint8_t agc_en, uint8_t agc_tgt)
{
	AT86_Write(0x0203, 0x02);			//RF24: set mode to TRXOFF; CMD=TRXOFF

	AT86_Write(0x020B, agc_en);			//RF24: AGC register (LSB=EN)
	AT86_Write(0x020C, agc_tgt<<5);		//RF24: AGC register (TGT: 0 - max)

	AT86_SetFreq(freq*(1.0f+ppm*1e-6));

	AT86_Write(0x020A,(4<<5)| 0xA);		//RF24: sample rate 400kHz, f_cut at 1x f_s/2

	HAL_GPIO_WritePin(FPGA_RST_GPIO_Port, FPGA_RST_Pin, 0);
	HAL_Delay(5);
	AT86_Write(0x0203, 0x05);			//RF24: set mode to RX; CMD=RX
}

//pwr: 0..0x1F
void AT86_TX24(uint32_t freq, float ppm, uint8_t pwr)
{
	AT86_Write(0x0203, 0x02);			//RF24: set mode to TRXOFF; CMD=TRXOFF

	AT86_Write(0x0214, (3<<5)|pwr);	//RF24: power output - max. 0x7F - default setting. range 0..0x1F
	AT86_SetFreq(freq*(1.0f+ppm*1e-6));

	AT86_Write(0x0212, 0);				//RF24:_TXCUTC=0; PA ramp and lowpass set to the minimum (80kHz)
	AT86_Write(0x0213, 0xA);			//RF24: sample rate 400kHz, f_cut at 1/4x f_s/2

	AT86_Write(0x0203, 0x03);			//RF24: set mode to TXPREP; CMD=TXPREP
	HAL_GPIO_WritePin(FPGA_RST_GPIO_Port, FPGA_RST_Pin, 0);
	HAL_Delay(5);
	AT86_Write(0x0203, 0x04);			//RF24: issue TX command
}
