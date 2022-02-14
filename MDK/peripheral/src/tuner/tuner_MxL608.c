
#if defined(TUNER_MXL608)

#include "vatek_system.h"
#include "tuner/MxL608/MxL608_TunerApi.h"
#include "tuner/MxL608/MxL608_OEM_Drv.h"

typedef struct _mxl608_handle
{
    Pvatek_i2c vi2c;
    uint8_t deviceID; //I2C addr
}mxl608_handle,*Pmxl608_handle;

#define MXL608_ERR(fmt,...)     VATEK_ERR(mxl608,fmt,##__VA_ARGS__)
#define MXL608_LOG(fmt,...)     VATEK_LOG(mxl608,fmt,##__VA_ARGS__)

static vatek_result initialize(uint8_t deviceID)
{
    MXL608_STATUS status;
    MXL608_BOOL singleSupply_3_3V;
    MXL608_XTAL_SET_CFG_T xtalCfg;
    MXL608_IF_OUT_CFG_T ifOutCfg;
    MXL608_AGC_CFG_T agcCfg;

    //Step 1 : Soft Reset MxL608
    status = MxLWare608_API_CfgDevSoftReset(deviceID);
    if (status != MXL608_SUCCESS)
    {
        MXL608_ERR("Error! MxLWare608_API_CfgDevSoftReset : %d",status);
        return vatek_result_hwfail;
    }

    //Step 2 : Overwrite Default    
    singleSupply_3_3V = MXL608_ENABLE; 
    status = MxLWare608_API_CfgDevOverwriteDefaults(deviceID, singleSupply_3_3V);
    if (status != MXL608_SUCCESS)
    {
        MXL608_ERR("Error! MxLWare608_API_CfgDevOverwriteDefaults : %d",status);    
        return vatek_result_hwfail;
    }
    
    //Step 3 : XTAL Setting
    xtalCfg.xtalFreqSel = MXL608_XTAL_16MHz;
    xtalCfg.xtalCap = 28;    
    xtalCfg.clkOutEnable = MXL608_DISABLE;
    xtalCfg.clkOutDiv = MXL608_DISABLE;
    xtalCfg.clkOutExt = MXL608_DISABLE; 
    xtalCfg.singleSupply_3_3V = MXL608_ENABLE;
    xtalCfg.XtalSharingMode = MXL608_DISABLE;
    status = MxLWare608_API_CfgDevXtal(deviceID, xtalCfg);
    if (status != MXL608_SUCCESS)
    {
        MXL608_ERR("Error! MxLWare608_API_CfgDevXtal : %d",status);    
        return vatek_result_hwfail;
    }
    status = MxLWare608_API_CfgTunerLoopThrough(deviceID, MXL608_DISABLE,NULL,NULL); 
    if (status != MXL608_SUCCESS)
    {
        MXL608_ERR("Error! MxLWare608_API_CfgTunerLoopThrough : %d",status);    
        return vatek_result_hwfail;
    }

    //Step 4 : IF Out setting
    ifOutCfg.ifOutFreq = MXL608_IF_5MHz;
    ifOutCfg.ifInversion = MXL608_ENABLE;
    ifOutCfg.gainLevel = 11;
    ifOutCfg.manualFreqSet = MXL608_DISABLE;
    ifOutCfg.manualIFOutFreqInKHz = 5000;
    status = MxLWare608_API_CfgTunerIFOutParam(deviceID, ifOutCfg);
    if (status != MXL608_SUCCESS)
    {
        MXL608_ERR("Error! MxLWare608_API_CfgTunerIFOutParam : %d",status);    
        return vatek_result_hwfail;
    }
    
    //Step 5 : AGC Setting
    agcCfg.agcType = MXL608_AGC_EXTERNAL;
    agcCfg.setPoint = 66;
    agcCfg.agcPolarityInverstion = MXL608_DISABLE;
    status = MxLWare608_API_CfgTunerAGC(deviceID, agcCfg);
    if (status != MXL608_SUCCESS)
    {
        MXL608_ERR("Error! MxLWare608_API_CfgTunerAGC : %d",status);    
        return vatek_result_hwfail;
    }
    
    return vatek_result_success;
}

vatek_result mxl608_create(Pvatek_i2c vi2c, Pmxl608_handle *hmxl608)
{
    vatek_result result = vatek_result_unknown;
    Pmxl608_handle newmxl608 = NULL; 
    
    if (vi2c == NULL || hmxl608 == NULL)
        return vatek_result_invalidparm;

    newmxl608 = (Pmxl608_handle)malloc(sizeof(mxl608_handle));
    if (newmxl608 == NULL)
        return vatek_result_memfail;

    MXL608_LOG("init mxl608");

    newmxl608->vi2c = vi2c;
    newmxl608->deviceID = 0x60;
    if (MxLWare608_OEM_SetActiveI2C(newmxl608->vi2c) != MXL608_SUCCESS)
        return vatek_result_hwfail;
    
    if ((result = initialize(newmxl608->deviceID)) != vatek_result_success)
    {   
        MXL608_ERR("mxl init fail");
        free(newmxl608);
        return result;
    }
    
    *hmxl608 = newmxl608;

    MXL608_LOG("init mxl608 ok!");
    
    return result;
}

vatek_result mxl608_destroy(Pmxl608_handle hmxl608)
{
    vatek_result result = vatek_result_success;

    if (hmxl608 == NULL)
        return vatek_result_invalidparm;
    
    free(hmxl608);
    hmxl608 = NULL;    
    
    return result;
}

