/**
  ******************************************************************************
  * File Name          : gpio.c
  * Description        : This file provides code for the configuration
  *                      of all used GPIO pins.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, VATEK_RST_Pin|EP_RST_Pin, GPIO_PIN_RESET); //|RF_RST_Pin|RF_CS_Pin|RF_DO_Pin|RF_CLK_Pin
                          

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(ENABLE_AUDIO_AV_GPIO_Port, ENABLE_AUDIO_AV_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(AV_RST_GPIO_Port, AV_RST_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : PEPin PEPin PEPin PEPin 
                           PEPin PEPin */
  GPIO_InitStruct.Pin = VATEK_RST_Pin|EP_RST_Pin;//|RF_RST_Pin|RF_CS_Pin |RF_DO_Pin|RF_CLK_Pin
	
	/*Configure GPIO pin Output Level for keypad*/
	HAL_GPIO_WritePin(GPIOE,UP_Pin|CENTER_Pin|DOWN_Pin|LEFT_Pin|RIGHT_Pin,GPIO_PIN_SET);
                          
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : PEPin PEPin PEPin */
  GPIO_InitStruct.Pin = HDMI_STATUS_Pin|EP_INT_Pin;//|RF_DI_Pin
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = ENABLE_AUDIO_AV_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(ENABLE_AUDIO_AV_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = AV_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(AV_RST_GPIO_Port, &GPIO_InitStruct);
	
	/*Configure GPIO pin : UP_Pin|CENTER_Pin|DOWN_Pin|LEFT_Pin|RIGHT_Pin*/
	GPIO_InitStruct.Pin = UP_Pin|CENTER_Pin|DOWN_Pin|LEFT_Pin|RIGHT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

//	GPIO_InitStruct.Pin = SWITCHE1_Pin;
//	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;//GPIO_MODE_IT_RISING_FALLING, GPIO_MODE_IT_RISING
//	GPIO_InitStruct.Pull = GPIO_NOPULL;
//	HAL_GPIO_Init(SWITCHE1_GPIO_Port, &GPIO_InitStruct);

//	HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 0);
//	HAL_NVIC_EnableIRQ(EXTI1_IRQn);

}

/* USER CODE BEGIN 2 */
//extern int vatek_all_rd_hal();

//void EXTI1_IRQHandler(void)
//{
//    HAL_GPIO_EXTI_IRQHandler(SWITCHE1_Pin);
//}

//void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
//{
//	if (GPIO_Pin == SWITCHE1_Pin)
//	{
//		GPIO_PinState pinState = HAL_GPIO_ReadPin(GPIOD, SWITCHE1_Pin);
//			
//		if (pinState == GPIO_PIN_RESET)
//		{
//			vatek_all_rd_hal();
//		}
//		else
//		{
//			
//		}
//	}
//}
/* USER CODE END 2 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
