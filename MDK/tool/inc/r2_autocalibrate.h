
#ifndef _R2_AUTOCALIBRATE_
#define _R2_AUTOCALIBRATE_

#ifdef __cplusplus
    extern "C" {
#endif

    #include "vatek_api.h"

    vatek_result r2_autocalibrate_init(Pboard_handle bh_main, Pboard_handle bh_rf, Pboard_handle bh_uart);
    vatek_result r2_autocalibrate_start(void);



#ifdef __cplusplus
    }
#endif

#endif

