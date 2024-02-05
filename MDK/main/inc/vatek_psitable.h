
#ifndef _VATEK_PSITABLE_
#define _VATEK_PSITABLE_

#ifdef __cplusplus
    extern "C" {
#endif

    vatek_result vatek_psitable_register(Phms_handle handle, Ppsitablelist_parm parm);
    vatek_result vatek_psitable_insert(Phms_handle handle, uint16_t tspacket_num, uint8_t *tspackets);
			
		//v1
		vatek_result vatek_psitable_register_v1(Phms_handle handle, Ppsitablelist_parm parm);

    #define PSITABLE_ERR(fmt,...)    VATEK_ERR(psi,fmt,##__VA_ARGS__)
    #define PSITABLE_LOG(fmt,...)    VATEK_LOG(psi,fmt,##__VA_ARGS__)

#ifdef __cplusplus
    }
#endif

#endif

