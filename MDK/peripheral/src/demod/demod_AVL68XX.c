
#if defined(DEMOD_AVL68XX)

#include "vatek_system.h"
#include "demod/AVL68XX/AVL_Demod.h"

typedef struct _avl68xx_handle
{
    Pvatek_i2c vi2c;
    uint32_t chipno;
}avl68xx_handle,*Pavl68xx_handle;

#define AVL68XX_ERR(fmt,...)     VATEK_ERR(avl68xx,fmt,##__VA_ARGS__)
#define AVL68XX_LOG(fmt,...)     VATEK_LOG(avl68xx,fmt,##__VA_ARGS__)

static vatek_result initialize(uint32_t chipno)
{
    AVL_ErrorCode return_code = AVL_EC_OK;

    return_code = AVL_Demod_Initialize(AVL_DVBTX, chipno);
    if(return_code != AVL_EC_OK)
    {
        AVL68XX_ERR("AVL_Demod_Initialize Failed! : %d",return_code);
        return vatek_result_hwfail;
    }

    AVL68XX_LOG("init avl68xx ok!");

    return vatek_result_success;
}

vatek_result avl68xx_create(Pvatek_i2c vi2c, Pavl68xx_handle *havl68xx)
{
    vatek_result result = vatek_result_unknown;
    Pavl68xx_handle newavl68xx = NULL; 
    
    if (vi2c == NULL || havl68xx == NULL)
        return vatek_result_invalidparm;

    newavl68xx = (Pavl68xx_handle)malloc(sizeof(avl68xx_handle));
    if (newavl68xx == NULL)
        return vatek_result_memfail;
    
    AVL68XX_LOG("reset avl68xx");
    if ((result = vatek_system_reset(sys_dev_type_demod_avl68xx, vi2c->hboard)) != vatek_result_success)
        return result;
    
    vatek_system_delay(2000); // wait mxl608 reboot
    
    AVL68XX_LOG("init avl68xx");

    newavl68xx->vi2c = vi2c;
    newavl68xx->chipno = 0;

    if (AVL_IBSP_SetActiveI2C(newavl68xx->vi2c) != 0)
        return vatek_result_hwfail;

    if ((result = initialize(newavl68xx->chipno)) != vatek_result_success)
    {   
        AVL68XX_ERR("avl init fail");
        free(newavl68xx);
        return result;
    }
    
    *havl68xx = newavl68xx;
    
    return result;
}

vatek_result avl68xx_destroy(Pavl68xx_handle havl68xx)
{
    vatek_result result = vatek_result_success;

    if (havl68xx == NULL)
        return vatek_result_invalidparm;
    
    free(havl68xx);
    havl68xx = NULL;    
    
    return result;
}

vatek_result avl68xx_i2cbypass(Pavl68xx_handle havl68xx, uint8_t bypasson)
{
    vatek_result result = vatek_result_success;
    AVL_ErrorCode return_code = AVL_EC_OK;

    if (havl68xx == NULL)
        return vatek_result_invalidparm;

    if (AVL_IBSP_SetActiveI2C(havl68xx->vi2c) != 0)
            return vatek_result_hwfail;
    
    if (bypasson)
        return_code = AVL_Demod_I2CBypassOn(havl68xx->chipno);
    else 
        return_code = AVL_Demod_I2CBypassOff(havl68xx->chipno);

    if (return_code != AVL_EC_OK)
    {
        AVL68XX_ERR("avl i2c bypass fail : %d", return_code);
        return vatek_result_hwfail;
    }
    
    return result;
}

