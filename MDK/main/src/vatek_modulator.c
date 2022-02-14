
#include "vatek_system.h"
#include "vatek_hms.h"
#include "vatek_modulator.h"

static vatek_result modulator_base_setparm(Phms_handle handle, modulator_base_parm parm)
{
    vatek_result result = vatek_result_unknown;
    uint32_t value = 0;

    if (parm.type > m_type_max || parm.type < m_type_min)
    {
        MOD_ERR("modulator type overrange");
        return vatek_result_overrange;
    }

    if (parm.ifmode > m_ifmode_max || parm.ifmode < m_ifmode_min)
    {
        MOD_ERR("modulator ifmode overrange");
        return vatek_result_overrange;
    }

    if (handle->type == hms_type_transform)
    {
        if ((result = vatek_hms_write_hal(handle, HALREG_TRANSFORM_MODE, TRANSFORM_BROADCAST)) != vatek_result_success)
            return result;
    }

    switch (parm.type)
    {
        case m_type_dvb_t:
            value = MOD_DVB_T;
            break;

        case m_type_j83a:
            value = MOD_J83A;
            break;

        case m_type_atsc:
            value = MOD_ATSC;
            break;

        case m_type_j83b:
            value = MOD_J83B;
            break;

        case m_type_dtmb:
            value = MOD_DTMB;
            break;

        case m_type_isdb_t:
            value = MOD_ISDB_T;
            break;

        case m_type_j83c:
            value = MOD_J83C;
            break;

        case m_type_dvb_t2:
            value = MOD_DVB_T2;
            break;
            
        default:
            value = MOD_DVB_T;
            break;
    }
    if ((result = vatek_hms_write_hal(handle, HALREG_MOD_MODE, value)) != vatek_result_success)
        return result;    

    switch (parm.ifmode)
    {
        case m_ifmode_disable:
            value = IFMODE_DISABLE;
            break;

        case m_ifmode_if_freq:
            value = IFMODE_IF_FREQ;
            break;

        case m_ifmode_iq_offset:
            value = IFMODE_IQ_OFFSET;
            break;
            
        default:
            value = IFMODE_DISABLE;
            break;
    }
    if ((result = vatek_hms_write_hal(handle, HALREG_MOD_IFMODE, value)) != vatek_result_success)
        return result;    

    if ((result = vatek_hms_write_hal(handle, HALREG_MOD_IFFREQ, parm.iffreq)) != vatek_result_success)
        return result; 

    if ((result = vatek_hms_write_hal(handle, HALREG_MOD_DACGAIN, parm.dacgain)) != vatek_result_success)
        return result; 

    if ((result = vatek_hms_write_hal(handle, HALREG_MOD_BW_SB, parm.bw_sb)) != vatek_result_success)
        return result; 

    return result;
}

