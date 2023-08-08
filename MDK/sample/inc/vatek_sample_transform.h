
#ifndef _VATEK_SAMPLE_TRANSFORM_
#define _VATEK_SAMPLE_TRANSFORM_

#ifdef __cplusplus
    extern "C" {
#endif

    #include "vatek_transform_api.h"

    typedef struct _sample_tf_init_parm
    {
        Pboard_handle bh_main;
        Pboard_handle bh_tuner;
        Pboard_handle bh_demod;
        Pboard_handle bh_rf;
    }sample_tf_init_parm, *Psample_tf_init_parm;

    vatek_result sample_tf_init(Phtransform bh_main, Pboard_handle bh_demod, Pboard_handle bh_tuner, Pboard_handle bh_rf);
    vatek_result sample_tf_capture(void);                           // capture specific table.
    vatek_result sample_tf_play_program(uint8_t program_num);       // transform specific program.
    vatek_result sample_tf_play_allprogram(void);                   // transform all program.
    vatek_result sample_tf_stop(void);
    vatek_result sample_tf_polling(void);
    vatek_result sample_tf_statusmsg(void);

    #define SAMPLE_ERR(fmt,...)    printf("sample  :  error [%s-%d]"fmt"\r\n",__func__ ,__LINE__,##__VA_ARGS__)
    #define SAMPLE_LOG(fmt,...)    printf("sample  :  "fmt"\r\n",##__VA_ARGS__)
    
#ifdef __cplusplus
    }
#endif

#endif

