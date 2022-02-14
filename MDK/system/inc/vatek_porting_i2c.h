
#ifndef _VATEK_PORTING_I2C_
#define _VATEK_PORTING_I2C_

#ifdef __cplusplus
        extern "C" {
#endif

    vatek_result vatek_porting_i2c_set_speed(Pboard_handle hboard, uint32_t speedkhz);
    vatek_result vatek_porting_i2c_start(Pboard_handle hboard, uint8_t devaddr, uint32_t restart);
    vatek_result vatek_porting_i2c_write(Pboard_handle hboard, uint8_t* pbuf, uint32_t len);
    vatek_result vatek_porting_i2c_read(Pboard_handle hboard, uint8_t* pbuf, uint32_t len);
    vatek_result vatek_porting_i2c_stop(Pboard_handle hboard);

#ifdef __cplusplus
    }
#endif

#endif