static vatek_result _avl68xx_lockfreq_dvbtx(Pavl68xx_handle havl68xx, Pdemod_lockparm parm)
{
    AVL_ErrorCode avl_code = AVL_EC_OK;
    AVL_DVBTxBandWidth nBand = AVL_DVBTx_BW_8M;

    if (havl68xx == NULL || parm == NULL)
        return vatek_result_invalidparm;

    if(parm->bandwidth <= 1700)
        nBand = AVL_DVBTx_BW_1M7;
    else if (parm->bandwidth > 1700 && parm->bandwidth <= 5000)
        nBand = AVL_DVBTx_BW_5M;
    else if (parm->bandwidth > 5000 && parm->bandwidth <= 6000)
        nBand = AVL_DVBTx_BW_6M;
    else if (parm->bandwidth > 6000 && parm->bandwidth <= 7000)
        nBand = AVL_DVBTx_BW_7M;
    else
        nBand = AVL_DVBTx_BW_8M;

    avl_code = AVL_Demod_DVBTxChannelScan(nBand, AVL_DVBTx_LockMode_ALL, havl68xx->chipno);
    if (avl_code != AVL_EC_OK)
    {
        AVL68XX_ERR("AVL_Demod_DVBTxChannelScan fail : %d", avl_code);
        return vatek_result_hwfail;
    }
    
    return vatek_result_success;
}

static vatek_result _avl68xx_lockfreq_dvbc(Pavl68xx_handle havl68xx)
{
    AVL_ErrorCode avl_code = AVL_EC_OK;

    if (havl68xx == NULL)
        return vatek_result_invalidparm;

    avl_code = AVL_Demod_DVBCAutoLock(havl68xx->chipno);
    if(avl_code != AVL_EC_OK)
    {
        AVL68XX_ERR("AVL_Demod_DVBCAutoLock fail : %d", avl_code);
        return vatek_result_hwfail;
    }

    return vatek_result_success;
}

static vatek_result _avl68xx_lockfreq_isdbt(Pavl68xx_handle havl68xx)
{
    AVL_ErrorCode avl_code = AVL_EC_OK;

    if (havl68xx == NULL)
        return vatek_result_invalidparm;

    avl_code = AVL_Demod_ISDBTAutoLock(havl68xx->chipno);
    if(avl_code != AVL_EC_OK)
    {
        AVL68XX_ERR("AVL_Demod_ISDBTAutoLock fail : %d", avl_code);
        return vatek_result_hwfail;
    }

    return vatek_result_success;
}

static vatek_result _avl68xx_lockfreq_dvbsx(Pavl68xx_handle havl68xx, Pdemod_lockparm parm)
{
    AVL_ErrorCode avl_code = AVL_EC_OK;

    if (havl68xx == NULL || parm == NULL)
        return vatek_result_invalidparm;

    avl_code = AVL_Demod_DVBSxAutoLock(parm->bandwidth*1000, havl68xx->chipno);
    if (avl_code != AVL_EC_OK)
    {
        AVL68XX_ERR("AVL_Demod_DVBSxAutoLock fail : %d", avl_code);
        return vatek_result_hwfail;
    }
    
    return vatek_result_success;
}

vatek_result avl68xx_lock(Pavl68xx_handle havl68xx, Pdemod_lockparm parm)
{
    vatek_result result = vatek_result_success;
    AVL_DemodMode demodmode = AVL_DVBTX;
    AVL_ErrorCode avl_code = AVL_EC_OK;
    
    if (havl68xx == NULL)
        return vatek_result_invalidparm; 
    
    if (AVL_IBSP_SetActiveI2C(havl68xx->vi2c) != 0)
        return vatek_result_hwfail;

    switch (parm->type)
    {
        case dm_type_dvbc:
            demodmode = AVL_DVBC;
            break;
        
        case dm_type_dvbtx:
            demodmode = AVL_DVBTX;
            break;
        
        case dm_type_dvbsx:
            demodmode = AVL_DVBSX;
            break;
        
        case dm_type_isdbt:
            demodmode = AVL_ISDBT;
            break;

        default:
            return vatek_result_unsupport;          
    }

    if ((avl_code = AVL_Demod_SetMode(demodmode, havl68xx->chipno)) != AVL_EC_OK)
    {
        AVL68XX_ERR("AVL_Demod_SetMode error ! : %d", avl_code);
        return vatek_result_hwfail;
    }

    switch (parm->type)
    {
        case dm_type_dvbc:
            result = _avl68xx_lockfreq_dvbc(havl68xx);
            break;
        
        case dm_type_dvbtx:
            result = _avl68xx_lockfreq_dvbtx(havl68xx, parm);
            break;
        
        case dm_type_dvbsx:
            result = _avl68xx_lockfreq_dvbsx(havl68xx, parm);
            break;
        
        case dm_type_isdbt:
            result = _avl68xx_lockfreq_isdbt(havl68xx);
            break;

        default:
            return vatek_result_unsupport;          
    }
    
    return result;
}

