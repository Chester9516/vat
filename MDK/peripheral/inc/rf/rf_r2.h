
#ifndef _RF_R2_
#define _RF_R2_

#ifdef __cplusplus
    extern "C" {
#endif

    typedef void* Pr2_handle;
    vatek_result r2_create(Pr2_board_handle hboard, Pr2_handle *hr2);
    vatek_result r2_destroy(Pr2_handle hr2);
    vatek_result r2_start(Pr2_handle hr2, uint32_t freq);
    vatek_result r2_stop(Pr2_handle hr2);
    vatek_result r2_getstatus(Pr2_handle hr2, rf_status *status);
    vatek_result r2_setcalibrate(Pr2_handle hr2, Pr2_calibrate parm);

#ifdef __cplusplus
    }
#endif

#endif

