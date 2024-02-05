
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vatek_sample_transform.h"

#define NIM 0

#define ATSC            0
#define J83B            1
#define DVBT            2
#define ISDBT           3
#define J83A						4
#define MOD_TYPE        DVBT 

//unit KHz
#define LOCK_FREQ       473000 
#define LOCK_SYMBOL     6000
#define RF_FREQ         474000


#define PSIPURE         1
#define PSIDEF          2
#define PSIBYPASS       3
#define PSI_TYPE        PSIDEF

#define CMDLINE         1


static Phtuner tuner_handle = NULL;
static Phdemod demod_handle = NULL;
static Phtransform tf_handle = NULL;
static Phrf rf_handle = NULL;

uint8_t g_serial = 0;
uint8_t g_clk_inverse = 0;

#if (CMDLINE == 1)
#include "uart_cmdline.h"
// for cmdline use start
static vatek_result _cmdline_help(void);
static vatek_result _sample_tf_play_allprogram(uint32_t rf_freq);
static vatek_result _sample_tf_play_program(uint32_t rf_freq, uint8_t program_num);
static vatek_result _sample_tf_lockfreq(uint32_t freq_khz, uint32_t bandwidth_khz);
vatek_result sample_tf_stop(void);
vatek_result sample_tf_statusmsg(void);
static cmdline_table cmdtable[] =
{
    {"tfhelp", (void *)_cmdline_help, 0},
    {"tfstop", (void *)sample_tf_stop, 0},
    {"tfplay", (void *)_sample_tf_play_program, 2},
    {"tfplayall", (void *)_sample_tf_play_allprogram, 1},
    {"tflock", (void *)_sample_tf_lockfreq, 2},
    {"tfmsg", (void *)sample_tf_statusmsg, 0},
};
static vatek_result _cmdline_help(void)
{
    SAMPLE_LOG("- tfstop");
    SAMPLE_LOG("\t stop transform");
    SAMPLE_LOG("\t example: tfstop");
    SAMPLE_LOG("");
    SAMPLE_LOG("- tfplay rf_freq_khz program_num");
    SAMPLE_LOG("\t start transform for specific program");
    SAMPLE_LOG("\t example: tfplay 473000 0");
    SAMPLE_LOG("");
    SAMPLE_LOG("- tfplayall rf_freq_khz");
    SAMPLE_LOG("\t start transform");
    SAMPLE_LOG("\t example: tfplayall 473000");
    SAMPLE_LOG("");
    SAMPLE_LOG("- tflock freq_khz symbol_khz");
    SAMPLE_LOG("\t lock tuner");
    SAMPLE_LOG("\t example: tflock 473000 6000");
    SAMPLE_LOG("");
    SAMPLE_LOG("- tfmsg");
    SAMPLE_LOG("\t print broadcast and chipset status");
    SAMPLE_LOG("\t example: tfmsg");
    SAMPLE_LOG("");
    return vatek_result_success;
}
// for cmdline use end
#endif

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
    mod_base_parm.bw_sb = 8;
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
		
		#elif (MOD_TYPE == J83A) //J83A
		modulator_base_parm mod_base_parm = {0};
		mod_base_parm.bw_sb = 5720;
		mod_base_parm.dacgain = 0;
		mod_base_parm.iffreq = 0;
		mod_base_parm.ifmode = m_ifmode_disable;
		mod_base_parm.type = m_type_j83a;
		modulator_j83a_parm j83a_parm = {0};
		j83a_parm.constellation = j83a_constellation_qam256;
		result = vatek_transform_modulator_setparm(tf_handle, mod_base_parm, (Pmodulator_j83a_parm)&j83a_parm);
		if(result != vatek_result_success)
		{
			SAMPLE_ERR("vatek_transform_modulator_setparm j83a fail: %d", result);
      return result;
		}
    #endif

    return result;
}

