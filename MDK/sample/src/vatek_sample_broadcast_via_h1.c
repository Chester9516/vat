
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vatek_sample_broadcast_via_h1.h"
#include "vatek_api.h"
#include "uart_cmdline.h"
#include "vatek_epg.h"
#include "vatek_rtcimpl.h"

#define _AC(ch) (ch+0x80)
#define PHY_SCALE_720P 0

#define SECOND_PHY  0 // this sample case is CVBS output
#define CVBS_CC     0 // this sample case is SECOND_PHY's closed caption

#define ATSC        0
#define DVBT        1
#define ISDBT       2 
#define J83A        3
#define J83B        4
#define MOD_TYPE    ATSC

#define PURE				0
#define DEFAULT			1
#define RULE				2
#define ISO					3
#define PSI_MODE		DEFAULT

#if PSI_MODE == RULE
	#define EPG_EN 1
#else
	#define EPG_EN 0
#endif

extern vatek_result vatek_phy_dump_reg(Phphy phy_handle);
extern vatek_result vatek_h1_change_clk(Phphy phy_handle);

extern int get_action_value();
/*EPG test*/
extern vatek_result xapp_polling_check_running(Phms_handle pi2c);
extern vatek_result vatek_epg_set_schedule();

static Phbroadcast bc_handle = NULL;
static Phphy hdmi_handle = NULL;
static Phphy cvbs_handle = NULL;
static Phphy phy_active_handle = NULL;
static Phrf rf_handle = NULL;
static phy_video_info g_phy_vi = {0};
static phy_audio_info g_phy_ai = {0};

uint8_t PAT[188] = 
{
	0x47, 0x40, 0x00, 0x12, 0x00, 0x00, 0xB0, 0x11, 0x00, 0x64, 0xC5, 0x00, 0x00, 0x00, 0x00, 0xE0,
	0x10, 0x00, 0x01, 0xE0, 0x20, 0x20, 0xE5, 0xF9, 0x22, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF          
};

uint8_t PMT[188] = 
{
0x47, 0x40, 0x20, 0x12, 0x00, 0x02, 0xB0, 0x3C, 0x00, 0x01, 0xC5, 0x00, 0x00, 0xE0, 0x32, 0xF0,
0x06, 0x05, 0x04, 0x47, 0x41, 0x39, 0x34, 0x02, 0xE0, 0x30, 0xF0, 0x09, 0x86, 0x07, 0xE1, 0x65,
0x6E, 0x67, 0xC1, 0x3F, 0xFF, 0x81, 0xE0, 0x31, 0xF0, 0x16, 0x05, 0x04, 0x41, 0x43, 0x2D, 0x33,
0x81, 0x08, 0x08, 0xB8, 0x05, 0xFF, 0x1F, 0x00, 0x00, 0x00, 0x0A, 0x04, 0x65, 0x6E, 0x67, 0x00,
0x88, 0x75, 0x5F, 0xE2, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF  
};

uint8_t MGT[376] = 
//uint8_t MGT[188] = 
{
0x47, 0x5F, 0xFB, 0x18, 0x00, 0xC7, 0xF0, 0x19, 0x00, 0x00, 0xC5, 0x00, 0x00, 0x00, 0x00, 0x01,
0x00, 0x00, 0xFF, 0xFB, 0xE2, 0x00, 0x00, 0x00, 0x02, 0xF0, 0x00, 0xF0, 0x00, 0xD4, 0x84, 0x05,
0xB5, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF 
#if 1
,
0x47, 0x5F, 0xFB, 0x19, 0x00, 0xC8, 0xF0, 0x3E, 0x00, 0x64, 0xC5, 0x00, 0x00, 0x00, 0x01, 0x00,
0x48, 0x00, 0x44, 0x00, 0x31, 0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x1C, 0x01,
0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x64, 0x00, 0x01, 0x0D, 0xC2, 0x00, 0x01, 0xFC, 0x11, 0xA1,
0x0F, 0xE0, 0x32, 0x02, 0x02, 0xE0, 0x30, 0x00, 0x00, 0x00, 0x81, 0xE0, 0x31, 0x65, 0x6E, 0x67,
0xFC, 0x00, 0x6D, 0x48, 0x36, 0xA9, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF   
#endif
};

uint8_t TVCT[188] = 
{
0x47, 0x5F, 0xFB, 0x19, 0x00, 0xC8, 0xF0, 0x3E, 0x00, 0x64, 0xC5, 0x00, 0x00, 0x00, 0x01, 0x00,
0x48, 0x00, 0x44, 0x00, 0x31, 0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x1C, 0x01,
0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x64, 0x00, 0x01, 0x0D, 0xC2, 0x00, 0x01, 0xFC, 0x11, 0xA1,
0x0F, 0xE0, 0x32, 0x02, 0x02, 0xE0, 0x30, 0x00, 0x00, 0x00, 0x81, 0xE0, 0x31, 0x65, 0x6E, 0x67,
0xFC, 0x00, 0x6D, 0x48, 0x36, 0xA9, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF   
};

