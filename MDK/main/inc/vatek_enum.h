
#ifndef _VATEK_ENUM_
#define _VATEK_ENUM_

#ifdef __cplusplus
    extern "C" {
#endif

    vatek_result vatek_enum_create(Phms_handle handle);
    vatek_result vatek_enum_destroy(Phms_handle handle);
    vatek_result vatek_enum_reset(Phms_handle handle);
    vatek_result vatek_enum_getlist(Phms_handle handle, Penum_list *list);


    #define ENUM_ERR(fmt,...)        VATEK_ERR(enum_,fmt,##__VA_ARGS__)
    #define ENUM_LOG(fmt,...)        VATEK_LOG(enum_,fmt,##__VA_ARGS__)

#ifdef __cplusplus
    }
#endif

#endif

