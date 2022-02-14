#ifndef _DRIVER_SX1804A_
#define _DRIVER_SX1804A_

#include <driver/bridge_driver.h>

#ifdef __cplusplus
extern "C" {
#endif

    HAL_API vatek_result sx1804a_check_support(void);
    HAL_API vatek_result sx1804a_open(hbridge_source* hsource);
    HAL_API vatek_result sx1804a_config(hbridge_source hsource,Pbavsource_param param);
    HAL_API vatek_result sx1804a_get_status(hbridge_source hsource,Pbridge_source pbsourcesttaus);
    HAL_API vatek_result sx1804a_set_output(hbridge_source hsource,int32_t isoutput);
    HAL_API void sx1804a_close(hbridge_source hsource);

#ifdef __cplusplus
}
#endif

#endif