static vatek_result _sample_tf_enum(Penum_list *list)
{
    vatek_result result = vatek_result_success;

    /* step-1 : set tsp input parm (from ts) */
    ts_input_parm ti_parm = {0};
    ti_parm.serial = g_serial;
    ti_parm.clk_inverse = g_clk_inverse;
    result = vatek_transform_tsp_setinputparm_ts(tf_handle, ti_parm);
    if (result != vatek_result_success)
    {
        SAMPLE_ERR("vatek_transform_tsp_setinputparm_ts fail: %d", result);
        return result;
    }

    /* step-2 : enum list */
#if 1
    result = vatek_transform_enum_getlist(tf_handle, list);
    if (result != vatek_result_success)
    {
        SAMPLE_ERR("vatek_transform_enum_getlist fail: %d", result);
        return result;
    }

    

    /* dump enum list */
    uint16_t idx = 0, jdx = 0;
    SAMPLE_LOG("========= ENUM LIST =========");
    SAMPLE_LOG("enum_list.program_num = %ld",(*list)->program_num);
    for (idx = 0; idx < (*list)->program_num; idx++)
    {
         SAMPLE_LOG("\tprogram[%d] pcr_pid = %lX", idx, (*list)->program[idx].pcr_pid);
         SAMPLE_LOG("\tprogram[%d] pmt_pid = %lX", idx, (*list)->program[idx].pmt_pid);
         SAMPLE_LOG("\tprogram[%d] stream_num = %ld", idx, (*list)->program[idx].stream_num);

         for (jdx = 0; jdx < (*list)->program[idx].stream_num; jdx++)
         {
             SAMPLE_LOG("\t\tstream[%d] type = %X", jdx, (*list)->program[idx].stream[jdx].type);
             SAMPLE_LOG("\t\tstream[%d] encode_type = %lX", jdx, (*list)->program[idx].stream[jdx].encode_type);
             SAMPLE_LOG("\t\tstream[%d] stream_pid = %lX", jdx, (*list)->program[idx].stream[jdx].stream_pid);
             SAMPLE_LOG("\t\tstream[%d] stream_type = %lX", jdx, (*list)->program[idx].stream[jdx].stream_type);
             SAMPLE_LOG("\t\tstream[%d] esinfo_len = %lX", jdx, (*list)->program[idx].stream[jdx].esinfo_len);
         }
    }
#endif

    return result;
}


