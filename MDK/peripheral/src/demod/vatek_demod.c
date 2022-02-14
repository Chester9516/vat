
#include "vatek_system.h"

#if defined(DEMOD_AVL68XX)
#include "demod/demod_AVL68XX.h"
#endif
#if defined(DEMOD_LGDT3305)
#include "demod/demod_LGDT3305.h"
#endif

typedef struct _hdemod
{
    demod_type type;
    Pvatek_i2c vi2c;
    void *hdriver;
}hdemod, *Phdemod;

vatek_result vatek_demod_create(Pboard_handle hboard, demod_type type, Phdemod *handle)
{
    vatek_result result = vatek_result_unknown;
    Phdemod newdemod = NULL;

    if (hboard == NULL || handle == NULL)
        return vatek_result_invalidparm;

    if (type > demod_type_max || type < demod_type_min)
        return vatek_result_overrange;

    *handle = NULL;
    newdemod = (Phdemod)malloc(sizeof(hdemod));
    if (newdemod == NULL)
        return vatek_result_memfail;

    if ((result = vatek_system_i2c_create(hboard, &newdemod->vi2c)) != vatek_result_success)
    {
        free(newdemod);
        return result;
    }

    switch (type)
    {
        #if defined(DEMOD_AVL68XX)
        case demod_type_avl68xx:
            newdemod->type = demod_type_avl68xx;
            result = avl68xx_create(newdemod->vi2c, (Pavl68xx_handle *)&newdemod->hdriver);
            break; 
        #endif

        #if defined(DEMOD_LGDT3305)
        case demod_type_lgdt3305:
            newdemod->type = demod_type_lgdt3305;
            result = lgdt3305_create(newdemod->vi2c, (Plgdt3305_handle *)&newdemod->hdriver);
            break; 
        #endif

        default:
            return vatek_result_unsupport;    
    }
    
    if (result == vatek_result_success)
        *handle = newdemod;
    else
        free(newdemod);
    
    return result;
}

vatek_result vatek_demod_destroy(Phdemod handle)
{
    vatek_result result = vatek_result_unknown;
  
    if (handle == NULL)
        return vatek_result_invalidparm;

    switch (handle->type)
    {
        #if defined(DEMOD_AVL68XX)
        case demod_type_avl68xx:
            result = avl68xx_destroy((Pavl68xx_handle)handle->hdriver);
            break;
        #endif
        
        #if defined(DEMOD_LGDT3305)
        case demod_type_lgdt3305:
            result = lgdt3305_destroy((Plgdt3305_handle)handle->hdriver);
            break; 
        #endif
        
        default:
            return vatek_result_unsupport;    
    }

    if (result == vatek_result_success)
    { 
        if ((result = vatek_system_i2c_destroy(handle->vi2c)) != vatek_result_success)
            return result;
        
        free(handle);
        handle = NULL;
    }

    return result;
}

vatek_result vatek_demod_i2cbypass(Phdemod handle, uint8_t bypasson)
{
    vatek_result result = vatek_result_unknown;
  
    if (handle == NULL)
        return vatek_result_invalidparm;

    switch (handle->type)
    {
        #if defined(DEMOD_AVL68XX)
        case demod_type_avl68xx:
            result = avl68xx_i2cbypass((Pavl68xx_handle)handle->hdriver, bypasson);
            break;
        #endif
        
        #if defined(DEMOD_LGDT3305)
        case demod_type_lgdt3305:
            result = lgdt3305_i2cbypass((Plgdt3305_handle)handle->hdriver, bypasson);
            break; 
        #endif
        
        default:
            return vatek_result_unsupport;    
    }

    return result;
}

vatek_result vatek_demod_lock(Phdemod handle, Pdemod_lockparm parm)
{
    vatek_result result = vatek_result_unknown;
  
    if (handle == NULL || parm == NULL)
        return vatek_result_invalidparm;

    switch (handle->type)
    {
        #if defined(DEMOD_AVL68XX)
        case demod_type_avl68xx:
            result = avl68xx_lock((Pavl68xx_handle)handle->hdriver, parm);
            break;
        #endif

        #if defined(DEMOD_LGDT3305)
        case demod_type_lgdt3305:
            result = lgdt3305_lock((Plgdt3305_handle)handle->hdriver, parm);
            break; 
        #endif
        
        default:
            return vatek_result_unsupport;    
    }
    
    return result;
}

vatek_result vatek_demod_getstatus(Phdemod handle, demod_status *status)
{
    vatek_result result = vatek_result_unknown;
  
    if (handle == NULL || status == NULL)
        return vatek_result_invalidparm;

    switch (handle->type)
    {
        #if defined(DEMOD_AVL68XX)
        case demod_type_avl68xx:
            result = avl68xx_getstatus((Pavl68xx_handle)handle->hdriver, status);
            break;
        #endif
            
        #if defined(DEMOD_LGDT3305)
        case demod_type_lgdt3305:
            result = lgdt3305_getstatus((Plgdt3305_handle)handle->hdriver, status);
            break; 
        #endif
        
        default:
            return vatek_result_unsupport;    
    }
    
    return result;
}

