
#if defined(RF_R2_VIA_VATEK)

#include "vatek_system.h"
#include "vatek_hms.h"

typedef struct _r2_viavatek_handle
{
    Phms_handle hms;
}r2_viavatek_handle,*Pr2_viavatek_handle;

#define R2_VIA_VATEK_ERR(fmt,...)    VATEK_ERR(r2,fmt,##__VA_ARGS__)
#define R2_VIA_VATEK_LOG(fmt,...)    VATEK_LOG(r2,fmt,##__VA_ARGS__)

static vatek_result r2_viavatek_ctrl(Phms_handle handle, uint32_t cmd, uint32_t *errcode)
{
    vatek_result result = vatek_result_unknown;
    uint32_t timeout = 0, mod = 0;

    if (handle == NULL)
        return vatek_result_invalidparm;
    
    if((result = vatek_hms_read_hal(handle, HALREG_MOD_MODE, &mod)) == vatek_result_success)
    {
        // modulator j83b, i-q swap.
        if(mod == MOD_J83B)
            result = vatek_hms_write_hal(handle, HALREG_R2_EN_CNTL, R2_EN_PATH_1);
        else result = vatek_hms_write_hal(handle, HALREG_R2_EN_CNTL, R2_EN_PATH_0);
        
        if(result != vatek_result_success) return result;
    }
        
    if ((result = vatek_hms_write_hal(handle, HALREG_BROADCAST_CNTL, cmd)) == vatek_result_success)
    {
        while (timeout < 100) //10 sec
        {
            vatek_system_delay(100);
            if ((result = vatek_hms_read_hal(handle, HALREG_BROADCAST_CNTL, errcode)) != vatek_result_success)
                return result;
            if (*errcode == 0)
                break;
            timeout++;
        }

        if (timeout >= 100)
            result = vatek_result_timeout;
        else
        {
            if ((result = vatek_hms_read_hal(handle, HALREG_SYS_ERRCODE, errcode)) != vatek_result_success)
                return result;

            if (*errcode != HALREG_ERR_SUCCESS)
                result = vatek_result_hwfail;
        }
    }

    return result;
}

vatek_result r2_viavatek_getstatus(Pr2_viavatek_handle hr2, rf_status *status)
{
    vatek_result result = vatek_result_unknown;
    uint32_t value = 0;
  
    if (hr2 == NULL)
        return vatek_result_invalidparm;

    if ((result = vatek_hms_read_hal(hr2->hms, HALREG_RF_COM_STATUS, &value)) != vatek_result_success)
        return result;

    switch (value)
    {
        case RF_STATUS_IDLE:
            *status = rf_status_idle;  
            break;

        case RF_STATUS_ACTIVE:
            *status = rf_status_active;
            break;

        case RF_STATUS_FAIL:
        default:
            *status = rf_status_fail;
            break;
    }

    return result;
}

vatek_result r2_viavatek_create(Pboard_handle hboard, Pr2_viavatek_handle *hr2)
{
    vatek_result result = vatek_result_unknown;
    Pr2_viavatek_handle newr2 = NULL; 
    
    if (hboard == NULL || hr2 == NULL)
        return vatek_result_invalidparm;

    newr2 = (Pr2_viavatek_handle)malloc(sizeof(r2_viavatek_handle));
    if (newr2 == NULL)
        return vatek_result_memfail;

    if ((result = vatek_hms_create(hboard, &newr2->hms)) != vatek_result_success)
        return result;

    newr2->hms->type = hms_type_r2;
    *hr2 = newr2;
    
    return result;
}

vatek_result r2_viavatek_destroy(Pr2_viavatek_handle hr2)
{
    vatek_result result = vatek_result_success;

    if (hr2 == NULL)
        return vatek_result_invalidparm;

    if ((result = vatek_hms_destroy(hr2->hms)) != vatek_result_success)
        return result;

    free(hr2);
    hr2 = NULL;
    
    return result;
}

vatek_result r2_viavatek_start(Pr2_viavatek_handle hr2, uint32_t freq)
{
    vatek_result result = vatek_result_unknown;
    rf_status status = rf_status_unknown;
    uint32_t errcode = 0;
        
    if (hr2 == NULL)
        return vatek_result_invalidparm;

    if ((result = r2_viavatek_getstatus(hr2, &status)) != vatek_result_success)
        return result;

    if (status != rf_status_idle)
        return vatek_result_badstatus;
    
    
    if ((result = vatek_hms_write_hal(hr2->hms, HALREG_RF_COM_FREQ, freq)) != vatek_result_success)
        return result;

    if ((result = r2_viavatek_ctrl(hr2->hms, BC_RFMIXER_START, &errcode)) != vatek_result_success)
    {
        R2_VIA_VATEK_ERR("BC_RFMIXER_START fail, errcode = 0x%lX", errcode);
        return result;
    }
    if ((result = r2_viavatek_getstatus(hr2, &status)) != vatek_result_success)
        return result;
    
    return result;
}

vatek_result r2_viavatek_stop(Pr2_viavatek_handle hr2)
{
    vatek_result result = vatek_result_unknown;
    rf_status status = rf_status_unknown;
    uint32_t errcode = 0;
    
    if (hr2 == NULL)
        return vatek_result_invalidparm;

    if ((result = r2_viavatek_getstatus(hr2, &status)) != vatek_result_success)
        return result;

    if (status != rf_status_active)
        return vatek_result_badstatus;

    if ((result = r2_viavatek_ctrl(hr2->hms, BC_RFMIXER_STOP, &errcode)) != vatek_result_success)
        return result;
    
    return result;
}

vatek_result r2_viavatek_setcalibrate(Pr2_viavatek_handle hr2, Pr2_calibrate parm)
{
    vatek_result result = vatek_result_unknown;
    rf_status status = rf_status_unknown;

    if (hr2 == NULL || parm == NULL)
        return vatek_result_invalidparm;

    if ((result = r2_viavatek_getstatus(hr2, &status)) != vatek_result_success)
        return result;

    if (status != rf_status_idle)
        return vatek_result_badstatus;
    
    if ((result = vatek_hms_write_hal(hr2->hms, HALREG_R2_EN_CNTL, R2_EN_PARAM)) != vatek_result_success)
        return result;

    if (parm->i_offset)
    {
        if ((result = vatek_hms_write_hal(hr2->hms, HALREG_R2_PARAM_I, parm->i_offset)) != vatek_result_success)
            return result;
    }
    
    if (parm->q_offset)
    {
        if ((result = vatek_hms_write_hal(hr2->hms, HALREG_R2_PARAM_Q, parm->q_offset)) != vatek_result_success)
            return result;
    }

    if (parm->image_offset)
    {
        if ((result = vatek_hms_write_hal(hr2->hms, HALREG_R2_PARAM_IMAGE, parm->image_offset)) != vatek_result_success)
            return result;
    }

    if (parm->phase_offset)
    {
        if ((result = vatek_hms_write_hal(hr2->hms, HALREG_R2_PARAM_PHASE, parm->phase_offset)) != vatek_result_success)
            return result;
    }

    if (parm->gpio)
    {
        if ((result = vatek_hms_write_hal(hr2->hms, HALREG_R2_PARAM_GPIO, parm->gpio)) != vatek_result_success)
            return result;
    }

    if (parm->gain)
    {
        if ((result = vatek_hms_write_hal(hr2->hms, HALREG_R2_PARAM_GAIN, parm->gain)) != vatek_result_success)
            return result;
    }

    return result;
}

#endif

