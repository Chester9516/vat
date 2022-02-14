
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vatek_sinewave_mode.h"


static Phbroadcast bc_handle = NULL;
static Phrf rf_handle = NULL;


vatek_result sinewave_mode_start(void)
{ 
    vatek_result result = vatek_result_unknown;
    
    modulator_base_parm mod_base_parm = {0};
    modulator_sinewave_parm sinewave_parm = {0};
    
    mod_base_parm.type = m_type_sinewave;
    sinewave_parm.freq = 100;    
    
    result = vatek_broadcast_modulator_setparm( bc_handle, mod_base_parm, (Pmodulator_advanced_parm)&sinewave_parm);
    if (result != vatek_result_success)
    {
        SAMPLE_ERR("sinewave mode fail: %d", result);
        return result;
    }

    /* start RF */
    result = vatek_rf_start(rf_handle, 473000);
    if (result != vatek_result_success)
    {
        SAMPLE_ERR("rf start fail: %d", result);
        return result;
    }
    
    /* start BROADCAST */
    result = vatek_broadcast_start(bc_handle);
    if (result != vatek_result_success)
    {
        SAMPLE_ERR("broadcast start fail: %d", result);
        return result;
    }

    return result;
}


vatek_result sinewave_mode_init(Pboard_handle bh_main, Pboard_handle bh_rf)
{
    vatek_result result = vatek_result_unknown;

    /* init VATEK broadcast */
    result = vatek_broadcast_create(bh_main, &bc_handle);
    if (result != vatek_result_success)
    {
        SAMPLE_ERR("broadcast create fail : %d", result);
        return result;
    }
    
    /* init RF : r2_via_vatek */
    result = vatek_rf_create(bh_rf, rf_type_r2_via_vatek, &rf_handle);  
    if (result != vatek_result_success)
    {
        SAMPLE_ERR("rf create fail : %d", result);
        return result;
    }
    
    /* broadcast from logo */
    result = sinewave_mode_start();
    if (result != vatek_result_success)
    {
        SAMPLE_ERR("sinewave fail: %d", result);
        return result;
    }

    return result;
}
