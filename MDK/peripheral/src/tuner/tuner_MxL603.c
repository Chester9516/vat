
#if defined(TUNER_MXL603)

#include "vatek_system.h"
#include "tuner/MxL603/MxL603_TunerApi.h"
#include "tuner/MxL603/MxL603_OEM_Drv.h"

typedef struct _mxl603_handle
{
    Pvatek_i2c vi2c;
    uint8_t deviceID; //I2C addr
}mxl603_handle,*Pmxl603_handle;

#define MXL603_ERR(fmt,...)     VATEK_ERR(mxl603,fmt,##__VA_ARGS__)
#define MXL603_LOG(fmt,...)     VATEK_LOG(mxl603,fmt,##__VA_ARGS__)

static vatek_result initialize(uint8_t deviceID)
{
    MXL_STATUS status;
    MXL_BOOL singleSupply_3_3V;
    MXL603_XTAL_SET_CFG_T xtalCfg;
    MXL603_IF_OUT_CFG_T ifOutCfg;
    MXL603_AGC_CFG_T agcCfg;
    MXL603_TUNER_MODE_CFG_T tunerModeCfg;
    MXL603_CHAN_TUNE_CFG_T chanTuneCfg;
    
    //Step 1 : Soft Reset MxL603
    status = MxLWare603_API_CfgDevSoftReset(deviceID);
    if (status != MXL_SUCCESS)
    {
        MXL603_ERR("MxLWare603_API_CfgDevSoftReset fail : %d", status);
        return vatek_result_hwfail;
    }
    
    //Step 2 : Overwrite Default
    singleSupply_3_3V = MXL_ENABLE;
    status = MxLWare603_API_CfgDevOverwriteDefaults(deviceID, singleSupply_3_3V);
    if (status != MXL_SUCCESS)
    {
        MXL603_ERR("MxLWare603_API_CfgDevOverwriteDefaults fail : %d", status);
        return vatek_result_hwfail;
    }
    
    //Step 3 : XTAL Setting
    xtalCfg.xtalFreqSel = MXL603_XTAL_16MHz;
    xtalCfg.xtalCap = 12;
    xtalCfg.clkOutEnable = MXL_ENABLE;
    xtalCfg.clkOutDiv = MXL_DISABLE;
    xtalCfg.clkOutExt = MXL_DISABLE;
    xtalCfg.singleSupply_3_3V = MXL_ENABLE;
    xtalCfg.XtalSharingMode = MXL_DISABLE;
    status = MxLWare603_API_CfgDevXtal(deviceID, xtalCfg);
    if (status != MXL_SUCCESS)
    {
        MXL603_ERR("MxLWare603_API_CfgDevXtal fail : %d", status);
        return vatek_result_hwfail;
    }
    
    //Step 4 : IF Out setting
    ifOutCfg.ifOutFreq = MXL603_IF_6MHz;
    ifOutCfg.ifInversion = MXL_DISABLE;
    ifOutCfg.gainLevel = 11;
    ifOutCfg.manualFreqSet = MXL_DISABLE;
    ifOutCfg.manualIFOutFreqInKHz = 0;
    status = MxLWare603_API_CfgTunerIFOutParam(deviceID, ifOutCfg);
    if (status != MXL_SUCCESS)
    {
        MXL603_ERR("MxLWare603_API_CfgTunerIFOutParam fail : %d", status);
        return vatek_result_hwfail;
    }
    
                                                                                                
    //Step 5 : AGC Setting
    agcCfg.agcType = MXL603_AGC_SELF;
    agcCfg.setPoint = 66;
    agcCfg.agcPolarityInverstion = MXL_DISABLE;
    status = MxLWare603_API_CfgTunerAGC(deviceID, agcCfg);
    if (status != MXL_SUCCESS)
    {
        MXL603_ERR("MxLWare603_API_CfgTunerAGC fail : %d", status);
        return vatek_result_hwfail;
    }
    
    //Step 6 : Application Mode setting  for IF Frequency BandWidth
    tunerModeCfg.signalMode = MXL603_DIG_ISDBT_ATSC;
    tunerModeCfg.ifOutFreqinKHz = 6000; 
    tunerModeCfg.xtalFreqSel = MXL603_XTAL_16MHz;
    tunerModeCfg.ifOutGainLevel = 11;
    status = MxLWare603_API_CfgTunerMode(deviceID, tunerModeCfg);
    if (status != MXL_SUCCESS)
    {
        MXL603_ERR("MxLWare603_API_CfgTunerMode fail : %d", status);
        return vatek_result_hwfail;
    }
    
    //Step 7 : Channel frequency & bandwidth setting for Channel Frequency
    chanTuneCfg.bandWidth  = MXL603_TERR_BW_6MHz;
    chanTuneCfg.signalMode = MXL603_DIG_ISDBT_ATSC;
    chanTuneCfg.freqInHz = 473000000;
    chanTuneCfg.xtalFreqSel = MXL603_XTAL_16MHz;
    chanTuneCfg.startTune = MXL_START_TUNE;
    status = MxLWare603_API_CfgTunerChanTune(deviceID, chanTuneCfg);
    if (status != MXL_SUCCESS)
    {
        MXL603_ERR("MxLWare603_API_CfgTunerChanTune fail : %d", status);
        return vatek_result_hwfail;
    }
    
    // Wait 15 ms
    MxLWare603_OEM_Sleep(15);

    return vatek_result_success;
}


