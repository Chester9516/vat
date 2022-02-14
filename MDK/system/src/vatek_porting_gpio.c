
#include "vatek_api.h"

#if defined(STM32F407xx) || defined(STM32F401xC)
#include <stm32f4xx.h>
#endif

vatek_result vatek_porting_gpio_write(gpio_pin pin, uint8_t val)
{
    vatek_result result = vatek_result_success;
    
#if defined(STM32F407xx) || defined(STM32F401xC)
    if (val)
        HAL_GPIO_WritePin((GPIO_TypeDef*)pin.port, pin.index, GPIO_PIN_SET);
    else
        HAL_GPIO_WritePin((GPIO_TypeDef*)pin.port, pin.index, GPIO_PIN_RESET);
#endif

    return result;
}

vatek_result vatek_porting_gpio_read(gpio_pin pin, uint8_t *val)
{
    vatek_result result = vatek_result_success;
    
#if defined(STM32F407xx) || defined(STM32F401xC)
    *val = HAL_GPIO_ReadPin((GPIO_TypeDef*)pin.port, pin.index);
#endif

    return result;
}

