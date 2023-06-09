/*
 * eeeprom_hal.c
 *
 *  Created on: May 31, 2023
 *      Author: morga
 */

#include "main.h"

#include "eeeprom_hal.h"
#include "nor_map.h"

#include "cmsis_os2.h"

#include <stdlib.h>

extern QSPI_HandleTypeDef QSPIHandle;
extern osMutexId_t NORAccessHandle;

void EEEPROM_HAL_erase_subsector(uint32_t subsector){
	osMutexAcquire(NORAccessHandle, 150);
	uint32_t address = subsector * SUBSECTOR_SIZE;
	BSP_QSPI_Erase_Block(address);
	osMutexRelease(NORAccessHandle);
}

inline void EEEPROM_HAL_qspi_read(uint8_t *data, uint32_t addr, uint32_t len){
	// QSPI sometimes randomly times-out when debugging...
	osMutexAcquire(NORAccessHandle, 150);
	if(BSP_QSPI_Read(data, addr, len) != HAL_OK){
		if(QSPIHandle.ErrorCode & HAL_QSPI_ERROR_TIMEOUT){
			printf("QSPI read timed out...\r\n");
			BSP_QSPI_DeInit();
			BSP_QSPI_Init();
			BSP_QSPI_Read(data, addr, len);
		}
	}
	osMutexRelease(NORAccessHandle);
}

inline void EEEPROM_HAL_qspi_write(uint8_t *data, uint32_t addr, uint32_t len){
	// QSPI sometimes randomly times-out when debugging...
	osMutexAcquire(NORAccessHandle, 150);
	if(BSP_QSPI_Write(data, addr, len) != HAL_OK){
		if(QSPIHandle.ErrorCode & HAL_QSPI_ERROR_TIMEOUT){
			printf("QSPI write timed out...\r\n");
			BSP_QSPI_DeInit();
			BSP_QSPI_Init();
			BSP_QSPI_Write(data, addr, len);
		}
	}
	osMutexRelease(NORAccessHandle);
}