vatek_result mxl603_create(Pvatek_i2c vi2c, Pmxl603_handle *hmxl603)
{
    vatek_result result = vatek_result_unknown;
    Pmxl603_handle newmxl603 = NULL; 
    
    if (vi2c == NULL || hmxl603 == NULL)
        return vatek_result_invalidparm;

    newmxl603 = (Pmxl603_handle)malloc(sizeof(mxl603_handle));
    if (newmxl603 == NULL)
        return vatek_result_memfail;

    MXL603_LOG("init mxl603");

    newmxl603->vi2c = vi2c;
    newmxl603->deviceID = 0xC0; //MXL603_I2C_ADDR
    
    if (MxLWare603_OEM_SetActiveI2C(newmxl603->vi2c) != MXL_SUCCESS)
        return vatek_result_hwfail;
 
    if ((result = initialize(newmxl603->deviceID)) != vatek_result_success)
    {   
        MXL603_ERR("mxl init fail");
        free(newmxl603);
        return result;
    }
    
    *hmxl603 = newmxl603;
    
    MXL603_LOG("init mxl603 ok!");
    
    return result;
}

vatek_result mxl603_destroy(Pmxl603_handle hmxl603)
{
    vatek_result result = vatek_result_success;

    if (hmxl603 == NULL)
        return vatek_result_invalidparm;
    
    free(hmxl603);
    hmxl603 = NULL;    
    
    return result;
}

