//#if defined (STM32F401xC)
//#include "stm32f4xx_hal.h"
//#endif
#include "main.h"

#define KEY_ON 0
#define KEY_OFF 1
#define KEY_HOLD 2
#define KEY_IDLE 3
#define KEY_ERROR 4

#define KEY_HOLDC 50
#define KEY_SHAKEC 5

typedef struct _key_condi{
	uint8_t key_phy;
	uint8_t key_logic;
	uint8_t key_onc;
	uint8_t key_offc;
	
}*Pkey_condi,key_condi;

uint8_t vatek_key_press(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);

uint8_t vatek_key_press_test();




