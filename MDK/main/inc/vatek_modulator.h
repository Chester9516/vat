
#ifndef _VATEK_MODULATOR_
#define _VATEK_MODULATOR_

#ifdef __cplusplus
    extern "C" {
#endif

    vatek_result vatek_modulator_setparm(Phms_handle handle, modulator_base_parm parm_base, Pmodulator_advanced_parm parm_adv);

    #define MOD_ERR(fmt,...)    VATEK_ERR(mod,fmt,##__VA_ARGS__)
    #define MOD_LOG(fmt,...)    VATEK_LOG(mod,fmt,##__VA_ARGS__)

#ifdef __cplusplus
    }
#endif

#endif