uint8_t NULL_PACKET[188] = 
{
0x47, 0x50, 0x50, 0x12, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF  
};

static vatek_result _sample_setmodulator_parm(void)
{
    vatek_result result = vatek_result_unknown;
//    int mod_val = get_action_value();
//		printf("mod val = %d\r\n",mod_val);
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
    mod_base_parm.ifmode = m_ifmode_iq_offset;
    mod_base_parm.iffreq = 143;
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
#if 1
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
    video_encode_parm ve_parm = {0,};
    audio_encode_parm ae_parm = {0,};
    ve_parm.type = ve_type_mpeg2;//ve_type_mpeg2; ve_type_h264
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
    em_parm.video_pid = 48;
    em_parm.audio_pid = 49;
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
#if (PSI_MODE == RULE)
		/* set EPG test */
		tsmux_pure_parm pure_parm = {0};
    pure_parm.pcr_pid = 0x50;
		pure_parm.en_function = 0;
		pure_parm.padding_pid = 0x1fff;
		result = vatek_broadcast_tsmux_setparm(bc_handle, tsmux_type_pure, &pure_parm);//tsmux_type_rule, tsmux_type_pure
		
		PVAT_RTC_DATETIME time = VATRTCGetTime(RTC_TIMEFMT_BIN);
		result = vatek_epg_sample(bc_handle);
		if(is_success(result)){
			result = xapp_polling_check_running(bc_handle);
			if(is_success(result)){
				printf("rtc set success\r\n");
			}
			else{
				printf("rtc set fail\r\n");
			}
		}
		else{
			printf("epg set fail\r\n");
			return result;
		}
#elif (PSI_MODE == DEFAULT)
		tsmux_default_parm rule_parm = {0};
    rule_parm.pcr_pid = 50;
		rule_parm.en_function = 0;
		rule_parm.padding_pid = 0x1fff;
		rule_parm.pmt_pid = 0x1001;
		result = vatek_broadcast_tsmux_setparm(bc_handle, tsmux_type_default, &rule_parm);//tsmux_type_rule, tsmux_type_pure
	#if (MOD_TYPE == DVBT)
		static uint8_t _dvb_service_name[] = "ASTek";//_DTV_RF_OUTPUT
		static uint8_t _dvb_network_name[] = "ASTEK";
		vatek_string service_name = {
			.len = sizeof(_dvb_service_name),
			.text = (uint8_t *)_dvb_service_name,
		};
		vatek_string network_name = {
			.len = sizeof(_dvb_network_name),
			.text = (uint8_t *)_dvb_network_name,
		};
		psispec_default_dvb_channel ch = {0};
		psispec_default_dvb_program pr = {0};
		ch.network_id = 1;
		ch.network_name = &network_name;
		ch.transport_stream_id = 2;
		pr.channel_no = 1;
		pr.original_network_id = 1;
		pr.program_number = 0x20;
		pr.service_name = &service_name;
		uint32_t i = 0;

		result = vatek_broadcast_psispec_default_init(bc_handle,psispec_default_dvb,dvb_taiwan);
		if(result != vatek_result_success){
			printf("dvb default init fail %d\r\n",result);
			return result;
		}
		
		result = vatek_broadcast_psispec_default_config(&ch,&pr);
		if(result != vatek_result_success){
			printf("dvb default config fail %d\r\n",result);
			return result;
		}
		result = vatek_broadcast_psispec_default_start();
		if(result != vatek_result_success){
			printf("dvb default start fail %d\r\n",result);
			return result;
		}
	#elif (MOD_TYPE == ISDBT)
	#if 1
	#if !EPG_EN
		static const uint8_t _arib_network_name[] = {0x1b,0x7e,_AC('V'),_AC('A'),_AC('T'),_AC('E'),_AC('K')};
		static const uint8_t _arib_service_name[] = {0x1b,0x7e,_AC('v'),_AC('a'),_AC('t'),_AC('e'),_AC('k')};
		static const uint8_t _arib_ts_name[] = {0x1b,0x7e,_AC('J'),_AC('A'),_AC('P'),_AC('A'),_AC('N')};
		
		vatek_string network_name = {
			.len = sizeof(_arib_network_name),
			.text = (uint8_t *)_arib_network_name,
		};
		vatek_string service_name = {
			.len = sizeof(_arib_service_name),
			.text = (uint8_t *)_arib_service_name,
		};
		vatek_string ts_name = {
			.len = sizeof(_arib_ts_name),
			.text = (uint8_t *)_arib_ts_name,
		};
		psispec_default_arib_channel arib_ch = {
			.region_id = 1,
			.network_name = &network_name,
			.broadcaster_id = 1,
			.remote_control_key_id = 2,
		};
		psispec_default_arib_program arib_pr = {	
			.service_no = 7,
			.copy_flag = arib_abnt_free,
			.ts_name = &ts_name,
			.service_name = &service_name,
			.main_lang.raw = { 'j','p','n','\0'},
			.sub_lang.raw = {'e','n','g','\0'},
		};
		result = vatek_broadcast_psispec_default_init(bc_handle,psispec_default_arib,arib_japan);
		if(result != vatek_result_success){
			printf("abnt default init fail %d\r\n",result);
			return result;
		}
		
		result = vatek_broadcast_psispec_default_config(&arib_ch,&arib_pr);
		if(result != vatek_result_success){
			printf("abnt default config fail %d\r\n",result);
			return result;
		}
		
		result = vatek_broadcast_psispec_default_start();
		if(result != vatek_result_success){
			printf("abnt default start fail %d\r\n",result);
			return result;
		}
	#endif
	#else
		static const uint8_t _abnt_network_name[] = {0x1b,0x7e,_AC('V'),_AC('A'),_AC('T'),_AC('E'),_AC('K')};
		static const uint8_t _abnt_service_name[] = {0x1b,0x7e,_AC('v'),_AC('a'),_AC('t'),_AC('e'),_AC('k')};
		static const uint8_t _abnt_ts_name[] = {0x1b,0x7e,_AC('J'),_AC('A'),_AC('P'),_AC('A'),_AC('N')};

		vatek_string network_name = {
			.len = sizeof(_abnt_network_name),
			.text = (uint8_t *)_abnt_network_name,
		};
		vatek_string service_name = {
			.len = sizeof(_abnt_service_name),
			.text = (uint8_t *)_abnt_service_name,
		};
		vatek_string ts_name = {
			.len = sizeof(_abnt_ts_name),
			.text = (uint8_t *)_abnt_ts_name,
		};
		psispec_default_abnt_channel abnt_ch = {
			.area_code = 1,
			.network_name = &network_name,
			.original_network_id = 1,
			.remote_control_key_id = 2,
		};
		psispec_default_abnt_program abnt_pr = {	
			.service_no = 7,
			.copy_flag = arib_abnt_free,
			.ts_name = &ts_name,
			.service_name = &service_name,
			.main_lang.raw = { 'j','p','n','\0'},
			.sub_lang.raw = {'e','n','g','\0'},
		};
		result = vatek_broadcast_psispec_default_init(bc_handle,psispec_default_abnt,abnt_brazil);
		if(result != vatek_result_success){
			printf("abnt default init fail %d\r\n",result);
			return result;
		}
		
		result = vatek_broadcast_psispec_default_config(&abnt_ch,&abnt_pr);
		if(result != vatek_result_success){
			printf("abnt default config fail %d\r\n",result);
			return result;
		}
		
		result = vatek_broadcast_psispec_default_start();
		if(result != vatek_result_success){
			printf("abnt default start fail %d\r\n",result);
			return result;
		}
	#endif
	#elif (MOD_TYPE == ATSC)
		static const uint8_t _psip_short_name[] = {0x00,'V',0x00,'A',0x00,'T',0x00,'E',0x00,'K',0x00,0x00,};
		static const uint8_t _psip_long_name[] = {0x01,'e','n','g',0x01,0x00,0x00,0x04,'V','A','T','V',0x00,0x00,0x00,0x00};
		vatek_string short_name = {
			.len = sizeof(_psip_short_name),
			.text = (uint8_t *)&_psip_short_name,
		};
		vatek_string long_name = {
			.len = sizeof(_psip_long_name),
			.text = (uint8_t *)&_psip_long_name,
		};
		
		result = vatek_broadcast_psispec_default_init(bc_handle,psispec_default_psip,atsc_usa);
		if(result != vatek_result_success){
			printf("psip default init fail %d\r\n",result);
			return result;
		}
		psispec_default_psip_channel ch = {0};
		psispec_default_psip_program pr = {0};
		ch.cc_mode = cc_mode_608;
		ch.daylight_saving = 0;
		ch.gps_utc_offset = 0;
		ch.psip_flags = 0;
		ch.transport_stream_id = 1;
		ch.short_name = &short_name;

		pr.channel_major = 31;
		pr.channel_minor = 1;
		pr.long_name = &long_name;
		pr.program_number = 0x01;
		pr.source_id = 2;
		result = vatek_broadcast_psispec_default_config(&ch,&pr);
		if(result != vatek_result_success){
			printf("psip default config fail %d\r\n",result);
			return result;
		}
		result = vatek_broadcast_psispec_default_start();
		if(result != vatek_result_success){
			printf("psip default start fail %d\r\n",result);
			return result;
		}
	#endif
#elif (PSI_MODE == PURE)
		psitablelist_parm psi_p = {0};
		psi_p.table_num = 3;
		psi_p.table[0].interval_ms = 90;
		psi_p.table[0].tspackets = &PAT[0];
		psi_p.table[0].tspacket_num = 1;
		psi_p.table[1].interval_ms = 90;
		psi_p.table[1].tspackets = &PMT[0];
		psi_p.table[1].tspacket_num = 1;
		psi_p.table[2].interval_ms = 120;
		psi_p.table[2].tspackets = &MGT[0];
		psi_p.table[2].tspacket_num = 2;
		result = vatek_broadcast_psitable_register(bc_handle,&psi_p);
#elif (PSI_MODE == ISO)
		/*Setting iso-13818-1 mode parmeter.*/
    psispec_default_iso13818_channel channel = 
    {
        .transport_stream_id = 1,
    };
    psispec_default_iso13818_program program = 
    {
        .program_number = 0x10,
    };

    //Register PSI table.
    result = vatek_broadcast_psispec_default_init( bc_handle, psispec_default_iso, psispec_country_undefined);
    if(is_success(result)) result = vatek_broadcast_psispec_default_config( &channel, &program);
    if(is_success(result)) result = vatek_broadcast_psispec_default_start();
    if(!is_success(result))
    {
        SAMPLE_ERR("vatek_broadcast_psispec_default_start fail: %d", result);
        return result;
    }
#endif

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
		
		/* start RF */
    result = vatek_rf_start(rf_handle, 473000);//473000
    if (!is_success(result))
    {
        SAMPLE_ERR("vatek_rf_start fail: %d", result);
        return result;
    }

    /* start BROADCAST */
    result = vatek_broadcast_start(bc_handle);
    if (!is_success(result))
    {
        SAMPLE_ERR("vatek_broadcast_start fail: %d", result);
        return result;
    }
#endif

    SAMPLE_LOG("broadcast ColorBar");

    return result;
}

