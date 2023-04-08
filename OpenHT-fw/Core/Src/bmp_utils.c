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

#include "bmp_utils.h"
#include "openht_ui.h"
#include <stdlib.h>
#include <stdio.h>

void get_bitmap_header(uint32_t width, uint32_t height, uint32_t color_depth, uint8_t* buffer, size_t bufsize)
{
    const char* bm = "BM";
    memcpy(buffer, bm, strlen(bm));
    buffer += strlen(bm);

    // Bitmap file header
    struct bmp_header_t* bmp = (struct bmp_header_t*)buffer;
    bmp->bfSize       = (uint32_t)(width * height * color_depth / 8);
    bmp->bfReserved   = 0;
    bmp->bfOffBits    = bufsize;

    // Bitmap information header
    bmp->biSize          = 40;
    bmp->biWidth         = width;
    bmp->biHeight        = -height;
    bmp->biPlanes        = 1;
    bmp->biBitCount      = color_depth;
    bmp->biCompression   = 3; // BI_BITFIELDS
    bmp->biSizeImage     = bmp->bfSize;
    bmp->biXPelsPerMeter = 2836;
    bmp->biYPelsPerMeter = 2836;
    bmp->biClrUsed       = 0; // zero defaults to 2^n
    bmp->biClrImportant  = 0;

    // BI_BITFIELDS
    bmp->bdMask[0] = 0xF800; // Red bitmask  : 1111 1000 | 0000 0000
    bmp->bdMask[1] = 0x07E0; // Green bitmask: 0000 0111 | 1110 0000
    bmp->bdMask[2] = 0x001F; // Blue bitmask : 0000 0000 | 0001 1111
}
