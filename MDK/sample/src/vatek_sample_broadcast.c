
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vatek_sample_broadcast.h"

#define SECOND_PHY  0 // this sample case is CVBS output
#define CVBS_CC     0 // this sample case is SECOND_PHY's closed caption
#define ATSC        0
#define J83B        1
#define DVBT        2
#define ISDBT       3
#define J83A        4
#define MOD_TYPE    ISDBT

static Phbroadcast bc_handle = NULL;
static Phphy hdmi_handle = NULL;
static Phphy cvbs_handle = NULL;
static Phphy phy_active_handle = NULL;
static Phrf rf_handle = NULL;
static phy_video_info g_phy_vi = {0};
static phy_audio_info g_phy_ai = {0};

static vatek_result _sample_setmodulator_parm(void)
{ 
    vatek_result result = vatek_result_unknown;
    
    /* set MODULATOR parameter */
    #if (MOD_TYPE == ATSC) // ATSC
    modulator_base_parm mod_base_parm = {0};
    modulator_atsc_parm mod_atsc_parm = {0};
    mod_base_parm.type = m_type_atsc;
    mod_base_parm.ifmode = m_ifmode_disable;
    mod_base_parm.iffreq = 0;
    mod_base_parm.dacgain = 0;
    mod_base_parm.bw_sb = 6;
    mod_atsc_parm.constellation = atsc_constellation_8vsb;
    result = vatek_broadcast_modulator_setparm(bc_handle, mod_base_parm, (Pmodulator_advanced_parm)&mod_atsc_parm);
    if (!is_success(result))
    {
        SAMPLE_ERR("vatek_broadcast_modulator_setparm fail: %d", result);
        return result;
    }
    #elif (MOD_TYPE == J83B) // J83B
    modulator_base_parm mod_base_parm = {0};
    modulator_j83b_parm mod_j83b_parm = {0};
    mod_base_parm.type = m_type_j83b;
    mod_base_parm.ifmode = m_ifmode_disable;
    mod_base_parm.iffreq = 0;
    mod_base_parm.dacgain = 0;
    mod_base_parm.bw_sb = 6;
    mod_j83b_parm.constellation = j83b_constellation_qam256;
    result = vatek_broadcast_modulator_setparm(bc_handle, mod_base_parm, (Pmodulator_advanced_parm)&mod_j83b_parm);
    if (!is_success(result))
    {
        SAMPLE_ERR("vatek_broadcast_modulator_setparm fail: %d", result);
        return result;
    }
    #elif (MOD_TYPE == DVBT) // dvbt
    modulator_base_parm mod_base_parm = {0};
    modulator_dvbt_parm mod_dvbt_parm = {0};
    mod_base_parm.type = m_type_dvb_t;
    mod_base_parm.ifmode = m_ifmode_disable;
    mod_base_parm.iffreq = 0;
    mod_base_parm.dacgain = 0;
    mod_base_parm.bw_sb = 6;
    mod_dvbt_parm.constellation = dvbt_constellation_qam64;
    mod_dvbt_parm.fft = dvbt_fft_8k;
    mod_dvbt_parm.guardinterval = dvbt_guardinterval_1_16;
    mod_dvbt_parm.coderate = dvbt_coderate_5_6;
    result = vatek_broadcast_modulator_setparm(bc_handle, mod_base_parm, (Pmodulator_advanced_parm)&mod_dvbt_parm);
    if (!is_success(result))
    {
        SAMPLE_ERR("vatek_broadcast_modulator_setparm fail: %d", result);
        return result;
    }
    #elif (MOD_TYPE == ISDBT) //ISDBT
    modulator_base_parm mod_base_parm = {0};
    modulator_isdbt_parm mod_isdbt_parm = {0};
    mod_base_parm.type = m_type_isdb_t;
    mod_base_parm.ifmode = m_ifmode_disable;
    mod_base_parm.iffreq = 0;
    mod_base_parm.dacgain = 0;
    mod_base_parm.bw_sb = 6;
    mod_isdbt_parm.constellation = isdbt_constellation_qam64;
    mod_isdbt_parm.fft = isdbt_fft_8k;
    mod_isdbt_parm.guardinterval = isdbt_guardinterval_1_16;
    mod_isdbt_parm.coderate = isdbt_coderate_5_6;
    mod_isdbt_parm.ti = isdbt_time_interleaved_mode3;
    result = vatek_broadcast_modulator_setparm(bc_handle, mod_base_parm, (Pmodulator_isdbt_parm)&mod_isdbt_parm);
    if (!is_success(result))
    {
        SAMPLE_ERR("vatek_broadcast_modulator_setparm fail: %d", result);
        return result;
    }
    #elif (MOD_TYPE == J83A) //J83A
    modulator_base_parm mod_base_parm = {0};
    modulator_j83a_parm mod_j83a_parm = {0};
    
    mod_base_parm.type = m_type_j83a;
    mod_base_parm.ifmode = m_ifmode_disable;
    mod_base_parm.iffreq = 0;
    mod_base_parm.dacgain = 0;
    mod_base_parm.bw_sb = 5720;
    mod_j83a_parm.constellation = j83a_constellation_qam256;
    
    result = vatek_broadcast_modulator_setparm( bc_handle, mod_base_parm, (Pmodulator_j83a_parm)&mod_j83a_parm);
    if (!is_success(result))
    {
        SAMPLE_ERR("vatek_broadcast_modulator_setparm fail: %d", result);
        return result;
    }  
    #endif

    return result;
}

