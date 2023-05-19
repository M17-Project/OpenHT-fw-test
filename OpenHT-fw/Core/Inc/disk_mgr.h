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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DISK_MGR_H
#define __DISK_MGR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "openht_types.h"
#include "stm32469i_discovery_sd.h"
#include "fatfs.h"

void test_fat(void);
FRESULT save_image(uint8_t *bmp_header, size_t bmp_headersize,
		uint8_t *img_buffer, size_t img_buffersize);

#ifdef __cplusplus
}
#endif

#endif /* __DISK_MGR_H */