vatek_result mxl608_lock(Pmxl608_handle hmxl608, Ptuner_lockparm parm)
{
    vatek_result result = vatek_result_success;
    MXL608_STATUS status; 
    MXL608_TUNER_MODE_CFG_T tunerModeCfg;
    MXL608_CHAN_TUNE_CFG_T chanTuneCfg;

    if (hmxl608 == NULL || parm == NULL)
        return vatek_result_invalidparm;
    
    if (MxLWare608_OEM_SetActiveI2C(hmxl608->vi2c) != MXL608_SUCCESS)
        return vatek_result_hwfail;

    //Step 6 : Application Mode setting
    switch (parm->type)
    {
        case tm_type_dtmb:
        case tm_type_dvbtx:    
            tunerModeCfg.signalMode = MXL608_DIG_DVB_T_DTMB;
            chanTuneCfg.signalMode = MXL608_DIG_DVB_T_DTMB;
            break;
        
        case tm_type_isdbt:
        case tm_type_atsc:
            tunerModeCfg.signalMode = MXL608_DIG_ISDBT_ATSC;
            chanTuneCfg.signalMode = MXL608_DIG_ISDBT_ATSC;
            break;
        
        case tm_type_j83b:
            tunerModeCfg.signalMode = MXL608_DIG_J83B;
            chanTuneCfg.signalMode = MXL608_DIG_J83B;
            break;
        
        case tm_type_dvbc:
            tunerModeCfg.signalMode = MXL608_DIG_DVB_C; 
            chanTuneCfg.signalMode = MXL608_DIG_DVB_C;
            break;
           
        default:
            return vatek_result_unsupport;
    }
    tunerModeCfg.ifOutFreqinKHz = 5000; 
    tunerModeCfg.xtalFreqSel = MXL608_XTAL_16MHz;
    tunerModeCfg.ifOutGainLevel = 11;
    status = MxLWare608_API_CfgTunerMode(hmxl608->deviceID, tunerModeCfg);
    if (status != MXL608_SUCCESS)
    {
        MXL608_ERR("Error! MxLWare608_API_CfgTunerMode : %d",status);
        return vatek_result_hwfail;  
    }
    
    switch (tunerModeCfg.signalMode)
    {
        case MXL608_DIG_DVB_T_DTMB:
        case MXL608_DIG_ISDBT_ATSC:
            if (parm->bandwidth <= 6000)
                chanTuneCfg.bandWidth = MXL608_TERR_BW_6MHz;
            else if (parm->bandwidth > 6000 && parm->bandwidth <= 7000)
                chanTuneCfg.bandWidth = MXL608_TERR_BW_7MHz;
            else 
                chanTuneCfg.bandWidth = MXL608_TERR_BW_8MHz;
            break;

        default:
            if (parm->bandwidth <= 6000)
                chanTuneCfg.bandWidth = MXL608_CABLE_BW_6MHz;
            else if (parm->bandwidth > 6000 && parm->bandwidth <= 7000)
                chanTuneCfg.bandWidth = MXL608_CABLE_BW_7MHz;
            else 
                chanTuneCfg.bandWidth = MXL608_CABLE_BW_8MHz;
            break;
    }
    chanTuneCfg.freqInHz = parm->freq * 1000;
    chanTuneCfg.xtalFreqSel = MXL608_XTAL_16MHz;
    chanTuneCfg.startTune = MXL608_START_TUNE;
    
    status = MxLWare608_API_CfgTunerChanTune(hmxl608->deviceID, chanTuneCfg);
    if (status != MXL608_SUCCESS)
    {
        MXL608_ERR("Error! MxLWare608_API_CfgTunerChanTune : %d",status);
        return vatek_result_hwfail;     
    }

    vatek_system_delay(15);
    
    return result;
}

vatek_result mxl608_getstatus(Pmxl608_handle hmxl608, tuner_status *status_t)
{
    vatek_result result = vatek_result_success;
    MXL608_STATUS status; 
    MXL608_BOOL refLockPtr = MXL608_UNLOCKED;
    MXL608_BOOL rfLockPtr = MXL608_UNLOCKED;        
    
    if (hmxl608 == NULL || status_t == NULL)
        return vatek_result_invalidparm;

    if (MxLWare608_OEM_SetActiveI2C(hmxl608->vi2c) != MXL608_SUCCESS)
        return vatek_result_hwfail;

    *status_t = tuner_status_unknown;
    status = MxLWare608_API_ReqTunerLockStatus(hmxl608->deviceID, &rfLockPtr, &refLockPtr);
    if (status == MXL608_TRUE)
    {
        if (MXL608_LOCKED == rfLockPtr && MXL608_LOCKED == refLockPtr)
            *status_t = tuner_status_lock;
        else
            *status_t = tuner_status_unlock;
    }

    return result; 
}

vatek_result mxl608_getrfstrength(Pmxl608_handle hmxl608, int32_t *power)
{
    vatek_result result = vatek_result_success;
    MXL608_STATUS status; 
    int16_t RSSI = 0;

    if (hmxl608 == NULL || power == NULL)
        return vatek_result_invalidparm;

    if (MxLWare608_OEM_SetActiveI2C(hmxl608->vi2c) != MXL608_SUCCESS)
        return vatek_result_hwfail;
    
    *power = 0;
    status = MxLWare608_API_ReqTunerRxPower(hmxl608->deviceID, &RSSI);
    if (status != MXL608_SUCCESS)
    {
        MXL608_ERR("Error! MxLWare608_API_ReqTunerRxPower : %d",status);
        return vatek_result_hwfail;
    }

    *power = (int32_t)RSSI/100;
    return result;
}

#endif

