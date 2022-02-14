
#ifndef _VATEK_PSISPEC_
#define _VATEK_PSISPEC_

#ifdef __cplusplus
    extern "C" {
#endif

    /* property rule table function */
    vatek_result vatek_psispec_rule_deinit(void);
    vatek_result vatek_psispec_rule_init(Ppsispec_rule rule);
    vatek_result vatek_psispec_rule_setruletable(const char *name, uint32_t version, uint32_t interval);
    vatek_result vatek_psispec_rule_setchannelprop(const char *name, void* buf, uint32_t len);
    vatek_result vatek_psispec_rule_setprogramprop_psip(Ppsip_props props);

    /* control function */
    vatek_result vatek_psispec_rule_start(Phms_handle handle);
    vatek_result vatek_psispec_rule_end(Phms_handle handle);
    vatek_result vatek_psispec_rule_start_v2(Phms_handle handle);
    vatek_result vatek_psispec_rule_end_v2(Phms_handle handle);
    vatek_result vatek_psispec_rule_addprogram(Phms_handle handle, Penum_program program);

    
    #define PSISPEC_ERR(fmt,...)    VATEK_ERR(psispec,fmt,##__VA_ARGS__)
    #define PSISPEC_LOG(fmt,...)    VATEK_LOG(psispec,fmt,##__VA_ARGS__)

#ifdef __cplusplus
    }
#endif

#endif

