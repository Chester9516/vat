
#ifndef _VATEK_TSP_
#define _VATEK_TSP_

#ifdef __cplusplus
    extern "C" {
#endif

    vatek_result vatek_tsp_setinputparm_ts(Phms_handle handle, ts_input_parm parm);
    vatek_result vatek_tsp_setfilterparm(Phms_handle handle, tsp_filter_parm parm);

    #define TSP_ERR(fmt,...)        VATEK_ERR(tsp,fmt,##__VA_ARGS__)
    #define TSP_LOG(fmt,...)        VATEK_LOG(tsp,fmt,##__VA_ARGS__)

#ifdef __cplusplus
    }
#endif

#endif