vatek_result sample_bc_broadcast_from_phy(void)
{ 
    // broadcast (phy) ==> (VATEK) ==> (rf)
    // use default psi, psi table via DEFAULTPSI API (vatek_broadcast_psispec_default_init/config/start)
    vatek_result result = vatek_result_unknown;

    /* get PHY video information */
		phy_video_info temp_phy_vi = {0};
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
		
		memcpy(&temp_phy_vi, &phy_vi, sizeof(phy_video_info));
		
  #if PHY_SCALE_720P
		result = vatek_phy_setvideoinfo(phy_active_handle, &temp_phy_vi);
		if(!is_success(result))
		{
			SAMPLE_ERR("vatek_phy_setvideoinfo fail: %d", result);
      return result;
		}
		SAMPLE_LOG("Video down scale 1080P to 720P");
		SAMPLE_LOG("scale resolution = %d", temp_phy_vi.resolution);
    SAMPLE_LOG("scale aspectrate = %d", temp_phy_vi.aspectrate);
    SAMPLE_LOG("scale buswidth_16 = %d", temp_phy_vi.buswidth_16);
    SAMPLE_LOG("scale separated_sync = %d", temp_phy_vi.separated_sync);
	#endif
		
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
    vi_parm.resolution = temp_phy_vi.resolution;
    vi_parm.aspectrate = temp_phy_vi.aspectrate;
    vi_parm.buswidth_16 = temp_phy_vi.buswidth_16;
    vi_parm.separated_sync = temp_phy_vi.separated_sync;
		vi_parm.ext_half_fps = 0;
		vi_parm.clk_inverse = 0;
		vi_parm.vsync_inverse = 0;
		vi_parm.offset_y = 0;
		vi_parm.field_inverse = 1;//if input = interlaced from H1, need to enable.
		
    ai_parm.samplerate = phy_ai.samplerate;
    result = vatek_broadcast_encoder_setinputparm_phy(bc_handle, vi_parm, ai_parm);
    if (!is_success(result))
    {
        SAMPLE_ERR("vatek_broadcast_encoder_setinputparm_phy fail: %d", result);
        return result;
    }

    /* set ENCODER encode parameter */
    video_encode_parm ve_parm = {0};
    audio_encode_parm ae_parm = {0};
    ve_parm.type = ve_type_mpeg2;//ve_type_mpeg2, ve_type_h264
		ve_parm.mux_bitrate = 0;//19000000;
		ve_parm.en_qcost = 1;
		ve_parm.en_drop_frame = 0;
		ve_parm.en_interlaced = 0;
		ve_parm.progressive_2_i = 0;
    ae_parm.type = ae_type_mp1_l2;//ae_type_mp1_l2, ae_type_ac_3, ae_type_aac_lc_adts
    ae_parm.channel = ae_channel_stereo;//ae_channel_stereo, ae_channel_mono_l, ae_channel_mono_r
    result = vatek_broadcast_encoder_setencodeparm(bc_handle, ve_parm, ae_parm);
    if (!is_success(result))
    {
        SAMPLE_ERR("vatek_broadcast_encoder_setencodeparm fail: %d", result);
        return result;
    }

    /* set ENCODER pes/ts mux parameter */
    encoder_mux_parm em_parm = {0};
    em_parm.video_pid = 48;
    em_parm.audio_pid = 49;
    result = vatek_broadcast_encoder_setmuxparm(bc_handle, em_parm);
    if (!is_success(result))
    {
        SAMPLE_ERR("vatek_broadcast_encoder_setmuxparm fail: %d", result);
        return result;
    }

    /* set ENCODER quality parameter */
    encoder_quality_parm q_parm = {0};
    q_parm.rcmode = q_rcmode_vbr;//q_rcmode_vbr, q_rcmode_unknown
    q_parm.minq = 5;
    q_parm.maxq = 15;
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
#if (PSI_MODE == RULE)
		/* set EPG test */
		tsmux_pure_parm pure_parm = {0};
    pure_parm.pcr_pid    = 0x50;
		pure_parm.en_function = 0;
		pure_parm.padding_pid = 0x1fff;
		
		result = vatek_broadcast_tsmux_setparm(bc_handle, tsmux_type_pure, &pure_parm);
		PVAT_RTC_DATETIME time = VATRTCGetTime(RTC_TIMEFMT_BIN);
		result = vatek_epg_sample(bc_handle);
		if(is_success(result)){
			result = xapp_polling_check_running(bc_handle);
			if(is_success(result)){
				printf("rtc set success\r\n");
			}
			else{
				printf("rtc set fail\r\n");
			}
		}
		else{
			printf("epg set fail\r\n");
			return result;
		}
#elif (PSI_MODE == DEFAULT)
		tsmux_default_parm rule_parm = {0};
    rule_parm.pcr_pid    = 50;
		rule_parm.en_function = 0;
		rule_parm.padding_pid = 0x1fff;
		rule_parm.pmt_pid = 0x1001;
		result = vatek_broadcast_tsmux_setparm(bc_handle, tsmux_type_default, &rule_parm);//tsmux_type_rule, tsmux_type_pure
	#if (MOD_TYPE == DVBT)
		static uint8_t _dvb_service_name[] = "ASTek";//_DTV_RF_OUTPUT
		static uint8_t _dvb_network_name[] = "ASTEK";
		vatek_string service_name = {
			.len = sizeof(_dvb_service_name),
			.text = (uint8_t *)_dvb_service_name,
		};
		vatek_string network_name = {
			.len = sizeof(_dvb_network_name),
			.text = (uint8_t *)_dvb_network_name,
		};
		psispec_default_dvb_channel ch = {0};
		psispec_default_dvb_program pr = {0};
		ch.network_id = 1;
		ch.network_name = &network_name;
		ch.transport_stream_id = 2;
		pr.channel_no = 1;
		pr.original_network_id = 1;
		pr.program_number = 0x20;
		pr.service_name = &service_name;
		uint32_t i = 0;

		result = vatek_broadcast_psispec_default_init(bc_handle,psispec_default_dvb,dvb_taiwan);
		if(result != vatek_result_success){
			printf("dvb default init fail %d\r\n",result);
			return result;
		}
		
		result = vatek_broadcast_psispec_default_config(&ch,&pr);
		if(result != vatek_result_success){
			printf("dvb default config fail %d\r\n",result);
			return result;
		}
		result = vatek_broadcast_psispec_default_start();
		if(result != vatek_result_success){
			printf("dvb default start fail %d\r\n",result);
			return result;
		}
	#elif (MOD_TYPE == ISDBT)
	#if 1
	#if !EPG_EN
		static const uint8_t _arib_network_name[] = {0x1b,0x7e,_AC('V'),_AC('A'),_AC('T'),_AC('E'),_AC('K')};
		static const uint8_t _arib_service_name[] = {0x1b,0x7e,_AC('v'),_AC('a'),_AC('t'),_AC('e'),_AC('k')};
		static const uint8_t _arib_ts_name[] = {0x1b,0x7e,_AC('J'),_AC('A'),_AC('P'),_AC('A'),_AC('N')};
		
		vatek_string network_name = {
			.len = sizeof(_arib_network_name),
			.text = (uint8_t *)_arib_network_name,
		};
		vatek_string service_name = {
			.len = sizeof(_arib_service_name),
			.text = (uint8_t *)_arib_service_name,
		};
		vatek_string ts_name = {
			.len = sizeof(_arib_ts_name),
			.text = (uint8_t *)_arib_ts_name,
		};
		psispec_default_arib_channel arib_ch = {
			.region_id = 1,
			.network_name = &network_name,
			.broadcaster_id = 1,
			.remote_control_key_id = 2,
		};
		psispec_default_arib_program arib_pr = {	
			.service_no = 7,
			.copy_flag = arib_abnt_free,
			.ts_name = &ts_name,
			.service_name = &service_name,
			.main_lang.raw = { 'j','p','n','\0'},
			.sub_lang.raw = {'e','n','g','\0'},
		};
		result = vatek_broadcast_psispec_default_init(bc_handle,psispec_default_arib,arib_japan);
		if(result != vatek_result_success){
			printf("abnt default init fail %d\r\n",result);
			return result;
		}
		
		result = vatek_broadcast_psispec_default_config(&arib_ch,&arib_pr);
		if(result != vatek_result_success){
			printf("abnt default config fail %d\r\n",result);
			return result;
		}
		
		result = vatek_broadcast_psispec_default_start();
		if(result != vatek_result_success){
			printf("abnt default start fail %d\r\n",result);
			return result;
		}
	#endif
	#else
		static const uint8_t _abnt_network_name[] = {0x1b,0x7e,_AC('V'),_AC('A'),_AC('T'),_AC('E'),_AC('K')};
		static const uint8_t _abnt_service_name[] = {0x1b,0x7e,_AC('v'),_AC('a'),_AC('t'),_AC('e'),_AC('k')};
		static const uint8_t _abnt_ts_name[] = {0x1b,0x7e,_AC('J'),_AC('A'),_AC('P'),_AC('A'),_AC('N')};

		vatek_string network_name = {
			.len = sizeof(_abnt_network_name),
			.text = (uint8_t *)_abnt_network_name,
		};
		vatek_string service_name = {
			.len = sizeof(_abnt_service_name),
			.text = (uint8_t *)_abnt_service_name,
		};
		vatek_string ts_name = {
			.len = sizeof(_abnt_ts_name),
			.text = (uint8_t *)_abnt_ts_name,
		};
		psispec_default_abnt_channel abnt_ch = {
			.area_code = 1,
			.network_name = &network_name,
			.original_network_id = 1,
			.remote_control_key_id = 2,
		};
		psispec_default_abnt_program abnt_pr = {	
			.service_no = 7,
			.copy_flag = arib_abnt_free,
			.ts_name = &ts_name,
			.service_name = &service_name,
			.main_lang.raw = { 'j','p','n','\0'},
			.sub_lang.raw = {'e','n','g','\0'},
		};
		result = vatek_broadcast_psispec_default_init(bc_handle,psispec_default_abnt,abnt_brazil);
		if(result != vatek_result_success){
			printf("abnt default init fail %d\r\n",result);
			return result;
		}
		
		result = vatek_broadcast_psispec_default_config(&abnt_ch,&abnt_pr);
		if(result != vatek_result_success){
			printf("abnt default config fail %d\r\n",result);
			return result;
		}
		
		result = vatek_broadcast_psispec_default_start();
		if(result != vatek_result_success){
			printf("abnt default start fail %d\r\n",result);
			return result;
		}
	#endif
	#elif (MOD_TYPE == ATSC)
		static const uint8_t _psip_short_name[] = {0x00,'V',0x00,'A',0x00,'T',0x00,'E',0x00,'K',0x00,0x00,};
		static const uint8_t _psip_long_name[] = {0x01,'e','n','g',0x01,0x00,0x00,0x04,'V','A','T','V',0x00,0x00,0x00,0x00};
		vatek_string short_name = {
			.len = sizeof(_psip_short_name),
			.text = (uint8_t *)&_psip_short_name,
		};
		vatek_string long_name = {
			.len = sizeof(_psip_long_name),
			.text = (uint8_t *)&_psip_long_name,
		};
		
		result = vatek_broadcast_psispec_default_init(bc_handle,psispec_default_psip,atsc_usa);
		if(result != vatek_result_success){
			printf("psip default init fail %d\r\n",result);
			return result;
		}
		psispec_default_psip_channel ch = {0};
		psispec_default_psip_program pr = {0};
		ch.cc_mode = cc_mode_608;
		ch.daylight_saving = 0;
		ch.gps_utc_offset = 0;
		ch.psip_flags = 0;
		ch.transport_stream_id = 1;
		ch.short_name = &short_name;

		pr.channel_major = 8;
		pr.channel_minor = 0;
		pr.long_name = &long_name;
		pr.program_number = 0x01;
		pr.source_id = 2;
		result = vatek_broadcast_psispec_default_config(&ch,&pr);
		if(result != vatek_result_success){
			printf("psip default config fail %d\r\n",result);
			return result;
		}
		result = vatek_broadcast_psispec_default_start();
		if(result != vatek_result_success){
			printf("psip default start fail %d\r\n",result);
			return result;
		}
	#endif
#elif (PSI_MODE == PURE)
		psitablelist_parm psi_p = {0};
		psi_p.table_num = 4;
		psi_p.table[0].interval_ms = 90;
		psi_p.table[0].tspackets = &PAT[0];
		psi_p.table[0].tspacket_num = 1;
		psi_p.table[1].interval_ms = 90;
		psi_p.table[1].tspackets = &PMT[0];
		psi_p.table[1].tspacket_num = 1;
		psi_p.table[2].interval_ms = 120;
		psi_p.table[2].tspackets = &MGT[0];
		psi_p.table[2].tspacket_num = 2;
		psi_p.table[3].interval_ms = 100;
		psi_p.table[3].tspackets = &TVCT[0];
		psi_p.table[3].tspacket_num = 2;
		result = vatek_broadcast_psitable_register(bc_handle,&psi_p);
#elif (PSI_MODE == ISO)
		/*Setting iso-13818-1 mode parmeter.*/
    psispec_default_iso13818_channel channel = 
    {
        .transport_stream_id = 1,
    };
    psispec_default_iso13818_program program = 
    {
        .program_number = 0x10,
    };

    //Register PSI table.
    result = vatek_broadcast_psispec_default_init( bc_handle, psispec_default_iso, psispec_country_undefined);
    if(is_success(result)) result = vatek_broadcast_psispec_default_config( &channel, &program);
    if(is_success(result)) result = vatek_broadcast_psispec_default_start();
    if(!is_success(result))
    {
        SAMPLE_ERR("vatek_broadcast_psispec_default_start fail: %d", result);
        return result;
    }
#endif

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
		
		/* start RF */
    result = vatek_rf_start(rf_handle, 473000);//473000, 474000
    if (!is_success(result))
    {
        SAMPLE_ERR("vatek_rf_start fail: %d", result);
        return result;
    }
    /* start BROADCAST */
    result = vatek_broadcast_start(bc_handle);
    if (!is_success(result))
    {
        SAMPLE_ERR("vatek_broadcast_start fail: %d", result);
        return result;
    }
    SAMPLE_LOG("broadcast VI");
		
    /* copy phy infomation to global variable when success broadcast */
    memcpy(&g_phy_vi, &phy_vi, sizeof(phy_video_info));
    memcpy(&g_phy_ai, &phy_ai, sizeof(phy_audio_info));
		
		vatek_phy_dump_reg(phy_active_handle);

    return result;
}

