/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
#define VATEK_RST_Pin GPIO_PIN_2
#define VATEK_RST_GPIO_Port GPIOE
#define HDMI_STATUS_Pin GPIO_PIN_3
#define HDMI_STATUS_GPIO_Port GPIOE
#define EP_INT_Pin GPIO_PIN_4
#define EP_INT_GPIO_Port GPIOE
#define EP_RST_Pin GPIO_PIN_5
#define EP_RST_GPIO_Port GPIOE
#define UP_Pin GPIO_PIN_11
#define UP_GPIO_Port GPIOE
#define CENTER_Pin GPIO_PIN_12
#define CENTER_GPIO_Port GPIOE
#define DOWN_Pin GPIO_PIN_13
#define DOWN_GPIO_Port GPIOE
#define LEFT_Pin GPIO_PIN_14
#define LEFT_GPIO_Port GPIOE
#define RIGHT_Pin GPIO_PIN_15
#define RIGHT_GPIO_Port GPIOE
#define ENABLE_AUDIO_AV_Pin GPIO_PIN_1
#define ENABLE_AUDIO_AV_GPIO_Port GPIOD
#define AV_RST_Pin GPIO_PIN_8
#define AV_RST_GPIO_Port GPIOB

#define IN_EX_SWITCH_Pin GPIO_PIN_0
#define IN_EX_SWITCH_GPIO_Port GPIOD

/*H1 define pin*/
#define H1_RST_GPIO_Port EP_RST_GPIO_Port
#define H1_RST_Pin EP_RST_Pin

/*LCD Pin*/
#define LCD_K_ON_Pin GPIO_PIN_8
#define LCD_RS_Pin GPIO_PIN_9
#define LCD_RW_ON_Pin GPIO_PIN_10
#define LCD_EE_ON_Pin GPIO_PIN_11
#define LCD_D4_Pin GPIO_PIN_12
#define LCD_D5_Pin GPIO_PIN_13
#define LCD_D6_Pin GPIO_PIN_14
#define LCD_D7_Pin GPIO_PIN_15
#define LCD_GPIO_Port GPIOD
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