vatek_result sample_bc_broadcast_from_logo(void)
{ 
    // broadcast (logo) ==> (VATEK) ==> (rf)
    // use simple psi (iso13818) 
    vatek_result result = vatek_result_unknown;
    
    /* set ENCODER input parameter from logo/colorbar*/
    logo_input_parm logo_parm = {0};
    logo_parm.type = logo_type_colorbar;
    result = vatek_broadcast_encoder_setinputparm_logo(bc_handle, logo_parm);
    if (!is_success(result))
    {
        SAMPLE_ERR("vatek_broadcast_encoder_setinputparm_logo fail: %d", result);
        return result;
    }

    /* set ENCODER encode parameter */
    video_encode_parm ve_parm = {0};
    audio_encode_parm ae_parm = {0};
    ve_parm.type = ve_type_h264;
    ae_parm.type = ae_type_mp1_l2;
    ae_parm.channel = ae_channel_stereo;
    result = vatek_broadcast_encoder_setencodeparm(bc_handle, ve_parm, ae_parm);
    if (!is_success(result))
    {
        SAMPLE_ERR("vatek_broadcast_encoder_setencodeparm fail: %d", result);
        return result;
    }

    /* set ENCODER pes/ts mux parameter */
    encoder_mux_parm em_parm = {0};
    em_parm.video_pid = 0x1002;
    em_parm.audio_pid = 0x1003;
    result = vatek_broadcast_encoder_setmuxparm(bc_handle, em_parm);
    if (!is_success(result))
    {
        SAMPLE_ERR("vatek_broadcast_encoder_setmuxparm fail: %d", result);
        return result;
    }

    /* set ENCODER quality parameter */
    encoder_quality_parm q_parm = {0};
    q_parm.rcmode = q_rcmode_vbr;
    q_parm.minq = 3;
    q_parm.maxq = 10;
    q_parm.gop = 16;
    q_parm.latency = 500;
    q_parm.bitrate = 19000000;
    result = vatek_broadcast_encoder_setqualityparm(bc_handle, q_parm);
    if (!is_success(result))
    {
        SAMPLE_ERR("vatek_broadcast_encoder_setqualityparm fail: %d", result);
        return result;
    }

    /* set TSMUX parameter */
    tsmux_iso13818_parm iso_parm = {0};
    iso_parm.pmtpid = 0x1001;
    iso_parm.tsid = 2;
    iso_parm.program_num = 0x10;
    iso_parm.pcr_pid = 0x100;
    iso_parm.padding_pid = 0x1FFF;
    result = vatek_broadcast_tsmux_setparm(bc_handle, tsmux_type_iso13818, &iso_parm);
    if (!is_success(result))
    {
        SAMPLE_ERR("vatek_broadcast_tsmux_setparm fail: %d", result);
        return result;
    }
    

    /* set MODULATOR parameter */
    result = _sample_setmodulator_parm();
    if (!is_success(result))
    {
        SAMPLE_ERR("_sample_setmodulator_parm fail: %d", result);
        return result;
    }

    /* start BROADCAST */
    result = vatek_broadcast_start(bc_handle);
    if (!is_success(result))
    {
        SAMPLE_ERR("vatek_broadcast_start fail: %d", result);
        return result;
    }else SAMPLE_LOG("ColorBar Broadcast Start");

    /* start RF */
    result = vatek_rf_start(rf_handle, 473000);
    if (!is_success(result))
    {
        SAMPLE_ERR("vatek_rf_start fail: %d", result);
        return result;
    }else SAMPLE_LOG("RF Start");

    return result;
}

