/*
 * eeeprom_hal.h
 *
 *  Created on: May 31, 2023
 *      Author: morga
 */

#ifndef INC_EEEPROM_HAL_H_
#define INC_EEEPROM_HAL_H_

void EEEPROM_HAL_erase_subsector(uint32_t subsector);

void EEEPROM_HAL_qspi_read(uint8_t *data, uint32_t addr, uint32_t len);

void EEEPROM_HAL_qspi_write(uint8_t *data, uint32_t addr, uint32_t len);

#endif /* INC_EEEPROM_HAL_H_ */
