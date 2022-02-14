
#ifndef _VATEK_SINEWAVE_MODE_
#define _VATEK_SINEWAVE_MODE_

#ifdef __cplusplus
    extern "C" {
#endif

    #include "vatek_api.h"

    /* main-sample */
    vatek_result sinewave_mode_init(Pboard_handle bh_main, Pboard_handle bh_rf);


    #define SAMPLE_ERR(fmt,...)    printf("sample  :  error [%s-%d]"fmt"\r\n",__func__ ,__LINE__,##__VA_ARGS__)
    #define SAMPLE_LOG(fmt,...)    printf("sample  :  "fmt"\r\n",##__VA_ARGS__)
    
#ifdef __cplusplus
    }
#endif

#endif