vatek_result sample_bc_broadcast_from_phy(void)
{ 
    // broadcast (phy) ==> (VATEK) ==> (rf)
    // use default psi, psi table via DEFAULTPSI API (vatek_broadcast_psispec_default_init/config/start)
    vatek_result result = vatek_result_unknown;

    /* get PHY video information */
    phy_video_info phy_vi = {0};
    result = vatek_phy_getvideoinfo(phy_active_handle, &phy_vi);
    if (!is_success(result))
    {
        SAMPLE_ERR("vatek_phy_getvideoinfo fail: %d", result);
        return result;
    }
    SAMPLE_LOG("resolution = %d", phy_vi.resolution);
    SAMPLE_LOG("aspectrate = %d", phy_vi.aspectrate);
    SAMPLE_LOG("buswidth_16 = %d", phy_vi.buswidth_16);
    SAMPLE_LOG("separated_sync = %d", phy_vi.separated_sync);
    
    /* get PHY audio information */
    phy_audio_info phy_ai = {0};
    result = vatek_phy_getaudioinfo(phy_active_handle, &phy_ai);
    if (!is_success(result))
    {
        SAMPLE_ERR("vatek_phy_getaudioinfo fail: %d", result);
        return result;
    }
    SAMPLE_LOG("samplerate = %d", phy_ai.samplerate);

    /* set ENCODER input parameter from PHY*/
    video_input_parm vi_parm = {0};
    audio_input_parm ai_parm = {0};
    vi_parm.resolution = phy_vi.resolution;
    vi_parm.aspectrate = phy_vi.aspectrate;
    vi_parm.buswidth_16 = phy_vi.buswidth_16;
    vi_parm.separated_sync = phy_vi.separated_sync;
    ai_parm.samplerate = phy_ai.samplerate;
    result = vatek_broadcast_encoder_setinputparm_phy(bc_handle, vi_parm, ai_parm);
    if (!is_success(result))
    {
        SAMPLE_ERR("vatek_broadcast_encoder_setinputparm_phy fail: %d", result);
        return result;
    };

    /* set ENCODER encode parameter */
    video_encode_parm ve_parm = {0};
    audio_encode_parm ae_parm = {0};
    ve_parm.type = ve_type_mpeg2;
    ae_parm.type = ae_type_aac_lc_adts;
    ae_parm.channel = ae_channel_mono_l;
    result = vatek_broadcast_encoder_setencodeparm(bc_handle, ve_parm, ae_parm);
    if (!is_success(result))
    {
        SAMPLE_ERR("vatek_broadcast_encoder_setencodeparm fail: %d", result);
        return result;
    }

    /* set ENCODER pes/ts mux parameter */
    encoder_mux_parm em_parm = {0};
    em_parm.video_pid = 0x1002;
    em_parm.audio_pid = 0x1003;
    result = vatek_broadcast_encoder_setmuxparm(bc_handle, em_parm);
    if (!is_success(result))
    {
        SAMPLE_ERR("vatek_broadcast_encoder_setmuxparm fail: %d", result);
        return result;
    }

    /* set ENCODER quality parameter */
    encoder_quality_parm q_parm = {0};
    q_parm.rcmode = q_rcmode_vbr;
    q_parm.minq = 3;
    q_parm.maxq = 10;
    q_parm.gop = 16;
    q_parm.latency = 500;
    q_parm.bitrate = 19000000;
    result = vatek_broadcast_encoder_setqualityparm(bc_handle, q_parm);
    if (!is_success(result))
    {
        SAMPLE_ERR("vatek_broadcast_encoder_setqualityparm fail: %d", result);
        return result;
    }
#if 0
    /* set TSMUX parameter */
    tsmux_default_parm default_parm = {0};
    default_parm.pmt_pid = 0x1000;
    default_parm.pcr_pid = 0x100;
    default_parm.padding_pid = 0x1FFF;
    result = vatek_broadcast_tsmux_setparm(bc_handle, tsmux_type_default, &default_parm);
    if (!is_success(result))
    {
        SAMPLE_ERR("vatek_broadcast_tsmux_setparm fail: %d", result);
        return result;
    }
    
    //setting iso-13818-1 mode parmeter with psi default mode.
    psispec_default_iso13818_channel channel = 
    {
        .transport_stream_id = 1,
    };
    psispec_default_iso13818_program program = 
    {
        .program_number = 0x10,
    };

    result = vatek_broadcast_psispec_default_init( bc_handle, psispec_default_iso, psispec_country_undefined);
    if(is_success(result)) result = vatek_broadcast_psispec_default_config( &channel, &program);
    if(is_success(result)) result = vatek_broadcast_psispec_default_start();
    if(!is_success(result))
    {
        SAMPLE_ERR("vatek_broadcast_psispec_default_start fail: %d", result);
        return result;
    };
#endif

    
    tsmux_default_parm default_parm  = {0};
    default_parm.pcr_pid = 0x100;
    default_parm.padding_pid = 0x1FFF;
    result = vatek_broadcast_tsmux_setparm(bc_handle, tsmux_type_default, &default_parm);    /** select tsmux_type_default */
    if (result != vatek_result_success)
    {
        SAMPLE_ERR("vatek_broadcast_tsmux_setparm fail: %d", result);
        return result;
    }

    #define _AC(ch)    (ch + 0x80)

    static const uint8_t _arib_network_name[] = { 0x1b, 0x7e, _AC('V'), _AC('A'), _AC('T'), _AC('E'), _AC('K')};
    static const uint8_t _arib_service_name[] = { 0x1b, 0x7e, _AC('D'), _AC('E'), _AC('M'), _AC('O')};
    static const uint8_t _arib_ts_name[]      = { 0x1b, 0x7e, _AC('T'), _AC('S')};

    vatek_string network_name = 
    {
        .len = sizeof(_arib_network_name),
        .text = (uint8_t *)_arib_network_name,
    };
    vatek_string service_name = 
    {
        .len = sizeof(_arib_service_name),
        .text = (uint8_t *)_arib_service_name,
    };
    vatek_string ts_name = 
    {
        .len = sizeof(_arib_ts_name),
        .text = (uint8_t *)_arib_ts_name,    
    };

    //Setting arib mode parmeter.
    psispec_default_arib_channel channel = 
    {
        .region_id = 23,
        .broadcaster_id = 1,
        .remote_control_key_id = 1,
        .network_name = &network_name,
    };
    psispec_default_arib_program program = 
    {
        .service_no = 1,
        .main_lang.raw  = { 'j', 'p', 'n', '\0'},
        .sub_lang.raw   = { 'e', 'n', 'g', '\0'},
        .copy_flag = arib_abnt_free,
        .service_name = &service_name,
        .ts_name = &ts_name,
    };

    result = vatek_broadcast_psispec_default_init( bc_handle, psispec_default_arib, psispec_country_undefined);
    if(is_success(result)) result = vatek_broadcast_psispec_default_config( &channel, &program);
    if(is_success(result)) result = vatek_broadcast_psispec_default_start();
    if(!is_success(result))
    {
        SAMPLE_ERR("vatek_broadcast_psispec_default_start fail: %d", result);
        return result;
    }

		
    /* set MODULATOR parameter */
    result = _sample_setmodulator_parm();
    if (!is_success(result))
    {
        SAMPLE_ERR("_sample_setmodulator_parm fail: %d", result);
        return result;
    };
        
    /* start BROADCAST */
    result = vatek_broadcast_start(bc_handle);
    if (!is_success(result))
    {
        SAMPLE_ERR("vatek_broadcast_start fail: %d", result);
        return result;
    }else SAMPLE_LOG("VI Broadcast Start");

    /* start RF */
    result = vatek_rf_start(rf_handle, 473000);
    if (!is_success(result))
    {
        SAMPLE_ERR("vatek_rf_start fail: %d", result);
        return result;
    }else SAMPLE_LOG("RF Start");

    /* copy phy infomation to global variable when success broadcast */
    memcpy(&g_phy_vi, &phy_vi, sizeof(phy_video_info));
    memcpy(&g_phy_ai, &phy_ai, sizeof(phy_audio_info));

    return result;
}

