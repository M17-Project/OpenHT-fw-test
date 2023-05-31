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

// User settings (sectors 0-3)
#define USER_SETTINGS_NB_SECTORS	4
#define USER_SETTINGS_START_SECTOR 	0
#define USER_SETTINGS_START_ADDRESS	(USER_SETTINGS_START_SECTOR*SUBSECTOR_SIZE)

// 4 reserved sectors (sectors 4-7)

//FPGA binaries table (sectors 8-12)
#define FPGA_BIN_TABLE_NB_SECTORS	4
#define FPGA_BIN_TABLE_START_SECTOR 8
#define FPGA_BIN_TABLE_START_ADDR 	((SUBSECTOR_SIZE)*FPGA_BIN_TABLE_START_SECTOR)

// FPGA binaries storage (sectors 13-2061)
#define FPGA_BIN_STORAGE_START_SECTOR 	13
#define FPGA_BIN_STORAGE_END_SECTOR 	FPGA_BIN_STORAGE_START_SECTOR+2048
#define FPGA_BIN_STORAGE_START_ADDR 	((SUBSECTOR_SIZE)*FPGA_BIN_STORAGE_START_SECTOR)
#define FPGA_BIN_MAX_SIZE				0x100000 // 1024kB

#endif /* INC_NOR_MAP_H_ */
