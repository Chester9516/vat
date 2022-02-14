
#ifndef _TUNER_MXL603_
#define _TUNER_MXL603_
    
#ifdef __cplusplus
    extern "C" {
#endif

    typedef void* Pmxl603_handle;
    vatek_result mxl603_create(Pvatek_i2c vi2c, Pmxl603_handle *hmxl603);
    vatek_result mxl603_destroy(Pmxl603_handle hmxl603);
    vatek_result mxl603_lock(Pmxl603_handle hmxl603, Ptuner_lockparm parm);
    vatek_result mxl603_getstatus(Pmxl603_handle hmxl603, tuner_status * status_t);
    vatek_result mxl603_getrfstrength(Pmxl603_handle hmxl603, int32_t * power);
    
#ifdef __cplusplus
    }
#endif

#endif