vatek_result sample_bc_broadcast_stop(void)
{
    vatek_result result = vatek_result_unknown;
    broadcast_status status = bc_status_unknown;

    /* check broadcast status */
    result = vatek_broadcast_bcstatus(bc_handle, &status);
    if (!is_success(result))
    {
        SAMPLE_ERR("vatek_broadcast_bcstatus fail : %d", result);
        return result;
    }

    if (status == bc_status_idle)
        return vatek_result_success;

    /* stop rf*/
    result = vatek_rf_stop(rf_handle);
    if (!is_success(result))
    {
        SAMPLE_ERR("vatek_rf_stop fail : %d", result);
        return result;
    }

    /* stop broadcast */
    result = vatek_broadcast_stop(bc_handle);
    if (!is_success(result))
    {
        SAMPLE_ERR("vatek_broadcast_stop fail : %d", result);
        return result;
    }

    return result;
}

vatek_result sample_bc_broadcast_signalischange(void)
{
    vatek_result result = vatek_result_negative;
    phy_video_info tmp_phy_vi = {0};
    phy_audio_info tmp_phy_ai = {0};
    phy_status cvbs_status = phy_status_unknown;
    phy_status hdmi_status = phy_status_unknown;

    /* get PHY status */
    result = vatek_phy_status(hdmi_handle, &hdmi_status);
    if (!is_success(result))
    {
        SAMPLE_ERR("vatek_phy_getstatus hdmi fail: %d", result);
    }
    if (SECOND_PHY == 1)
    {
        result = vatek_phy_status(cvbs_handle, &cvbs_status);
        if (!is_success(result))
        {
            SAMPLE_ERR("vatek_phy_getstatus cvbs fail: %d", result);
        }
    }

    if (hdmi_status == phy_status_active)
    {
        /* enable phy(hdmi), disable phy(cvbs) */
        if ((result = vatek_phy_enable(hdmi_handle, 1)) != vatek_result_success)
            SAMPLE_ERR("vatek_phy_enable hdmi fail: %d", result);
        if (SECOND_PHY == 1 && (result = vatek_phy_enable(cvbs_handle, 0)) != vatek_result_success)
            SAMPLE_ERR("vatek_phy_enable cvbs fail: %d", result);
        phy_active_handle = hdmi_handle;
    }
    else if (cvbs_status == phy_status_active)
    {
        /* disable phy(hdmi), enable phy(cvbs) */
        if ((result = vatek_phy_enable(hdmi_handle, 0)) != vatek_result_success)
            SAMPLE_ERR("vatek_phy_enable hdmi fail: %d", result);
        if (SECOND_PHY == 1 && (result = vatek_phy_enable(cvbs_handle, 1)) != vatek_result_success)
            SAMPLE_ERR("vatek_phy_enable cvbs fail: %d", result);
        phy_active_handle = cvbs_handle;
    }
    else
    {
        /* all phy signal lost or error*/
        /* disable phy(hdmi), disable phy(cvbs) */
        if ((result = vatek_phy_enable(hdmi_handle, 0)) != vatek_result_success)
            SAMPLE_ERR("vatek_phy_enable hdmi fail: %d", result);
        if (SECOND_PHY == 1 && (result = vatek_phy_enable(cvbs_handle, 0)) != vatek_result_success)
            SAMPLE_ERR("vatek_phy_enable cvbs fail: %d", result);
        memcpy(&g_phy_vi, &tmp_phy_vi, sizeof(phy_video_info));
        memcpy(&g_phy_ai, &tmp_phy_ai, sizeof(phy_audio_info));
        return vatek_result_idle;
    }

    /* get PHY video information */
    result = vatek_phy_getvideoinfo(phy_active_handle, &tmp_phy_vi);
    if (!is_success(result))
    {
        SAMPLE_ERR("vatek_phy_getvideoinfo fail: %d", result);
        return result;
    }

    /* get PHY audio information */
    result = vatek_phy_getaudioinfo(phy_active_handle, &tmp_phy_ai);
    if (!is_success(result))
    {
        SAMPLE_ERR("vatek_phy_getaudioinfo fail: %d", result);
        return result;
    }

    /* compare audio/video information */
    if (memcmp(&tmp_phy_vi, &g_phy_vi, sizeof(phy_video_info)) != 0 || memcmp(&tmp_phy_ai, &g_phy_ai, sizeof(phy_audio_info)) != 0)
    {
        SAMPLE_LOG("PHY audio/video information change");
        SAMPLE_LOG("--- video info ---");
        SAMPLE_LOG("resolution     : %02d - %02d", tmp_phy_vi.resolution, g_phy_vi.resolution);
        SAMPLE_LOG("aspectrate     : %02d - %02d", tmp_phy_vi.aspectrate, g_phy_vi.aspectrate);
        SAMPLE_LOG("buswidth_16    : %02d - %02d", tmp_phy_vi.buswidth_16, g_phy_vi.buswidth_16);
        SAMPLE_LOG("separated_sync : %02d - %02d", tmp_phy_vi.separated_sync, g_phy_vi.separated_sync);
        SAMPLE_LOG("--- audio info ---");
        SAMPLE_LOG("samplerate     : %02d - %02d", tmp_phy_ai.samplerate, g_phy_ai.samplerate);
        result = vatek_result_success;
    }
    else
        result = vatek_result_negative;

    return result;
}

