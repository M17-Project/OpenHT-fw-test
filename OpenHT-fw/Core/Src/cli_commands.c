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

#include "cli_commands.h"

#include "main.h"
#include "task_radio.h"
#include "user_settings.h"
#include "radio_settings.h"
#include "openht_types.h"
#include "datatypes.h"
#include "task_audio_out.h"

#include "stm32469i_discovery_qspi.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char nor_help[] = "Perform actions on the NOR flash\r\n"
				  "Usage: nor\r\n"
				  "\tfull_erase\tPerform a full erase of the NOR flash\r\n"
				  "\terase_bit\tPerform a full erase of the bitstream storage area\r\n"
				  "\tload_bit\tLoads the bitstream file from the SD card\r\n"
			 	  "\terase_user\tErase the user configuration\r\n"
				  "\terase_radio\tErase the radio configuration.\r\n";

char radio_help[] = "Perform actions on the radio subsystem\r\n"
				    "Usage: radio\r\n"
				    "\ttx\tSwitch the radio to TX mode (simulates a PTT press)\r\n"
				    "\trx\tSwitch the radio to RX mode (simulates a PTT release)\r\n"
				    "\tfreq x\tSets the frequency of the radio to x Hz.\r\n";

char audio_help[] = "Perform actions on the audio output subsystem\r\n"
					"Usage: audio\r\n"
					"\tspk\tSwitch the audio output to speaker\r\n"
					"\thp\tSwitch the audio output to headphones\r\n"
					"\tvol [0-100]\tSets the audio output volume to x.\r\n";

uint8_t cli_nor_cmd(int argc, char *argv[]){
	if(argc != 2){
		printf("Command %s takes one argument. For usage type \"help %s\"\r\n", argv[0], argv[0]);
		return EXIT_FAILURE;
	}

	if(!strcmp(argv[1], "full_erase")){
		printf("Starting bulk erasing of NOR flash chip...\r\n");
		BSP_QSPI_Erase_Chip();
		printf("Done!\r\n");
		return EXIT_SUCCESS;
	}else if(!strcmp(argv[1], "erase_bit")){
		printf("Starting erasing of bitstream storage...\r\n");
		task_radio_event(ERASE_BITSTREAM_STORAGE);
		return EXIT_SUCCESS;
	}else if(!strcmp(argv[1], "load_bit")){
		printf("Starting bitstream download from uSD card...\r\n");
		task_radio_event(DOWNLOAD_BITSTREAM);
		return EXIT_SUCCESS;
	}else if(!strcmp(argv[1], "erase_user")){
		printf("Starting erasing of user settings...\r\n");
		user_settings_reset();
		printf("Done!\r\n");
		return EXIT_SUCCESS;
	}else if(!strcmp(argv[1], "erase_radio")){
		printf("Starting erasing of radio settings...\r\n");
		radio_settings_reset();
		printf("Done!\r\n");
		return EXIT_SUCCESS;
	}else{
		printf("Unknown command \"%s\"", argv[1]);
		return EXIT_FAILURE;
	}
}

uint8_t cli_radio_cmd(int argc, char *argv[]){
	if(argc < 2){
		printf("Incorrect usage for command %s. For usage type \"help %s\"\r\n", argv[0], argv[0]);
		return EXIT_FAILURE;
	}

	if(!strcmp(argv[1], "tx")){
		printf("Switching radio to TX.\r\n");
		task_radio_event(START_TX);
		return EXIT_SUCCESS;
	}else if(!strcmp(argv[1], "rx")){
		printf("Switching radio to RX.\r\n");
		task_radio_event(START_RX);
		return EXIT_SUCCESS;
	}

	if(argc < 3){
		printf("Incorrect usage for command %s. For usage type \"help %s\"\r\n", argv[0], argv[0]);
		return EXIT_FAILURE;
	}

	if(!strcmp(argv[1], "freq")){
		freq_t f = strtol(argv[2], NULL, 10);
		uint32_t megs = f/1000000;
		uint32_t kilos = (f%1000000)/1000;
		uint32_t hertz = f%1000;

		printf("Setting radio RX and TX frequency to %lu.%03lu.%03lu Hz.\r\n", megs, kilos, hertz);
		radio_settings_set_tx_freq(f);
		radio_settings_set_rx_freq(f);

		return EXIT_SUCCESS;
	}

	printf("Incorrect usage for command %s. For usage type \"help %s\"\r\n", argv[0], argv[0]);
	return EXIT_FAILURE;
}

uint8_t cli_audio_cmd(int argc, char *argv[]){
	if(argc == 2){
		if(!strcmp(argv[1], "spk")){
			printf("Switching audio output to speaker.\r\n");
			audio_output_speaker(true);
			return EXIT_SUCCESS;
		}else if(!strcmp(argv[1], "hp")){
			printf("Switching audio output to headphones.\r\n");
			audio_output_speaker(false);
			return EXIT_SUCCESS;
		}
	}else if(argc == 3){
		if(!strcmp(argv[1], "vol")){
			unsigned long int volume = strtoul(argv[2], NULL, 10);
			if(volume > 100){
				volume = 100;
			}
			audio_output_volume(volume);

			printf("Set audio output volume to %lu.\r\n", volume);
			return EXIT_SUCCESS;
		}
	}

	printf("Incorrect usage for command %s. For usage type \"help %s\"\r\n", argv[0], argv[0]);
	return EXIT_FAILURE;
}
