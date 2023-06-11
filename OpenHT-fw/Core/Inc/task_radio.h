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

#ifndef INC_TASK_RADIO_H_
#define INC_TASK_RADIO_H_

#include "main.h"

#include <stdlib.h>

void StartTaskRadio(void *argument);

/**
 * Loads a binary image from the SD card to the NOR flash.
 *
 * @return bool		EXIT_SUCCESS in case of success, EXIT_FAILURE in case of failure
 */
bool download_fpga_binary_file();

/**
 * Erase the FPGA binary storage
 *
 * @return bool		EXIT_SUCCESS in case of success, EXIT_FAILURE in case of failure
 */
bool erase_fpga_storage();

/**
 * Soft-reset the FPGA (i.e. return the internal state of the FPGA to default)
 *
 * @note 	All the internal registers of the FPGA are reset to their default value meaning that
 * 			the configurations must be sent again
 *
 * @return bool		EXIT_SUCCESS in case of success, EXIT_FAILURE in case of failure
 */
bool fpga_soft_reset();

/**
 *
 */
bool upload_fpga_binary();


#endif /* INC_TASK_RADIO_H_ */
