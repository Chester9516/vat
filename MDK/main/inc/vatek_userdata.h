
#ifndef _VATEK_USERDATA_
#define _VATEK_USERDATA_

#ifdef __cplusplus
    extern "C" {
#endif

    vatek_result vatek_userdata_setvideocodec(video_encode_type type);
    vatek_result vatek_userdata_raw_insert(Phms_handle handle, uint16_t data_num, uint8_t *data);
    vatek_result vatek_userdata_cc_insert(Phms_handle handle, uint16_t cc_num, Puserdata_cc_parm cc);

    #define USERDATA_ERR(fmt,...)    VATEK_ERR(userdata,fmt,##__VA_ARGS__)
    #define USERDATA_LOG(fmt,...)    VATEK_LOG(userdata,fmt,##__VA_ARGS__)

#ifdef __cplusplus
    }
#endif

#endif

