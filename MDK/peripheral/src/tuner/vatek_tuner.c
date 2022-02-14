
#include "vatek_system.h"

#if defined(TUNER_MXL608)
#include "tuner/tuner_MxL608.h"
#endif
#if defined(TUNER_MXL603)
#include "tuner/tuner_MxL603.h"
#endif


typedef struct _htuner
{
    tuner_type type;
    Pvatek_i2c vi2c;
    void *hdriver;
    void *hdemod;
    fpdemod_i2cbypass i2cbypass;
}htuner, *Phtuner;

static vatek_result demod_i2c_bypass(Phtuner handle, uint8_t bypass)
{
    vatek_result result = vatek_result_unknown;

    if (handle == NULL)
        return vatek_result_invalidparm;

    if (handle->i2cbypass == NULL || handle->hdemod == NULL)
        return vatek_result_success; //no i2c bypaas function

    result = handle->i2cbypass(handle->hdemod, bypass);
    if (result != vatek_result_success)
    {
        VATEK_ERR(tuner,"demod i2c bypass error !");
    }
    
    return result;
}

vatek_result vatek_tuner_create(Ptuner_board_handle tunerboard, tuner_type type, Phtuner *handle)
{
    vatek_result result = vatek_result_unknown;
    Phtuner newtuner = NULL;

    if (tunerboard == NULL || handle == NULL)
        return vatek_result_invalidparm;

    if (type > tuner_type_max || type < tuner_type_min)
        return vatek_result_overrange;

    *handle = NULL;
    newtuner = (Phtuner)malloc(sizeof(htuner));
    if (newtuner == NULL)
        return vatek_result_memfail;

    newtuner->hdemod = tunerboard->hdemod;
    newtuner->i2cbypass = tunerboard->i2cbypass;
    if ((result = vatek_system_i2c_create(tunerboard->hboard, &newtuner->vi2c)) != vatek_result_success)
    {
        free(newtuner);
        return result;
    }
    
    demod_i2c_bypass(newtuner, 1);
    switch (type)
    {
        #if defined(TUNER_MXL608)
        case tuner_type_mxl608:
            newtuner->type = tuner_type_mxl608;
            result = mxl608_create(newtuner->vi2c, (Pmxl608_handle *)&newtuner->hdriver);
            break; 
        #endif
        
        #if defined(TUNER_MXL603)
        case tuner_type_mxl603:
            newtuner->type = tuner_type_mxl603;
            result = mxl603_create(newtuner->vi2c, (Pmxl603_handle *)&newtuner->hdriver);
            break; 
        #endif

        default:
            return vatek_result_unsupport;    
    }
    demod_i2c_bypass(newtuner, 0);

    if (result != vatek_result_success)
    {
        free(newtuner);
        return result;
    }

    *handle = newtuner;

    return result;
}

vatek_result vatek_tuner_destroy(Phtuner handle)
{
    vatek_result result = vatek_result_unknown;
  
    if (handle == NULL)
        return vatek_result_invalidparm;

    demod_i2c_bypass(handle, 1);
    switch (handle->type)
    {
        #if defined(TUNER_MXL608)
        case tuner_type_mxl608:
            result = mxl608_destroy((Pmxl608_handle)handle->hdriver);
            break;
        #endif

        #if defined(TUNER_MXL603)
        case tuner_type_mxl603:
            result = mxl603_destroy((Pmxl603_handle)handle->hdriver);
            break;
        #endif
        
        default:
            return vatek_result_unsupport;    
    }
    demod_i2c_bypass(handle, 0);

    if (result == vatek_result_success)
    { 
        if ((result = vatek_system_i2c_destroy(handle->vi2c)) != vatek_result_success)
            return result;
        
        free(handle);
        handle = NULL;
    }

    return result;
}

vatek_result vatek_tuner_lock(Phtuner handle, Ptuner_lockparm parm)
{
    vatek_result result = vatek_result_unknown;
  
    if (handle == NULL || parm == NULL)
        return vatek_result_invalidparm;
    
    demod_i2c_bypass(handle, 1);
    switch (handle->type)
    {
        #if defined(TUNER_MXL608)
        case tuner_type_mxl608:
            result = mxl608_lock((Pmxl608_handle)handle->hdriver, parm);
            break;
        #endif
        
        #if defined(TUNER_MXL603)
        case tuner_type_mxl603:
            result = mxl603_lock((Pmxl603_handle)handle->hdriver, parm);
            break;
        #endif
        
        default:
            return vatek_result_unsupport;    
    }
    demod_i2c_bypass(handle, 0);
    
    return result;
}

vatek_result vatek_tuner_getstatus(Phtuner handle, tuner_status *status)
{
    vatek_result result = vatek_result_unknown;
  
    if (handle == NULL || status == NULL)
        return vatek_result_invalidparm;

    demod_i2c_bypass(handle, 1);
    switch (handle->type)
    {
        #if defined(TUNER_MXL608)
        case tuner_type_mxl608:
            result = mxl608_getstatus((Pmxl608_handle)handle->hdriver, status);
            break;
        #endif
        
        #if defined(TUNER_MXL603)
        case tuner_type_mxl603:
            result = mxl603_getstatus((Pmxl603_handle)handle->hdriver, status);
            break;
        #endif
        
        default:
            return vatek_result_unsupport;    
    }
    demod_i2c_bypass(handle, 0);
    
    return result;
}

vatek_result vatek_tuner_getrfstrength(Phtuner handle, int32_t *power)
{
    vatek_result result = vatek_result_unknown;
  
    if (handle == NULL || power == NULL)
        return vatek_result_invalidparm;

    demod_i2c_bypass(handle, 1);
    switch (handle->type)
    {
        #if defined(TUNER_MXL608)
        case tuner_type_mxl608:
            result = mxl608_getrfstrength((Pmxl608_handle)handle->hdriver, power);
            break;
        #endif
        
        #if defined(TUNER_MXL603)
        case tuner_type_mxl603:
            result = mxl603_getrfstrength((Pmxl603_handle)handle->hdriver, power);
            break;
        #endif
        
        default:
            return vatek_result_unsupport;    
    }
    demod_i2c_bypass(handle, 0);
    
    return result;
}

