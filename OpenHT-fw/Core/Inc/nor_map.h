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

#ifndef INC_NOR_MAP_H_
#define INC_NOR_MAP_H_

// memory mapped
#define START_MMP_NOR_ADDR 	0x90000000U
#define END_MMP_NOR_ADDR 	0x91000000U
#define SUBSECTOR_SIZE 		4096U 			// bytes per subsector

#define USER_SETTINGS_START_SECTOR 0
#define USER_SETTINGS_END_SECTOR 3

// 4 reserved sectors
#define FPGA_BIN_TABLE_START_SECTOR 8
#define FPGA_BIN_TABLE_END_SECTOR 11

#define FPGA_BIN_TABLE_START_ADDR 	((SUBSECTOR_SIZE)*FPGA_BIN_TABLE_START_SECTOR)
#define FPGA_BIN_TABLE_END_ADDR 	((SUBSECTOR_SIZE)*(1+FPGA_BIN_TABLE_END_SECTOR) -1)

#define FPGA_BIN_STORAGE_START_SECTOR 20
#define FPGA_BIN_STORAGE_END_SECTOR FPGA_BIN_STORAGE_START_SECTOR+2048

#endif /* INC_NOR_MAP_H_ */