vatek_result sample_bc_broadcast_stop(void)
{
    /* stop broadcast process. first should be stop vatek_broadcast_stop, next stop vatek_phy_stop */
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

		/* stop broadcast */
    //result = vatek_broadcast_stop(bc_handle);
		vatek_broadcast_reset(bc_handle);
    if (!is_success(result))
    {
        SAMPLE_ERR("vatek_broadcast_stop fail : %d", result);
        return result;
    }
		
//    /* stop rf*/
//    result = vatek_rf_stop(rf_handle);
//    if (!is_success(result))
//    {
//        SAMPLE_ERR("vatek_rf_stop fail : %d", result);
//        return result;
//    }
    
    result = vatek_phy_enable(phy_active_handle, 0);
    if (!is_success(result))
    {
        SAMPLE_ERR("vatek_phy_stop fail : %d", result);
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
        /* disable phy(cvbs) */
        if (SECOND_PHY == 1 && (result = vatek_phy_enable(cvbs_handle, 0)) != vatek_result_success)
            SAMPLE_ERR("vatek_phy_enable cvbs fail: %d", result);

        phy_active_handle = hdmi_handle;
    }
    else if (cvbs_status == phy_status_active)
    {
        /* disable phy(hdmi) */
        if ((result = vatek_phy_enable(hdmi_handle, 0)) != vatek_result_success)
            SAMPLE_ERR("vatek_phy_enable hdmi fail: %d", result);   
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
				vatek_phy_dump_reg(phy_active_handle);
				if(tmp_phy_vi.resolution == vi_resolution_1440p60 || tmp_phy_vi.resolution == vi_resolution_1440i60 || tmp_phy_vi.resolution == vi_resolution_1440p59_94 || tmp_phy_vi.resolution == vi_resolution_1440i59_94)
					vatek_h1_change_clk(phy_active_handle);
//				uint8_t phy_val = 0;
//				vatek_phy_read(phy_active_handle, 0x30a0, &phy_val);
//				vatek_phy_write(phy_active_handle,0x30a0, phy_val | 0x4);
    }
    else
        result = vatek_result_negative;

    return result;
}

