
#ifndef _TUNER_MXL608_
#define _TUNER_MXL608_
    
#ifdef __cplusplus
    extern "C" {
#endif

    typedef void* Pmxl608_handle;
    vatek_result mxl608_create(Pvatek_i2c vi2c, Pmxl608_handle *hmxl608);
    vatek_result mxl608_destroy(Pmxl608_handle hmxl608);
    vatek_result mxl608_lock(Pmxl608_handle hmxl608, Ptuner_lockparm parm);
    vatek_result mxl608_getstatus(Pmxl608_handle hmxl608, tuner_status *status_t);
    vatek_result mxl608_getrfstrength(Pmxl608_handle hmxl608, int32_t *power);
    
#ifdef __cplusplus
    }
#endif

#endif

