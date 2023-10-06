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

#include "radio_hal.h"

#include "main.h"
#include "ui/openht_ui.h"
#include "openht_types.h"
#include "at86rf215.h"
#include "fpga_reg_defs.h"
#include "cps.h"

#include "../shell/inc/sys_command_line.h"
#include <cmsis_os2.h>

#include <stdbool.h>
#include <string.h>
#include <math.h>

#define SPI_PORT_ACTIVATION_KEY	0x8AF4C6A4 // Swapped for endianness

extern SPI_HandleTypeDef 	hspi1;

void radio_on(){
	HAL_GPIO_WritePin(MAIN_KILL_GPIO_Port, MAIN_KILL_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(FPGA_PROGRAMN_GPIO_Port, FPGA_PROGRAMN_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(FPGA_NSS_GPIO_Port, FPGA_NSS_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(FPGA_RST_GPIO_Port, FPGA_RST_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(XCVR_NSS_GPIO_Port, XCVR_NSS_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(SW_CTL1_GPIO_Port, SW_CTL1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(SW_CTL2_GPIO_Port, SW_CTL2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(RF_RST_GPIO_Port, RF_RST_Pin, GPIO_PIN_RESET);
}

void radio_off(){
	HAL_GPIO_WritePin(MAIN_KILL_GPIO_Port, MAIN_KILL_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(FPGA_PROGRAMN_GPIO_Port, FPGA_PROGRAMN_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(FPGA_NSS_GPIO_Port, FPGA_NSS_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(FPGA_RST_GPIO_Port, FPGA_RST_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(XCVR_NSS_GPIO_Port, XCVR_NSS_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(SW_CTL1_GPIO_Port, SW_CTL1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(SW_CTL2_GPIO_Port, SW_CTL2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(RF_RST_GPIO_Port, RF_RST_Pin, GPIO_PIN_RESET);
}

void radio_configure_rx(uint32_t freq, float ppm, openht_mode_t mode, fmInfo_t fm, openht_radio_agc agc){
	XCVR_stop_operation();

	// TODO: CTCSS doesn't work on RX yet
	uint16_t ctcss = 0;
	if(fm.rxToneEn){
		ctcss = fm.rxTone << 2;
	}

	// TODO: Since we only support FM at the moment, this is fine
	uint16_t fm_mode = TX_CTRL_FMN;
	if (mode == OpMode_WFM) {
		fm_mode = TX_CTRL_FMW;
	}

	// Set frequency
	uint32_t val = round(freq*(1.0f+ppm/1e6));

	if(freq > 1e9){
		LOG(CLI_LOG_RADIO, "Radio set in RX 2.4G.\r\n");

		// Configure switch
		radio_sw_24();

		// Configure 2.4G transceiver
		XCVR_write_reg(RF24_CMD, RFn_CMD_TRXOFF);
		if(agc != AGC_OFF){
			XCVR_write_reg(RF24_AGCC, RFn_AGCC_EN); // TODO manage AGC enabled/disabled
			switch(agc){
			case AGC_neg21:
				XCVR_write_reg(RF24_AGCS, RFn_AGCS_TGT_n21);
				break;
			case AGC_neg24:
				XCVR_write_reg(RF24_AGCS, RFn_AGCS_TGT_n24);
				break;
			case AGC_neg30:
				XCVR_write_reg(RF24_AGCS, RFn_AGCS_TGT_n30);
				break;
			case AGC_neg33:
				XCVR_write_reg(RF24_AGCS, RFn_AGCS_TGT_n33);
				break;
			case AGC_neg36:
				XCVR_write_reg(RF24_AGCS, RFn_AGCS_TGT_n36);
				break;
			case AGC_neg39:
				XCVR_write_reg(RF24_AGCS, RFn_AGCS_TGT_n39);
				break;
			case AGC_neg42:
				XCVR_write_reg(RF24_AGCS, RFn_AGCS_TGT_n42);
				break;
			default:
				break;
			}
		}

		// Set frequency
		val = round((val-2366000000)/(406250.0/1024.0));
		XCVR_write_reg(RF24_CCF0L, (uint8_t)(val>>8));
		XCVR_write_reg(RF24_CCF0H, (uint8_t)(val>>16));
		XCVR_write_reg(RF24_CNL, (uint8_t)(val));
		XCVR_write_reg(RF24_CNM, RFn_CNM_CM_FINE3);

		XCVR_write_reg(RF24_RXBWC, RFn_RXBWC_BW160K);
		XCVR_write_reg(RF24_RXDFE, RFn_RXDFE_RCUT_1 | RFn_RXDFE_SR_400K);

		HAL_GPIO_WritePin(FPGA_RST_GPIO_Port, FPGA_RST_Pin, GPIO_PIN_RESET);
		osDelay(2);
		HAL_GPIO_WritePin(FPGA_RST_GPIO_Port, FPGA_RST_Pin, GPIO_PIN_SET);
		osDelay(2);

		//FPGA_write_reg(CR_2, CH_RX_12_5 | fm_mode | ctcss);
		FPGA_write_reg(TX_CTRL, TX_CTRL_MOD_FM | fm_mode);
		FPGA_write_reg(COM_CTRL, COM_CTRL_RX);

		XCVR_write_reg(RF24_CMD, RFn_CMD_RX);

	}else{
		LOG(CLI_LOG_RADIO, "Radio set in RX Sub-GHz.\r\n");

		// Sub GHz
		// Disable 2.4G transceiver
		XCVR_write_reg(RF24_CMD, RFn_CMD_TRXOFF);

		// Configure switch
		HAL_GPIO_WritePin(SW_CTL1_GPIO_Port, SW_CTL1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(SW_CTL2_GPIO_Port, SW_CTL2_Pin, GPIO_PIN_SET);

		// Configure Sub GHz transceiver
		XCVR_write_reg(RF09_CMD, RFn_CMD_TRXOFF);
		XCVR_write_reg(RF09_AGCC, RFn_AGCC_EN); // TODO manage AGC enabled/disabled
		XCVR_write_reg(RF09_AGCS, RFn_AGCS_TGT_n30);

		// Set frequency
		val = round((val-377000000)/(203125.0/2048.0));
		XCVR_write_reg(RF09_CCF0L, (uint8_t)(val>>8));
		XCVR_write_reg(RF09_CCF0H, (uint8_t)(val>>16));
		XCVR_write_reg(RF09_CNL, (uint8_t)(val));
		XCVR_write_reg(RF09_CNM, RFn_CNM_CM_FINE3);

		XCVR_write_reg(RF09_RXBWC, RFn_RXBWC_BW160K);
		XCVR_write_reg(RF09_RXDFE, RFn_RXDFE_RCUT_1 | RFn_RXDFE_SR_400K);

		HAL_GPIO_WritePin(FPGA_RST_GPIO_Port, FPGA_RST_Pin, GPIO_PIN_RESET);
		osDelay(2);
		HAL_GPIO_WritePin(FPGA_RST_GPIO_Port, FPGA_RST_Pin, GPIO_PIN_SET);
		osDelay(2);

		//FPGA_write_reg(CR_1, MOD_FM | PD_ON | DEM_FM | BAND_09);
		//FPGA_write_reg(CR_2, CH_RX_12_5 | fm_mode | ctcss);

		// TODO: need confirmation that this is correct
		FPGA_write_reg(COM_IO, COM_IO_IO3_PLL_LOCK);
		//FPGA_write_reg(COM_IO, COM_IO_IO3_TX_AE);
		FPGA_write_reg(TX_CTRL, TX_CTRL_MOD_FM | fm_mode);
		FPGA_write_reg(COM_CTRL, COM_CTRL_RX);

		XCVR_write_reg(RF09_CMD, RFn_CMD_RX);

	}
}

void radio_configure_tx(uint32_t freq, float ppm, openht_mode_t mode, fmInfo_t fm, xcvr_settings_t xcvr_settings){
	// Switch FPGA to TX
	XCVR_stop_operation();

	if(xcvr_settings.tx_pwr > RFn_PAC_TXPWR_MAX){
		xcvr_settings.tx_pwr = RFn_PAC_TXPWR_MAX;
	}

	// Set frequency
	uint32_t val = round(freq*(1.0f+ppm/1e6));

	openht_band_t band = Band_09;

	if(freq>1e9){
		LOG(CLI_LOG_RADIO, "Radio set in TX 2.4G.\r\n");

		// Configure switch
		radio_sw_24();

		// Configure 2.4G transceiver
		XCVR_write_reg(RF24_PAC, RFn_PAC_PACUR_MAX | xcvr_settings.tx_pwr);

		// Set frequency
		val = round((val-2366000000)/(406250.0/1024.0));
		XCVR_write_reg(RF24_CCF0L, (uint8_t)(val>>8));
		XCVR_write_reg(RF24_CCF0H, (uint8_t)(val>>16));
		XCVR_write_reg(RF24_CNL, (uint8_t)(val));
		XCVR_write_reg(RF24_CNM, RFn_CNM_CM_FINE3);

		/* */
		XCVR_write_reg(RF24_TXCUTC, RFn_TXCUTC_PARAMP4 | RFn_TXCUTC_FLC_80K);
		XCVR_write_reg(RF24_TXDFE, RFn_TXDFE_RCUT_0_25 | RFn_TXDFE_SR_400K);
		XCVR_write_reg(RF24_CMD, RFn_CMD_TXPREP);

		band = Band_24;
	}else{
		LOG(CLI_LOG_RADIO, "Radio set in TX Sub-GHz.\r\n");

		// Configure switch
		radio_sw_09();

		// Configure SubGHZ Transceiver
		XCVR_write_reg(RF09_PAC, RFn_PAC_PACUR_MAX | xcvr_settings.tx_pwr);

		// Set frequency
		val = round((val-377000000)/(203125.0/2048.0));
		XCVR_write_reg(RF09_CCF0L, (uint8_t)(val>>8));
		XCVR_write_reg(RF09_CCF0H, (uint8_t)(val>>16));
		XCVR_write_reg(RF09_CNL, (uint8_t)(val));
		XCVR_write_reg(RF09_CNM, RFn_CNM_CM_FINE1);

		/* */
		XCVR_write_reg(RF09_TXCUTC, RFn_TXCUTC_PARAMP4 | RFn_TXCUTC_FLC_80K);
		XCVR_write_reg(RF09_TXDFE, RFn_TXDFE_RCUT_0_25 | RFn_TXDFE_SR_400K);
		XCVR_write_reg(RF09_CMD, RFn_CMD_TXPREP);

		band = Band_09;
	}

	// reset FPGA...
	HAL_GPIO_WritePin(FPGA_RST_GPIO_Port, FPGA_RST_Pin, GPIO_PIN_RESET);
	osDelay(2);
	HAL_GPIO_WritePin(FPGA_RST_GPIO_Port, FPGA_RST_Pin, GPIO_PIN_SET);
	osDelay(2);

	bool supported_mode = false;
	LOG(CLI_LOG_RADIO, "Radio mode: %s\r\n", openht_get_mode_str(mode));
	switch (mode) {
		case OpMode_AM: {
			FPGA_write_reg(COM_IO, COM_IO_IO3_TX_AE);
			FPGA_write_reg(TX_CTRL, TX_CTRL_MOD_AM);
			FPGA_write_reg(COM_CTRL, COM_CTRL_TX);

			//FPGA_write_reg(CR_1, MOD_AM | PD_ON | band);

	        //FPGA_write_reg(CR_2, FIFO_TX);


			//I/Q branches gain+offset
			//FPGA_write_reg(I_GAIN, (int16_t)round(xcvr_settings.balance_i/1000.0*16384.0)); //setting of 1000 is 1.0, just as 0x4000
			//FPGA_write_reg(Q_GAIN, (int16_t)round(xcvr_settings.balance_q/1000.0*16384.0));
			//FPGA_write_reg(I_OFFS_NULL, xcvr_settings.offset_i);
			//FPGA_write_reg(Q_OFFS_NULL, xcvr_settings.offset_q);

			// set DPD_1, DPD_2, DPD_3
	        //FPGA_write_reg(DPD_1, (int16_t)round((float)xcvr_settings.dpd1*16.384f) & 0xFFFF);
	        //FPGA_write_reg(DPD_2, (int16_t)round((float)xcvr_settings.dpd2*16.384f) & 0xFFFF);
	        //FPGA_write_reg(DPD_3, (int16_t)round((float)xcvr_settings.dpd3*16.384f) & 0xFFFF);

			supported_mode = true;
		}
		break;

		case OpMode_NFM:
		case OpMode_WFM: {
			uint16_t fm_mode = (mode == OpMode_WFM) ? TX_CTRL_FMW : TX_CTRL_FMN;

			uint16_t ctcss = 0;
			if(fm.txToneEn){
				ctcss = fm.txTone << 2;
			}

			FPGA_write_reg(COM_IO, COM_IO_IO3_TX_AE);
			FPGA_write_reg(TX_CTRL, TX_CTRL_MOD_FM | fm_mode);
			//FPGA_write_reg(CR_2, FIFO_TX | CH_RX_12_5 | fm_mode | ctcss);
			FPGA_write_reg(COM_CTRL, COM_CTRL_TX);

			//I/Q branches gain+offset
			//FPGA_write_reg(I_GAIN, (int16_t)round(xcvr_settings.balance_i/1000.0*16384.0)); //setting of 1000 is 1.0, just as 0x4000
			//FPGA_write_reg(Q_GAIN, (int16_t)round(xcvr_settings.balance_q/1000.0*16384.0));
			//FPGA_write_reg(I_OFFS_NULL, xcvr_settings.offset_i);
			//FPGA_write_reg(Q_OFFS_NULL, xcvr_settings.offset_q);

			//for a constant envelope modulation, we don't need DPD anymore
			//FPGA_write_reg(DPD_1, (int16_t)0x4000);
			//FPGA_write_reg(DPD_2, (int16_t)0);
			//FPGA_write_reg(DPD_3, (int16_t)0);

			supported_mode = true;
		}
		break;

		case OpMode_LSB:
		case OpMode_USB: {
			uint16_t ssb_mode = (mode == OpMode_LSB) ? TX_CTRL_LSB : TX_CTRL_USB;

			FPGA_write_reg(COM_IO, COM_IO_IO3_TX_AE);
			FPGA_write_reg(TX_CTRL, ssb_mode);
	        //FPGA_write_reg(CR_1, ssb_mode | MOD_SSB | PD_ON | band);
	        //FPGA_write_reg(CR_2, FIFO_TX);
	        FPGA_write_reg(COM_CTRL, COM_CTRL_TX);

	        //I branch gain to +0.75
	        //FPGA_write_reg(I_GAIN, 0x3000); //needed to compensate for the Hilbert block's gain
	        //FPGA_write_reg(Q_GAIN, 0x4000);

			// set DPD_1, DPD_2, DPD_3
	        //FPGA_write_reg(DPD_1, (int16_t)round((float)xcvr_settings.dpd1*16.384f) & 0xFFFF);
	        //FPGA_write_reg(DPD_2, (int16_t)round((float)xcvr_settings.dpd2*16.384f) & 0xFFFF);
	        //FPGA_write_reg(DPD_3, (int16_t)round((float)xcvr_settings.dpd3*16.384f) & 0xFFFF);

			supported_mode = true;
		}
		break;

		case OpMode_M17: {
			LOG(CLI_LOG_RADIO, "TODO Supported Mode\r\n");
		}
		break;

		case OpMode_FreeDV: {
			LOG(CLI_LOG_RADIO, "TODO Supported Mode\r\n");
		}
		break;

		//TEST1 mode - FM carrier at +1kHz
		case OpMode_TEST1: {
			//control regs setting
			FPGA_write_reg(COM_IO, COM_IO_IO3_TX_AE);
			//FPGA_write_reg(CR_1, MOD_FM | PD_OFF | DEM_FM | band);
			//FPGA_write_reg(CR_2, CH_RX_12_5 | FM_TX_N | CTCSS_TX_NONE);
			FPGA_write_reg(COM_CTRL, COM_CTRL_TX);

			//modulation word to +1kHz: round(1000/400e3*2^21)
			//FPGA_write_reg(MOD_IN, 0x7B14); //0x147B, little endian

			//I/Q branches gain+offset
			//FPGA_write_reg(I_GAIN, (int16_t)round(xcvr_settings.balance_i/1000.0*16384.0)); //setting of 1000 is 1.0, just as 0x4000
			//FPGA_write_reg(Q_GAIN, (int16_t)round(xcvr_settings.balance_q/1000.0*16384.0));
			//FPGA_write_reg(I_OFFS_NULL, xcvr_settings.offset_i);
			//FPGA_write_reg(Q_OFFS_NULL, xcvr_settings.offset_q);
			LOG(CLI_LOG_FPGA, "Balance: I=0x%04X, Q=0x%04X\r\n",
						(int16_t)((float)xcvr_settings.balance_i/1000.0*16384.0),
						(int16_t)((float)xcvr_settings.balance_q/1000.0*16384.0) );
			LOG(CLI_LOG_FPGA, "Offsets: I=%d, Q=%d\r\n", xcvr_settings.offset_i, xcvr_settings.offset_q);

			//for a constant envelope modulation, we don't need DPD anymore
			//FPGA_write_reg(DPD_1, (int16_t)0x4000);
			//FPGA_write_reg(DPD_2, (int16_t)0);
			//FPGA_write_reg(DPD_3, (int16_t)0);

			supported_mode = true;
		}
		break;

		default: {
			LOG(CLI_LOG_RADIO, "Unsupported Mode\r\n");
			supported_mode = false;
		}
		break;
	}

	// if mode is supported, start tx'ing in the correct band
	if (supported_mode) {
		if (band == Band_09) {
			XCVR_write_reg(RF09_CMD, RFn_CMD_TX);
		} else if (band == Band_24) {
			XCVR_write_reg(RF24_CMD, RFn_CMD_TX);
		}
	}

}

void radio_sw_09(){
	HAL_GPIO_WritePin(SW_CTL1_GPIO_Port, SW_CTL1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(SW_CTL2_GPIO_Port, SW_CTL2_Pin, GPIO_PIN_SET);
}

void radio_sw_24(){
	HAL_GPIO_WritePin(SW_CTL1_GPIO_Port, SW_CTL1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(SW_CTL2_GPIO_Port, SW_CTL2_Pin, GPIO_PIN_RESET);
}

inline void FPGA_chip_select(bool selected){
	if(selected){
		HAL_GPIO_WritePin(FPGA_NSS_GPIO_Port, FPGA_NSS_Pin, GPIO_PIN_RESET);
		//FPGA_NSS_GPIO_Port->BSRR=(uint32_t)FPGA_NSS_Pin<<16;
	}else{
		HAL_GPIO_WritePin(FPGA_NSS_GPIO_Port, FPGA_NSS_Pin, GPIO_PIN_SET);
		//FPGA_NSS_GPIO_Port->BSRR=FPGA_NSS_Pin;
	}
}

inline void XCVR_chip_select(bool selected){
	if(selected){
		HAL_GPIO_WritePin(XCVR_NSS_GPIO_Port, XCVR_NSS_Pin, GPIO_PIN_RESET);
		//XCVR_NSS_GPIO_Port->BSRR=(uint32_t)XCVR_NSS_Pin<<16;
	}else{
		HAL_GPIO_WritePin(XCVR_NSS_GPIO_Port, XCVR_NSS_Pin, GPIO_PIN_SET);
		//XCVR_NSS_GPIO_Port->BSRR=XCVR_NSS_Pin;
	}
}

void FPGA_send_bitstream(uint32_t address, size_t length){
	uint8_t cmd_buffer[4] = {0};
	uint8_t *start_addr = (uint8_t *)(address);

	cmd_buffer[0] = 0x7A; // LSC_BITSTREAM_BURST
	FPGA_chip_select(true);
	HAL_SPI_Transmit_DMA(&hspi1, cmd_buffer, 4);
	wait_spi_xfer_done(WAIT_TIMEOUT);


	uint32_t sent = 0;
	while(sent < length){
		uint32_t bytes_left = length - sent;
		uint16_t to_send = (UINT16_MAX < bytes_left)?UINT16_MAX:bytes_left;
		HAL_SPI_Transmit_DMA(&hspi1, start_addr + sent, to_send);
		LOG(CLI_LOG_FPGA, "Sent %lu bitstream bytes.\r\n", sent);
		wait_spi_xfer_done(WAIT_TIMEOUT);
		sent += to_send;
	}
	LOG(CLI_LOG_FPGA, "Sent %lu bitstream bytes.\r\n", sent);
	FPGA_chip_select(false);
}

uint32_t FPGA_write_reg(uint16_t addr, uint16_t data){
	uint8_t buffer[4];

	FPGA_chip_select(true);
	*(uint16_t *)buffer = addr | REG_WR;
	buffer[2] = (data>>8) & 0xFF;
	buffer[3] = (uint8_t)data;
	HAL_SPI_Transmit_DMA(&hspi1, buffer, 4);
	wait_spi_xfer_done(WAIT_TIMEOUT);
	FPGA_chip_select(false);

	return EXIT_SUCCESS;
}

uint32_t FPGA_read_reg(uint16_t addr, uint16_t *data){
	uint8_t bufferTX[4];
	uint8_t bufferRX[4] = {0};

	FPGA_chip_select(true);
	*(uint16_t *)bufferTX = addr;
	*(uint16_t *)(bufferTX + 2) = 0;
	HAL_SPI_TransmitReceive_DMA(&hspi1, bufferTX, bufferRX, 4);
	wait_spi_xfer_done(WAIT_TIMEOUT);
	*data = ((uint16_t)bufferRX[2]<<8) + bufferRX[3];
	FPGA_chip_select(false);

	return EXIT_SUCCESS;
}

uint32_t XCVR_write_reg(uint16_t addr, uint8_t data){
	uint8_t buffer[3];

	XCVR_chip_select(true);
	*(uint16_t *)buffer = addr | (1<<7);
	buffer[2] = data;
	HAL_SPI_Transmit_DMA(&hspi1, buffer, 3);
	wait_spi_xfer_done(WAIT_TIMEOUT);
	XCVR_chip_select(false);

	return EXIT_SUCCESS;
}

uint32_t XCVR_read_reg(uint16_t addr, uint8_t *data){
	uint8_t bufferTX[3];
	uint8_t bufferRX[3] = {0};

	XCVR_chip_select(true);
	*(uint16_t *)bufferTX = addr;
	bufferTX[2] = 0;
	HAL_SPI_TransmitReceive_DMA(&hspi1, bufferTX, bufferRX, 3);
	wait_spi_xfer_done(WAIT_TIMEOUT);
	*data = bufferRX[2];
	XCVR_chip_select(false);

	return EXIT_SUCCESS;
}

uint32_t FPGA_config_classA(uint32_t length, uint8_t *tx, uint8_t *rx){
	uint32_t res = 0;
	FPGA_chip_select(true);
	res = HAL_SPI_TransmitReceive_DMA(&hspi1, tx, rx, length);
	if(res != HAL_OK){
		LOG(CLI_LOG_FPGA, "HAL_SPI_TransmitReceive_DMA returned %lu.\r\n", res);
	}
	wait_spi_xfer_done(WAIT_TIMEOUT);
	FPGA_chip_select(false);
	return EXIT_SUCCESS;
}

uint32_t FPGA_config_classB(uint32_t length, uint8_t *tx){
	uint32_t res = 0;
	FPGA_chip_select(true);
	res = HAL_SPI_Transmit_DMA(&hspi1, tx, length);
	if(res != HAL_OK){
		LOG(CLI_LOG_FPGA, "HAL_SPI_TransmitReceive_DMA returned %lu.\r\n", res);
	}
	wait_spi_xfer_done(WAIT_TIMEOUT);
	FPGA_chip_select(false);
	return EXIT_SUCCESS;
}

uint32_t FPGA_config_classC(uint8_t cmd){
	uint32_t res = 0;
	uint8_t tx[4] = {cmd, 0, 0, 0};
	FPGA_chip_select(true);
	res = HAL_SPI_Transmit_DMA(&hspi1, tx, 4);
	if(res != HAL_OK){
		LOG(CLI_LOG_FPGA, "HAL_SPI_TransmitReceive_DMA returned %lu.\r\n", res);
	}
	wait_spi_xfer_done(WAIT_TIMEOUT);
	FPGA_chip_select(false);
	return EXIT_SUCCESS;
}

uint32_t FPGA_check_status_register(uint8_t *reg, bool print){
	uint8_t rx[12] = {0};
	uint8_t tx[12] = {0x3C, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	FPGA_config_classA(12, tx, rx);

	if(print){
		LOG(CLI_LOG_FPGA, "Status register is 0x%02x%02x%02x%02x%02x%02x%02x%02x.\r\n",
					rx[4], rx[5], rx[6], rx[7], rx[8], rx[9], rx[10], rx[11]);
	}

	if(reg != NULL){
		memcpy(reg, rx+4, 8);
	}

	return EXIT_SUCCESS;
}

void FPGA_wait_busy(){
	// LSC-CHECK-BUSY (5 bits)
	uint8_t tx[5] = {0xF0, 0, 0, 0, 0};
	uint8_t rx[5] = {0};

	while(true){ // Wait for device to not be busy anymore
		FPGA_config_classA(5, tx, rx);
		if(rx[4] == 0){
			LOG(CLI_LOG_FPGA, "FPGA not busy anymore.\r\n");
			break;
		}else{
			osDelay(2);
		}
	}
}

void FPGA_config_enable_sspi(){
	uint8_t cmd_buffer[5];
	// First set PROGRAMN low
	HAL_GPIO_WritePin(FPGA_PROGRAMN_GPIO_Port, FPGA_PROGRAMN_Pin, GPIO_PIN_SET);
	osDelay(2);
	HAL_GPIO_WritePin(FPGA_PROGRAMN_GPIO_Port, FPGA_PROGRAMN_Pin, GPIO_PIN_RESET);
	osDelay(2); // Must wait for at least 50ns

	// Drive SCSN low then clock in the key
	FPGA_chip_select(true);
	*(uint32_t *)(cmd_buffer+1) = SPI_PORT_ACTIVATION_KEY;
	FPGA_config_classB(5, cmd_buffer);
	osDelay(50);
}

void XCVR_stop_operation(){
	XCVR_write_reg(RF09_CMD, RFn_CMD_TRXOFF);
	XCVR_write_reg(RF24_CMD, RFn_CMD_TRXOFF);

}
