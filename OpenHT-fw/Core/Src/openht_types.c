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

#include "openht_types.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

struct mode_datum openht_mode_data[] =
{
    {OpMode_M17,    "M17"},   // digital
    {OpMode_FreeDV, "FreeDV"},// digital
    {OpMode_NFM,    "NFM"},   // 12.5k
    {OpMode_WFM,    "WFM"},   // 25k
    {OpMode_AM,     "AM"},
    {OpMode_LSB,    "LSB"},   // Lower
    {OpMode_USB,    "USB"},   // Upper
    {OpMode_TEST1,  "TEST1"}, // Debug
    {OpMode_TEST2,  "TEST2"}, // Debug
    {OpMode_TEST3,  "TEST3"}, // Debug
};

uint32_t openht_mode_count = sizeof(openht_mode_data)/sizeof(openht_mode_data[0]);

char * openht_get_mode_str(openht_mode_t mode)
{
	for (int i = 0; i < openht_mode_count; i++) {
		if (openht_mode_data[i].mode == mode) {
			return openht_mode_data[i].mode_name;
		}
	}

	return "\0";
}

struct fpga_status_datum openht_fpga_status_data[] =
{
    {FPGA_Offline, "Offline"},
	{FPGA_Online, "Online"},
    {FPGA_Loading,  "Loading"},
    {FPGA_Running,  "Running"},
	{FPGA_Error, "Error"},
};

uint32_t openht_fpga_status_count = sizeof(openht_fpga_status_data)/sizeof(openht_fpga_status_data[0]);

char * openht_get_fpga_status_str(openht_fpga_status_t fpga_status)
{
	for (int i = 0; i < openht_fpga_status_count; i++) {
		if (openht_fpga_status_data[i].status == fpga_status) {
			return openht_fpga_status_data[i].status_name;
		}
	}

	return "\0";
}

