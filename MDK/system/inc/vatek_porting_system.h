
#ifndef _VATEK_PORTING_SYSTEM_
#define _VATEK_PORTING_SYSTEM_

#ifdef __cplusplus
    extern "C" {
#endif

    uint32_t vatek_porting_system_gettick(void);
    vatek_result vatek_porting_system_delay(uint32_t msec);
    vatek_result vatek_porting_system_mainreset(Pboard_handle hboard);
    vatek_result vatek_porting_system_phyreset(Pboard_handle hboard, phy_type type);
    vatek_result vatek_porting_system_rfreset(Pboard_handle hboard, rf_type type);
    vatek_result vatek_porting_system_tunerreset(Pboard_handle hboard, tuner_type type);
    vatek_result vatek_porting_system_demodreset(Pboard_handle hboard, demod_type type);

#ifdef __cplusplus
    }
#endif
    
#endif

