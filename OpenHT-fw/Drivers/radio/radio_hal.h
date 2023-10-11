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

#ifndef RADIO_RADIO_HAL_H_
#define RADIO_RADIO_HAL_H_

#include "openht_types.h"
#include "radio_settings.h"
#include "cps.h"

#include <stdbool.h>

#ifdef DEBUG
#define WAIT_TIMEOUT osWaitForever
#else
#define WAIT_TIMEOUT 1500
#endif

#define FPGA_MAJOR (0)
#define FPGA_MINOR (4)

void radio_on();

void radio_off();

// AGC target. negative to disable
void radio_configure_rx(uint32_t freq, float ppm, openht_mode_t mode, fmInfo_t fm, openht_radio_agc agc);

void radio_configure_tx(uint32_t freq, float ppm, openht_mode_t mode, fmInfo_t fm, xcvr_settings_t xcvr_settings);

void radio_sw_09();

void radio_sw_24();

void FPGA_chip_select(bool selected);

void XCVR_chip_select(bool selected);

void FPGA_send_bitstream(uint32_t address, size_t length);

uint32_t FPGA_write_reg(uint16_t addr, uint16_t data);

uint32_t FPGA_read_reg(uint16_t addr, uint16_t *data);

uint32_t XCVR_write_reg(uint16_t addr, uint8_t data);

uint32_t XCVR_read_reg(uint16_t addr, uint8_t *data);

uint32_t FPGA_config_classA(uint32_t length, uint8_t *tx, uint8_t *rx);

uint32_t FPGA_config_classB(uint32_t length, uint8_t *tx);

uint32_t FPGA_config_classC(uint8_t cmd);

uint32_t FPGA_check_status_register(uint8_t *reg, bool print);

void FPGA_wait_busy();

void FPGA_config_enable_sspi();

void XCVR_stop_operation();



#endif /* RADIO_RADIO_HAL_H_ */
