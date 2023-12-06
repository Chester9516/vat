
#ifndef _VATEK_HMS_
#define _VATEK_HMS_

    #include "vatek_system.h"
//    #include "hal/hal_common_define.h"
    #include "hal/halservice_base.h"
    #include "hal/halservice_broadcast.h"
    #include "hal/halreg_rfmixer.h"
    #include "hal/halreg_mux.h"
    #include "hal/halreg_rfmixer.h"
    #include "hal/halservice_transform.h"
    //#include "hal/hal_register/halreg_licensed.h"
	#include "hal/halreg_playload.h"
		

#ifdef __cplusplus
    extern "C" {
#endif

    typedef enum
    {
        hms_type_broadcast      = 0,
        hms_type_transform      = 1,
        hms_type_r2             = 2,
    }hms_type;

    typedef struct _hms_handle
    {
        Pvatek_i2c vi2c;
        hms_type type;
    }hms_handle, *Phms_handle;

    vatek_result vatek_hms_create(Pboard_handle hboard, Phms_handle *handle);
    vatek_result vatek_hms_destroy(Phms_handle handle);
    vatek_result vatek_hms_reset(Phms_handle handle);
    vatek_result vatek_hms_read_hal(Phms_handle handle, uint32_t addr, uint32_t *val);
    vatek_result vatek_hms_write_hal(Phms_handle handle, uint32_t addr, uint32_t val);
    vatek_result vatek_hms_read_halbuf(Phms_handle handle, uint32_t addr, uint8_t* pbuf, uint32_t len);
    vatek_result vatek_hms_write_halbuf(Phms_handle handle, uint32_t addr, uint8_t* pbuf, uint32_t len);
    vatek_result vatek_hms_issystemidle(Phms_handle handle);
    vatek_result vatek_hms_issystemrun(Phms_handle handle);
		
		/*v1 control address i2c*/
		vatek_result vatek_hms_read_hal_v1(Phms_handle handle, uint32_t addr, uint32_t *val);
		vatek_result vatek_hms_write_hal_v1(Phms_handle handle, uint32_t addr, uint32_t val);
		vatek_result vatek_hms_read_halbuf_v1(Phms_handle handle, uint32_t addr, uint8_t* pbuf, uint32_t len);
		vatek_result vatek_hms_write_halbuf_v1(Phms_handle handle, uint32_t addr, uint8_t* pbuf, uint32_t len);
		vatek_result vatek_hms_issystemidle_v1(Phms_handle handle);
		vatek_result vatek_hms_issystemrun_v1(Phms_handle handle);
    
		
#if	defined(DEBUG)
    vatek_result vatek_hms_read_reg(Phms_handle handle, uint32_t addr, uint32_t *val);
    vatek_result vatek_hms_write_reg(Phms_handle handle, uint32_t addr, uint32_t val);
		
		vatek_result vatek_hms_read_reg_v1(Phms_handle handle, uint32_t addr, uint32_t *val);
    vatek_result vatek_hms_write_reg_v1(Phms_handle handle, uint32_t addr, uint32_t val);
#endif		

#ifdef __cplusplus
    }
#endif

#endif

