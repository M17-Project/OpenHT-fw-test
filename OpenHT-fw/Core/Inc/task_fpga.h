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

#ifndef INC_TASK_FPGA_H_
#define INC_TASK_FPGA_H_

#include "main.h"
#include <stdlib.h>

void StartTaskFPGA(void *argument);

/**
 * Loads a binary image from the SD card to the NOR flash. Image will be stored as index index.
 *
 * @param filename 	Null-terminated string containing the filename to load as an FPGA binary image
 * @param index		The index used to store the file.
 *
 * @return bool		EXIT_SUCCESS in case of success, EXIT_FAILURE in case of failure
 */
bool download_fpga_binary_file();

#endif /* INC_TASK_FPGA_H_ */