static vatek_result _sample_tf_lockfreq(uint32_t freq_khz, uint32_t bandwidth_khz)
{
    vatek_result result = vatek_result_unknown;
    uint32_t timeout = 15000;
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
    prog.pcr_pid = 0x111;
    prog.pmt_pid = 0x300;
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
        v_stream.stream_pid = 0x301;
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
        aparam.type = ae_type_mp1_l2;
        aparam.channel = ae_channel_stereo;
        aparam.samplerate = ai_samplerate_48K;
        
        //Add stream parameter.
        psispec_default_stream_tr a_stream;
        a_stream.type = psi_def_stream_type_audio;
        a_stream.stream_pid = 0x311;
        a_stream.es_type = 0x03;
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

static vatek_result _sample_tf_capture(void)
{
    vatek_result result = vatek_result_unknown;
    
    /* step-1 : set tsp input parm (from ts) */
    ts_input_parm ti_parm = {0};
    ti_parm.serial = g_serial;
    ti_parm.clk_inverse = g_clk_inverse;
    result = vatek_transform_tsp_setinputparm_ts(tf_handle, ti_parm);
    if (result != vatek_result_success)
    {
        SAMPLE_ERR("vatek_transform_tsp_setinputparm_ts fail: %d", result);
        return result;
    }
#if 1
    /* step-2 : capture table (from ts) */
    Ppsitable_parm table = NULL;
    capture_param capture = {0};
    capture.pid = 0x60;
    capture.table_id = 0x40;
    capture.section_num = 1;
    result = vatek_transform_capture(tf_handle, &table, &capture);
    if (result != vatek_result_success)
    {
        SAMPLE_ERR("vatek_transform_capture fail: %d", result);
        return result;
    }
#endif
    
    // print table.
//    printf("PMT table = %c",table->tspackets[0]);
    return result;
}

static vatek_result _sample_tf_play_program(uint32_t rf_freq, uint8_t program_num)
{
    vatek_result result = vatek_result_unknown;

    /* step-2 : enum all program list */
    Penum_list list = NULL;
    result = _sample_tf_enum(&list);
    if (result != vatek_result_success)
    {
        SAMPLE_ERR("_sample_tf_enum fail: %d", result);
        return result;
    }

    /* step-3.1 : set program filter from enum */
    /* only pass the program_num's video pid / audio pid / pcr pid */
    tsp_filter_parm filter_parm = {0};
    uint16_t idx = 0;
    filter_parm.filter_num = 0;
//		filter_parm.filter[filter_parm.filter_num++].pid = 0x301; //test
//		filter_parm.filter[filter_parm.filter_num++].pid = 0x311; //test
    filter_parm.filter[filter_parm.filter_num++].pid = list->program[program_num].pcr_pid;  //pcr pid
    for (idx = 0; idx < list->program[program_num].stream_num; idx++)
    {
        
        if (list->program[program_num].stream[idx].type == stream_type_video || 
            list->program[program_num].stream[idx].type == stream_type_audio)
        {
            /* use original stream pid */
            filter_parm.filter[filter_parm.filter_num++].pid = list->program[program_num].stream[idx].stream_pid; //audio & video pid
        }
        else
        {
            /* drop other stream type */
            ;
        }
    } 
    result = vatek_transform_tsp_setfilterparm(tf_handle, filter_parm);
    if (result != vatek_result_success)
    {
        SAMPLE_ERR("vatek_transform_tsp_setfilterparm fail: %d", result);
        return result;
    }

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
//    /* step-3.2 : re-gen PAT & PMT, set PSITABLE parameter*/
//    static uint8_t PAT[188] = {
//			0x47,0x40,0x00,0x11,0x00,0x00,0xB0,0x0D,0x00,0x02, //10 per line
//			0xC3,0x00,0x00,0x00,0x01,0xE0,0x11,0x00,0x7C,0xEC,
//			0x1A,0xFF
//		};
//		static uint8_t PMT[188] = {
//			0x47,0x40,0x11,0x1C,0x00,0x02,0xB0,0x17,0x00,0x01,
//			0xC3,0x00,0x00,0xE1,0x00,0xF0,0x00,0x02,0xF0,0x02,
//			0xF0,0x00,0x81,0xF0,0x03,0xF0,0x00,0x33,0xAC,0x4E,
//			0x7C,0xFF
//		};
//    _sample_gen_pat(PAT, program_num+1, list->program[program_num].pmt_pid);
//    _sample_gen_pmt(PMT, program_num+1, &list->program[program_num]);
//    psitablelist_parm psi_parm = {0};
//    psi_parm.table_num = 2;
//    psi_parm.table[0].interval_ms = 500;
//    psi_parm.table[0].tspacket_num = 1;
//    psi_parm.table[0].tspackets = PAT;
//    psi_parm.table[1].interval_ms = 500;
//    psi_parm.table[1].tspacket_num = 1;
//    psi_parm.table[1].tspackets = PMT;
//    result = vatek_transform_psitable_register(tf_handle, &psi_parm);
//    if (result != vatek_result_success)
//    {
//        SAMPLE_ERR("vatek_transform_psitable_register fail: %d", result);
//        return result;
//    }

//    /* set tsmux parm */
//    tsmux_pure_parm pure_parm = {0};
//    pure_parm.padding_pid = 0x1FFF;
//    result = vatek_transform_tsmux_setparm(tf_handle, tsmux_type_pure);
//    if (result != vatek_result_success)
//    {
//        SAMPLE_ERR("vatek_transform_tsmux_setparm fail: %d", result);
//        return result;
//    }
#elif (PSI_TYPE==PSIDEF)
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

#endif

    /* step-4 : set tranform setting */
    /* set tsp input parm (from ts) */
    ts_input_parm ti_parm = {0};
    ti_parm.serial = g_serial;
    ti_parm.clk_inverse = g_clk_inverse;
		ti_parm.pcr_mode = pcr_retag;
    result = vatek_transform_tsp_setinputparm_ts(tf_handle, ti_parm);
    if (result != vatek_result_success)
    {
        SAMPLE_ERR("vatek_transform_tsin_setparm fail: %d", result);
        return result;
    }

    /* set modulator parm */
    result = _sample_setmodulator_parm();
    if (result != vatek_result_success)
    {
        SAMPLE_ERR("_sample_setmodulator_parm fail: %d", result);
        return result;
    }

    /* step-5 : start rf */
    result = vatek_rf_start(rf_handle, rf_freq);
    if (result != vatek_result_success)
    {
        SAMPLE_ERR("vatek_rf_start fail: %d", result);
        return result;
    }

    /* step-6 : start transform */
    result = vatek_transform_start(tf_handle);
    if (result != vatek_result_success)
    {
        SAMPLE_ERR("vatek_transform_start fail: %d", result);
        return result;
    }
		broadcast_status a3_s = bc_status_unknown;
		while(a3_s != bc_status_wait_source){
			vatek_transform_bcstatus(tf_handle, &a3_s);
		}
		uint32_t a_tick = vatek_system_gettick();
		while(vatek_system_gettick() - a_tick < 10000){
			printf("count down 10 second.\r\n");
		}
		result = _sample_tf_lockfreq(LOCK_FREQ, LOCK_SYMBOL);
    if (result != vatek_result_success)
    {
        SAMPLE_ERR("sample_tf_lockfreq fail: %d", result);
        return result;
    }

    SAMPLE_LOG("========= START TRANSFORM (program[%d]) =========",program_num);

    return result;
}

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

vatek_result sample_tf_capture(void)
{
    vatek_result result = vatek_result_unknown;

#if NIM
    /* step-1 : lock tuner & demod signal */
    result = _sample_tf_lockfreq(LOCK_FREQ, LOCK_SYMBOL);
    if (result != vatek_result_success)
    {
        SAMPLE_ERR("sample_tf_lockfreq fail: %d", result);
        return result;
    }
#endif
	
    return _sample_tf_capture();
}

vatek_result sample_tf_play_program(uint8_t program_num)
{
    vatek_result result = vatek_result_unknown;

#if NIM
    /* step-1 : lock tuner & demod signal */
    result = _sample_tf_lockfreq(LOCK_FREQ, LOCK_SYMBOL);
    if (result != vatek_result_success)
    {
        SAMPLE_ERR("sample_tf_lockfreq fail: %d", result);
        return result;
    }
#endif
	
    return _sample_tf_play_program(RF_FREQ, program_num);
}

vatek_result sample_tf_play_allprogram(void)
{
    vatek_result result = vatek_result_unknown;
#if NIM
    /* step-1 : lock tuner & demod signal */
    result = _sample_tf_lockfreq(LOCK_FREQ, LOCK_SYMBOL);
    if (result != vatek_result_success)
    {
        SAMPLE_ERR("sample_tf_lockfreq fail: %d", result);
        return result;
    }
#endif
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
    
    #if (CMDLINE == 1)
    // for cmdline use
    uart_cmdline_init(cmdtable, sizeof(cmdtable)/sizeof(cmdline_table));
    #endif

    /* init VATEK transform */
    result = vatek_transform_create(bh_main, &tf_handle);
    if (result != vatek_result_success)
    {
        SAMPLE_ERR("vatek_transform_create fail : %d", result);
        return result;
    }
#if NIM
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
#endif
    /* init RF : r2_via_vatek */
    result = vatek_rf_create(bh_rf, rf_type_r2_via_vatek, &rf_handle);  
    if (result != vatek_result_success)
    {
        SAMPLE_ERR("rf create fail : %d", result);
        return result;
    }

    return result;
}

vatek_result sample_tf_statusmsg(void)
{
    vatek_result result = vatek_result_unknown;
    chip_status s_chip = chip_status_unknown;
    broadcast_status s_bc = bc_status_unknown;

    result = vatek_transform_chipstatus(tf_handle, &s_chip);
    if (result != vatek_result_success)
    {
        SAMPLE_ERR("vatek_transform_chipstatus fail : %d", result);
        return result;
    }
    SAMPLE_LOG("chip status : %d", s_chip);
    
    result = vatek_transform_bcstatus(tf_handle, &s_bc);
    if (result != vatek_result_success)
    {
        SAMPLE_ERR("vatek_transform_chipstatus fail : %d", result);
        return result;
    }
    SAMPLE_LOG("broadcast status : %d", s_bc);

    return result;
}

vatek_result sample_tf_polling(void)
{
    #if (CMDLINE == 1)
    // for cmdline use
    uart_cmdline_polling();
    #endif
    
    return vatek_result_success;
}
