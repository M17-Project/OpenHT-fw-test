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

#include "utils/str_formatting.h"

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>

static int _num_places (freq_t n)
{
    if (n < 10) return 1;
    return 1 + _num_places (n / 10);
}

void get_display_str_from_freq(freq_t i, char buffer[])
{
	// char buffer should be at least 9 chars long.
	// Output will be in MHz using 8 significant digits so..
	// 439.12345 or 2432.1234
	uint32_t left_side = 0;
	uint32_t right_side = 0;
	int num_of_decimals = 0;

	// GHz
	if (i >= 1000000000) {
		left_side = i / 1000000;
		right_side = i - (left_side * 1000000);
		right_side = right_side / 100;
		num_of_decimals = 4;
	} else if (i >= 100000000) { // MHz
		left_side = i / 1000000;
		right_side = i - (left_side * 1000000);
		right_side = right_side / 10;
		num_of_decimals = 5;
	}

	// don't expect anything less than 100 MHz here... (for now!)

	sprintf(buffer, "%lu.%0*lu", left_side, num_of_decimals, right_side);
}

freq_t get_freq_from_str(const char *str)
{
	freq_t num = 0;  // uint32 max: 4294967295

	// converting string to number
	for (int i = 0; str[i] != '\0'; i++) {
		freq_t digit = str[i] - 48;

		// if it is a "_" treat as a 0
		if (digit == 47) {
			digit = 0;
		}

		// if it is a digit 0-9
		if (digit >= 0 && digit <= 9) {

			// check for overflow and handle it
		    if (num > UINT_MAX / 10) {
		    	return UINT_MAX;
		    }
		    num *= 10;

		    // check for overflow and handle it
		    if (num > UINT_MAX - digit) {
		    	return UINT_MAX;
		    }
			num += digit;
		}
	}

	return num;
}

// formats a str from a numeric freq
// if prepend_blank is 0 - output includes leading underscores and thous separators
// if prepend_blank is 1 - output includes spaces instead of underscores and thous separators
// if prepend_blank is -1 - output does not include any padding to the left most number
void get_str_from_freq(freq_t i, char b[], int prepend_blank)
{
	char const digit[] = "0123456789";
	char *p = b;

	if (i == 0) {
		strcpy(p, EMPTY_FREQ);
		return;
	}

	//Move to end of str
	int str_i;
	for (str_i = 0; b[str_i] != '\0'; ++str_i)
		;
	p = p + str_i;

	if (prepend_blank == -1) {
		int nums = _num_places(i);
		int mod = nums % 3;
		int thous = (nums / 3);
		if (mod == 0) thous--;

		int discard_len = str_i - (nums + thous);

		for (int idx = 0; idx < discard_len; idx++)
		{
			*--p = '\0';
		}
	}

	int thousands = 0;
	do { // add digits backwards, inserting thousands separator
		if (thousands == 3) {
			if (i == 0 && prepend_blank == 1) {
				*--p = ' ';
			} else {
				*--p = '.';
			}
			thousands = 0;
		} else {
			if (i) {
				*--p = digit[i % 10];
				i = i / 10;
			} else if (prepend_blank == 1){
				*--p = ' ';
			} else { // prepend underscores
				*--p = '_';
			}
			thousands++;
		}
	} while (p > b);
}