vatek_result sample_bc_broadcast(uint8_t fromlogo)
{
    vatek_result result = vatek_result_unknown;
    
    /* stop broadcast */
    result = sample_bc_broadcast_stop();
    if (!is_success(result))
    {
        SAMPLE_ERR("sample_broadcast_stop fail : %d", result);
        return result;
    }
    
    if (fromlogo)
    {
        /* start broadcast from colorbar */
        result = sample_bc_broadcast_from_logo();
        if (!is_success(result))
        {
            SAMPLE_ERR("sample_broadcast_from_logo fail : %d", result);
            return result;
        }
    }
    else
    {
        /* start broadcast from phy */
        result = sample_bc_broadcast_from_phy();
        if (!is_success(result))
        {
            SAMPLE_ERR("sample_broadcast_from_phy fail : %d", result);
            return result;
        }
    }
    
    return result;
}

vatek_result sample_bc_polling_cc(void)
{
    vatek_result result = vatek_result_unknown;
    static uint8_t total_num = 0;
    static uint32_t write_tick = 0;
    static userdata_cc_parm cc_parm[MAX_USERDATA_CC_NUM] = {0};
    
    /* get cc data from cvbs */
    result = vatek_phy_getccdata(cvbs_handle, &cc_parm[total_num]);
    if (result == vatek_result_success)
    {
        cc_parm[total_num].attr = 0xFC | cc_parm[total_num].attr;
        total_num++;
    }
    
    /* write cc date to VATEK, when interval >= 100ms (about 6-7 frames (60FPS), half MAX_USERDATA_CC_NUM) or total_num = MAX_USERDATA_CC_NUM */
    if (vatek_system_gettick() - write_tick >= 100 || total_num == MAX_USERDATA_CC_NUM)
    { 
        if (total_num >= 1)
        {
            result = vatek_broadcast_userdata_insert_cc(bc_handle, total_num, cc_parm);
            if (!is_success(result))
            {
                SAMPLE_ERR("vatek_broadcast_userdata_insert_cc fail : %d", result);
                return result;
            }
            total_num = 0;
            memset(cc_parm, '\0', sizeof(cc_parm));
        }
        write_tick = vatek_system_gettick();
    }

    return vatek_result_success;
}

