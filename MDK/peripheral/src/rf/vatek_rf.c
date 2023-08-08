
#include "vatek_system.h"

#if defined(RF_R2_VIA_VATEK)
#include "rf/rf_r2_via_vatek.h"
#endif

#if defined(RF_R2)
#include "rf/rf_r2.h"
#endif

typedef struct _hrf
{
    rf_type type;
    Pvatek_i2c vi2c;
    void *hdriver;
}hrf, *Phrf;

vatek_result vatek_rf_create(Pboard_handle hboard, rf_type type, Phrf *handle)
{
    vatek_result result = vatek_result_unknown;
    Phrf newrf = NULL;

    if (hboard == NULL || handle == NULL)
        return vatek_result_invalidparm;

    if (type > rf_type_max /*|| type < rf_type_min*/)
        return vatek_result_overrange;

    *handle = NULL;
    newrf = (Phrf)malloc(sizeof(hrf));
    if (newrf == NULL)
        return vatek_result_memfail;

    /*
    if ((result = vatek_system_i2c_create(hboard, &newrf->vi2c)) != vatek_result_success)
    {
        free(newrf);
        return result;
    }
    */
    
    switch (type)
    {
        #if defined(RF_R2_VIA_VATEK)
        case rf_type_r2_via_vatek:
		   //add R2 Reset		  	
			if((result = vatek_system_reset( sys_dev_type_rf_r2, hboard)) != vatek_result_success)
        		return result; 
		  	
		  	vatek_system_delay(1000);
            //r2 via vatek chipset use board i2c
            newrf->type = rf_type_r2_via_vatek;
            result = r2_viavatek_create(hboard, (Pr2_viavatek_handle *)&newrf->hdriver);
            break; 
        #endif

        #if defined(RF_R2)
        case rf_type_r2:
            //r2 use board gpio simulation spi
            newrf->type = rf_type_r2;
            result = r2_create((Pr2_board_handle)hboard, (Pr2_handle *)&newrf->hdriver);
            break; 
        #endif
        
        default:
            return vatek_result_unsupport;    
    }
    
    if (result == vatek_result_success)
        *handle = newrf;
    
    return result;
}

vatek_result vatek_rf_destroy(Phrf handle)
{
    vatek_result result = vatek_result_unknown;
  
    if (handle == NULL)
        return vatek_result_invalidparm;

    switch (handle->type)
    {
        #if defined(RF_R2_VIA_VATEK)
        case rf_type_r2_via_vatek:
            result = r2_viavatek_destroy((Pr2_viavatek_handle)handle->hdriver);
            break;
        #endif

        #if defined(RF_R2)
        case rf_type_r2:
            result = r2_destroy((Pr2_handle)handle->hdriver);
            break;
        #endif
        
        default:
            return vatek_result_unsupport;    
    }

    if (result == vatek_result_success)
    { 
        /*
        if ((result = vatek_system_i2c_destroy(handle->vi2c)) != vatek_result_success)
            return result;
        */
        free(handle);
        handle = NULL;
    }

    return result;
}

vatek_result vatek_rf_start(Phrf handle, uint32_t freq)
{
    vatek_result result = vatek_result_unknown;
  
    if (handle == NULL)
        return vatek_result_invalidparm;

    switch (handle->type)
    {
        #if defined(RF_R2_VIA_VATEK)
        case rf_type_r2_via_vatek:
            result = r2_viavatek_start((Pr2_viavatek_handle)handle->hdriver, freq);
            break;
        #endif
        
        #if defined(RF_R2)
        case rf_type_r2:
            result = r2_start((Pr2_handle)handle->hdriver, freq);
            break; 
        #endif
        
        default:
            return vatek_result_unsupport;    
    }

    return result;
}

vatek_result vatek_rf_stop(Phrf handle)
{
    vatek_result result = vatek_result_unknown;
  
    if (handle == NULL)
        return vatek_result_invalidparm;

    switch (handle->type)
    {
        #if defined(RF_R2_VIA_VATEK)
        case rf_type_r2_via_vatek:
            result = r2_viavatek_stop((Pr2_viavatek_handle)handle->hdriver);
            break;
        #endif

        #if defined(RF_R2)
        case rf_type_r2:
            result = r2_stop((Pr2_handle)handle->hdriver);
            break; 
        #endif
        
        default:
            return vatek_result_unsupport;    
    }

    return result;
}

vatek_result vatek_rf_getstatus(Phrf handle, rf_status *status)
{
    vatek_result result = vatek_result_unknown;
  
    if (handle == NULL)
        return vatek_result_invalidparm;

    switch (handle->type)
    {
        #if defined(RF_R2_VIA_VATEK)
        case rf_type_r2_via_vatek:
            result = r2_viavatek_getstatus((Pr2_viavatek_handle)handle->hdriver, status);
            break;
        #endif

        #if defined(RF_R2)
        case rf_type_r2:
            result = r2_getstatus((Pr2_handle)handle->hdriver, status);
            break;
        #endif
        
        default:
            return vatek_result_unsupport;    
    }

    return result;
}

vatek_result vatek_rf_setcalibrate(Phrf handle, Prf_calibrate parm)
{
    vatek_result result = vatek_result_unknown;
  
    if (handle == NULL)
        return vatek_result_invalidparm;

    switch (handle->type)
    {
        #if defined(RF_R2_VIA_VATEK)
        case rf_type_r2_via_vatek:
            result = r2_viavatek_setcalibrate((Pr2_viavatek_handle)handle->hdriver, (Pr2_calibrate)parm);
            break;
        #endif

        #if defined(RF_R2)
        case rf_type_r2:
            result = r2_setcalibrate((Pr2_handle)handle->hdriver, (Pr2_calibrate)parm);
            break;
        #endif
        
        default:
            return vatek_result_unsupport;    
    }

    return result;
}

vatek_result vatek_rf_savecalibrate(Phrf handle, Prf_calibrate parm)
{
    vatek_result result = vatek_result_unknown;
  
    if (handle == NULL)
        return vatek_result_invalidparm;

    switch (handle->type)
    {
        #if defined(RF_R2_VIA_VATEK)
        case rf_type_r2_via_vatek:
            result = r2_viavatek_savecalibrate((Pr2_viavatek_handle)handle->hdriver);
            break;
        #endif
        
        default:
            return vatek_result_unsupport;    
    }

    return result;
}

vatek_result vatek_device_r2_apply(Phrf handle, int r2_power)
{
    vatek_result result = vatek_result_unknown;
  	
	
    if (handle == NULL)
        return vatek_result_invalidparm;

    switch (handle->type)
    {
        #if defined(RF_R2_VIA_VATEK)
        case rf_type_r2_via_vatek:
            //result = rfmixer_r2_adjust_pagain((Pr2_viavatek_handle)handle->hdriver, r2_power);
            break;
        #endif
        
        default:
            return vatek_result_unsupport;    
    }

    return result;
}


