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

#include "disk_mgr.h"

#include "openht_types.h"

#include "stm32469i_discovery.h"
#include "fatfs.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

static uint16_t screen_capture_num = 0;

static openht_res_t _fat_error_handler(void);
static FRESULT _scan_files(char *path);
static void _capture_name(const char *fname);
static uint32_t _num_from_str(const char *str, uint32_t start, uint32_t end);

int32_t save_image(const char * in_filename, uint8_t *bmp_header, size_t bmp_headersize,
		uint8_t *img_buffer, size_t img_buffersize)
{
	int32_t ret_val = 0;
	FRESULT res;
	uint32_t byteswritten;
	FIL the_file;
	char filename[16];

	// if the input filename is empty string, grab the next "SCREEN" filename sequence
	// else use the input name as the filename
	if (strcmp(in_filename, "") == 0) {
		// get next filename
		char buff[256];
		strcpy(buff, "/");
		res = _scan_files(buff);
		sprintf(filename, "SCREEN%02d.BMP", screen_capture_num + 1);
		ret_val = screen_capture_num + 1;
	} else {
		sprintf(filename, "%s.BMP", in_filename);
		ret_val = 0;
	}

	if (f_open(&the_file, filename, FA_CREATE_ALWAYS | FA_WRITE)
			!= FR_OK) {
		res = _fat_error_handler();
	} else {
		bool write_ok = true;
		res = f_write(&the_file, bmp_header, bmp_headersize,
				(void*) &byteswritten);

		if ((byteswritten == 0) || (res != FR_OK)) {
			res = _fat_error_handler();
			write_ok = false;
		} else {
			res = f_write(&the_file, img_buffer, img_buffersize,
					(void*) &byteswritten);
			if ((byteswritten == 0) || (res != FR_OK)) {
				res = _fat_error_handler();
				write_ok = false;
			}
		}

		f_close(&the_file);

		if (!write_ok) {
			// TODO: DISK ERROR occasionally so need to remove file
			//res = f_unlink(filename);
			BSP_LED_On(LED_RED);
		}
	}

	if (res != FR_OK) {
		ret_val = -1;
	}

	return ret_val;
}

static int max(int a, int b)
{
	return a > b ? a : b;
}

static void _capture_name(const char *fname)
{
	// if the filename has a BMP extension
	if (strstr(fname, ".BMP") != NULL) {
		if (strncmp(fname, "SCREEN", 6) == 0) {
			screen_capture_num = max(screen_capture_num,
					_num_from_str(fname, 6, 7));
		}
	}
}

static uint32_t _num_from_str(const char *str, uint32_t start, uint32_t end)
{
	uint32_t num = 0;

	// converting string to number
	for (int i = start; i <= end; i++) {
		int test = str[i] - 48;

		// if it is a digit 0-9
		if (test >= 0 && test <= 9) {
			num = num * 10 + test;
		}
	}

	return num;
}

static FRESULT _scan_files(char *path) /* Start node to be scanned (***also used as work area***) */
{
	FRESULT res;
	DIR dir;
	UINT i;
	static FILINFO fno;

	res = f_opendir(&dir, path); /* Open the directory */
	if (res == FR_OK) {
		for (;;) {
			res = f_readdir(&dir, &fno); /* Read a directory item */
			if (res != FR_OK || fno.fname[0] == 0)
				break; /* Break on error or end of dir */
			if (fno.fattrib & AM_DIR) { /* It is a directory */
				i = strlen(path);
				sprintf(&path[i], "/%s", fno.fname);
				res = _scan_files(path); /* Enter the directory */
				if (res != FR_OK)
					break;
				path[i] = 0;
			} else { /* It is a file. */
				printf("%s/%s\n", path, fno.fname);
				_capture_name(fno.fname);
			}
		}
		f_closedir(&dir);
	}

	return res;
}

static openht_res_t _fat_error_handler(void)
{
	//BSP_LED_On(LED_ORANGE);
	return OPENHT_ERR;
}