vatek_result avl68xx_getstatus(Pavl68xx_handle havl68xx, demod_status *status)
{
    AVL_uchar demodLocked = 0;
    AVL_ErrorCode avl_code = AVL_EC_OK;
         
    if (havl68xx== NULL || status == NULL)
        return vatek_result_invalidparm;

    if (AVL_IBSP_SetActiveI2C(havl68xx->vi2c) != 0)
        return vatek_result_hwfail;

    *status = demod_status_unknown;

    //TODO: detect signal
    /*
    AVL_uchar ucNosignal = 0;

    avl_code = AVL_Demod_DVBTxSignalDetection(&ucNosignal, havl68xx->chipno);
    if(ucNosignal == 0)//ucNosignal=1:signal exist 0:nosignal
    {
        printf("[AVL_Check_LockStatus] ----DVBTx NoSignal Channel! ---- \n");
    }
    else 
        printf("ucNosignal = %d \n", ucNosignal);
    */  
    avl_code = AVL_Demod_GetLockStatus(&demodLocked, havl68xx->chipno);
    if (avl_code != AVL_EC_OK)
    {
        AVL68XX_ERR("AVL_Demod_GetLockStatus error ! : %d", avl_code);
        return vatek_result_hwfail;
    }

    if (demodLocked)
        *status = demod_status_lock;
    else
        *status = demod_status_unlock;

    return vatek_result_success; 
}

vatek_result avl68xx_getSQI(Pavl68xx_handle havl68xx, uint32_t *SQI)
{
    AVL_ErrorCode avl_code = AVL_EC_OK;
    AVL_uint16 signal_quality = 0;

    if (havl68xx == NULL || SQI == NULL)
        return vatek_result_invalidparm;
    
    avl_code = AVL_Demod_GetSQI(&signal_quality, havl68xx->chipno);
    if (avl_code != AVL_EC_OK)
    {
        AVL68XX_ERR("AVL_Demod_GetSQI error ! : %d", avl_code);
        return vatek_result_hwfail;
    }

    *SQI = signal_quality;

    return vatek_result_success;
}

vatek_result avl68xx_getSNR(Pavl68xx_handle havl68xx, uint32_t *SNR)
{
    AVL_ErrorCode avl_code = AVL_EC_OK;
    AVL_uint32 snr_dbx100 = 0;

    if (havl68xx == NULL || SNR == NULL)
        return vatek_result_invalidparm;
    
    avl_code = AVL_Demod_GetSNR(&snr_dbx100, havl68xx->chipno);
    if (avl_code != AVL_EC_OK)
    {
        AVL68XX_ERR("AVL_Demod_GetSNR error ! : %d", avl_code);
        return vatek_result_hwfail;
    }

    *SNR = snr_dbx100/100;

    return vatek_result_success;
}

vatek_result avl68xx_getPER(Pavl68xx_handle havl68xx, uint32_t *PER)
{
    AVL_ErrorCode avl_code = AVL_EC_OK;
    AVL_uint32 per_1e9 = 0;

    if (havl68xx == NULL || PER == NULL)
        return vatek_result_invalidparm;
    
    avl_code = AVL_Demod_GetPER(&per_1e9, havl68xx->chipno);
    if (avl_code != AVL_EC_OK)
    {
        AVL68XX_ERR("AVL_Demod_GetPER error ! : %d", avl_code);
        return vatek_result_hwfail;
    }
    
    *PER = per_1e9/1e9;
    
    return vatek_result_success;
}

#endif

