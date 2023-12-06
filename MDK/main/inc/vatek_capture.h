
#ifndef _VATEK_CAPTURE_
#define _VATEK_CAPTURE_

#ifdef __cplusplus
    extern "C" {
#endif

    vatek_result vatek_capture_create(Phms_handle handle, Pcapture_param param);
    vatek_result vatek_capture_destroy(Phms_handle handle);
    vatek_result vatek_capture_reset(Phms_handle handle);
    vatek_result vatek_capture_gettable(Phms_handle handle, Ppsitable_parm *raw);

    #define CAPTURE_ERR(fmt,...)        VATEK_ERR(enum_,fmt,##__VA_ARGS__)
    #define CAPTURE_LOG(fmt,...)        VATEK_LOG(enum_,fmt,##__VA_ARGS__)

#ifdef __cplusplus
    }
#endif

#endif