vatek_result mxl603_lock(Pmxl603_handle hmxl603, Ptuner_lockparm parm)
{
    MXL_STATUS status;
    MXL603_TUNER_MODE_CFG_T tunerModeCfg;
    MXL603_CHAN_TUNE_CFG_T chanTuneCfg;

    if (hmxl603 == NULL || parm == NULL)
        return vatek_result_invalidparm;
    
    if (MxLWare603_OEM_SetActiveI2C(hmxl603->vi2c) != MXL_SUCCESS)
        return vatek_result_hwfail;

    //Step 6 : Application Mode setting
    switch (parm->type)
    {
        case tm_type_atsc:
            tunerModeCfg.signalMode = MXL603_DIG_ISDBT_ATSC;
            chanTuneCfg.signalMode = MXL603_DIG_ISDBT_ATSC;
            break;
           
        default:
            return vatek_result_unsupport;
    }
    tunerModeCfg.ifOutFreqinKHz = 6000;
    tunerModeCfg.xtalFreqSel = MXL603_XTAL_16MHz;
    tunerModeCfg.ifOutGainLevel = 11;
    status = MxLWare603_API_CfgTunerMode(hmxl603->deviceID, tunerModeCfg);
    if (status != MXL_SUCCESS)
    {
        MXL603_ERR("MxLWare603_API_CfgTunerMode fail : %d", status);
        return vatek_result_hwfail;
    }

    switch (tunerModeCfg.signalMode)
    {
        case MXL603_DIG_DVB_T_DTMB:
        case MXL603_DIG_ISDBT_ATSC:
            if (parm->bandwidth <= 6000)
                chanTuneCfg.bandWidth = MXL603_TERR_BW_6MHz;
            else if (parm->bandwidth > 6000 && parm->bandwidth <= 7000)
                chanTuneCfg.bandWidth = MXL603_TERR_BW_7MHz;
            else 
                chanTuneCfg.bandWidth = MXL603_TERR_BW_8MHz;
            break;

        default:
            if (parm->bandwidth <= 6000)
                chanTuneCfg.bandWidth = MXL603_CABLE_BW_6MHz;
            else if (parm->bandwidth > 6000 && parm->bandwidth <= 7000)
                chanTuneCfg.bandWidth = MXL603_CABLE_BW_7MHz;
            else 
                chanTuneCfg.bandWidth = MXL603_CABLE_BW_8MHz;
            break;
    }
    chanTuneCfg.freqInHz = parm->freq * 1000;
    chanTuneCfg.xtalFreqSel = MXL603_XTAL_16MHz;
    chanTuneCfg.startTune = MXL_START_TUNE;
    status = MxLWare603_API_CfgTunerChanTune(hmxl603->deviceID, chanTuneCfg);
    if (status != MXL_SUCCESS)
    {
        MXL603_ERR("MxLWare603_API_CfgTunerChanTune : %d",status);
        return vatek_result_hwfail;     
    }

    return vatek_result_success;
}

vatek_result mxl603_getstatus(Pmxl603_handle hmxl603, tuner_status *status_t)
{
    vatek_result result = vatek_result_success;
    MXL_STATUS status; 
    MXL_BOOL refLockPtr = MXL_UNLOCKED;
    MXL_BOOL rfLockPtr = MXL_UNLOCKED;        
    
    if (hmxl603 == NULL || status_t == NULL)
        return vatek_result_invalidparm;

    if (MxLWare603_OEM_SetActiveI2C(hmxl603->vi2c) != MXL_SUCCESS)
        return vatek_result_hwfail;
    
    *status_t = tuner_status_unknown;
    status = MxLWare603_API_ReqTunerLockStatus(hmxl603->deviceID, &rfLockPtr, &refLockPtr);
    if (status == MXL_TRUE)
    {
        if (MXL_LOCKED == rfLockPtr && MXL_LOCKED == refLockPtr)
            *status_t = tuner_status_lock;
        else
            *status_t = tuner_status_unlock;
    }
    
    return result; 
}

vatek_result mxl603_getrfstrength(Pmxl603_handle hmxl603, int32_t *power)
{
    vatek_result result = vatek_result_success;
    
    MXL_STATUS status; 
    int16_t pwr = 0;

    if (hmxl603 == NULL || power == NULL)
        return vatek_result_invalidparm;

    if (MxLWare603_OEM_SetActiveI2C(hmxl603->vi2c) != MXL_SUCCESS)
        return vatek_result_hwfail;
    
    *power = 0;
    status = MxLWare603_API_ReqTunerRxPower(hmxl603->deviceID, &pwr);
    if (status != MXL_SUCCESS)
    {
        MXL603_ERR("MxLWare603_API_ReqTunerRxPower : %d",status);
        return vatek_result_hwfail;
    }

    *power = (int32_t)pwr;

    return result;
}

#endif