static vatek_result modulator_dvbt_setparm(Phms_handle handle, Pmodulator_dvbt_parm parm)
{
    vatek_result result = vatek_result_unknown;

    if (parm->constellation > dvbt_constellation_max || parm->constellation < dvbt_constellation_min)
    {
        MOD_ERR("dvbt constellation overrange");
        return vatek_result_overrange;
    }

    if (parm->fft > dvbt_fft_max || parm->fft < dvbt_fft_min)
    {
        MOD_ERR("dvbt fft overrange");
        return vatek_result_overrange;
    }

    if (parm->guardinterval > dvbt_guardinterval_max || parm->guardinterval < dvbt_guardinterval_min)
    {
        MOD_ERR("dvbt guardinterval overrange");
        return vatek_result_overrange;
    }
        
    if (parm->coderate > dvbt_coderate_max || parm->coderate < dvbt_coderate_min)
    {
        MOD_ERR("dvbt coderate overrange");
        return vatek_result_overrange;
    }

    uint32_t value;
    switch (parm->constellation)
    {
        case dvbt_constellation_qpsk:
            value = DVB_T_QPSK;
            break;

        case dvbt_constellation_qam16:
            value = DVB_T_QAM16;
            break;

        case dvbt_constellation_qam64:
            value = DVB_T_QAM64;
            break;

        default:
            value = DVB_T_QAM64;
            break;
    }
    if ((result = vatek_hms_write_hal(handle, HALREG_DVB_T_CONSTELLATION, value)) != vatek_result_success)
        return result; 

    switch (parm->fft)
    {
        case dvbt_fft_2k:
            value = DVB_T_FFT2K;
            break;

        case dvbt_fft_4k:
            value = DVB_T_FFT4K;
            break;

        case dvbt_fft_8k:
            value = DVB_T_FFT8K;
            break;

        default:
            value = DVB_T_FFT8K;
            break;
    }
    if ((result = vatek_hms_write_hal(handle, HALREG_DVB_T_FFT, value)) != vatek_result_success)
        return result; 

    switch (parm->guardinterval)
    {
        case dvbt_guardinterval_1_32:
            value = DVB_T_GI_1_32;
            break;

        case dvbt_guardinterval_1_16:
            value = DVB_T_GI_1_16;
            break;

        case dvbt_guardinterval_1_8:
            value = DVB_T_GI_1_8;
            break;

        case dvbt_guardinterval_1_4:
            value = DVB_T_GI_1_4;
            break;

        default:
            value = DVB_T_GI_1_32;
            break;
    }
    if ((result = vatek_hms_write_hal(handle, HALREG_DVB_T_GUARDINTERVAL, value)) != vatek_result_success)
        return result; 

    switch (parm->coderate)
    {
        case dvbt_coderate_1_2:
            value = DVB_T_CODERATE_1_2;
            break;

        case dvbt_coderate_2_3:
            value = DVB_T_CODERATE_2_3;
            break;

        case dvbt_coderate_3_4:
            value = DVB_T_CODERATE_3_4;
            break;

        case dvbt_coderate_5_6:
            value = DVB_T_CODERATE_5_6;
            break;
        
        case dvbt_coderate_7_8:
            value = DVB_T_CODERATE_7_8;
            break;

        default:
            value = DVB_T_CODERATE_7_8;
            break;
    }
    if ((result = vatek_hms_write_hal(handle, HALREG_DVB_T_CODERATE, value)) != vatek_result_success)
        return result; 
    
    return result;
}

static vatek_result modulator_j83a_setparm(Phms_handle handle, Pmodulator_j83a_parm parm)
{
    vatek_result result = vatek_result_unknown;

    if (parm->constellation > j83a_constellation_max || parm->constellation < j83a_constellation_min)
    {
        MOD_ERR("j83a constellation overrange");
        return vatek_result_overrange;
    }

    uint32_t value;
    switch (parm->constellation)
    {
        case j83a_constellation_qam16:
            value = J83A_QAM16;
            break;
            
        case j83a_constellation_qam32:
            value = J83A_QAM32;
            break;

        case j83a_constellation_qam64:
            value = J83A_QAM64;
            break;

        case j83a_constellation_qam128:
            value = J83A_QAM128;
            break;

        case j83a_constellation_qam256:
            value = J83A_QAM256;
            break;
            
        default:
            value = J83A_QAM256;
            break;
    }
    if ((result = vatek_hms_write_hal(handle, HALREG_J83A_CONSTELLATION, value)) != vatek_result_success)
        return result; 
    
    return result;
}

static vatek_result modulator_atsc_setparm(Phms_handle handle, Pmodulator_atsc_parm parm)
{
    vatek_result result = vatek_result_unknown;

    if (parm->constellation > atsc_constellation_max || parm->constellation < atsc_constellation_min)
    {
        MOD_ERR("atsc constellation overrange");
        return vatek_result_overrange;
    }

    if ((result = vatek_hms_write_hal(handle, HALREG_ATSC_CONSTELLATION, ATSC_8VSB)) != vatek_result_success)
        return result; 
    
    return result;
}

