
#ifndef _VATEK_SAMPLE_BROADCAST_
#define _VATEK_SAMPLE_BROADCAST_

#ifdef __cplusplus
    extern "C" {
#endif

    #include "vatek_api.h"

    /* main-sample */
    vatek_result sample_bc_init(Pboard_handle bh_main, Pboard_handle bh_phy, Pboard_handle bh_rf);
    vatek_result sample_bc_polling(void);

    /* sub-sample */
    vatek_result sample_bc_broadcast_from_phy(void);
    vatek_result sample_bc_broadcast_from_logo(void);
    vatek_result sample_bc_broadcast_stop(void);
    vatek_result sample_bc_broadcast_signalischange(void);
    vatek_result sample_bc_polling_cc(void);
		uint32_t vatek_check_stop(void);

    #define SAMPLE_ERR(fmt,...)    printf("sample  :  error [%s-%d]"fmt"\r\n",__func__ ,__LINE__,##__VA_ARGS__)
    #define SAMPLE_LOG(fmt,...)    printf("sample  :  "fmt"\r\n",##__VA_ARGS__)
    
    #define is_success(result)     (result==vatek_result_success)
    
#ifdef __cplusplus
    }
#endif

#endif

