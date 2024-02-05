#if defined(STM32F401xC)
#include "stdio.h"
#include "vatek_sample_keypad_stm32f401.h"

uint8_t vatek_key_press_test()
{
	while(1){
		switch(vatek_key_press(GPIOE, UP_Pin)){
			case KEY_ON:
				printf("key pressed\r\n");
				break;
			case KEY_OFF:
				printf("key released\r\n");
				break;
			case KEY_HOLD:
				printf("key hold\r\n");
				break;
			case KEY_ERROR:
				printf("key error occur\r\n");
				break;
		}
		
		switch(vatek_key_press(GPIOE, CENTER_Pin)){
			case KEY_ON:
				printf("key pressed\r\n");
				break;
			case KEY_OFF:
				printf("key released\r\n");
				break;
			case KEY_HOLD:
				printf("key hold\r\n");
				break;
			case KEY_ERROR:
				printf("key error occur\r\n");
				break;
		}
		
		switch(vatek_key_press(GPIOE, DOWN_Pin)){
			case KEY_ON:
				printf("key pressed\r\n");
				break;
			case KEY_OFF:
				printf("key released\r\n");
				break;
			case KEY_HOLD:
				printf("key hold\r\n");
				break;
			case KEY_ERROR:
				printf("key error occur\r\n");
				break;
		}
		
		switch(vatek_key_press(GPIOE, LEFT_Pin)){
			case KEY_ON:
				printf("key pressed\r\n");
				break;
			case KEY_OFF:
				printf("key released\r\n");
				break;
			case KEY_HOLD:
				printf("key hold\r\n");
				break;
			case KEY_ERROR:
				printf("key error occur\r\n");
				break;
		}
		
		switch(vatek_key_press(GPIOE, RIGHT_Pin)){
			case KEY_ON:
				printf("key pressed\r\n");
				break;
			case KEY_OFF:
				printf("key released\r\n");
				break;
			case KEY_HOLD:
				printf("key hold\r\n");
				break;
			case KEY_ERROR:
				printf("key error occur\r\n");
				break;
		}
		HAL_Delay(20);
	}
}

static key_condi key[5] = {
	{KEY_OFF, KEY_OFF, 0, 0}, //UP
	{KEY_OFF, KEY_OFF, 0, 0}, //CENTER
	{KEY_OFF, KEY_OFF, 0, 0}, //DOWN
	{KEY_OFF, KEY_OFF, 0, 0}, //LEFT
	{KEY_OFF, KEY_OFF, 0, 0}, //RIGHT
};

uint8_t vatek_key_press(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	key_condi *key_temp;
	
	switch ((uint32_t)GPIOx){
		case (uint32_t)GPIOE:
			switch (GPIO_Pin){
				case UP_Pin:
					key_temp = &key[0];
					break;
				case CENTER_Pin:
					key_temp = &key[1];
					break;
				case DOWN_Pin:
					key_temp = &key[2];
					break;
				case LEFT_Pin:
					key_temp = &key[3];
					break;
				case RIGHT_Pin:
					key_temp = &key[4];
					break;
				default:
					printf("wrong key pad pin\r\n");
					return KEY_IDLE;
			}
			break;
			
		default:
			printf("error: key do not exist\r\n");
			return KEY_IDLE;
	}
	
	key_temp->key_phy = HAL_GPIO_ReadPin(GPIOx, GPIO_Pin);
	
	switch(key_temp->key_logic){
		case KEY_ON:
			switch(key_temp->key_phy){
				case KEY_ON: //make sure key is pressing
					key_temp->key_offc = 0;
					key_temp->key_onc++;
					if(key_temp->key_onc >= KEY_HOLDC){
						key_temp->key_onc = 0;
						key_temp->key_logic = KEY_HOLD;
						return KEY_HOLD;
					}
					return KEY_IDLE;
					
				case KEY_OFF:
					key_temp->key_offc++;
					if(key_temp->key_offc <= KEY_SHAKEC){
						key_temp->key_offc = 0;
						key_temp->key_logic = KEY_OFF;
						return KEY_OFF;
					}
					return KEY_IDLE;
					
				default:
						break;
			}
		case KEY_OFF:
			switch(key_temp->key_phy){
				case KEY_ON:
					key_temp->key_onc++;
					if(key_temp->key_onc >= KEY_SHAKEC){
						key_temp->key_onc = 0;
						key_temp->key_logic = KEY_ON;
						return KEY_ON;
					}
					return KEY_IDLE;
					
				case KEY_OFF:
					key_temp->key_onc = 0;
					return KEY_IDLE;
				
				default:
					break;
			}
		case KEY_HOLD:
			switch(key_temp->key_phy){
				case KEY_ON:
					key_temp->key_offc = 0;
					return KEY_HOLD;
				case KEY_OFF:
					key_temp->key_offc++;
					if(key_temp->key_offc >= KEY_SHAKEC){
						key_temp->key_offc = 0;
						key_temp->key_logic = KEY_OFF;
						return KEY_OFF;
					}
					return KEY_IDLE;
				default:
					break;
			}
		default:
			break;
	}
	return KEY_ERROR;
}

#endif