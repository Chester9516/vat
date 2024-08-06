#include "stdio.h"
#include "gpio.h"
#include "stm32f4xx_hal.h"

const int lookup_table[16] = {
    1, 2, 3, 4,
    5, 6, 7, 8,
    9, 10, 11, 12,
    13, 14, 15, 16
};

int read_gpio_state() {
   int state = 0;
//    state |= (HAL_GPIO_ReadPin(SWITCHD0_GPIO_Port, SWITCHD0_Pin) == GPIO_PIN_SET) << 0;
//    state |= (HAL_GPIO_ReadPin(SWITCHD1_GPIO_Port, SWITCHD1_Pin) == GPIO_PIN_SET) << 1;
//    state |= (HAL_GPIO_ReadPin(SWITCHE0_GPIO_Port, SWITCHE0_Pin) == GPIO_PIN_SET) << 2;
//    state |= (HAL_GPIO_ReadPin(SWITCHE1_GPIO_Port, SWITCHE1_Pin) == GPIO_PIN_SET) << 3;
	state = HAL_GPIO_ReadPin(SWITCHD0_GPIO_Port, SWITCHD0_Pin);
	printf("%d ",state);
	state = HAL_GPIO_ReadPin(SWITCHD1_GPIO_Port, SWITCHD1_Pin);
	printf("%d ",state);
	state = HAL_GPIO_ReadPin(SWITCHE0_GPIO_Port, SWITCHE0_Pin);
	printf("%d ",state);
	state = HAL_GPIO_ReadPin(SWITCHE1_GPIO_Port, SWITCHE1_Pin);
	printf("%d\r\n",state);
    return state;
}

int get_action_value()
{
    int state = read_gpio_state();
    if (state >= 0 && state < 16) {
        return lookup_table[state];
    } else {
        return -1; 
    }
}
