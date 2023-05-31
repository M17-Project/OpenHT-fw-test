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

#ifndef INC_EEEPROM_H_
#define INC_EEEPROM_H_

#include <stdlib.h>

typedef struct s_EEPROMInternalData EEPROMInternalData_t;

typedef enum {
	EEEPROM_ADDRESS_1BYTE = 1,
	EEEPROM_ADDRESS_2BYTES = 2,
	EEEPROM_ADDRESS_4BYTES = 4
} EEEPROMAddrSize;

typedef struct {
	uint32_t 	start_address;	/*< Address at which the EEEPROM starts */

	/**
	 *  Pointer to the function that will be called when a sector needs to be erased
	 *
	 *  @param[in]	sector	The sector to erase
	 */
	void		(*erase_page)(uint32_t sector);

	/**
	 *  Pointer to the function that will be called to read data from the underlying memory.
	 *  If this pointer is set to null then the memory will be accessed in a memory-mapped mode
	 *  (i.e. no functions calls, just use the memory like traditional system memory)
	 *
	 *  @param[out]	data	The location where data read will be saved
	 *  @param[in]	addr	Address to read to
	 *  @param[in	length	Number of bytes to read
	 */
	void 		(*read)(uint8_t *data, uint32_t addr, uint32_t length);

	/**
	 *  Pointer to the function that will be called to read data from the underlying memory.
	 *  If this pointer is set to null then the memory will be accessed in a memory-mapped mode
	 *  (i.e. no functions calls, just use the memory like tradicional system memory)
	 *
	 *  @param[in]	data	Pointer to the data to write
	 *  @param[in]	addr	Address to write to
	 *  @param[in	length	Number of bytes to write
	 */
	void		(*write)(uint8_t *data, uint32_t addr, uint32_t length);

	EEPROMInternalData_t 	*priv;
	uint32_t 				page_offset;	/*< Sector offset of the first sector used (will be used when calling the erase function)*/
	uint16_t 				page_size;		/*< Number of bytes per page */
	uint8_t 				number_pages;	/*< Number of pages used for the implementation */
	uint8_t 				data_size;		/*< Size of the data to store in the EEPROM (in bytes) */
	EEEPROMAddrSize 		address_size;	/*< Address size for each entry (in bytes). Must be between 1 and 4 */
	uint8_t 				alignment;		/*< Data alignment in bytes */
} EEEPROMHandle_t;


/**
 * Initializes the internal state of the EEEPROM handle given its parameters
 *
 * @param *handle pointer to an EEEPROMHandle_t structure describing the EEEPROM to initialize
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
bool EEEPROM_init(EEEPROMHandle_t *handle);

/**
 * Writes some data to an EEEPROM specified by the handle
 *
 * @param *handle	pointer to an EEEPROMHandle_t structure describing the EEEPROM to write to
 * @param *address	EEEPROM address to write to
 * @param *data		data to write to the EEEPROM
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
bool EEEPROM_write_data(EEEPROMHandle_t *handle, uint32_t address, const void *data);

/**
 * Writes some data to an EEEPROM specified by the handle
 *
 * @param *handle	pointer to an EEEPROMHandle_t structure describing the EEEPROM used
 * @param *address	EEEPROM address to read from
 * @param *data		pointer where the data read will be stored
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
bool EEEPROM_read_data(EEEPROMHandle_t *handle, uint32_t address, void *data);

/**
 * Completely erases the EEEPROM specified by the handle
 *
 * @param *handle	pointer to an EEEPROMHandle_t structure describing the EEEPROM to erase
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
bool EEEPROM_erase(EEEPROMHandle_t *handle);

/**
 * Deletes the content of a handle and frees the memory used.
 *
 * @param *handle	pointer to an EEEPROMHandle_t structure describing the EEEPROM to erase
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
bool EEEPROM_delete(EEEPROMHandle_t *handle);

#endif /* INC_EEEPROM_H_ */
