/*
 * task_general.c
 *
 *  Created on: Jun 10, 2023
 *      Author: morga
 */

#include "task_general.h"
#include "main.h"
#include "../shell/inc/sys_command_line.h"
#include "ui/openht_ui.h"

#include <cmsis_os2.h>
#include <fatfs.h>

/* Flags */
#define GENERAL_USD_DETECT				(1<<0)
#define GENERAL_USD_START_TIMER			(1<<1)

#define GENERAL_ALL_FLAGS	(GENERAL_USD_DETECT | GENERAL_USD_START_TIMER)

/* private variables */
osTimerId_t uSD_debounce_timer = NULL;
const osTimerAttr_t uSD_debounce_timer_attr = {
		.name = "uSD_Debounce"
};

osThreadId_t general_thread_id = NULL;

static bool uSD_mounted = false;
static bool user_button_pressed = false;

/* Private functions */
void uSD_timer_expired(void *arguments);

/* functions implementations */

/**
  * @brief  Function implementing the generalTask thread.
  * @param  argument: Not used
  * @retval None
  */
void StartGeneralTask(void *argument)
{

	general_thread_id = osThreadGetId();

	// Mount SD fatfs
	FATFS fs;
	if(f_mount(&fs, "0:", 1) == FR_OK){
		uSD_mounted = true;
	}

	uSD_debounce_timer = osTimerNew(uSD_timer_expired, osTimerOnce, NULL, &uSD_debounce_timer_attr);

	GPIO_InitTypeDef uSD_DetectGpio; //PG2
	uSD_DetectGpio.Mode = GPIO_MODE_IT_RISING_FALLING;
	uSD_DetectGpio.Pull = GPIO_NOPULL;
	uSD_DetectGpio.Speed = GPIO_SPEED_LOW;
	uSD_DetectGpio.Pin = GPIO_PIN_2;

	HAL_GPIO_Init(GPIOG, &uSD_DetectGpio);
	HAL_NVIC_SetPriority(EXTI2_IRQn, 5, 0);
	HAL_NVIC_EnableIRQ(EXTI2_IRQn);

	/* Infinite loop */
	for(;;)
	{
		uint32_t result = osThreadFlagsWait(GENERAL_ALL_FLAGS, osFlagsNoClear, 10);

		if(result == osErrorTimeout){
			// avoid hw interrupts with lvgl ui code, this simply polls the state of the blue hw user button
			// and calls the appropriate function in the ui code to handle the "event"
			// this also avoids any need to de-bounce
			if (BSP_PB_GetState(BUTTON_USER) == 1 && !user_button_pressed) {
				user_button_pressed = true;
				on_userbutton_press();
			}
			if (BSP_PB_GetState(BUTTON_USER) == 0 && user_button_pressed) {
				user_button_pressed = false;
				on_userbutton_release();
			}
			CLI_RUN();
		}else if(result == GENERAL_USD_START_TIMER){
			osThreadFlagsClear(GENERAL_USD_START_TIMER);
			osTimerStart(uSD_debounce_timer, 5);
		}else if(result == GENERAL_USD_DETECT){
			osThreadFlagsClear(GENERAL_USD_DETECT);

			LOG(CLI_LOG_GENERAL, "Processing uSD state change.\r\n");

			// Pin SET means no card inserted
			GPIO_PinState pin_state = HAL_GPIO_ReadPin(uSD_Detect_GPIO_Port, uSD_Detect_Pin);
			if(pin_state == GPIO_PIN_SET && uSD_mounted){
				FRESULT fres = f_mount(NULL, "0:", 1);
				if(fres != FR_OK){
					LOG(CLI_LOG_GENERAL, "Error unmounting SD card.\r\n");
				}else{
					LOG(CLI_LOG_GENERAL, "SD card unmounted.\r\n");
					uSD_mounted = false;
				}
			}else if(pin_state == GPIO_PIN_RESET && !uSD_mounted){
				BSP_SD_Init();
				FRESULT fres = f_mount(&fs, "0:", 1);
				if(fres != FR_OK){
					LOG(CLI_LOG_GENERAL, "Error mounting SD card.\r\n");
				}else{
					LOG(CLI_LOG_GENERAL, "SD card mounted.\r\n");
					uSD_mounted = true;
				}
			}
		}
	}
}

void general_uSD_Detect_it(){
	// CMSIS OS2 does not allow to start a timer from an ISR...
	uint32_t result = osThreadFlagsSet(general_thread_id, GENERAL_USD_START_TIMER);
	if(result >= 0x80000000){
		ERR("Could not start uSD debounce timer: osThreadFlagsSet returned 0x%08lx.\r\n", result);
	}
}

void uSD_timer_expired(void *arguments){
	uint32_t result = osThreadFlagsSet(general_thread_id, GENERAL_USD_DETECT);
	if(result >= 0x80000000){
		ERR("Could not process uSD detect pin change: osThreadFlagsSet returned 0x%08lx.\r\n", result);
	}
}