vatek_result sample_bc_init(Pboard_handle bh_main, Pboard_handle bh_phy, Pboard_handle bh_rf)
{
    #if defined(PHY_EP9555E)
      phy_type phy_hdmi =  phy_type_ep9555e;
    #elif  defined(PHY_EP9351)
      phy_type phy_hdmi =  phy_type_ep9351;
    #endif
    
    vatek_result result = vatek_result_unknown;

    /* init VATEK broadcast */
    result = vatek_broadcast_create(bh_main, &bc_handle);
    if (!is_success(result))
    {
        SAMPLE_ERR("broadcast create fail : %d", result);
        return result;
    }

    /* init phy */
    result = vatek_phy_create(bh_phy, phy_hdmi, &hdmi_handle);  
    if (!is_success(result))
    {
        SAMPLE_ERR("phy(hdmi) create fail : %d", result);
        return result;
    }
    if (SECOND_PHY == 1 || CVBS_CC == 1)
    {
        /* init phy : adv7182a */
        result = vatek_phy_create(bh_phy, phy_type_adv7182a_cvbs, &cvbs_handle);  
        if (!is_success(result))
        {
            SAMPLE_ERR("phy(cvbs) create fail : %d", result);
            return result;
        }
    }
    phy_active_handle = hdmi_handle;
    
    /* init RF : r2_via_vatek */
    result = vatek_rf_create(bh_rf, rf_type_r2_via_vatek, &rf_handle);  
    if (!is_success(result))
    {
        SAMPLE_ERR("rf create fail : %d", result);
        return result;
    }
    
    /* broadcast from logo */
    result = sample_bc_broadcast_from_logo();
    if (!is_success(result))
    {
        SAMPLE_ERR("sample_broadcast_from_logo fail: %d", result);
        return result;
    }

    return result;
}