vatek_result sample_bc_broadcast(uint8_t fromlogo)
{
    vatek_result result = vatek_result_unknown;
    uint8_t h1_val = 0;
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
        /* enable phy output */
        result = vatek_phy_enable(phy_active_handle, 1);
        if (!is_success(result))
        {
            SAMPLE_ERR("phy enable fail : %d", result);
            return result;
        }
				
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
//    static userdata_cc_parm cc_parm[MAX_USERDATA_CC_NUM] = {0};
    uint8_t cc[3][30] = {
        {   0xFC, 0x94, 0x20, 0xFC, 0x13, 0xf4, 0xFC, 0x97, 0xa2, 0xFC, 0x91, 0xae, 0xFC, 0x54, 0x45, 0xFC, 
            0xcb, 0xa1, 0xFC, 0x91, 0x20, 0xFC, 0x94, 0x2c, 0xFC, 0x94, 0x2f, 0xFC, 0x94, 0xae  },
        {   0xFC, 0x94, 0x20, 0xFC, 0x13, 0xf2, 0xFC, 0x97, 0xa1, 0xFC, 0x91, 0xae, 0xFC, 0x52, 0xce, 0xFC,
            0x91, 0x20, 0xFC, 0x94, 0x2c, 0xFC, 0x94, 0x2f, 0xFC, 0x94, 0xae, 0x00, 0x00, 0x00  },
        {   0xFC, 0x94, 0x20, 0xFC, 0x13, 0xf2, 0xFC, 0x97, 0xa1, 0xFC, 0x91, 0xae, 0xFC, 0x38, 0x29, 0xFC,
            0x91, 0x20, 0xFC, 0x94, 0x58, 0xFC, 0x94, 0x2c, 0xFC, 0x94, 0x2f, 0xFC, 0x94, 0xae  },

        };
		static uint32_t idx = 0;
    /* get cc data from cvbs */
//    result = vatek_phy_getccdata(cvbs_handle, &cc_parm[total_num]);
//    if (result == vatek_result_success)
//    {
//        cc_parm[total_num].attr = 0xFC | cc_parm[total_num].attr;
//        total_num++;
//    }
    
    /* write cc date to VATEK, when interval >= 100ms (about 6-7 frames (60FPS), half MAX_USERDATA_CC_NUM) or total_num = MAX_USERDATA_CC_NUM */
//    if (vatek_system_gettick() - write_tick >= 100 || total_num == MAX_USERDATA_CC_NUM)
//    { 
//        if (total_num >= 1)
//        {
//            result = vatek_broadcast_userdata_insert_cc(bc_handle, total_num, (Puserdata_cc_parm)&cc);//(Puserdata_cc_parm)&cc[idx]), cc_parm
//            if (!is_success(result))
//            {
//                SAMPLE_ERR("vatek_broadcast_userdata_insert_cc fail : %d", result);
//                return result;
//            }
//            total_num = 0;
//            memset(cc_parm, '\0', sizeof(cc_parm));
//        }
//        write_tick = vatek_system_gettick();
//    }

		if ((result = vatek_broadcast_userdata_insert_cc(bc_handle, 10, (Puserdata_cc_parm)&cc[idx])) != vatek_result_success)
					return result;
		idx++;
		if (idx >= 3){
				idx = 0;
				
		}

    return vatek_result_success;
}

