
#ifndef _VATEK_SYSTEM_
#define _VATEK_SYSTEM_

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "vatek_define.h"
    #include "vatek_define_periphral.h"

#ifdef __cplusplus
    extern "C" {
#endif

    typedef vatek_result(*fpi2c_set_speed)(Pboard_handle hboard, uint32_t speedkhz);
    typedef vatek_result(*fpi2c_start)(Pboard_handle hboard, uint8_t devaddr, uint32_t restart);
    typedef vatek_result(*fpi2c_write)(Pboard_handle hboard, uint8_t* pbuf, uint32_t len);
    typedef vatek_result(*fpi2c_read)(Pboard_handle hboard, uint8_t* pbuf, uint32_t len);
    typedef vatek_result(*fpi2c_stop)(Pboard_handle hboard);

    typedef struct _vatek_i2c
    {
        Pboard_handle hboard;
        fpi2c_set_speed set_speed;
        fpi2c_start start;
        fpi2c_write write;
        fpi2c_read read;
        fpi2c_stop stop;
    }vatek_i2c,*Pvatek_i2c;

    typedef vatek_result(*fpgpio_write)(gpio_pin pin, uint8_t val);
    typedef vatek_result(*fpgpio_read)(gpio_pin pin, uint8_t *val);

    typedef struct _vatek_gpio
    {
        fpgpio_write write;
        fpgpio_read read;
    }vatek_gpio,*Pvatek_gpio;
    
    typedef vatek_result(*fpuart_receive)(Pboard_handle hboard, uint8_t* pbuf, uint32_t len, uint32_t timeout);
    typedef vatek_result(*fpuart_transmit)(Pboard_handle hboard, uint8_t* pbuf, uint32_t len, uint32_t timeout);
    
    typedef struct _vatek_uart
    {
        Pboard_handle hboard;
        fpuart_receive receive;
        fpuart_transmit transmit;
    }vatek_uart, *Pvatek_uart;
    
    typedef enum _system_device_type 
    {
        sys_dev_type_unknown            = 0,
        sys_dev_type_main               = 0x100,
        sys_dev_type_rf                 = 0x200,
        sys_dev_type_rf_r2              = sys_dev_type_rf|rf_type_r2,
        sys_dev_type_phy                = 0x400,
        sys_dev_type_phy_ep9555e        = sys_dev_type_phy|phy_type_ep9555e,
        sys_dev_type_phy_ep9351         = sys_dev_type_phy|phy_type_ep9351,
        sys_dev_type_phy_adv7182a_cvbs  = sys_dev_type_phy|phy_type_adv7182a_cvbs,
        sys_dev_type_phy_adv7182a_ypbpr = sys_dev_type_phy|phy_type_adv7182a_ypbpr,
        sys_dev_type_phy_adv7180        = sys_dev_type_phy|phy_type_adv7180,
        sys_dev_type_phy_adv7611        = sys_dev_type_phy|phy_type_adv7611,
        sys_dev_type_phy_h1             = sys_dev_type_phy|phy_type_h1,
        sys_dev_type_tuner              = 0x500,
        sys_dev_type_tuner_mxl608       = sys_dev_type_tuner|tuner_type_mxl608,
        sys_dev_type_demod              = 0x600,
        sys_dev_type_demod_avl68xx      = sys_dev_type_demod|demod_type_avl68xx,
        sys_dev_type_demod_lgdt3305     = sys_dev_type_demod|demod_type_lgdt3305,
    }system_device_type;
    
    vatek_result vatek_system_i2c_create(Pboard_handle hboard, Pvatek_i2c *vi2c);
    vatek_result vatek_system_i2c_destroy(Pvatek_i2c vi2c);
    vatek_result vatek_system_gpio_create(Pvatek_gpio *vgpio);
    vatek_result vatek_system_gpio_destroy(Pvatek_gpio vgpio);
    vatek_result vatek_system_uart_create(Pboard_handle hboard, Pvatek_uart *vuart);
    vatek_result vatek_system_uart_destroy(Pvatek_uart vuart);
    uint32_t vatek_system_gettick(void);
    vatek_result vatek_system_delay(uint32_t msec);
    vatek_result vatek_system_reset(system_device_type type, Pboard_handle hboard);

    #define VATEK_ERR(dev, fmt,...)          printf("[%-8s : %08lx] : error [%s-%d]"fmt"\r\n",#dev,vatek_system_gettick(),__func__ ,__LINE__,##__VA_ARGS__)
    #define VATEK_LOG(dev, fmt,...)          printf("[%-8s : %08lx] : "fmt"\r\n",#dev,vatek_system_gettick(),##__VA_ARGS__)

#ifdef __cplusplus
    }
#endif

#endif

