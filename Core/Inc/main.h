/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#include "MenuHelper.h"
#include "FrameHelper.h"
#include "ItemHelper.h"
#include "PopupHelper.h"
#include "fonts.h"
#include "ScreenAgent.h"

#include "stm32746g_qspi.h"
#include "LogAgent.h"
#include "LEDAgent.h"
#include "PushButtonAgent.h"
#include "ms5607Agent.h"
#include "GeneralHelper.h"
#include "PowerAgent.h"

#include "Bno055Agent.h"
#include "bno_config.h"

#include "cmd_interp.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define GPIO_Output_BNO055_Reset__Pin GPIO_PIN_4
#define GPIO_Output_BNO055_Reset__GPIO_Port GPIOE
#define GPIO_Output_MS5607_CS__SN_Pin GPIO_PIN_5
#define GPIO_Output_MS5607_CS__SN_GPIO_Port GPIOE
#define GPIO_Output_SPI1_CS_Pin GPIO_PIN_5
#define GPIO_Output_SPI1_CS_GPIO_Port GPIOC
#define GPIO_Output_ms5607_CS_Pin GPIO_PIN_0
#define GPIO_Output_ms5607_CS_GPIO_Port GPIOB
#define GPIO_Output_Hold_Pin GPIO_PIN_1
#define GPIO_Output_Hold_GPIO_Port GPIOB
#define GPIO_Output_WriteProtect_Pin GPIO_PIN_7
#define GPIO_Output_WriteProtect_GPIO_Port GPIOE
#define TIM1_CH1_LED_BLUE_Pin GPIO_PIN_9
#define TIM1_CH1_LED_BLUE_GPIO_Port GPIOE
#define TIM1_CH2_LED_GREEN_Pin GPIO_PIN_11
#define TIM1_CH2_LED_GREEN_GPIO_Port GPIOE
#define TIM1_CH3_LED_Red_Pin GPIO_PIN_13
#define TIM1_CH3_LED_Red_GPIO_Port GPIOE
#define TIM1_CH4_Mosfet2_Pin GPIO_PIN_14
#define TIM1_CH4_Mosfet2_GPIO_Port GPIOE
#define TIM2_Ch2_Mosfet3_Pin GPIO_PIN_10
#define TIM2_Ch2_Mosfet3_GPIO_Port GPIOB
#define GPIO_Output_QC_DN_Signal2_Pin GPIO_PIN_9
#define GPIO_Output_QC_DN_Signal2_GPIO_Port GPIOD
#define GPIO_Input_2_Pin GPIO_PIN_15
#define GPIO_Input_2_GPIO_Port GPIOD
#define GPIO_Output__3_Pin GPIO_PIN_7
#define GPIO_Output__3_GPIO_Port GPIOC
#define TIM2_CH1_Mosfet1_Pin GPIO_PIN_15
#define TIM2_CH1_Mosfet1_GPIO_Port GPIOA
#define GPIO_Output_ChargeEN__Pin GPIO_PIN_0
#define GPIO_Output_ChargeEN__GPIO_Port GPIOD
#define GPIO_Input_ChargeDetect_Pin GPIO_PIN_2
#define GPIO_Input_ChargeDetect_GPIO_Port GPIOD
#define GPIO_Output_QC_DP_Signal1_Pin GPIO_PIN_3
#define GPIO_Output_QC_DP_Signal1_GPIO_Port GPIOD
#define GPIO_Output_QC_DP_Signal2_Pin GPIO_PIN_5
#define GPIO_Output_QC_DP_Signal2_GPIO_Port GPIOD
#define GPIO_Output_QC_DN_Signal1_Pin GPIO_PIN_7
#define GPIO_Output_QC_DN_Signal1_GPIO_Port GPIOD
#define GPIO_EXTI0_BNO_Int_Pin GPIO_PIN_0
#define GPIO_EXTI0_BNO_Int_GPIO_Port GPIOE
#define GPIO_EXTI0_BNO_Int_EXTI_IRQn EXTI0_IRQn
#define GPIO_Input_BNO055_BI_Ind_Pin GPIO_PIN_1
#define GPIO_Input_BNO055_BI_Ind_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */

typedef enum sState {INIT, IDLE, OPERATIONAL} tState;

#define SD_CS_Pin GPIO_PIN_5
#define SD_CS_GPIO_Port GPIOC
/* USER CODE BEGIN Private defines */
#define SD_SPI_HANDLE hspi1

extern bool isRecordingActivated;
extern tCURSOR_DATA currentCursorPosition;
extern tBARODATA ms5607Baro;
extern tDATACHANNEL usbDataChannel;
extern tState unitState;

extern uint8_t baroReadingArray[100];

extern float versionID;
extern float buildID;


/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