static vatek_result modulator_j83b_setparm(Phms_handle handle, Pmodulator_j83b_parm parm)
{
    vatek_result result = vatek_result_unknown;
    
    if (parm->constellation > j83b_constellation_max || parm->constellation < j83b_constellation_min)
    {
        MOD_ERR("j83b constellation overrange");
        return vatek_result_overrange;
    }

    uint32_t value;
    switch (parm->constellation)
    {
        case j83b_constellation_qam64:
            value = J83B_QAM64;
            break;
            
        case j83b_constellation_qam256:
            value = J83B_QAM256;
            break;
            
        default:
            value = J83B_QAM256;
            break;
    }
    if ((result = vatek_hms_write_hal(handle, HALREG_J83B_CONSTELLATION, value)) != vatek_result_success)
        return result; 
    
    return result;
}

static vatek_result modulator_dtmb_setparm(Phms_handle handle, Pmodulator_dtmb_parm parm)
{
    vatek_result result = vatek_result_unknown;
    
    if (parm->constellation > dtmb_constellation_max || parm->constellation < dtmb_constellation_min)
    {
        MOD_ERR("dtmb constellation overrange");
        return vatek_result_overrange;
    }

    if (parm->ti > dtmb_time_interleaved_max || parm->ti < dtmb_time_interleaved_min)
    {
        MOD_ERR("dtmb timeinterleaved overrange");
        return vatek_result_overrange;
    }

    if (parm->coderate > dtmb_coderate_max || parm->coderate < dtmb_coderate_min)
    {
        MOD_ERR("dtmb coderate overrange");
        return vatek_result_overrange;
    }

    if (parm->carriermode > dtmb_carriermode_max || parm->carriermode < dtmb_carriermode_min)
    {
        MOD_ERR("dtmb carriermode overrange");
        return vatek_result_overrange;
    }

    if (parm->syncframe > dtmb_syncframe_max || parm->syncframe < dtmb_syncframe_min)
    {
        MOD_ERR("dtmb syncframe overrange");
        return vatek_result_overrange;
    }

    uint32_t value;
    switch (parm->constellation)
    {
        case dtmb_constellation_qpsk:
            value = DTMB_QPSK;
            break;
            
        case dtmb_constellation_qam4_nr:
            value = DTMB_QAM4_NR;
            break;
        
        case dtmb_constellation_qam16:
            value = DTMB_QAM16;
            break;

        case dtmb_constellation_qam32:
            value = DTMB_QAM32;
            break;

        case dtmb_constellation_qam64:
            value = DTMB_QAM64;
            break;
            
        default:
            value = DTMB_QAM64;
            break;
    }
    if ((result = vatek_hms_write_hal(handle, HALREG_DTMB_CONSTELLATION, value)) != vatek_result_success)
        return result; 
    
    switch (parm->ti)
    {
        case dtmb_time_interleaved_disable:
            value = DTMB_TI_DISABLE;
            break;
            
        case dtmb_time_interleaved_240:
            value = DTMB_TI_240;
            break;
        
        case dtmb_time_interleaved_720:
            value = DTMB_TI_720;
            break;
            
        default:
            value = DTMB_TI_DISABLE;
            break;
    }
    if ((result = vatek_hms_write_hal(handle, HALREG_DTMB_TIME_INTERLEAVED, value)) != vatek_result_success)
        return result; 

    switch (parm->coderate)
    {
        case dtmb_coderate_0_4:
            value = DTMB_CODERATE_0_4;
            break;
            
        case dtmb_coderate_0_6:
            value = DTMB_CODERATE_0_6;
            break;
        
        case dtmb_coderate_0_8:
            value = DTMB_CODERATE_0_8;
            break;
            
        default:
            value = DTMB_CODERATE_0_8;
            break;
    }
    if ((result = vatek_hms_write_hal(handle, HALREG_DTMB_CODERATE, value)) != vatek_result_success)
        return result; 

    switch (parm->carriermode)
    {
        case dtmb_carriermode_3780:
            value = DTMB_CARRIER_3780;
            break;
            
        case dtmb_carriermode_1:
            value = DTMB_CARRIER_1;
            break;

        default:
            value = DTMB_CARRIER_3780;
            break;
    }
    if ((result = vatek_hms_write_hal(handle, HALREG_DTMB_CARRIERMODE, value)) != vatek_result_success)
        return result;
    
    switch (parm->syncframe)
    {
        case dtmb_syncframe_420:
            value = DTMB_SYNC_420;
            break;
            
        case dtmb_syncframe_595:
            value = DTMB_SYNC_595;
            break;
        
        case dtmb_syncframe_945:
            value = DTMB_SYNC_945;
            break;
            
        default:
            value = DTMB_SYNC_420;
            break;
    }
    if ((result = vatek_hms_write_hal(handle, HALREG_DTMB_SYNCFRAME, value)) != vatek_result_success)
        return result;
    
    return result;
}

