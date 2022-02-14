
#ifndef _RF_R2_VIA_VATEK_
#define _RF_R2_VIA_VATEK_

#ifdef __cplusplus
    extern "C" {
#endif

    typedef void* Pr2_viavatek_handle;
    vatek_result r2_viavatek_create(Pboard_handle hboard, Pr2_viavatek_handle *hr2);
    vatek_result r2_viavatek_destroy(Pr2_viavatek_handle hr2);
    vatek_result r2_viavatek_start(Pr2_viavatek_handle hr2, uint32_t freq);
    vatek_result r2_viavatek_stop(Pr2_viavatek_handle hr2);
    vatek_result r2_viavatek_getstatus(Pr2_viavatek_handle hr2, rf_status *status);
    vatek_result r2_viavatek_setcalibrate(Pr2_viavatek_handle hr2, Pr2_calibrate parm);

#ifdef __cplusplus
    }
#endif

#endif

