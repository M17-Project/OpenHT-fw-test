/* USER CODE BEGIN Header */
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
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "openht_types.h"
#include "stm32469i_discovery.h" /* DISCOVERY includes component */
#include "stm32469i_discovery_qspi.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
#define CLI_NAME openHT

#define CLI_ADDITIONAL_LOG_CATEGORIES \
	X(EEEPROM, false) \
	X(FPGA, true) \
	X(RADIO, true) \
	X(GUI, true) \
	X(GENERAL, true) \
	X(MIC, true) \

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void wait_spi_xfer_done(uint32_t timeout);
//void reset_spi1_flag();
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define SPKR_HP_Pin GPIO_PIN_3
#define SPKR_HP_GPIO_Port GPIOE
#define AUDIO_RST_Pin GPIO_PIN_2
#define AUDIO_RST_GPIO_Port GPIOE
#define IO1_Pin GPIO_PIN_14
#define IO1_GPIO_Port GPIOG
#define FMC_NBL1_Pin GPIO_PIN_1
#define FMC_NBL1_GPIO_Port GPIOE
#define FMC_NBL0_Pin GPIO_PIN_0
#define FMC_NBL0_GPIO_Port GPIOE
#define I2C1_SCL_Pin GPIO_PIN_8
#define I2C1_SCL_GPIO_Port GPIOB
#define XCVR_NSS_Pin GPIO_PIN_15
#define XCVR_NSS_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define I2C1_SDA_Pin GPIO_PIN_9
#define I2C1_SDA_GPIO_Port GPIOB
#define SDNCAS_Pin GPIO_PIN_15
#define SDNCAS_GPIO_Port GPIOG
#define IO2_Pin GPIO_PIN_11
#define IO2_GPIO_Port GPIOG
#define D2_Pin GPIO_PIN_0
#define D2_GPIO_Port GPIOD
#define FMC_NBL2_Pin GPIO_PIN_4
#define FMC_NBL2_GPIO_Port GPIOI
#define MAIN_KILL_Pin GPIO_PIN_12
#define MAIN_KILL_GPIO_Port GPIOG
#define IO3_Pin GPIO_PIN_10
#define IO3_GPIO_Port GPIOG
#define IO3_EXTI_IRQn EXTI15_10_IRQn
#define LED3_Pin GPIO_PIN_5
#define LED3_GPIO_Port GPIOD
#define IO6_Pin GPIO_PIN_3
#define IO6_GPIO_Port GPIOD
#define D3_Pin GPIO_PIN_1
#define D3_GPIO_Port GPIOD
#define D27_Pin GPIO_PIN_3
#define D27_GPIO_Port GPIOI
#define D26_Pin GPIO_PIN_2
#define D26_GPIO_Port GPIOI
#define TP_Pin GPIO_PIN_13
#define TP_GPIO_Port GPIOC
#define A0_Pin GPIO_PIN_0
#define A0_GPIO_Port GPIOF
#define FMC_NBL3_Pin GPIO_PIN_5
#define FMC_NBL3_GPIO_Port GPIOI
#define D29_Pin GPIO_PIN_7
#define D29_GPIO_Port GPIOI
#define D31_Pin GPIO_PIN_10
#define D31_GPIO_Port GPIOI
#define D28_Pin GPIO_PIN_6
#define D28_GPIO_Port GPIOI
#define LED4_Pin GPIO_PIN_3
#define LED4_GPIO_Port GPIOK
#define IO0_Pin GPIO_PIN_9
#define IO0_GPIO_Port GPIOG
#define LED2_Pin GPIO_PIN_4
#define LED2_GPIO_Port GPIOD
#define D23_Pin GPIO_PIN_15
#define D23_GPIO_Port GPIOH
#define D25_Pin GPIO_PIN_1
#define D25_GPIO_Port GPIOI
#define A1_Pin GPIO_PIN_1
#define A1_GPIO_Port GPIOF
#define D30_Pin GPIO_PIN_9
#define D30_GPIO_Port GPIOI
#define D21_Pin GPIO_PIN_13
#define D21_GPIO_Port GPIOH
#define D22_Pin GPIO_PIN_14
#define D22_GPIO_Port GPIOH
#define D24_Pin GPIO_PIN_0
#define D24_GPIO_Port GPIOI
#define A2_Pin GPIO_PIN_2
#define A2_GPIO_Port GPIOF
#define SW_CTL1_Pin GPIO_PIN_7
#define SW_CTL1_GPIO_Port GPIOC
#define A3_Pin GPIO_PIN_3
#define A3_GPIO_Port GPIOF
#define SDCLK_Pin GPIO_PIN_8
#define SDCLK_GPIO_Port GPIOG
#define A4_Pin GPIO_PIN_4
#define A4_GPIO_Port GPIOF
#define SDNE0_Pin GPIO_PIN_3
#define SDNE0_GPIO_Port GPIOH
#define LED1_Pin GPIO_PIN_6
#define LED1_GPIO_Port GPIOG
#define A5_Pin GPIO_PIN_5
#define A5_GPIO_Port GPIOF
#define SDCKE0_Pin GPIO_PIN_2
#define SDCKE0_GPIO_Port GPIOH
#define D1_Pin GPIO_PIN_15
#define D1_GPIO_Port GPIOD
#define SW_CTL2_Pin GPIO_PIN_13
#define SW_CTL2_GPIO_Port GPIOB
#define D15_Pin GPIO_PIN_10
#define D15_GPIO_Port GPIOD
#define FPGA_PROGRAMN_Pin GPIO_PIN_3
#define FPGA_PROGRAMN_GPIO_Port GPIOC
#define D0_Pin GPIO_PIN_14
#define D0_GPIO_Port GPIOD
#define RF_RST_Pin GPIO_PIN_12
#define RF_RST_GPIO_Port GPIOB
#define D14_Pin GPIO_PIN_9
#define D14_GPIO_Port GPIOD
#define D13_Pin GPIO_PIN_8
#define D13_GPIO_Port GPIOD
#define SDNWE_Pin GPIO_PIN_0
#define SDNWE_GPIO_Port GPIOC
#define FPGA_DONE_Pin GPIO_PIN_2
#define FPGA_DONE_GPIO_Port GPIOC
#define OTG_FS1_PowerSwitchOn_Pin GPIO_PIN_2
#define OTG_FS1_PowerSwitchOn_GPIO_Port GPIOB
#define A6_Pin GPIO_PIN_12
#define A6_GPIO_Port GPIOF
#define A11_Pin GPIO_PIN_1
#define A11_GPIO_Port GPIOG
#define A9_Pin GPIO_PIN_15
#define A9_GPIO_Port GPIOF
#define uSD_Detect_Pin GPIO_PIN_2
#define uSD_Detect_GPIO_Port GPIOG
#define D20_Pin GPIO_PIN_12
#define D20_GPIO_Port GPIOH
#define IRQ_XCVR_Pin GPIO_PIN_1
#define IRQ_XCVR_GPIO_Port GPIOA
#define WAKEUP_Pin GPIO_PIN_0
#define WAKEUP_GPIO_Port GPIOA
#define Batt_Measurement_Pin GPIO_PIN_4
#define Batt_Measurement_GPIO_Port GPIOA
#define FPGA_INITN_Pin GPIO_PIN_4
#define FPGA_INITN_GPIO_Port GPIOC
#define A7_Pin GPIO_PIN_13
#define A7_GPIO_Port GPIOF
#define A10_Pin GPIO_PIN_0
#define A10_GPIO_Port GPIOG
#define D5_Pin GPIO_PIN_8
#define D5_GPIO_Port GPIOE
#define LCD_RST_Pin GPIO_PIN_7
#define LCD_RST_GPIO_Port GPIOH
#define D17_Pin GPIO_PIN_9
#define D17_GPIO_Port GPIOH
#define D19_Pin GPIO_PIN_11
#define D19_GPIO_Port GPIOH
#define PB_INT_Pin GPIO_PIN_2
#define PB_INT_GPIO_Port GPIOA
#define PB_INT_EXTI_IRQn EXTI2_IRQn
#define PTT_Pin GPIO_PIN_5
#define PTT_GPIO_Port GPIOC
#define PTT_EXTI_IRQn EXTI9_5_IRQn
#define A8_Pin GPIO_PIN_14
#define A8_GPIO_Port GPIOF
#define DSI_TE_Pin GPIO_PIN_2
#define DSI_TE_GPIO_Port GPIOJ
#define SDNMT48LC4M32B2B5_6A_RAS_RAS___Pin GPIO_PIN_11
#define SDNMT48LC4M32B2B5_6A_RAS_RAS___GPIO_Port GPIOF
#define D6_Pin GPIO_PIN_9
#define D6_GPIO_Port GPIOE
#define D8_Pin GPIO_PIN_11
#define D8_GPIO_Port GPIOE
#define D11_Pin GPIO_PIN_14
#define D11_GPIO_Port GPIOE
#define STLK_RX_Pin GPIO_PIN_10
#define STLK_RX_GPIO_Port GPIOB
#define FPGA_RST_Pin GPIO_PIN_6
#define FPGA_RST_GPIO_Port GPIOH
#define D16_Pin GPIO_PIN_8
#define D16_GPIO_Port GPIOH
#define D18_Pin GPIO_PIN_10
#define D18_GPIO_Port GPIOH
#define FPGA_NSS_Pin GPIO_PIN_7
#define FPGA_NSS_GPIO_Port GPIOA
#define PGOOD_Pin GPIO_PIN_1
#define PGOOD_GPIO_Port GPIOB
#define EXT_RESET_Pin GPIO_PIN_0
#define EXT_RESET_GPIO_Port GPIOB
#define D4_Pin GPIO_PIN_7
#define D4_GPIO_Port GPIOE
#define D7_Pin GPIO_PIN_10
#define D7_GPIO_Port GPIOE
#define D9_Pin GPIO_PIN_12
#define D9_GPIO_Port GPIOE
#define D12_Pin GPIO_PIN_15
#define D12_GPIO_Port GPIOE
#define D10_Pin GPIO_PIN_13
#define D10_GPIO_Port GPIOE
#define STLK_TX_Pin GPIO_PIN_11
#define STLK_TX_GPIO_Port GPIOB
#define IO5_Pin GPIO_PIN_14
#define IO5_GPIO_Port GPIOB
#define IO4_Pin GPIO_PIN_15
#define IO4_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