static vatek_result modulator_isdbt_setparm(Phms_handle handle, Pmodulator_isdbt_parm parm)
{
    vatek_result result = vatek_result_unknown;

    if (parm->constellation > isdbt_constellation_max || parm->constellation < isdbt_constellation_min)
    {
        MOD_ERR("isdbt constellation overrange");
        return vatek_result_overrange;
    }

    if (parm->fft > isdbt_fft_max || parm->fft < isdbt_fft_min)
    {
        MOD_ERR("isdbt fft overrange");
        return vatek_result_overrange;
    }

    if (parm->guardinterval > isdbt_guardinterval_max || parm->guardinterval < isdbt_guardinterval_min)
    {
        MOD_ERR("isdbt guardinterval overrange");
        return vatek_result_overrange;
    }

    if (parm->coderate > isdbt_coderate_max || parm->coderate < isdbt_coderate_min)
    {
        MOD_ERR("isdbt coderate overrange");
        return vatek_result_overrange;
    }
            
    if (parm->ti > isdbt_time_interleaved_max || parm->ti < isdbt_time_interleaved_min)
    {
        MOD_ERR("isdbt timeinterleaved overrange");
        return vatek_result_overrange;
    }

    uint32_t value = 0;
    switch (parm->constellation)
    {
        case isdbt_constellation_dqpsk:
            value = ISDB_T_DQPSK;
            break;
            
        case isdbt_constellation_qpsk:
            value = ISDB_T_QPSK;
            break;
        
        case isdbt_constellation_qam16:
            value = ISDB_T_QAM16;
            break;

        case isdbt_constellation_qam64:
            value = ISDB_T_QAM64;
            break;
            
        default:
            value = ISDB_T_QAM64;
            break;
    }
    if ((result = vatek_hms_write_hal(handle, HALREG_ISDB_T_CONSTELLATION, value)) != vatek_result_success)
        return result;     

    switch (parm->fft)
    {
        case isdbt_fft_2k:
            value = ISDB_T_FFT2K;
            break;
            
        case isdbt_fft_4k:
            value = ISDB_T_FFT4K;
            break;
        
        case isdbt_fft_8k:
            value = ISDB_T_FFT8K;
            break;
            
        default:
            value = ISDB_T_FFT8K;
            break;
    }
    if ((result = vatek_hms_write_hal(handle, HALREG_ISDB_T_FFT, value)) != vatek_result_success)
        return result; 

    switch (parm->guardinterval)
    {
        case isdbt_guardinterval_1_32:
            value = ISDB_T_GI_1_32;
            break;
            
        case isdbt_guardinterval_1_16:
            value = ISDB_T_GI_1_16;
            break;
        
        case isdbt_guardinterval_1_8:
            value = ISDB_T_GI_1_8;
            break;

        case isdbt_guardinterval_1_4:
            value = ISDB_T_GI_1_4;
            break;
        
        default:
            value = ISDB_T_GI_1_32;
            break;
    }
    if ((result = vatek_hms_write_hal(handle, HALREG_ISDB_T_GUARDINTERVAL, value)) != vatek_result_success)
        return result; 

    switch (parm->coderate)
    {
        case isdbt_coderate_1_2:
            value = ISDB_T_CODERATE_1_2;
            break;
            
        case isdbt_coderate_2_3:
            value = ISDB_T_CODERATE_2_3;
            break;
        
        case isdbt_coderate_3_4:
            value = ISDB_T_CODERATE_3_4;
            break;
        
        case isdbt_coderate_5_6:
            value = ISDB_T_CODERATE_5_6;
            break;
        
        case isdbt_coderate_7_8:
            value = ISDB_T_CODERATE_7_8;
            break;
            
        default:
            value = ISDB_T_CODERATE_7_8;
            break;
    }
    if ((result = vatek_hms_write_hal(handle, HALREG_ISDB_T_CODERATE, value)) != vatek_result_success)
        return result; 

    switch (parm->ti)
    {
        case isdbt_time_interleaved_disable:
            value = ISDB_T_TI_DISABLE;
            break;
            
        case isdbt_time_interleaved_mode1:
            value = ISDB_T_TI_MODE1;
            break;
        
        case isdbt_time_interleaved_mode2:
            value = ISDB_T_TI_MODE2;
            break;

        case isdbt_time_interleaved_mode3:
            value = ISDB_T_TI_MODE3;
            break;
        
        default:
            value = ISDB_T_TI_DISABLE;
            break;
    }
    if ((result = vatek_hms_write_hal(handle, HALREG_ISDB_T_TIME_INTERLEAVED, value)) != vatek_result_success)
        return result;
    
    value = 0;
    if (parm->ac1_en)
        value |= ISDB_T_EN_AC_1;
    if (parm->ac2_en)
        value |= ISDB_T_EN_AC_2;
    if (parm->emergency_en)
        value |= ISDB_T_EN_EMERGENCY;
    if ((result = vatek_hms_write_hal(handle, HALREG_ISDB_T_FLAGS, value)) != vatek_result_success)
        return result;

    return result;
}