vatek_result sample_bc_init(Pboard_handle bh_main, Pboard_handle bh_phy, Pboard_handle bh_rf)
{
    vatek_result result = vatek_result_unknown;

    /* init VATEK broadcast */
    result = vatek_broadcast_create(bh_main, &bc_handle);
    if (!is_success(result))
    {
        SAMPLE_ERR("broadcast create fail : %d", result);
        return result;
    }

    /* init phy */
    result = vatek_phy_create(bh_phy, phy_type_h1, &hdmi_handle);  //phy_type_h1
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
		
//		uart_cmdline_get_interface(bc_handle);
    
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
//		vatek_phy_dump_reg(phy_active_handle);
#if EPG_EN
		xapp_polling_check_running(bc_handle);
#endif

    /* check chip status is active */
		#if 0 //astrometa bug test
		if(c_status == bc_status_broadcast){
			if((result = vatek_broadcast_psitable_insert(bc_handle, 1, NULL_PACKET)) != vatek_result_success){
				printf("insert PSI table fail\r\n");
			}
		}
		uint32_t hal_val = 0, reg_val = 0;
		vatek_hms_read_hal(bc_handle, HALREG_SYS_STATUS_0, &hal_val);
		printf("(0x%x)HALREG_SYS_STATUS_0(0x20) = 0x%08x\r\n",vatek_system_gettick(),hal_val);
		vatek_hms_read_reg(bc_handle, 0x60a, &reg_val);
		printf("(0x%x)0x60a = 0x%08x\r\n",vatek_system_gettick(),reg_val);
    if (!is_success(result))
    {
        SAMPLE_ERR("vatek_broadcast_chipstatus fail : %d", result);
        return result;
    }
		if(hal_val != SYS_STATUS_IDLE){
			if(hal_val != SYS_STATUS_RUN){
				if(hal_val != SYS_STATUS_ERRTAG){
					if(hal_val != SYS_STATUS_LOADER_FAIL){
						if(hal_val != SYS_STATUS_SERVICE_FAIL){
							if(hal_val != SYS_STATUS_EXCEPTION_FAIL){
								if(hal_val != SYS_STATUS_BADSTATUS){
									if(hal_val != SYS_STATUS_UNKNOWN_FAIL){
										printf("I2C read HAL fail, status = 0x%x\r\n",hal_val);
										printf("I2C read REG fail, REG val = 0x%x\r\n",reg_val);
										printf("hal_val address = 0x%x\r\n",&hal_val);
										printf("reg_val address = 0x%x\r\b",&reg_val);
										return result;
									}
								}
							}
						}
					}
				}
			}
		}
#endif
    result = vatek_broadcast_chipstatus(bc_handle, &c_status);
    if (!(c_status == chip_status_running || c_status == chip_status_wait_command))
    {
        SAMPLE_LOG("VATEK chip not running/waitcmd, c_status = %d",c_status);
//				result = vatek_broadcast_stop(bc_handle);
//        return result;
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

vatek_result vatek_rtc_init(RTC_HandleTypeDef* rtc)
{
		VATRTCInit(rtc);
}

uint32_t vatek_check_stop()
{
	vatek_result result = vatek_result_success;
	static uint32_t tick = 0;
	if(vatek_system_gettick() - tick < 20000)
		return 0;
	
	result = vatek_broadcast_stop(bc_handle);
	printf("bc stop, %d\r\n",result);
	return 1;
}