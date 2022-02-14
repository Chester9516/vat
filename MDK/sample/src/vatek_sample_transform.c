
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vatek_sample_transform.h"

#define ATSC            0
#define J83B            1
#define DVBT            2
#define ISDBT           3
#define MOD_TYPE        DVBT 

//unit KHz
#define LOCK_FREQ       473000 
#define LOCK_SYMBOL     6000
#define RF_FREQ         473000


#define PSIPURE         1
#define PSIDEF          2
#define PSIBYPASS       3
#define PSI_TYPE        PSIDEF


static Phtuner tuner_handle = NULL;
static Phdemod demod_handle = NULL;
static Phtransform tf_handle = NULL;
static Phrf rf_handle = NULL;

uint8_t g_serial = 0;
uint8_t g_clk_inverse = 0;

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
    result = vatek_transform_modulator_setparm(tf_handle, mod_base_parm, (Pmodulator_advanced_parm)&mod_atsc_parm);
    if (result != vatek_result_success)
    {
        SAMPLE_ERR("vatek_transform_modulator_setparm fail: %d", result);
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
    result = vatek_transform_modulator_setparm(tf_handle, mod_base_parm, (Pmodulator_advanced_parm)&mod_j83b_parm);
    if (result != vatek_result_success)
    {
        SAMPLE_ERR("vatek_transform_modulator_setparm fail: %d", result);
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
    result = vatek_transform_modulator_setparm(tf_handle, mod_base_parm, (Pmodulator_advanced_parm)&mod_dvbt_parm);
    if (result != vatek_result_success)
    {
        SAMPLE_ERR("vatek_transform_modulator_setparm fail: %d", result);
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
    result = vatek_transform_modulator_setparm(tf_handle, mod_base_parm, (Pmodulator_isdbt_parm)&mod_isdbt_parm);
    if (result != vatek_result_success)
    {
        SAMPLE_ERR("vatek_transform_modulator_setparm fail: %d", result);
        return result;
    }
    #endif

    return result;
}

static vatek_result _sample_tf_lockfreq(uint32_t freq_khz, uint32_t bandwidth_khz)
{
    vatek_result result = vatek_result_unknown;
    uint32_t timeout = 2000;
    uint32_t tick = 0;
    
    tuner_lockparm freqparm;
    freqparm.bandwidth = bandwidth_khz;
    freqparm.freq = freq_khz;
    freqparm.type = tm_type_dvbtx;
    result = vatek_tuner_lock(tuner_handle, &freqparm);  
    if (result != vatek_result_success)
    {
        SAMPLE_ERR("vatek_tuner_lock fail : %d", result);
        return result;
    }

    tuner_status status = tuner_status_unknown;
    tick = vatek_system_gettick();
    while(1)
    {
        if (vatek_system_gettick() - tick >= timeout)
        {
            SAMPLE_ERR("tuner lock timeout");
            return vatek_result_timeout;
        }
        result = vatek_tuner_getstatus(tuner_handle, &status);  
        if (result != vatek_result_success)
        {
            SAMPLE_ERR("vatek_tuner_getstatus fail : %d", result);
            return result;
        }
        if (status == tuner_status_lock)
            break;
        vatek_system_delay(10);
    }

    SAMPLE_LOG("========= TUNER LOCK =========");
   
    demod_lockparm demodparm;
    demodparm.bandwidth = bandwidth_khz;
    demodparm.type = dm_type_dvbtx;
    result = vatek_demod_lock(demod_handle, &demodparm);  
    if (result != vatek_result_success)
    {
        SAMPLE_ERR("vatek_demod_lock fail : %d", result);
        return result;
    }

    demod_status status_d = demod_status_unknown;
    tick = vatek_system_gettick();
    while(1)
    {
        if (vatek_system_gettick() - tick >= timeout)
        {
            SAMPLE_ERR("demod lock timeout");
            return vatek_result_timeout;
        }
        result = vatek_demod_getstatus(demod_handle, &status_d);  
        if (result != vatek_result_success)
        {
            SAMPLE_ERR("vatek_demod_getstatus fail : %d", result);
            return result;
        }
        if (status_d == demod_status_lock)
            break;
        
        vatek_system_delay(10);
    }

    SAMPLE_LOG("========= DEMOD LOCK =========");

    return result;
}

#if (PSI_TYPE==PSIPURE)
static vatek_result _sample_tf_pure_psitable(void)
{
    vatek_result result = vatek_result_unknown;
    
    /* set PSITABLE parameter */	
    uint8_t PAT[] = 
    {
        0x47, 0x40, 0x00, 0x11, 		/* PID : 0x0 */
        0x00, 0x00, 0xB0, 0x0D,
        0x00, 0x02, 								/* tsid : 0x0002 */
        0xC3, 0x00, 0x00,
        0x00, 0x10, 								/* program number */
        0xF0, 0x01, 								/* pmt_PID: 0x1001 */
        0x40, 0x6E, 0xC8, 0x8E, 		/* CRC */
    };

    uint8_t PMT[] = 
    { 
        0x47, 0x50, 0x01, 0x1C,     /* PID : 0x1001 */
        0x00, 0x02, 0xB0, 0x17,     
        0x00, 0x10,                 /* program number*/ 
        0xC3, 0x00, 0x00, 
        0xE1, 0x00,                 /* PCR_PID: 0x100 */ 
        0xF0, 0x00, 
        0x02, 0xF0, 0x02,           /* stream type : 0x2 (video mpeg2), PID : 0x1002 */
        0xF0, 0x00, 
        0x03, 0xF0, 0x03,           /* stream type : 0x3 (audio mpeg1-l2), PID : 0x1003 */ 
        0xF0, 0x00, 
        0x1D, 0x30, 0xDB, 0x47,     /* CRC */
    };

    //PAT 188 Bytes
    uint8_t *pat_buf = (uint8_t*)malloc(PSI_TSPACKET_LEN);
    memset(pat_buf, 0xFF, PSI_TSPACKET_LEN);
    memcpy(&pat_buf[0], PAT, sizeof(PAT)/sizeof(uint8_t));

    //PMT 188 Bytes
    uint8_t *pmt_buf = (uint8_t*)malloc(PSI_TSPACKET_LEN);
    memset(pmt_buf, 0xFF, PSI_TSPACKET_LEN);
    memcpy(&pmt_buf[0], PMT, sizeof(PMT)/sizeof(uint8_t));
        
    /** register psi table to broadcast serivce */
    psitablelist_parm psi_parm = {0};
    psi_parm.table_num = 2;
    psi_parm.table[0].interval_ms = 500;
    psi_parm.table[0].tspacket_num = 1;
    psi_parm.table[0].tspackets = pat_buf;
    psi_parm.table[1].interval_ms = 500;
    psi_parm.table[1].tspacket_num = 1;
    psi_parm.table[1].tspackets = pmt_buf;
    result = vatek_transform_psitable_register(tf_handle, &psi_parm);
    if (result != vatek_result_success)
    {
        SAMPLE_ERR("vatek_broadcast_psitable_register fail: %d", result);
        return result;
    }
}

#elif (PSI_TYPE==PSIDEF)
static vatek_result _smaple_tf_default_psitable(void)
{
    vatek_result result = vatek_result_unknown;
    
    //Set PSI type and country according to the modulator.
    if((result = vatek_transform_psispec_default_init(tf_handle, psispec_default_iso, psispec_country_undefined)) != vatek_result_success)
    {
        SAMPLE_LOG("default psi init fail : %d", result);
    }
    
    //Set channel parameter.
    psispec_default_iso13818_channel chan;
    chan.transport_stream_id = 1;
    result = vatek_transform_psispec_default_channel_config(&chan);
    if(result!=vatek_result_success)
    {
        SAMPLE_LOG("default psi channel config fail : %d", result);
    }
    
    //Add program parameter.
    psispec_default_program_tr prog;
    prog.pcr_pid = 0x100;
    prog.pmt_pid = 0x1000;
    prog._prog.iso_prog.program_number = 0x10;
    if((result = vatek_transform_psispec_default_program_add(&prog)) == vatek_result_success)
    {
        video_param vparam;
        vparam.type = ve_type_mpeg2;
        vparam.resolution = vi_resolution_1080p30;
        vparam.aspectrate = vi_aspectrate_16_9;
        
        //Add stream parameter.
        psispec_default_stream_tr v_stream;
        v_stream.type = psi_def_stream_type_video;
        v_stream.stream_pid = 0x1002;
        v_stream.es_type = 0x02;
        v_stream.es_len = 10;
        v_stream._stream.video_param = &vparam;
        v_stream.es = NULL;
        if((result = vatek_transform_psispec_default_stream_add(&v_stream)) != vatek_result_success)
        {
            SAMPLE_LOG("default psi stream add fail : %d", result);
            return result;
        }
    
        audio_param aparam;
        aparam.type = ae_type_ac_3;
        aparam.channel = ae_channel_stereo;
        aparam.samplerate = ai_samplerate_48K;
        
        //Add stream parameter.
        psispec_default_stream_tr a_stream;
        a_stream.type = psi_def_stream_type_audio;
        a_stream.stream_pid = 0x1003;
        a_stream.es_type = 0x06;
        a_stream.es_len = 10;
        a_stream._stream.audio_param = &aparam;
        a_stream.es = NULL;
        if((result = vatek_transform_psispec_default_stream_add(&a_stream)) != vatek_result_success)
        {
            SAMPLE_LOG("default psi stream add fail : %d", result);
            return result;
        }
        
        //Set program parameter finish.
        if((result = vatek_transform_psispec_default_program_end()) != vatek_result_success)
        {
            SAMPLE_LOG("default psi program add fail : %d", result);
            return result;
        }
        
        //Start Defalt PSI Table.
        if((result = vatek_transform_psispec_default_start()) != vatek_result_success)
        {
            SAMPLE_LOG("default psi start fail : %d", result);
            return result;
        }
        
    }else SAMPLE_LOG("default psi program add fail : %d", result);
    
    return result;
}
#endif

static vatek_result _sample_tf_play_allprogram(uint32_t rf_freq)
{
    vatek_result result = vatek_result_unknown;
    
    /* step-2 : set tranform setting */
    /* set tsp input parm (from ts) */
    ts_input_parm ti_parm = {0};
    ti_parm.serial      = g_serial;
    ti_parm.clk_inverse = g_clk_inverse;
    ti_parm.pcr_mode    = pcr_disable;
    result = vatek_transform_tsp_setinputparm_ts(tf_handle, ti_parm);
    if (result != vatek_result_success)
    {
        SAMPLE_ERR("vatek_transform_tsin_setparm fail: %d", result);
        return result;
    }else SAMPLE_LOG("TS Setting");

#if (PSI_TYPE==PSIPURE)
    /* set tsmux parm */
    result = vatek_transform_tsmux_setparm(tf_handle, tsmux_type_pure);
    if (result != vatek_result_success)
    {
        SAMPLE_ERR("vatek_transform_tsmux_setparm fail: %d", result);
        return result;
    }
    
    /* register pure mode of the psi table */
    result = _sample_tf_pure_psitable();
    if (result != vatek_result_success)
    {
        SAMPLE_ERR("vatek_broadcast_psitable_register fail: %d", result);
        return result;
    }else SAMPLE_LOG("PURE PSI Table OK");
#elif (PSI_TYPE==PSIDEF)
    /* set tsmux parm */
    result = vatek_transform_tsmux_setparm(tf_handle, tsmux_type_default);
    if (result != vatek_result_success)
    {
        SAMPLE_ERR("vatek_transform_tsmux_setparm fail: %d", result);
        return result;
    } 
    
    /* register default mode of the psi table */
    result = _smaple_tf_default_psitable();
    if(result != vatek_result_success)
    {
        SAMPLE_ERR("vatek_transform_default_psi fail: %d", result);
        return result;
    }else SAMPLE_LOG("DEFAULT PSI Table OK");
    
#elif (PSI_TYPE==PSIBYPASS)
    SAMPLE_LOG("PSI Table BYPASS");
#endif

    /* set modulator parm */
    result = _sample_setmodulator_parm();
    if (result != vatek_result_success)
    {
        SAMPLE_ERR("_sample_setmodulator_parm fail: %d", result);
        return result;
    }else SAMPLE_LOG("Modulator OK");
    
    /* step-3 : start rf */
    result = vatek_rf_start(rf_handle, rf_freq);
    if (result != vatek_result_success)
    {
        SAMPLE_ERR("vatek_rf_start fail: %d", result);
        return result;
    }else SAMPLE_LOG("RF OK");

    /* step-4 : start tranform */
    result = vatek_transform_start(tf_handle);
    if (result != vatek_result_success)
    {
        SAMPLE_ERR("vatek_transform_start fail: %d", result);
        return result;
    }

    SAMPLE_LOG("========= START TRANSFORM (all program) =========");

    return result;
}

vatek_result sample_tf_play_allprogram(void)
{
    vatek_result result = vatek_result_unknown;

    /* step-1 : lock tuner & demod signal */
    result = _sample_tf_lockfreq(LOCK_FREQ, LOCK_SYMBOL);
    if (result != vatek_result_success)
    {
        SAMPLE_ERR("sample_tf_lockfreq fail: %d", result);
        return result;
    }

    return _sample_tf_play_allprogram(RF_FREQ);
}

vatek_result sample_tf_stop(void)
{
    vatek_result result = vatek_result_unknown;
    broadcast_status status = bc_status_unknown;

    /* check broadcast status */
    result = vatek_transform_bcstatus(tf_handle, &status);
    if (result != vatek_result_success)
    {
        SAMPLE_ERR("vatek_transform_bcstatus fail : %d", result);
        return result;
    }

    if (status == bc_status_idle)
        return vatek_result_success;

    /* stop rf */
    result = vatek_rf_stop(rf_handle);
    if (result != vatek_result_success)
    {
        SAMPLE_ERR("vatek_rf_stop fail : %d", result);
        //return result;
    }

    /* stop transform */
    result = vatek_transform_stop(tf_handle);
    if (result != vatek_result_success)
    {
        SAMPLE_ERR("vatek_transform_stop fail : %d", result);
        return result;
    }

    return result;
}

vatek_result sample_tf_init(Phtransform bh_main, Pboard_handle bh_demod, Pboard_handle bh_tuner, Pboard_handle bh_rf)
{
    vatek_result result = vatek_result_unknown;

    /* init VATEK transform */
    result = vatek_transform_create(bh_main, &tf_handle);
    if (result != vatek_result_success)
    {
        SAMPLE_ERR("vatek_transform_create fail : %d", result);
        return result;
    }

    /* init demod : avl68xx */
    result = vatek_demod_create(bh_demod, demod_type_avl68xx, &demod_handle);  
    if (result != vatek_result_success)
    {
        SAMPLE_ERR("demod create fail : %d", result);
        return result;
    }

    /* init tuner : mxl608 (i2c via demod) */
    tuner_board_handle tbh;
    tbh.hboard = bh_tuner;
    tbh.hdemod = demod_handle;
    tbh.i2cbypass = vatek_demod_i2cbypass;
    result = vatek_tuner_create(&tbh, tuner_type_mxl608, &tuner_handle);  
    if (result != vatek_result_success)
    {
        SAMPLE_ERR("tuner create fail : %d", result);
        return result;
    }

    /* init RF : r2_via_vatek */
    result = vatek_rf_create(bh_rf, rf_type_r2_via_vatek, &rf_handle);  
    if (result != vatek_result_success)
    {
        SAMPLE_ERR("rf create fail : %d", result);
        return result;
    }

    return result;
}
