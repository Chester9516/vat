
#include "vatek_system.h"
#include "vatek_hms.h"
#include "vatek_tsp.h"

vatek_result vatek_tsp_setinputparm_ts(Phms_handle handle, ts_input_parm parm)
{
    vatek_result result = vatek_result_unknown;
    uint32_t val = 0;

    if (handle == NULL)
        return vatek_result_invalidparm;

    if ((result = vatek_hms_issystemidle(handle)) != vatek_result_success)
        return result;
    
    if (parm.serial)
        val = TS_PIN_SERIAL;

    if (parm.clk_inverse)
        val |= TS_PIN_NEGATIVE_CLK;

    if (parm.valid_inverse)
        val |= TS_PIN_NEGATIVE_VALID;

    if (parm.sync_inverse)
        val |= TS_PIN_NEGATIVE_SYNC;
        
    if ((result = vatek_hms_write_hal(handle, HALREG_TSIN_FLAGS, val)) != vatek_result_success)
        return result;

    if ((result = vatek_hms_write_hal(handle, HALREG_TSIN_MODE, STREAM_MODE_REMUX)) != vatek_result_success)
        return result;

    if ((result = vatek_hms_write_hal(handle, HALREG_TRANSFORM_INPUT, STREAM_TSIN)) != vatek_result_success)
        return result;
    
    val = 0;
    if(parm.pcr_mode == pcr_disable)
        val = PCR_MODE_DISABLE;
    else if(parm.pcr_mode == pcr_adjust)
        val = PCR_MODE_ADJUST;
    else if(parm.pcr_mode == pcr_retag)
        val = PCR_MODE_RETAG;
    if((result = vatek_hms_write_hal(handle, HALREG_TSIN_PCR_MODE, val)) != vatek_result_success)
        return result;

    return result;
}

vatek_result vatek_tsp_setfilterparm(Phms_handle handle, tsp_filter_parm parm)
{
    vatek_result result = vatek_result_unknown;
    uint32_t val = 0;
    uint16_t idx = 0;
    uint32_t filter_addr = HALREG_MUX_FILTER_0;

    if (handle == NULL || parm.filter_num == 0)
        return vatek_result_invalidparm;

    if (parm.filter_num >= MUX_FILTER_NAX_NUMS)
        parm.filter_num = MUX_FILTER_NAX_NUMS;

    val = MUX_FILTER_EN_TAG | (MUX_FILTER_MASK & parm.filter_num);
    if ((result = vatek_hms_write_hal(handle, HALREG_MUX_FILTER_EN, val)) != vatek_result_success)
        return result;
 
    for (idx = 0; idx < parm.filter_num; idx++)
    {
        if (parm.filter[idx].replace)
            val = FILTER_REPLACE(parm.filter[idx].pid, parm.filter[idx].replace_pid);
        else
            val = FILTER_NO_REPLACE(parm.filter[idx].pid);

        if ((result = vatek_hms_write_hal(handle, filter_addr, val)) != vatek_result_success)
            return result;
        filter_addr++;
    }
    
    return result;
}

