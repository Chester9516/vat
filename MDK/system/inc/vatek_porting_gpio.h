
#ifndef _VATEK_PORTING_GPIO_
#define _VATEK_PORTING_GPIO_

#ifdef __cplusplus
        extern "C" {
#endif

    vatek_result vatek_porting_gpio_write(gpio_pin pin, uint8_t val);
    vatek_result vatek_porting_gpio_read(gpio_pin pin, uint8_t *val);

#ifdef __cplusplus
    }
#endif

#endif

