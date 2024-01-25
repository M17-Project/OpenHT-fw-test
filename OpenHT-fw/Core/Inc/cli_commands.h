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

#ifndef INC_CLI_COMMANDS_H_
#define INC_CLI_COMMANDS_H_

#include "main.h"

#include <stdlib.h>

extern char nor_help[];
uint8_t cli_nor_cmd(int argc, char *argv[]);

extern char radio_help[];
uint8_t cli_radio_cmd(int argc, char *argv[]);

extern char audio_help[];
uint8_t cli_audio_cmd(int argc, char *argv[]);

#endif /* INC_CLI_COMMANDS_H_ */
