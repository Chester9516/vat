
#ifndef _VATEK_PORTING_UART_
#define _VATEK_PORTING_UART_

#ifdef __cplusplus
        extern "C" {
#endif

    vatek_result vatek_porting_uart_receive(Pboard_handle hboard, uint8_t* pbuf, uint32_t len, uint32_t timeout);
    vatek_result vatek_porting_uart_transmit(Pboard_handle hboard, uint8_t* pbuf, uint32_t len, uint32_t timeout);

#ifdef __cplusplus
    }
#endif

#endif

