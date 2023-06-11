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

#ifndef INC_UTILS_STR_FORMATTING_H_
#define INC_UTILS_STR_FORMATTING_H_

#include "openht_types.h"

#define EMPTY_FREQ "_.___.___.___"

void get_display_str_from_freq(freq_t i, char buffer[]);
freq_t get_freq_from_str(const char *str);
void get_str_from_freq(uint32_t i, char b[], int prepend_blank);



#endif /* INC_UTILS_STR_FORMATTING_H_ */