vatek_result sample_bc_polling(void)
{
    //this function is a simple case for broadcast polling    
    vatek_result result = vatek_result_success;
    static uint8_t signal_lost = 1; // broadcast from logo when call sample_init, so default signal_lost is 1
    broadcast_status bc_status = bc_status_unknown;
    chip_status c_status = chip_status_unknown;
    static uint32_t tick = 0;

    #if (CVBS_CC == 1)
    if ((result = sample_bc_polling_cc()) !=  vatek_result_success)
    {
        SAMPLE_ERR("sample_polling_cc fail : %d", result);
    }
    #endif
    
    /* polling interval is 2000 */
    if ((vatek_system_gettick() - tick) <= 2000)
        return result;
    tick = vatek_system_gettick();

    /* check chip status is active */
    result = vatek_broadcast_chipstatus(bc_handle, &c_status);
    if (!is_success(result))
    {
        SAMPLE_ERR("vatek_broadcast_chipstatus fail : %d", result);
        return result;
    }
    if (!(c_status == chip_status_running || c_status == chip_status_wait_command))
    {
        SAMPLE_LOG("VATEK chip not running/waitcmd, c_status = %d",c_status);
        return result;
    } 

    /* check input signal change */
    result = sample_bc_broadcast_signalischange();
    if (result == vatek_result_success) /* signal change */
    {
        SAMPLE_LOG("signal change");
        result = sample_bc_broadcast(0);
        if (!is_success(result))
        {
            SAMPLE_ERR("sample_broadcast fail : %d", result);
            return result;
        }
        signal_lost = 0;
    }
    else if (result == vatek_result_idle) /* signal lost */
    {   
        if (signal_lost)
            return result;
        
        SAMPLE_LOG("signal lost");
        result = sample_bc_broadcast(1);
        if (!is_success(result))
        {
            SAMPLE_ERR("sample_broadcast fail : %d", result);
            return result;
        }
        signal_lost = 1;
    }
    else    /* normal play case */
    {
            
        /* polling broadcast status */
        result = vatek_broadcast_bcstatus(bc_handle, &bc_status);
        if (!is_success(result))
        {
            SAMPLE_ERR("vatek_broadcast_bcstatus fail : %d", result);
            return result;
        }
       
        /* check status */
        if (bc_status != bc_status_broadcast)
        {
            /* re-broadcast */
            SAMPLE_LOG("something wrong, re-broadcast");
            result = sample_bc_broadcast(signal_lost);
            if (!is_success(result))
            {
                SAMPLE_ERR("_sample_broadcast fail : %d", result);
                return result;
            }
        }
    }

    return result;
}