static vatek_result modulator_j83c_setparm(Phms_handle handle, Pmodulator_j83c_parm parm)
{
    vatek_result result = vatek_result_unknown;
    
    if (parm->constellation > j83c_constellation_max || parm->constellation < j83c_constellation_min)
    {
        MOD_ERR("j83c constellation overrange");
        return vatek_result_overrange;
    }
    
    uint32_t value = 0;
    switch (parm->constellation)
    {
        case j83c_constellation_qam64:
            value = J83C_QAM64;
            break;
            
        case j83c_constellation_qam256:
            value = J83C_QAM256;
            break;
            
        default:
            value = J83C_QAM256;
            break;
    }
    if ((result = vatek_hms_write_hal(handle, HALREG_J83C_CONSTELLATION, value)) != vatek_result_success)
        return result; 

    return result;
}

static vatek_result modulator_dvbt2_setparm(Phms_handle handle, Pmodulator_dvbt2_parm parm)
{
    vatek_result result = vatek_result_unknown;

    if (parm->version > dvbt2_version_max || parm->version < dvbt2_version_min)
    {
        MOD_ERR("dvbt2 version overrange");
        return vatek_result_overrange;
    }
    
    if (parm->issy > dvbt2_issy_max || parm->issy < dvbt2_issy_min)
    {
        MOD_ERR("dvbt2 issy overrange");
        return vatek_result_overrange;
    }

    if (parm->nti > dvbt2_nti_max || parm->nti < dvbt2_nti_min)
    {
        MOD_ERR("dvbt2 nti overrange");
        return vatek_result_overrange;
    }

    if (parm->l1_constellation > dvbt2_l1_constellation_max || parm->l1_constellation < dvbt2_l1_constellation_min)
    {
        MOD_ERR("dvbt2 l1_constellation overrange");
        return vatek_result_overrange;
    }

    if (parm->plp_constellation > dvbt2_plp_constellation_max || parm->plp_constellation < dvbt2_plp_constellation_min)
    {
        MOD_ERR("dvbt2 plp_constellation overrange");
        return vatek_result_overrange;
    }

    if (parm->fft > dvbt2_fft_max || parm->fft < dvbt2_fft_min)
    {
        MOD_ERR("dvbt2 fft overrange");
        return vatek_result_overrange;
    }

    if (parm->coderate > dvbt2_coderate_max || parm->coderate < dvbt2_coderate_min)
    {
        MOD_ERR("dvbt2 coderate overrange");
        return vatek_result_overrange;
    }
    
    if (parm->guardinterval > dvbt2_guardinterval_max || parm->guardinterval < dvbt2_guardinterval_min)
    {
        MOD_ERR("dvbt2 guardinterval overrange");
        return vatek_result_overrange;
    }
    
    if (parm->pilotpattern > dvbt2_pilotpattern_max || parm->pilotpattern < dvbt2_pilotpattern_min)
    {
        MOD_ERR("dvbt2 pilotpattern overrange");
        return vatek_result_overrange;
    }
    
    if (parm->fec > dvbt2_fec_max || parm->fec < dvbt2_fec_min)
    {
        MOD_ERR("dvbt2 fec overrange");
        return vatek_result_overrange;
    }

    uint32_t value = 0;
    switch (parm->version)
    {
        case dvbt2_version_1_1_1:
            value = T2_VER_1_1_1;
            break;
            
        case dvbt2_version_1_1_1_lite:
            value = T2_VER_1_1_1 | T2_PROFILE_LITE;
            break;
        
        case dvbt2_version_1_2_1:
            value = T2_VER_1_2_1;
            break;
            
        case dvbt2_version_1_2_1_lite:
            value = T2_VER_1_2_1 | T2_PROFILE_LITE;
            break;
        
        case dvbt2_version_1_3_1:
            value = T2_VER_1_3_1;
            break;
            
        case dvbt2_version_1_3_1_lite:
            value = T2_VER_1_3_1 | T2_PROFILE_LITE;
            break;
        
        default:
            value = T2_VER_1_3_1;
            break;
    }
    if ((result = vatek_hms_write_hal(handle, HALREG_DVB_T2_VERSION, value)) != vatek_result_success)
        return result; 

    value = 0;
    if (parm->extend_carrier_mode_en)
        value |= T2EN_EXTEND_CARRIER_MODE;
    if (parm->constellation_rotation_en)
        value |= T2EN_CONSTELLATION_ROTATION;
    if (parm->input_ts_hem_en)
        value |= T2EN_INPUT_TS_HEM;
    if (parm->delete_null_packet_en)
        value |= T2EN_DELETE_NULL_PACKET;
    if (parm->vbr_coding_en)
        value |= T2EN_VBR_CODING;
    if (parm->time_interval_en)
        value |= T2EN_TIME_INTERVAL;
    if ((result = vatek_hms_write_hal(handle, HALREG_DVB_T2_FLAGS, value)) != vatek_result_success)
        return result;

    switch (parm->issy)
    {
        case dvbt2_issy_disable:
            value = T2_ISSY_DISABLE;
            break;
            
        case dvbt2_issy_short:
            value = T2_ISSY_SHORT;
            break;
        
        case dvbt2_issy_long:
            value = T2_ISSY_LONG;
            break;
            
        default:
            value = T2_ISSY_DISABLE;
            break;
    }
    if ((result = vatek_hms_write_hal(handle, HALREG_DVB_T2_ISSY, value)) != vatek_result_success)
        return result; 

    switch (parm->nti)
    {
        case dvbt2_nti_disable:
            value = T2_NTI_DISABLE;
            break;
            
        case dvbt2_nti_1:
            value = 1;
            break;
        
        case dvbt2_nti_2:
            value = 2;
            break;
        
        case dvbt2_nti_3:
            value = 3;
            break;
        
        default:
            value = T2_NTI_DISABLE;
            break;
    }
    if ((result = vatek_hms_write_hal(handle, HALREG_DVB_T2_NTI, value)) != vatek_result_success)
        return result; 

    switch (parm->l1_constellation)
    {
        case dvbt2_l1_constellation_bpsk:
            value = T2_L1_BPSK;
            break;
            
        case dvbt2_l1_constellation_qpsk:
            value = T2_L1_QPSK;
            break;
        
        case dvbt2_l1_constellation_qam16:
            value = T2_L1_QAM16;
            break;
        
        case dvbt2_l1_constellation_qam64:
            value = T2_L1_QAM64;
            break;
        
        default:
            value = T2_L1_QAM64;
            break;
    }
    if ((result = vatek_hms_write_hal(handle, HALREG_DVB_T2_L1_CONSTELLATION, value)) != vatek_result_success)
        return result;

    switch (parm->plp_constellation)
    {
        case dvbt2_plp_constellation_qpsk:
            value = T2_PLP_QPSK;
            break;
            
        case dvbt2_plp_constellation_qam16:
            value = T2_PLP_QAM16;
            break;
        
        case dvbt2_plp_constellation_qam64:
            value = T2_PLP_QAM64;
            break;
        
        case dvbt2_plp_constellation_qam256:
            value = T2_PLP_QAM256;
            break;
        
        default:
            value = T2_PLP_QAM256;
            break;
    }
    if ((result = vatek_hms_write_hal(handle, HALREG_DVB_T2_PLP_CONSTELLATION, value)) != vatek_result_success)
        return result;

    switch (parm->fft)
    {
        case dvbt2_fft_1k:
            value = T2_FFT_1K;
            break;
            
        case dvbt2_fft_2k:
            value = T2_FFT_2K;
            break;
        
        case dvbt2_fft_4k:
            value = T2_FFT_4K;
            break;
        
        case dvbt2_fft_8k:
            value = T2_FFT_8K;
            break;

        case dvbt2_fft_16k:
            value = T2_FFT_16K;
            break;
        
        case dvbt2_fft_32k:
            value = T2_FFT_32K;
            break;
        
        default:
            value = T2_FFT_32K;
            break;
    }
    if ((result = vatek_hms_write_hal(handle, HALREG_DVB_T2_FFT, value)) != vatek_result_success)
        return result;    

    switch (parm->coderate)
    {
        case dvbt2_coderate_1_3:
            value = T2_CODERATE_1_3;
            break;
            
        case dvbt2_coderate_2_5:
            value = T2_CODERATE_2_5;
            break;
        
        case dvbt2_coderate_1_2:
            value = T2_CODERATE_1_2;
            break;
        
        case dvbt2_coderate_3_5:
            value = T2_CODERATE_3_5;
            break;

        case dvbt2_coderate_2_3:
            value = T2_CODERATE_2_3;
            break;
        
        case dvbt2_coderate_3_4:
            value = T2_CODERATE_3_4;
            break;
        
        case dvbt2_coderate_4_5:
            value = T2_CODERATE_4_5;
            break;
        
        case dvbt2_coderate_5_6:
            value = T2_CODERATE_5_6;
            break;
        
        default:
            value = T2_CODERATE_5_6;
            break;
    }
    if ((result = vatek_hms_write_hal(handle, HALREG_DVB_T2_CODERATE, value)) != vatek_result_success)
        return result;
    
    switch (parm->guardinterval)
    {
        case dvbt2_guardinterval_1_128:
            value = T2_GI_1_128;
            break;
            
        case dvbt2_guardinterval_1_32:
            value = T2_GI_1_32;
            break;
        
        case dvbt2_guardinterval_1_16:
            value = T2_GI_1_16;
            break;
        
        case dvbt2_guardinterval_19_256:
            value = T2_GI_19_256;
            break;

        case dvbt2_guardinterval_19_128:
            value = T2_GI_19_128;
            break;
        
        case dvbt2_guardinterval_1_8:
            value = T2_GI_1_8;
            break;
        
        case dvbt2_guardinterval_1_4:
            value = T2_GI_1_4;
            break;
        
        default:
            value = T2_GI_1_128;
            break;
    }
    if ((result = vatek_hms_write_hal(handle, HALREG_DVB_T2_GUARDINTERVAL, value)) != vatek_result_success)
        return result;

    switch (parm->pilotpattern)
    {
        case dvbt2_pilotpattern_1:
            value = T2_PP_1;
            break;
            
        case dvbt2_pilotpattern_2:
            value = T2_PP_2;
            break;
        
        case dvbt2_pilotpattern_3:
            value = T2_PP_3;
            break;
        
        case dvbt2_pilotpattern_4:
            value = T2_PP_4;
            break;

        case dvbt2_pilotpattern_5:
            value = T2_PP_5;
            break;
        
        case dvbt2_pilotpattern_6:
            value = T2_PP_6;
            break;
        
        case dvbt2_pilotpattern_7:
            value = T2_PP_7;
            break;
        
        case dvbt2_pilotpattern_8:
            value = T2_PP_8;
            break;
        
        default:
            value = T2_PP_1;
            break;
    }
    if ((result = vatek_hms_write_hal(handle, HALREG_DVB_T2_PILOTPATTERN, value)) != vatek_result_success)
        return result;

    switch (parm->fec)
    {
        case dvbt2_fec_16200:
            value = T2_FEC_16200;
            break;
            
        case dvbt2_fec_64800:
            value = T2_FEC_64800;
            break;
        
        default:
            value = T2_FEC_64800;
            break;
    }
    if ((result = vatek_hms_write_hal(handle, HALREG_DVB_T2_FECTYPE, value)) != vatek_result_success)
        return result;   
    
    if ((result = vatek_hms_write_hal(handle, HALREG_DVB_T2_NID, parm->nid)) != vatek_result_success)
        return result;

    if ((result = vatek_hms_write_hal(handle, HALREG_DVB_T2_SID, parm->sid)) != vatek_result_success)
        return result;

    if ((result = vatek_hms_write_hal(handle, HALREG_DVB_T2_FECBN, parm->fecbn)) != vatek_result_success)
        return result;

    if ((result = vatek_hms_write_hal(handle, HALREG_DVB_T2_SBN, parm->sbn)) != vatek_result_success)
        return result;

    return result;
}

