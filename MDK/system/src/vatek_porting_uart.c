
#include "vatek_api.h"

#if defined(STM32F407xx) || defined(STM32F401xC)
#include <stm32f4xx.h>
#endif

vatek_result vatek_porting_uart_receive(Pboard_handle hboard, uint8_t* pbuf, uint32_t len, uint32_t timeout)
{
    vatek_result result = vatek_result_success;

#if defined(STM32F407xx) || defined(STM32F401xC)
    if (HAL_UART_Receive(hboard, pbuf, len, timeout) != HAL_OK)
        result = vatek_result_uartfail;
#endif

    return result;
}

vatek_result vatek_porting_uart_transmit(Pboard_handle hboard, uint8_t* pbuf, uint32_t len, uint32_t timeout)
{
    vatek_result result = vatek_result_success;

#if defined(STM32F407xx) || defined(STM32F401xC)
    if (HAL_UART_Transmit(hboard, pbuf, len, timeout) != HAL_OK)
        result = vatek_result_uartfail;
#endif

    return result;
}

