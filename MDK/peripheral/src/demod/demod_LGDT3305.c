
#if defined(DEMOD_LGDT3305)

#include "vatek_system.h"
#include "demod/LGDT3305/LGDT_Util.h"
#include "demod/LGDT3305/LGDT3305API.h"

typedef struct _lgdt3305_handle
{
    Pvatek_i2c vi2c;
}lgdt3305_handle,*Plgdt3305_handle;

#define LGDT3305_ERR(fmt,...)     VATEK_ERR(lgdt3305,fmt,##__VA_ARGS__)
#define LGDT3305_LOG(fmt,...)     VATEK_LOG(lgdt3305,fmt,##__VA_ARGS__)

vatek_result lgdt3305_create(Pvatek_i2c vi2c, Plgdt3305_handle *hlgdt3305)
{
    vatek_result result = vatek_result_unknown;
    Plgdt3305_handle newlgdt3305 = NULL; 
    LgdtReturn_t lgdtret = LGDT_ERR_NO;
    
    if (vi2c == NULL || hlgdt3305 == NULL)
        return vatek_result_invalidparm;

    newlgdt3305 = (Plgdt3305_handle)malloc(sizeof(lgdt3305_handle));
    if (newlgdt3305 == NULL)
        return vatek_result_memfail;
    
    LGDT3305_LOG("reset lgdt3305");

    if ((result = vatek_system_reset(sys_dev_type_demod_lgdt3305, vi2c->hboard)) != vatek_result_success)
        return result;
    
    vatek_system_delay(2000); // wait lgdt3305 reboot
    
    LGDT3305_LOG("init lgdt3305");
    
    newlgdt3305->vi2c = vi2c;

    if (Lgdt_SetActiveI2C(newlgdt3305->vi2c) != 0)
        return vatek_result_hwfail;
    
    lgdtret = LgdtInitFAT(LGDT_VSB, false);
    if (lgdtret != LGDT_ERR_NO)
    {
        LGDT3305_ERR("LgdtInitFAT fail : 0x%X", lgdtret);
        return vatek_result_hwfail;
    }

    *hlgdt3305 = newlgdt3305;
    LGDT3305_LOG("init lgdt3305 ok");
    return result;
}

vatek_result lgdt3305_destroy(Plgdt3305_handle hlgdt3305)
{
    vatek_result result = vatek_result_success;

    if (hlgdt3305 == NULL)
        return vatek_result_invalidparm;
    
    free(hlgdt3305);
    hlgdt3305 = NULL;    
    
    return result;
}

vatek_result lgdt3305_i2cbypass(Plgdt3305_handle hlgdt3305, uint8_t bypasson)
{
    vatek_result result = vatek_result_success;
    LgdtReturn_t lgdtret = LGDT_ERR_NO;

    if (hlgdt3305 == NULL)
        return vatek_result_invalidparm;
    
    if (Lgdt_SetActiveI2C(hlgdt3305->vi2c) != 0)
        return vatek_result_hwfail;
    
    lgdtret = LgdtRepeaterI2C((bool)bypasson);
    if (lgdtret != LGDT_ERR_NO)
    {
        LGDT3305_ERR("LgdtRepeaterI2C fail : %d", lgdtret);
        return vatek_result_hwfail;
    }
    
    return result;
}

vatek_result lgdt3305_lock(Plgdt3305_handle hlgdt3305, Pdemod_lockparm parm)
{
    vatek_result result = vatek_result_success;
    LgdtOperMode_t modeOper = LGDT_VSB;
    LgdtReturn_t lgdtret = LGDT_ERR_NO;

    if (hlgdt3305 == NULL)
        return vatek_result_invalidparm; 

    if (Lgdt_SetActiveI2C(hlgdt3305->vi2c) != 0)
            return vatek_result_hwfail;

    switch (parm->type)
    {
        case dm_type_atsc:
            modeOper = LGDT_VSB;
            break;
        default:
            return vatek_result_unsupport;          
    }

    lgdtret = LgdtInitFAT(modeOper, false);
    if (lgdtret != LGDT_ERR_NO)
    {
        LGDT3305_ERR("LgdtInitFAT fail : 0x%X", lgdtret);
        return vatek_result_hwfail;
    }

    lgdtret = LgdtAlterMpegMode(LGDT_PARAL);
    if (lgdtret != LGDT_ERR_NO)
    {
        LGDT3305_ERR("LgdtAlterMpegMode fail : %d", lgdtret);
        return vatek_result_hwfail;
    }

    lgdtret = LgdtAlterMpegPolarity(1, 1);
    if (lgdtret != LGDT_ERR_NO)
    {
        LGDT3305_ERR("LgdtAlterMpegPolarity fail : %d", lgdtret);
        return vatek_result_hwfail;
    }
    
    return result;
}

vatek_result lgdt3305_getstatus(Plgdt3305_handle hlgdt3305, demod_status *status)
{
    if (hlgdt3305== NULL || status == NULL)
        return vatek_result_invalidparm;

    if (Lgdt_SetActiveI2C(hlgdt3305->vi2c) != 0)
        return vatek_result_hwfail;

    *status = demod_status_unknown;
    
    if (lg3305_lock_check())
        *status = demod_status_lock;
    else
        *status = demod_status_unlock;

    return vatek_result_success; 
}

vatek_result lgdt3305_getSNR(Plgdt3305_handle hlgdt3305, uint32_t *SNR)
{
    LgdtReturn_t lgdtret = LGDT_ERR_NO;
    DATA_DOUBLE nSNR = 0;
    
    if (hlgdt3305== NULL || SNR == NULL)
        return vatek_result_invalidparm;
    
    lgdtret = LgdtCheckSnrFAT(&nSNR);
    if (lgdtret != LGDT_ERR_NO)
    {
        LGDT3305_ERR("LgdtCheckSnrFAT fail : %d", lgdtret);
        return vatek_result_hwfail;   
    }

    *SNR = (uint32_t)nSNR;
    
    return vatek_result_success;
}

vatek_result lgdt3305_getPER(Plgdt3305_handle hlgdt3305, uint32_t *PER)
{
    LgdtReturn_t lgdtret = LGDT_ERR_NO;
    DATA16 nErr = 0;
    
    if (hlgdt3305== NULL || PER == NULL)
        return vatek_result_invalidparm;

    lgdtret = LgdtCheckPacketErr(&nErr);
    if (lgdtret != LGDT_ERR_NO)
    {
        LGDT3305_ERR("LgdtCheckPacketErr fail : %d", lgdtret);
        return vatek_result_hwfail;   
    } 

    *PER = nErr;

    return vatek_result_success;
}

#endif

