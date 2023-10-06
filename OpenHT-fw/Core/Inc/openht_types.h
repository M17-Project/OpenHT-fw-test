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

#ifndef INC_OPENHT_TYPES_H_
#define INC_OPENHT_TYPES_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "datatypes.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

// Version defines for the FW version as displayed in UI About
static const uint8_t OPENHT_MAJOR_VER = 0;
static const uint8_t OPENHT_MINOR_VER = 2;

typedef enum {
	AGC_OFF,
	AGC_neg21,
	AGC_neg24,
	AGC_neg27,
	AGC_neg30,
	AGC_neg33,
	AGC_neg36,
	AGC_neg39,
	AGC_neg42,
} openht_radio_agc;

typedef enum
{
	OPENHT_OK = 0, /* (0) Succeeded */
	OPENHT_ERR, /* (1) A error occurred */
} openht_res_t;

typedef enum
{
    OpMode_M17,
	OpMode_FreeDV,
	OpMode_NFM,
	OpMode_WFM,
	OpMode_AM,
	OpMode_LSB,
	OpMode_USB,
	OpMode_TEST1,
	OpMode_TEST2,
	OpMode_TEST3,
} openht_mode_t;

struct mode_datum
{
	openht_mode_t mode;
    char          mode_name[10];
};
extern struct mode_datum openht_mode_data[];
extern uint32_t openht_mode_count;
char * openht_get_mode_str(openht_mode_t mode);

//---

typedef enum
{
    FPGA_Offline,
	FPGA_Online,
    FPGA_Loading,
	FPGA_Running,
    FPGA_Error,
} openht_fpga_status_t;

struct fpga_status_datum
{
	openht_fpga_status_t status;
    char        		 status_name[10];
};
extern struct fpga_status_datum openht_fpga_status_data[];
extern uint32_t openht_fpga_status_count;
char * openht_get_fpga_status_str(openht_fpga_status_t fpga_status);

typedef enum
{
    Band_09,
	Band_24,
} openht_band_t;


#ifdef __cplusplus
}
#endif

#endif /* INC_OPENHT_TYPES_H_ */