vatek_result vatek_modulator_setparm(Phms_handle handle, modulator_base_parm parm_base, Pmodulator_advanced_parm parm_adv)
{
    vatek_result result = vatek_result_unknown;

    if (handle == NULL || parm_adv == NULL)
        return vatek_result_invalidparm;

    if ((result = vatek_hms_issystemidle(handle)) != vatek_result_success)
        return result;
    
    if ((result = modulator_base_setparm(handle, parm_base)) != vatek_result_success)
    {
        MOD_ERR("base setparm fail");
        return result;
    }

    switch (parm_base.type)
    {
        case m_type_dvb_t:
            result = modulator_dvbt_setparm(handle, (Pmodulator_dvbt_parm)parm_adv);
            break;
            
        case m_type_j83a:
            result = modulator_j83a_setparm(handle, (Pmodulator_j83a_parm)parm_adv);
            break;
        
        case m_type_atsc:
            result = modulator_atsc_setparm(handle, (Pmodulator_atsc_parm)parm_adv);
            break;
        
        case m_type_j83b:
            result = modulator_j83b_setparm(handle, (Pmodulator_j83b_parm)parm_adv);
            break;
        
        case m_type_dtmb:
            result = modulator_dtmb_setparm(handle, (Pmodulator_dtmb_parm)parm_adv);
            break;
        
        case m_type_isdb_t:
            result = modulator_isdbt_setparm(handle, (Pmodulator_isdbt_parm)parm_adv);
            break;
        
        case m_type_j83c:
            result = modulator_j83c_setparm(handle, (Pmodulator_j83c_parm)parm_adv);
            break;
        
        case m_type_dvb_t2:
            result = modulator_dvbt2_setparm(handle, (Pmodulator_dvbt2_parm)parm_adv);
            break;
        
        default:
            result = modulator_dvbt_setparm(handle, (Pmodulator_dvbt_parm)parm_adv);
            break;
    }

    return result;
}

