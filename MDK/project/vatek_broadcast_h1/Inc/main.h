/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
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
#define RF_RST_Pin GPIO_PIN_11
#define RF_RST_GPIO_Port GPIOE
#define RF_CS_Pin GPIO_PIN_12
#define RF_CS_GPIO_Port GPIOE
#define RF_DO_Pin GPIO_PIN_13
#define RF_DO_GPIO_Port GPIOE
#define RF_CLK_Pin GPIO_PIN_14
#define RF_CLK_GPIO_Port GPIOE
#define RF_DI_Pin GPIO_PIN_15
#define RF_DI_GPIO_Port GPIOE
#define ENABLE_AUDIO_AV_Pin GPIO_PIN_1
#define ENABLE_AUDIO_AV_GPIO_Port GPIOD
#define AV_RST_Pin GPIO_PIN_8
#define AV_RST_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */
#define H1_INT_Pin          EP_INT_Pin
#define H1_INT_GPIO_Port    EP_INT_GPIO_Port
#define H1_RST_Pin          EP_RST_Pin
#define H1_RST_GPIO_Port    EP_RST_GPIO_Port
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
