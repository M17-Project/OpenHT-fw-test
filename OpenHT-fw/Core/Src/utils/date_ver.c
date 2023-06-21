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

// Note: Taken from https://stackoverflow.com/questions/17739390/different-format-of-date-macro

const char BUILD_DATE_VERSION[6+1] =
{
   // YY year
   __DATE__[9], __DATE__[10],

   // First month letter, Oct Nov Dec = '1' otherwise '0'
   (__DATE__[0] == 'O' || __DATE__[0] == 'N' || __DATE__[0] == 'D') ? '1' : '0',

   // Second month letter
   (__DATE__[0] == 'J') ? ( (__DATE__[1] == 'a') ? '1' :       // Jan, Jun or Jul
                            ((__DATE__[2] == 'n') ? '6' : '7') ) :
   (__DATE__[0] == 'F') ? '2' :                                // Feb
   (__DATE__[0] == 'M') ? (__DATE__[2] == 'r') ? '3' : '5' :   // Mar or May
   (__DATE__[0] == 'A') ? (__DATE__[1] == 'p') ? '4' : '8' :   // Apr or Aug
   (__DATE__[0] == 'S') ? '9' :                                // Sep
   (__DATE__[0] == 'O') ? '0' :                                // Oct
   (__DATE__[0] == 'N') ? '1' :                                // Nov
   (__DATE__[0] == 'D') ? '2' :                                // Dec
   0,

   // First day letter, replace space with digit
   __DATE__[4]==' ' ? '0' : __DATE__[4],

   // Second day letter
   __DATE__[5],

  '\0'
};
