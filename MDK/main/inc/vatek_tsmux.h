
#ifndef _VATEK_TSMUX_
#define _VATEK_TSMUX_

#ifdef __cplusplus
    extern "C" {
#endif

    vatek_result vatek_tsmux_setparm(Phms_handle handle, tsmux_type type, Ptsmux_parm parm);

    #define TSMUX_ERR(fmt,...)    VATEK_ERR(tsmux,fmt,##__VA_ARGS__)
    #define TSMUX_LOG(fmt,...)    VATEK_LOG(tsmux,fmt,##__VA_ARGS__)

#ifdef __cplusplus
    }
#endif

#endif

