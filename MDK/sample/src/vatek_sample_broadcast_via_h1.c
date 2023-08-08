
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include "vatek_sample_broadcast_via_h1.h"
//#include "vatek_hms.h"
//#include "vatek_api.h"
//#include "main.h"

//#define SECOND_PHY  1 // this sample case is CVBS output
//#define CVBS_CC     1 // this sample case is SECOND_PHY's closed caption
//#define ATSC        0
//#define J83B        1
//#define DVBT        2
//#define ISDBT       3
//#define J83A        4
//#define MOD_TYPE    ATSC

//static Phbroadcast bc_handle = NULL;
//static Phphy hdmi_handle = NULL;
//static Phphy cvbs_handle = NULL;
//static Phphy phy_active_handle = NULL;
//static Phrf rf_handle = NULL;
//static phy_video_info g_phy_vi = {vi_resolution_unknown};
//static phy_audio_info g_phy_ai = {ai_samplerate_unknown};


//static vatek_result _sample_setmodulator_parm(void)
//{
//    vatek_result result = vatek_result_unknown;
//    
//    /* set MODULATOR parameter */
//    #if (MOD_TYPE == ATSC) // ATSC
//    modulator_base_parm mod_base_parm = {m_type_unknown};
//    modulator_atsc_parm mod_atsc_parm = {atsc_constellation_unknown};
//    mod_base_parm.type = m_type_atsc;
//    mod_base_parm.ifmode = m_ifmode_disable;
//    mod_base_parm.iffreq = 0;
//    mod_base_parm.dacgain = 0;
//    mod_base_parm.bw_sb = 6;
//    mod_atsc_parm.constellation = atsc_constellation_8vsb;
//    result = vatek_broadcast_modulator_setparm(bc_handle, mod_base_parm, (Pmodulator_advanced_parm)&mod_atsc_parm);
//    if (!is_success(result))
//    {
//        SAMPLE_ERR("vatek_broadcast_modulator_setparm fail: %d", result);
//        return result;
//    }
//    #elif (MOD_TYPE == J83B) // J83B
//    modulator_base_parm mod_base_parm = {0};
//    modulator_j83b_parm mod_j83b_parm = {0};
//    mod_base_parm.type = m_type_j83b;
//    mod_base_parm.ifmode = m_ifmode_disable;
//    mod_base_parm.iffreq = 0;
//    mod_base_parm.dacgain = 0;
//    mod_base_parm.bw_sb = 6;
//    mod_j83b_parm.constellation = j83b_constellation_qam256;
//    result = vatek_broadcast_modulator_setparm(bc_handle, mod_base_parm, (Pmodulator_advanced_parm)&mod_j83b_parm);
//    if (!is_success(result))
//    {
//        SAMPLE_ERR("vatek_broadcast_modulator_setparm fail: %d", result);
//        return result;
//    }
//    #elif (MOD_TYPE == DVBT) // dvbt
//    modulator_base_parm mod_base_parm = {0};
//    modulator_dvbt_parm mod_dvbt_parm = {0};
//    mod_base_parm.type = m_type_dvb_t;
//    mod_base_parm.ifmode = m_ifmode_disable;
//    mod_base_parm.iffreq = 0;
//    mod_base_parm.dacgain = 0;
//    mod_base_parm.bw_sb = 6;
//    mod_dvbt_parm.constellation = dvbt_constellation_qam64;
//    mod_dvbt_parm.fft = dvbt_fft_8k;
//    mod_dvbt_parm.guardinterval = dvbt_guardinterval_1_16;
//    mod_dvbt_parm.coderate = dvbt_coderate_5_6;
//    result = vatek_broadcast_modulator_setparm(bc_handle, mod_base_parm, (Pmodulator_advanced_parm)&mod_dvbt_parm);
//    if (!is_success(result))
//    {
//        SAMPLE_ERR("vatek_broadcast_modulator_setparm fail: %d", result);
//        return result;
//    }
//    #elif (MOD_TYPE == ISDBT) //ISDBT
//    modulator_base_parm mod_base_parm = {0};
//    modulator_isdbt_parm mod_isdbt_parm = {0};
//    mod_base_parm.type = m_type_isdb_t;
//    mod_base_parm.ifmode = m_ifmode_disable;
//    mod_base_parm.iffreq = 0;
//    mod_base_parm.dacgain = 0;
//    mod_base_parm.bw_sb = 6;
//    mod_isdbt_parm.constellation = isdbt_constellation_qam64;
//    mod_isdbt_parm.fft = isdbt_fft_8k;
//    mod_isdbt_parm.guardinterval = isdbt_guardinterval_1_16;
//    mod_isdbt_parm.coderate = isdbt_coderate_5_6;
//    mod_isdbt_parm.ti = isdbt_time_interleaved_mode3;
//    result = vatek_broadcast_modulator_setparm(bc_handle, mod_base_parm, (Pmodulator_isdbt_parm)&mod_isdbt_parm);
//    if (!is_success(result))
//    {
//        SAMPLE_ERR("vatek_broadcast_modulator_setparm fail: %d", result);
//        return result;
//    }
//    #elif (MOD_TYPE == J83A) //J83A
//    modulator_base_parm mod_base_parm = {0};
//    modulator_j83a_parm mod_j83a_parm = {0};
//    
//    mod_base_parm.type = m_type_j83a;
//    mod_base_parm.ifmode = m_ifmode_disable;
//    mod_base_parm.iffreq = 0;
//    mod_base_parm.dacgain = 0;
//    mod_base_parm.bw_sb = 5720;
//    mod_j83a_parm.constellation = j83a_constellation_qam256;
//    
//    result = vatek_broadcast_modulator_setparm( bc_handle, mod_base_parm, (Pmodulator_j83a_parm)&mod_j83a_parm);
//    if (!is_success(result))
//    {
//        SAMPLE_ERR("vatek_broadcast_modulator_setparm fail: %d", result);
//        return result;
//    }  
//    #endif

//    return result;
//}

//static const uint8_t _psip_service_name[] = "BQ";
//static const uint8_t _psip_long_name[] = "";

//vatek_string psip_service_name = {
//	.len = sizeof(_psip_service_name),
//	.text = (uint8_t *)_psip_service_name,
//};	

//vatek_string psip_long_name = {
//	.len = sizeof(_psip_long_name),
//	.text = (uint8_t *)_psip_long_name,
//};	



//psispec_default_psip_channel psip_ch = {0};
//psispec_default_psip_program psip_pr = {0};

//psitablelist_parm psi_parm = {0};
//#if 0
//uint8_t PAT[188] = 
//{
//    0x47, 0x40, 0x00, 0x11, 		/* 0  PID : 0x0 */
//    0x00, 
//    0x00, 							/* 5  table_id: 0x0 */
//    0xB0, 0x0D,						/* 6  etc 4bit/section length 12bit = 13 */
//    0x00, 0x01, 					/* 8  tsid : 0x0001 */
//    0xC3, 0x00, 0x00,				/* 10  reserved 2bit/version number 5bit/current next 1bit/section number 8bit/last section number 8bit */
//    0x00, 0x01, 					/* 13 program number */
//    0xE0, 0x50, 					/* 15 reserved 3bit/pmt_PID 13bit : 0x1001 */
//    0xDC, 0xD9, 0xE4, 0x84, 		/* 17 CRC */    
//    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF    
//};

//uint8_t PMT[188] = 
//{ 
//    0x47, 0x40, 0x50, 0x11,     /* 0  PID : 0x0050 */
//    0x00, 
//    0x02, 						/* 5  table_id: 0x2 = PMT */
//    0xB0, 0x17,     			/* 6  etc 4bit/section length 12bit = 23 */
//    0x00, 0x01,                 /* 8  program number = 0x0001*/ 
//    0xC3, 0x00, 0x00, 			/* reserved 2bit/version number 5bit = 1 /current_next_indicator 1bit/section_number 8bit/last_section_number 8bit */
//    0xE1, 0x00,                 /* PCR_PID: 0x100 */ 
//    0xF0, 0x00, 
//    0x02, 0xF0, 0x02,           /* stream type : 0x2 (video mpeg2), PID : 0x1002 */
//    0xF0, 0x00, 
//    0x03, 0xF0, 0x03,           /* stream type : 0x3 (audio mpeg1-l2), PID : 0x1003 */ 
//    0xF0, 0x00, 
//    0x1D, 0x30, 0xDB, 0x47,     /* CRC */
//};    

//uint8_t MGT[188] = 
//{ 
//    0x47, 0x50, 0x01, 0x1C,     /* PID : 0x1001 */
//    0x00, 0x02, 0xB0, 0x17,     
//    0x00, 0x10,                 /* program number*/ 
//    0xC3, 0x00, 0x00, 
//    0xE1, 0x00,                 /* PCR_PID: 0x100 */ 
//    0xF0, 0x00, 
//    0x02, 0xF0, 0x02,           /* stream type : 0x2 (video mpeg2), PID : 0x1002 */
//    0xF0, 0x00, 
//    0x03, 0xF0, 0x03,           /* stream type : 0x3 (audio mpeg1-l2), PID : 0x1003 */ 
//    0xF0, 0x00, 
//    0x1D, 0x30, 0xDB, 0x47,     /* CRC */
//};    

//uint8_t TVCT[188] = 
//{ 
//    0x47, 0x50, 0x01, 0x1C,     /* PID : 0x1001 */
//    0x00, 0x02, 0xB0, 0x17,     
//    0x00, 0x10,                 /* program number*/ 
//    0xC3, 0x00, 0x00, 
//    0xE1, 0x00,                 /* PCR_PID: 0x100 */ 
//    0xF0, 0x00, 
//    0x02, 0xF0, 0x02,           /* stream type : 0x2 (video mpeg2), PID : 0x1002 */
//    0xF0, 0x00, 
//    0x03, 0xF0, 0x03,           /* stream type : 0x3 (audio mpeg1-l2), PID : 0x1003 */ 
//    0xF0, 0x00, 
//    0x1D, 0x30, 0xDB, 0x47,     /* CRC */
//};    
//#endif

//#if 0
//uint8_t PAT[188] = 
//{
//#if 0	
//    0x47, 0x40, 0x00, 0x10, 0x00, 0x00, 0xB0, 0x11, 0x00, 0x64, 0xC3, 0x00, 0x00, 0x00, 0x00, 0xE0,
//	0x10, 0x00, 0x01, 0xE0, 0x20, 0x3C, 0x8C, 0xD2, 0x28, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF 
//#endif 
//	0x47, 0x40, 0x00, 0x11, 0x00, 0x00, 0xB0, 0x0D, 0x00, 0x01, 0xC1, 0x00, 0x00, 0x00, 0x01, 0xE0,
//	0x50, 0x42, 0x77, 0x34, 0xA6, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF

//};

//uint8_t PMT[188] = 
//{     
//	0x47, 0x40, 0x20, 0x10, 0x00, 0x02, 0xB0, 0x3C, 0x00, 0x01, 0xC3, 0x00, 0x00, 0xE0, 0x32, 0xF0,
//	0x06, 0x05, 0x04, 0x47, 0x41, 0x39, 0x34, 0x02, 0xE0, 0x30, 0xF0, 0x09, 0x86, 0x07, 0xE1, 0x65,
//	0x6E, 0x67, 0xC1, 0x3F, 0xFF, 0x81, 0xE0, 0x31, 0xF0, 0x16, 0x05, 0x04, 0x41, 0x43, 0x2D, 0x33,
//	0x81, 0x08, 0x08, 0xB8, 0x05, 0xFF, 0x1F, 0x00, 0x00, 0x00, 0x0A, 0x04, 0x65, 0x6E, 0x67, 0x00,
//	0x8A, 0xAF, 0xA4, 0x58, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF						   
//};    

//uint8_t MGT[188] = 
//{ 
//    0x47, 0x5F, 0xFB, 0x1A, 0x00, 0xC7, 0xF0, 0x19, 0x00, 0x00, 0xC3, 0x00, 0x00, 0x00, 0x00, 0x01,
//	0x00, 0x00, 0xFF, 0xFB, 0xE1, 0x00, 0x00, 0x00, 0x3E, 0xF0, 0x00, 0xF0, 0x00, 0xFA, 0x1D, 0xD7,
//	0x7E, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF 
//};    

//uint8_t TVCT[188] = 
//{ 
//    0x47, 0x5F, 0xFB, 0x1B, 0x00, 0xC8, 0xF0, 0x3E, 0x00, 0x64, 0xC3, 0x00, 0x00, 0x00, 0x01, 0x00,
//	0x48, 0x00, 0x44, 0x00, 0x31, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x18, 0x01,
//	0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x64, 0x00, 0x01, 0x0D, 0xC2, 0x00, 0x01, 0xFC, 0x11, 0xA1,
//	0x0F, 0xE0, 0x32, 0x02, 0x02, 0xE0, 0x30, 0x00, 0x00, 0x00, 0x81, 0xE0, 0x31, 0x65, 0x6E, 0x67,
//	0xFC, 0x00, 0x0E, 0x42, 0x7C, 0x58, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
//};   


//uint8_t PMT_Logo[188] = 
//{ 
//    0x47, 0x40, 0x50, 0x1F, 0x00, 0x02, 0xB0, 0x3C, 0x00, 0x01, 0xC3, 0x00, 0x00, 0xE0, 0x32, 0xF0,
//	0x06, 0x05, 0x04, 0x47, 0x41, 0x39, 0x34, 0x02, 0xE0, 0x30, 0xF0, 0x09, 0x86, 0x07, 0xE1, 0x65,
//	0x6E, 0x67, 0xC1, 0x3F, 0xFF, 0x81, 0xE0, 0x31, 0xF0, 0x16, 0x05, 0x04, 0x41, 0x43, 0x2D, 0x33,
//	0x81, 0x08, 0x08, 0xB8, 0x05, 0xFF, 0x1F, 0x00, 0x00, 0x00, 0x0A, 0x04, 0x65, 0x6E, 0x67, 0x00,
//	0x8A, 0xAF, 0xA4, 0x58, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
//}; 

///*
//uint8_t PMT[188] = 
//{     
//	0x47, 0x40, 0x20, 0x10, 0x00, 0x02, 
//	0xB0, 0x3C, 
//	0x00, 0x01, 
//	0xC3, 0x00, 0x00, 
//	0xE0, 0x32, 
//	0xF0, 0x06, 
//	0x05, 0x04, 0x47, 0x41, 0x39, 0x34, 
//	0x02, 0xE0, 0x30, 
//	0xF0, 0x09, 
//	0x86, 0x07, 0xE1, 0x65,	0x6E, 0x67, 0xC1, 0x3F, 0xFF, 
//	0x81, 0xE0, 0x31, 0xF0, 0x16, 0x05, 0x04, 0x41, 0x43, 0x2D, 0x33,
//	0x81, 0x08, 0x08, 0xB8, 0x05, 0xFF, 0x1F, 0x00, 0x00, 0x00, 0x0A, 0x04, 0x65, 0x6E, 0x67, 0x00,
//	0x8A, 0xAF, 0xA4, 0x58, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF						   
//};
//*/

//#endif

////vatek table
//#if 1
//uint8_t PAT[188] = 
//{
//	0x47, 0x40, 0x00, 0x12, 0x00, 0x00, 0xB0, 0x11, 0x00, 0x64, 0xC5, 0x00, 0x00, 0x00, 0x00, 0xE0,
//	0x10, 0x00, 0x01, 0xE0, 0x20, 0x20, 0xE5, 0xF9, 0x22, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF          
//};

//uint8_t PMT[188] = 
//{
//0x47, 0x40, 0x20, 0x12, 0x00, 0x02, 0xB0, 0x3C, 0x00, 0x01, 0xC5, 0x00, 0x00, 0xE0, 0x32, 0xF0,
//0x06, 0x05, 0x04, 0x47, 0x41, 0x39, 0x34, 0x02, 0xE0, 0x30, 0xF0, 0x09, 0x86, 0x07, 0xE1, 0x65,
//0x6E, 0x67, 0xC1, 0x3F, 0xFF, 0x81, 0xE0, 0x31, 0xF0, 0x16, 0x05, 0x04, 0x41, 0x43, 0x2D, 0x33,
//0x81, 0x08, 0x08, 0xB8, 0x05, 0xFF, 0x1F, 0x00, 0x00, 0x00, 0x0A, 0x04, 0x65, 0x6E, 0x67, 0x00,
//0x88, 0x75, 0x5F, 0xE2, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF  
//};

//uint8_t MGT[376] = 
////uint8_t MGT[188] = 
//{
//0x47, 0x5F, 0xFB, 0x18, 0x00, 0xC7, 0xF0, 0x19, 0x00, 0x00, 0xC5, 0x00, 0x00, 0x00, 0x00, 0x01,
//0x00, 0x00, 0xFF, 0xFB, 0xE2, 0x00, 0x00, 0x00, 0x02, 0xF0, 0x00, 0xF0, 0x00, 0xD4, 0x84, 0x05,
//0xB5, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF 
//#if 1
//,
//0x47, 0x5F, 0xFB, 0x19, 0x00, 0xC8, 0xF0, 0x3E, 0x00, 0x64, 0xC5, 0x00, 0x00, 0x00, 0x01, 0x00,
//0x48, 0x00, 0x44, 0x00, 0x31, 0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x1C, 0x01,
//0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x64, 0x00, 0x01, 0x0D, 0xC2, 0x00, 0x01, 0xFC, 0x11, 0xA1,
//0x0F, 0xE0, 0x32, 0x02, 0x02, 0xE0, 0x30, 0x00, 0x00, 0x00, 0x81, 0xE0, 0x31, 0x65, 0x6E, 0x67,
//0xFC, 0x00, 0x6D, 0x48, 0x36, 0xA9, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF   
//#endif
//};

//uint8_t TVCT[188] = 
//{
//0x47, 0x5F, 0xFB, 0x19, 0x00, 0xC8, 0xF0, 0x3E, 0x00, 0x64, 0xC5, 0x00, 0x00, 0x00, 0x01, 0x00,
//0x48, 0x00, 0x44, 0x00, 0x31, 0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x1C, 0x01,
//0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x64, 0x00, 0x01, 0x0D, 0xC2, 0x00, 0x01, 0xFC, 0x11, 0xA1,
//0x0F, 0xE0, 0x32, 0x02, 0x02, 0xE0, 0x30, 0x00, 0x00, 0x00, 0x81, 0xE0, 0x31, 0x65, 0x6E, 0x67,
//0xFC, 0x00, 0x6D, 0x48, 0x36, 0xA9, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF   
//};

//#endif
//vatek_result sample_bc_broadcast_from_logo(void)
//{ 
//    // broadcast (logo) ==> (VATEK) ==> (rf)
//    // use simple psi (iso13818) 
//	vatek_result result = vatek_result_unknown;
//	if(color_bar_sel == 2){
//		result = vatek_result_success;
//		return result;	
//	}
//    
//    
//    /* set ENCODER input parameter from logo/colorbar*/
//    logo_input_parm logo_parm = {logo_type_unknown};
//    logo_parm.type = logo_type_colorbar;
//    result = vatek_broadcast_encoder_setinputparm_logo(bc_handle, logo_parm);
//    if (!is_success(result))
//    {
//        SAMPLE_ERR("vatek_broadcast_encoder_setinputparm_logo fail: %d", result);
//        return result;
//    }

//    /* set ENCODER encode parameter */
//    video_encode_parm ve_parm = {ve_type_unknown,};
//    audio_encode_parm ae_parm = {ae_type_unknown,};
//    ve_parm.type = ve_type_mpeg2;
//    ae_parm.type = ae_type_ac_3;//ae_type_mp1_l2;
//    ae_parm.channel = ae_channel_stereo;
//    result = vatek_broadcast_encoder_setencodeparm(bc_handle, ve_parm, ae_parm);
//    if (!is_success(result))
//    {
//        SAMPLE_ERR("vatek_broadcast_encoder_setencodeparm fail: %d", result);
//        return result;
//    }

//    /* set ENCODER pes/ts mux parameter */
//    encoder_mux_parm em_parm = {0};
//    em_parm.video_pid = 48;//pmt_gen_param.elementary_PID[0];//48;//256;//0x1002;
//    em_parm.audio_pid = 49;//pmt_gen_param.elementary_PID[1];//49;//512;//0x1003;
//    result = vatek_broadcast_encoder_setmuxparm(bc_handle, em_parm);
//    if (!is_success(result))
//    {
//        SAMPLE_ERR("vatek_broadcast_encoder_setmuxparm fail: %d", result);
//        return result;
//    }

//    /* set ENCODER quality parameter */
//    encoder_quality_parm q_parm = {q_rcmode_unknown};
//    q_parm.rcmode = q_rcmode_vbr;
//    q_parm.minq = config_minq;//5;//3;
//    q_parm.maxq = config_maxq;//20;//10;
//    q_parm.gop = config_gop;//16;
//    q_parm.latency = (uint32_t)latency_ms;//500;
//    q_parm.bitrate = (bit_rate * 10000);//19000000;
//    result = vatek_broadcast_encoder_setqualityparm(bc_handle, q_parm);
//    if (!is_success(result))
//    {
//        SAMPLE_ERR("vatek_broadcast_encoder_setqualityparm fail: %d", result);
//        return result;
//    }
//    
//    /* set TSMUX parameter */
//	psi_parm.table_num = 3;
//#if 1
//	PAT_PACKETIZER(&pat_gen_param,&PAT[0]);
//	PMT_PACKETIZER(&pmt_gen_param,1,&PMT[0],pat_gen_param);
//	MGT_PACKETIZER(&mgt_gen_param,1,&MGT[0]);
//	VCT_PACKETIZER(&vct_gen_param,&MGT[188]);
//#endif	

//#if 1
//	psi_parm.table[0].interval_ms = 90;//240;
//	psi_parm.table[0].tspacket_num = 1;
//	psi_parm.table[0].tspackets = &PAT[0];
//	
//	psi_parm.table[1].interval_ms = 90;//240;
//	psi_parm.table[1].tspacket_num = 1;
//	psi_parm.table[1].tspackets = &PMT[0];
//	
//	psi_parm.table[2].interval_ms = 120;
//	psi_parm.table[2].tspacket_num = 2;
//	psi_parm.table[2].tspackets = &MGT[0];
//#endif

//#if 0
//	psi_parm.table[0].interval_ms = 240;//240;
//	psi_parm.table[0].tspacket_num = 1;
//	psi_parm.table[0].tspackets = &PAT[0];
//	
//	psi_parm.table[1].interval_ms = 240;//240;
//	psi_parm.table[1].tspacket_num = 1;
//	psi_parm.table[1].tspackets = &PMT[0];
//	
//	psi_parm.table[2].interval_ms = 100;
//	psi_parm.table[2].tspacket_num = 1;
//	psi_parm.table[2].tspackets = &MGT[0];
//	
//	psi_parm.table[3].interval_ms = 300;
//	psi_parm.table[3].tspacket_num = 1;
//	psi_parm.table[3].tspackets = &TVCT[0];
//#endif
//	
//	result = vatek_broadcast_psitable_register(bc_handle,&psi_parm);
//    if (result != vatek_result_success)
//    {
//        SAMPLE_ERR("vatek_broadcast_psitable_register fail: %d", result);
//        return result;
//    };
//	
//#if 1	
//    tsmux_default_parm default_parm  = {0};
//    default_parm.pcr_pid = 50;//pmt_gen_param.PCR_PID;//50;//0x100;    
//    default_parm.pmt_pid = 32;//pmt_gen_param.PMT_PID;//32;//80;//0x1000;
//	default_parm.padding_pid = 0x1FFF;
//	default_parm.mux_bitrate = 19000000;//19390000;//15000000;
//#else
//	tsmux_pure_parm pure_parm = {0};
//	pure_parm.pcr_pid = 0x100;
//	pure_parm.padding_pid = 0x1FFF;	
//#endif	
//	result = vatek_broadcast_tsmux_setparm(bc_handle, tsmux_type_default, &default_parm);
//    //result = vatek_broadcast_tsmux_setparm(bc_handle, tsmux_type_pure, &pure_parm);    /** select tsmux_type_default */
//    if (result != vatek_result_success)
//    {
//        SAMPLE_ERR("vatek_broadcast_tsmux_setparm fail: %d", result);
//        return result;
//    }
//    
//    /* set MODULATOR parameter */
//    result = _sample_setmodulator_parm();
//    if (!is_success(result))
//    {
//        SAMPLE_ERR("_sample_setmodulator_parm fail: %d", result);
//        return result;
//    }

//    /* start BROADCAST */
//    result = vatek_broadcast_start(bc_handle);
//    if (!is_success(result))
//    {
//        SAMPLE_ERR("vatek_broadcast_start fail: %d", result);
//        return result;
//    }

//    /* start RF */
//    //result = vatek_rf_start(rf_handle, 57000);
//	outfreq = (freqency_calc(outchmode, outch) * 1000);
//	result = vatek_rf_start(rf_handle, outfreq);
//	
//	
//	
//    if (!is_success(result))
//    {
//        SAMPLE_ERR("vatek_rf_start fail: %d", result);
//        return result;
//    }
//	//result = vatek_device_r2_apply(rf_handle, rf_level);

//    
//    SAMPLE_LOG("broadcast ColorBar");
//	video_out_status = 1;//color bar
//    return result;
//}


//vatek_result sample_bc_broadcast_from_phy(void)
//{ 
//    // broadcast (phy) ==> (VATEK) ==> (rf)
//    // use default psi, psi table via DEFAULTPSI API (vatek_broadcast_psispec_default_init/config/start)
//    vatek_result result = vatek_result_unknown;

//    /* get PHY video information */
//    phy_video_info phy_vi = {vi_resolution_unknown};
//    result = vatek_phy_getvideoinfo(phy_active_handle, &phy_vi);
//    if (!is_success(result))
//    {
//        SAMPLE_ERR("vatek_phy_getvideoinfo fail: %d", result);
//        return result;
//    }
//    SAMPLE_LOG("resolution = %d", phy_vi.resolution);
//    SAMPLE_LOG("aspectrate = %d", phy_vi.aspectrate);
//    SAMPLE_LOG("buswidth_16 = %d", phy_vi.buswidth_16);
//    SAMPLE_LOG("separated_sync = %d", phy_vi.separated_sync);
//    
//    /* get PHY audio information */
//    phy_audio_info phy_ai = {ai_samplerate_unknown};
//    result = vatek_phy_getaudioinfo(phy_active_handle, &phy_ai);
//    if (!is_success(result))
//    {
//        SAMPLE_ERR("vatek_phy_getaudioinfo fail: %d", result);
//        return result;
//    }
//    SAMPLE_LOG("samplerate = %d", phy_ai.samplerate);

//    /* set ENCODER input parameter from PHY*/
//    video_input_parm vi_parm = {vi_resolution_unknown};
//    audio_input_parm ai_parm = {ai_samplerate_unknown};
//    vi_parm.resolution = phy_vi.resolution;
//    vi_parm.aspectrate = phy_vi.aspectrate;
//    vi_parm.buswidth_16 = phy_vi.buswidth_16;
//    vi_parm.separated_sync = phy_vi.separated_sync;
//#if 0	
//	if(audio_samplerate != 0 && audio_samplerate <= 3){
//		phy_ai.samplerate = audio_samplerate;	  
//	}
//#endif	
//    //ai_parm.samplerate = ai_samplerate_48K;//phy_ai.samplerate;
//    audio_samplerate = phy_ai.samplerate;
//	ai_parm.samplerate = phy_ai.samplerate;
//    result = vatek_broadcast_encoder_setinputparm_phy(bc_handle, vi_parm, ai_parm);
//    if (!is_success(result))
//    {
//        SAMPLE_ERR("vatek_broadcast_encoder_setinputparm_phy fail: %d", result);
//        return result;
//    }

//    /* set ENCODER encode parameter */
//    video_encode_parm ve_parm = {ve_type_unknown};
//    audio_encode_parm ae_parm = {ae_type_unknown};
//    ve_parm.type = ve_type_mpeg2;//ve_type_mpeg2, ve_type_h264
//    ae_parm.type = ae_type_ac_3;//ae_type_mp1_l2;
//    ae_parm.channel = ae_channel_stereo;
//    result = vatek_broadcast_encoder_setencodeparm(bc_handle, ve_parm, ae_parm);
//    if (!is_success(result))
//    {
//        SAMPLE_ERR("vatek_broadcast_encoder_setencodeparm fail: %d", result);
//        return result;
//    }

//    /* set ENCODER pes/ts mux parameter */
//    encoder_mux_parm em_parm = {0};
//    em_parm.video_pid = 48;//pmt_gen_param.elementary_PID[0];//48;//256;//0x1002;
//    em_parm.audio_pid = 49;//pmt_gen_param.elementary_PID[1];//49;//512;//0x1003;
//    result = vatek_broadcast_encoder_setmuxparm(bc_handle, em_parm);
//    if (!is_success(result))
//    {
//        SAMPLE_ERR("vatek_broadcast_encoder_setmuxparm fail: %d", result);
//        return result;
//    }

//    /* set ENCODER quality parameter */
//    encoder_quality_parm q_parm = {q_rcmode_unknown};
//    q_parm.rcmode = q_rcmode_vbr;
//    q_parm.minq = config_minq;//5;
//    q_parm.maxq = config_maxq;//20;//20;
//    q_parm.gop = config_gop;//16;
//    q_parm.latency = (uint32_t)latency_ms;
//    q_parm.bitrate = (bit_rate * 10000); //18000000;//22000000;
//    result = vatek_broadcast_encoder_setqualityparm(bc_handle, q_parm);
//    if (!is_success(result))
//    {
//        SAMPLE_ERR("vatek_broadcast_encoder_setqualityparm fail: %d", result);
//        return result;
//    }
//    
//    /* set TSMUX parameter */
//	
//	psi_parm.table_num = 3;

//#if 1
//	PAT_PACKETIZER(&pat_gen_param,&PAT[0]);
//	PMT_PACKETIZER(&pmt_gen_param,1,&PMT[0],pat_gen_param);
//	MGT_PACKETIZER(&mgt_gen_param,1,&MGT[0]);
//	VCT_PACKETIZER(&vct_gen_param,&MGT[188]);
//#endif
//	
///*	
//	psi_parm.table[0].interval_ms = 240;
//	psi_parm.table[0].tspacket_num = 1;
//	psi_parm.table[0].tspackets = &PAT[0];
//	
//	psi_parm.table[1].interval_ms = 240;
//	psi_parm.table[1].tspacket_num = 1;
//	psi_parm.table[1].tspackets = &PMT[0];
//	
//	psi_parm.table[2].interval_ms = 100;
//	psi_parm.table[2].tspacket_num = 1;
//	psi_parm.table[2].tspackets = &MGT[0];

//	psi_parm.table[3].interval_ms = 300;
//	psi_parm.table[3].tspacket_num = 1;
//	psi_parm.table[3].tspackets = &TVCT[0];
//*/
//#if 1
//	psi_parm.table[0].interval_ms = 90;//240;
//	psi_parm.table[0].tspacket_num = 1;
//	psi_parm.table[0].tspackets = &PAT[0];
//	
//	psi_parm.table[1].interval_ms = 90;//240;
//	psi_parm.table[1].tspacket_num = 1;
//	psi_parm.table[1].tspackets = &PMT[0];
//	
//	psi_parm.table[2].interval_ms = 120;
//	psi_parm.table[2].tspacket_num = 2;
//	psi_parm.table[2].tspackets = &MGT[0];
//#endif

//#if 0
//		psi_parm.table[0].interval_ms = 240;//240;
//		psi_parm.table[0].tspacket_num = 1;
//		psi_parm.table[0].tspackets = &PAT[0];
//		
//		psi_parm.table[1].interval_ms = 240;//240;
//		psi_parm.table[1].tspacket_num = 1;
//		psi_parm.table[1].tspackets = &PMT[0];
//		
//		psi_parm.table[2].interval_ms = 100;
//		psi_parm.table[2].tspacket_num = 1;
//		psi_parm.table[2].tspackets = &MGT[0];
//		
//		psi_parm.table[3].interval_ms = 300;
//		psi_parm.table[3].tspacket_num = 1;
//		psi_parm.table[3].tspackets = &TVCT[0];
//#endif

//	result = vatek_broadcast_psitable_register(bc_handle,&psi_parm);
//    if (result != vatek_result_success)
//    {
//        SAMPLE_ERR("vatek_broadcast_psitable_register fail: %d", result);
//        return result;
//    };

//	
//#if 1
//    tsmux_default_parm default_parm = {0};
//    default_parm.pcr_pid    = 50;//pmt_gen_param.PCR_PID;//50;//0x100;
//    default_parm.pmt_pid    = 32;//pmt_gen_param.PMT_PID;//32;//80;//0x1000;
//    default_parm.padding_pid = 0x1FFF;
//	default_parm.mux_bitrate = 19390000;//15000000;
//#else
//	tsmux_pure_parm pure_parm = {0};
//	pure_parm.pcr_pid = 0x100;
//	pure_parm.padding_pid = 0x1FFF;	
//#endif
//    result = vatek_broadcast_tsmux_setparm(bc_handle, tsmux_type_default, &default_parm);
//    if (!is_success(result))
//    {
//        SAMPLE_ERR("vatek_broadcast_tsmux_setparm fail: %d", result);
//        return result;
//    }
//    /* set MODULATOR parameter */
//    result = _sample_setmodulator_parm();
//    if (!is_success(result))
//    {
//        SAMPLE_ERR("_sample_setmodulator_parm fail: %d", result);
//        return result;
//    }

//    /* start BROADCAST */
//    result = vatek_broadcast_start(bc_handle);
//    if (!is_success(result))
//    {
//        SAMPLE_ERR("vatek_broadcast_start fail: %d", result);
//        return result;
//    }

//    /* start RF */
//    //result = vatek_rf_start(rf_handle, 57000);
//	outfreq = (freqency_calc(outchmode, outch) * 1000);
//	result = vatek_rf_start(rf_handle, outfreq);
//    if (!is_success(result))
//    {
//        SAMPLE_ERR("vatek_rf_start fail: %d", result);
//        return result;
//    }
//	//result = vatek_device_r2_apply(rf_handle, rf_level);
//    
//    SAMPLE_LOG("broadcast VI");

//    /* copy phy infomation to global variable when success broadcast */
//    memcpy(&g_phy_vi, &phy_vi, sizeof(phy_video_info));
//    memcpy(&g_phy_ai, &phy_ai, sizeof(phy_audio_info));
//	
//	vatek_phy_get_h1_info(phy_active_handle);
//	
//	video_out_status = 0;//video
//	init_colorbar = 0;
//	
//    return result;
//}

//vatek_result sample_bc_broadcast_stop(void)
//{
//    /* stop broadcast process. first should be stop vatek_broadcast_stop, next stop vatek_phy_stop */
//    vatek_result result = vatek_result_unknown;
//    broadcast_status status = bc_status_unknown;

//    /* check broadcast status */
//    result = vatek_broadcast_bcstatus(bc_handle, &status);
//    if (!is_success(result))
//    {
//        SAMPLE_ERR("vatek_broadcast_bcstatus fail : %d", result);
//        return result;
//    }

//    if (status == bc_status_idle)
//        return vatek_result_success;

//    /* stop rf*/
//    result = vatek_rf_stop(rf_handle);
//    if (!is_success(result))
//    {
//        SAMPLE_ERR("vatek_rf_stop fail : %d", result);
//        return result;
//    }

//    /* stop broadcast */
//    result = vatek_broadcast_stop(bc_handle);
//    if (!is_success(result))
//    {
//        SAMPLE_ERR("vatek_broadcast_stop fail : %d", result);
//        return result;
//    }
//    
//    result = vatek_phy_enable(phy_active_handle, 0);
//    if (!is_success(result))
//    {
//        SAMPLE_ERR("vatek_phy_stop fail : %d", result);
//        return result;
//    }

//    return result;
//}
//uint8_t test_flag2 = 0;
//vatek_result sample_bc_broadcast_signalischange(void)
//{
//    vatek_result result = vatek_result_negative;
//	//vatek_result result2 = vatek_result_negative;
//    phy_video_info tmp_phy_vi = {vi_resolution_unknown};
//    phy_audio_info tmp_phy_ai = {ai_samplerate_unknown};
//    phy_status cvbs_status = phy_status_unknown;
//    phy_status hdmi_status = phy_status_unknown;

//    /* get PHY status */
//    result = vatek_phy_status(hdmi_handle, &hdmi_status);
//    if (!is_success(result))
//    {
//        SAMPLE_ERR("vatek_phy_getstatus hdmi fail: %d", result);
//    }
//    if (SECOND_PHY == 1)
//    {
//        result = vatek_phy_status(cvbs_handle, &cvbs_status);
//        if (!is_success(result))
//        {
//            SAMPLE_ERR("vatek_phy_getstatus cvbs fail: %d", result);
//        }
//    }

//    if ( (inmode == AUTO || inmode == HDMI) && (hdmi_status == phy_status_active) )
//    {
//        /* disable phy(cvbs) */
//        if (SECOND_PHY == 1 && (result = vatek_phy_enable(cvbs_handle, 0)) != vatek_result_success)
//            SAMPLE_ERR("vatek_phy_enable cvbs fail: %d", result);

//        phy_active_handle = hdmi_handle;
//		inmode_status = HDMI;
//		if(audio_sel == AUTO){
//		  AUDIO_SW_SEL(GPIO_PIN_RESET);
//		}
//    }
//    else if ( (inmode == AUTO || inmode == COMPOSITE) && (cvbs_status == phy_status_active) )
//    {
//        /* disable phy(hdmi) */
//        if ((result = vatek_phy_enable(hdmi_handle, 0)) != vatek_result_success)
//            SAMPLE_ERR("vatek_phy_enable hdmi fail: %d", result);   
//        phy_active_handle = cvbs_handle;
//		inmode_status = COMPOSITE;
//		if(audio_sel == AUTO){
//		  AUDIO_SW_SEL(GPIO_PIN_SET);
//		}
//    }
//    else
//    {
//        /* all phy signal lost or error*/
//        /* disable phy(hdmi), disable phy(cvbs) */
//        if ((result = vatek_phy_enable(hdmi_handle, 0)) != vatek_result_success)
//            SAMPLE_ERR("vatek_phy_enable hdmi fail: %d", result);
//        if (SECOND_PHY == 1 && (result = vatek_phy_enable(cvbs_handle, 0)) != vatek_result_success)
//            SAMPLE_ERR("vatek_phy_enable cvbs fail: %d", result);
//        
//        memcpy(&g_phy_vi, &tmp_phy_vi, sizeof(phy_video_info));
//        memcpy(&g_phy_ai, &tmp_phy_ai, sizeof(phy_audio_info));
//		
//		inmode_status = AUTO;
//        return vatek_result_idle;
//    }

//    /* get PHY video information */
//    result = vatek_phy_getvideoinfo(phy_active_handle, &tmp_phy_vi);
//    if (!is_success(result))
//    {
//        SAMPLE_ERR("vatek_phy_getvideoinfo fail: %d", result);
//        return result;
//    }

//    /* get PHY audio information */
//    result = vatek_phy_getaudioinfo(phy_active_handle, &tmp_phy_ai);
//    if (!is_success(result))
//    {
//        SAMPLE_ERR("vatek_phy_getaudioinfo fail: %d", result);
//        return result;
//    }

//    /* compare audio/video information */
//    if (memcmp(&tmp_phy_vi, &g_phy_vi, sizeof(phy_video_info)) != 0 || memcmp(&tmp_phy_ai, &g_phy_ai, sizeof(phy_audio_info)) != 0)
//    {
//        SAMPLE_LOG("PHY audio/video information change");
//        SAMPLE_LOG("--- video info ---");
//        SAMPLE_LOG("resolution     : %02d - %02d", tmp_phy_vi.resolution, g_phy_vi.resolution);
//        SAMPLE_LOG("aspectrate     : %02d - %02d", tmp_phy_vi.aspectrate, g_phy_vi.aspectrate);
//        SAMPLE_LOG("buswidth_16    : %02d - %02d", tmp_phy_vi.buswidth_16, g_phy_vi.buswidth_16);
//        SAMPLE_LOG("separated_sync : %02d - %02d", tmp_phy_vi.separated_sync, g_phy_vi.separated_sync);
//        SAMPLE_LOG("--- audio info ---");
//        SAMPLE_LOG("samplerate     : %02d - %02d", tmp_phy_ai.samplerate, g_phy_ai.samplerate);
//        result = vatek_result_success;
//    }
//    else
//        result = vatek_result_negative;

//    return result;
//}

//uint8_t h1_test_flag = 0;
//vatek_result sample_bc_broadcast(uint8_t fromlogo)
//{
//    vatek_result result = vatek_result_unknown;
//    
//    /* stop broadcast */
//	if(	h1_test_flag == 1){
//		H1_Reset();
//	}
//	B2_Reset();
//	delay_ms(3000);
//    result = sample_bc_broadcast_stop();
//    if (!is_success(result))
//    {
//        SAMPLE_ERR("sample_broadcast_stop fail : %d", result);
//        return result;
//    }
//    
//    if (fromlogo)
//    {
//        /* start broadcast from colorbar */
//		if(color_bar_sel == 2){
//			result = vatek_result_success;
//			return result;
//		}
//		else{
//			result = sample_bc_broadcast_from_logo();
//			if (!is_success(result))
//			{
//				SAMPLE_ERR("sample_broadcast_from_logo fail : %d", result);
//				return result;
//			}
//		}
//    }
//    else
//    {
//        /* enable phy output */
//        result = vatek_phy_enable(phy_active_handle, 1);
//        if (!is_success(result))
//        {
//            SAMPLE_ERR("phy enable fail : %d", result);
//            return result;
//        }
//        
//        /* start broadcast from phy */
//        result = sample_bc_broadcast_from_phy();
//        if (!is_success(result))
//        {
//            SAMPLE_ERR("sample_broadcast_from_phy fail : %d", result);
//            return result;
//        }
//    }
//    
//    return result;
//}

//vatek_result sample_bc_polling_cc(void)
//{
//    vatek_result result = vatek_result_unknown;
//    static uint8_t total_num = 0;
//    static uint32_t write_tick = 0;
//    static userdata_cc_parm cc_parm[MAX_USERDATA_CC_NUM] = {0};
//    
//    /* get cc data from cvbs */
//    result = vatek_phy_getccdata(cvbs_handle, &cc_parm[total_num]);
//    if (result == vatek_result_success)
//    {
//        cc_parm[total_num].attr = 0xFC | cc_parm[total_num].attr;
//        total_num++;
//    }
//    
//    /* write cc date to VATEK, when interval >= 100ms (about 6-7 frames (60FPS), half MAX_USERDATA_CC_NUM) or total_num = MAX_USERDATA_CC_NUM */
//    if (vatek_system_gettick() - write_tick >= 100 || total_num == MAX_USERDATA_CC_NUM)
//    { 
//        if (total_num >= 1)
//        {
//            result = vatek_broadcast_userdata_insert_cc(bc_handle, total_num, cc_parm);
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

//    return vatek_result_success;
//}

//vatek_result sample_bc_init(Pboard_handle bh_main, Pboard_handle bh_phy, Pboard_handle bh_rf)
//{
//    vatek_result result = vatek_result_unknown;

//    /* init VATEK broadcast */
//    result = vatek_broadcast_create(bh_main, &bc_handle);
//    if (!is_success(result))
//    {
//        SAMPLE_ERR("broadcast create fail : %d", result);
//        //return result;
//    }	

//    /* init phy */
//    result = vatek_phy_create(bh_phy, phy_type_h1, &hdmi_handle);  
//    if (!is_success(result))
//    {
//        SAMPLE_ERR("phy(hdmi) create fail : %d", result);
//        //return result;
//    }
//    if (SECOND_PHY == 1 || CVBS_CC == 1)
//    {
//        /* init phy : adv7182a */
//        result = vatek_phy_create(bh_phy, phy_type_adv7182a_cvbs, &cvbs_handle);  
//        if (!is_success(result))
//        {
//            SAMPLE_ERR("phy(cvbs) create fail : %d", result);
//            //return result;
//        }
//    }
//    phy_active_handle = hdmi_handle;
//    
//    /* init RF : r2_via_vatek */
//    result = vatek_rf_create(bh_rf, rf_type_r2_via_vatek, &rf_handle);  
//    if (!is_success(result))
//    {
//        SAMPLE_ERR("rf create fail : %d", result);
//        return result;
//    }
//    
//    /* broadcast from logo */
//	//LCD_Write_String(1, 0,"   system init..		   ");
//	
//	result = sample_bc_broadcast(1);
//    //result = sample_bc_broadcast_from_logo();
//    if (!is_success(result))
//    {
//        SAMPLE_ERR("sample_broadcast_from_logo fail: %d", result);
//        return result;
//    }

//    return result;
//}

////edit by mhkim
//broadcast_status bc_status_main = bc_status_unknown;
//chip_status c_status_main = chip_status_unknown;
//uint8_t test_return1 = 0;
//uint8_t test_return2 = 0;
//uint8_t signal_lost = 1; // broadcast from logo when call sample_init, so default signal_lost is 1
//uint8_t restart_flag = 1;
//uint8_t test_reset_flag = 0;
//vatek_result sample_bc_polling(void)
//{
//    //this function is a simple case for broadcast polling    
//    vatek_result result = vatek_result_success;
//    
//    broadcast_status bc_status = bc_status_unknown;
//    chip_status c_status = chip_status_unknown;
//    //static uint32_t tick = 0;
//   // vatek_result result2 = vatek_result_success;

//    #if (CVBS_CC == 1)
//    if ((result = sample_bc_polling_cc()) !=  vatek_result_success)
//    {
//        SAMPLE_ERR("sample_polling_cc fail : %d", result);
//    }
//    #endif
//    
//    /* polling interval is 2000 */
//    //if ((vatek_system_gettick() - tick) <= 2000)
//    //    return result;
//    //tick = vatek_system_gettick();

//    /* check chip status is active */
//    result = vatek_broadcast_chipstatus(bc_handle, &c_status);
//	c_status_main = c_status;
//    if (!is_success(result))
//    {
//        SAMPLE_ERR("vatek_broadcast_chipstatus fail : %d", result);
//		//return result;		
//    }
//    if (!(c_status == chip_status_running || c_status == chip_status_wait_command))
//    {
//        SAMPLE_LOG("VATEK chip not running/waitcmd, c_status = %d",c_status);		
//		//LCD_Write_String(1, 0,"   system reset..       ");
//		//B2_Reset();
//		//delay_ms(3000);
//        //!!!
//        //result2 = sample_bc_broadcast_stop();
//		//return result;		
//    } 

//    /* check input signal change */
//    result = sample_bc_broadcast_signalischange();
//	vatek_phy_get_h1_input();
//	
//    if (result == vatek_result_success) /* signal change */
//    {
//	    reconfig_flag = 1;
//        SAMPLE_LOG("signal change");		
//		if(inmode_status == HDMI){		
//			if(!system_init_wait){
//				LCD_Write_String(0,4,"SIGNAL CHANGE           ");
//			}
//		}
//		
//		if(test_reset_flag == 1){		  
//		  result = vatek_broadcast_destroy(bc_handle);
//		  result = vatek_phy_destroy(hdmi_handle);
//		  result = vatek_phy_destroy(cvbs_handle);
//		  result = vatek_rf_destroy(rf_handle);
//		  test_reset_flag = 0;
//		  system_init = 1;			  
//		  delay_ms(3000);
//		  return result;
//		}
//        result = sample_bc_broadcast(0);
//		//result = sample_bc_broadcast(0);
//        if (!is_success(result))
//        {
//            SAMPLE_ERR("sample_broadcast fail : %d", result);
//            //return result;
//        }
//        signal_lost = 0;
//		vatek_broadcast_dump(bc_handle);
//		reconfig_flag = 0;
//		system_init_wait = 0;
//    }
//    else if (result == vatek_result_idle) /* signal lost */
//    {   
//	  	if (signal_lost == 1){
//			reconfig_flag = 0;
//			signal_lost = 1;
//			return result;
//	  	}
//        reconfig_flag = 1;
//        SAMPLE_LOG("signal lost");
//		if(inmode_status == HDMI){		
//			if(!system_init_wait){
//				LCD_Write_String(0,4,"SIGNAL LOST             ");
//			}
//		}
//        result = sample_bc_broadcast(1);
//        if (!is_success(result))
//        {
//            SAMPLE_ERR("sample_broadcast fail : %d", result);
//            //return result;
//        }
//        signal_lost = 1;
//		vatek_broadcast_dump(bc_handle);
//		reconfig_flag = 0;
//		system_init_wait = 0;
//    }
//    else    /* normal play case */
//    {
//            
//        /* polling broadcast status */
//        result = vatek_broadcast_bcstatus(bc_handle, &bc_status);
//		bc_status_main = bc_status;  
//        if (!is_success(result))
//        {
//            SAMPLE_ERR("vatek_broadcast_bcstatus fail : %d", result);
//            //return result;
//        }
//		if(test_reset_flag == 1){		  
//		  result = vatek_broadcast_destroy(bc_handle);
//		  result = vatek_phy_destroy(hdmi_handle);
//		  result = vatek_phy_destroy(cvbs_handle);
//		  result = vatek_rf_destroy(rf_handle);
//		  test_reset_flag = 0;
//		  system_init = 1;			  
//		  delay_ms(3000);
//		  return result;
//		}
//       
//        /* check status */
//        if (bc_status != bc_status_broadcast)
//        {
//            /* re-broadcast */
//		  	reconfig_flag = 1;
//            SAMPLE_LOG("something wrong, re-broadcast");
//			if(inmode_status == HDMI){		
//				if(!system_init_wait){  
//					LCD_Write_String(0,4,"ENC ERROR               ");
//				}
//			}
//			
//#if 0
//			//result2 = vatek_phy_get_h1_input();
//			vatek_phy_set_h1_timing();
//#endif
//			if(restart_flag == 1){
//				result = sample_bc_broadcast(signal_lost);
//				if (!is_success(result))
//				{
//					SAMPLE_ERR("_sample_broadcast fail : %d", result);
//					//return result;
//				}
//			}
//		
//			reconfig_flag = 0;
//        }
//		else if((color_bar_sel != 1) &&  (video_out_status == 1) ){
//			result = sample_bc_broadcast(signal_lost);
//			if (!is_success(result))
//			{
//				SAMPLE_ERR("_sample_broadcast fail : %d", result);
//				//return result;
//			}
//		}		
//    }
//	if(signal_lost == 0 && c_status_main == chip_status_running && bc_status_main == bc_status_broadcast){
//		encoding_flag = 1;	  
//	}
//	else{
//	  	encoding_flag = 0;	  
//	}
//    return result;
//}


//#if 1 //may
//hdmi_video_timing v_timing_main = {0};
//phy_status cvbs_status = phy_status_unknown;
//phy_status hdmi_status = phy_status_unknown;
//vatek_result vatek_phy_get_h1_input(void)
//{
//    vatek_result result = vatek_result_negative;
//	//vatek_result result2 = vatek_result_negative;
//    //phy_video_info tmp_phy_vi = {0};
//    //phy_audio_info tmp_phy_ai = {0};
//	
//    //phy_status cvbs_status = phy_status_unknown;
//    //phy_status hdmi_status = phy_status_unknown;

//    /* get PHY status */
//    result = vatek_phy_status(hdmi_handle, &hdmi_status);
//    if (!is_success(result))
//    {
//        SAMPLE_ERR("vatek_phy_getstatus hdmi fail: %d", result);
//    }
//    if (SECOND_PHY == 1)
//    {
//        result = vatek_phy_status(cvbs_handle, &cvbs_status);
//        if (!is_success(result))
//        {
//            SAMPLE_ERR("vatek_phy_getstatus cvbs fail: %d", result);
//        }
//    }

//    if (hdmi_status == phy_status_active)
//    {
//        /* disable phy(cvbs) */
//        if (SECOND_PHY == 1 && (result = vatek_phy_enable(cvbs_handle, 0)) != vatek_result_success)
//            SAMPLE_ERR("vatek_phy_enable cvbs fail: %d", result);

//        phy_active_handle = hdmi_handle;
//		
//    }
//    else if (cvbs_status == phy_status_active)
//    {
//        /* disable phy(hdmi) */
//        if ((result = vatek_phy_enable(hdmi_handle, 0)) != vatek_result_success)
//            SAMPLE_ERR("vatek_phy_enable hdmi fail: %d", result);   
//        phy_active_handle = cvbs_handle;
//		
//    }
//    else
//    {
//        /* all phy signal lost or error*/
//        /* disable phy(hdmi), disable phy(cvbs) */
//        if ((result = vatek_phy_enable(hdmi_handle, 0)) != vatek_result_success)
//            SAMPLE_ERR("vatek_phy_enable hdmi fail: %d", result);
//        if (SECOND_PHY == 1 && (result = vatek_phy_enable(cvbs_handle, 0)) != vatek_result_success)
//            SAMPLE_ERR("vatek_phy_enable cvbs fail: %d", result);
//        
//        //memcpy(&g_phy_vi, &tmp_phy_vi, sizeof(phy_video_info));
//        //memcpy(&g_phy_ai, &tmp_phy_ai, sizeof(phy_audio_info));
//        return vatek_result_idle;
//    }
//	//result2 = get_timing(phy_active_handle);	
//	//result = vatek_phy_gettiming(phy_active_handle, &v_timing_main);
//	
//    return result;
//}
//vatek_result vatek_phy_set_h1_timing(void)
//{
//    vatek_result result = vatek_result_negative;
//	//vatek_result result2 = vatek_result_negative;
//    //phy_video_info tmp_phy_vi = {0};
//    //phy_audio_info tmp_phy_ai = {0};
//    phy_status cvbs_status = phy_status_unknown;
//    phy_status hdmi_status = phy_status_unknown;


//    /* get PHY status */
//    result = vatek_phy_status(hdmi_handle, &hdmi_status);
//    if (!is_success(result))
//    {
//        SAMPLE_ERR("vatek_phy_getstatus hdmi fail: %d", result);
//    }
//    if (SECOND_PHY == 1)
//    {
//        result = vatek_phy_status(cvbs_handle, &cvbs_status);
//        if (!is_success(result))
//        {
//            SAMPLE_ERR("vatek_phy_getstatus cvbs fail: %d", result);
//        }
//    }

//    if (hdmi_status == phy_status_active)
//    {
//        /* disable phy(cvbs) */
//        if (SECOND_PHY == 1 && (result = vatek_phy_enable(cvbs_handle, 0)) != vatek_result_success)
//            SAMPLE_ERR("vatek_phy_enable cvbs fail: %d", result);

//        phy_active_handle = hdmi_handle;
//		inmode_status = HDMI;
//		if(audio_sel == AUTO){
//		  AUDIO_SW_SEL(GPIO_PIN_RESET);
//		}
//    }
//    else if (cvbs_status == phy_status_active)
//    {
//        /* disable phy(hdmi) */
//        if ((result = vatek_phy_enable(hdmi_handle, 0)) != vatek_result_success)
//            SAMPLE_ERR("vatek_phy_enable hdmi fail: %d", result);   
//        phy_active_handle = cvbs_handle;
//		inmode_status = COMPOSITE;
//		if(audio_sel == AUTO){
//		  AUDIO_SW_SEL(GPIO_PIN_SET);
//		}
//    }
//    else
//    {
//        /* all phy signal lost or error*/
//        /* disable phy(hdmi), disable phy(cvbs) */
//        if ((result = vatek_phy_enable(hdmi_handle, 0)) != vatek_result_success)
//            SAMPLE_ERR("vatek_phy_enable hdmi fail: %d", result);
//        if (SECOND_PHY == 1 && (result = vatek_phy_enable(cvbs_handle, 0)) != vatek_result_success)
//            SAMPLE_ERR("vatek_phy_enable cvbs fail: %d", result);
//        
//        //memcpy(&g_phy_vi, &tmp_phy_vi, sizeof(phy_video_info));
//        //memcpy(&g_phy_ai, &tmp_phy_ai, sizeof(phy_audio_info));
//        return vatek_result_idle;
//    }
//		
//	result = vatek_phy_settiming(phy_active_handle, v_timing_main);
//	
//    return result;
//}


//void Out_Set(void){	  	
//	
//  	vatek_result result = vatek_result_success;
//	//vatek_result result2 = vatek_result_success;
//	outfreq = (freqency_calc(outchmode, outch) * 1000);
//  	//outfreq =
//	result = vatek_rf_stop(rf_handle);
//    if (!is_success(result))
//    {
//        SAMPLE_ERR("vatek_rf_stop fail : %d", result);
//    }
//	result = vatek_rf_start(rf_handle, outfreq);
//    if (!is_success(result))
//    {
//        SAMPLE_ERR("vatek_rf_start fail: %d", result);        
//    }
//	vatek_broadcast_dump(bc_handle);  
//}

//void Out_Gain_Set(uint8_t level){
//	vatek_result result = vatek_result_success;
//	
//	result = vatek_device_r2_apply(rf_handle, level);   
//	if (!is_success(result))
//    {
//        SAMPLE_ERR("vatek_rf_gain_set fail : %d", result);
//    }
//}


//#endif


//#if 0
//vatek_result modulator_shift(){
//	vatek_result result = vatek_result_unknown;
//	static uint32_t atick = 0;
//	uint32_t btick = vatek_system_gettick();
//	/*button reset*/
//	uint8_t left_b = HAL_GPIO_ReadPin(LEFT_GPIO_Port,LEFT_Pin);
//	uint8_t right_b = HAL_GPIO_ReadPin(RIGHT_GPIO_Port,RIGHT_Pin);
//	uint8_t up_b = HAL_GPIO_ReadPin(UP_GPIO_Port,UP_Pin);
//	uint8_t down_b = HAL_GPIO_ReadPin(DOWN_GPIO_Port,DOWN_Pin);
//	uint8_t center_b = HAL_GPIO_ReadPin(CENTER_GPIO_Port,CENTER_Pin);
//	/*button reset*/
//	
//	modulator_dvbt_parm dvb_p = {0};
//	/*modulator parameter array*/
//	
//													
//	static uint32_t mod_num = 0;
//	static uint32_t parm_num=0,constellation_num=0,fft_num=0;//setting array number
//	if(up_b == 0 || down_b == 0 || center_b == 0 || left_b == 0 || right_b == 0){

//		result = vatek_system_delay(1000);
//		
//		if(center_b == 0){
//			//center button use to restart chip
//			result = vatek_broadcast_stop(bc_handle);
//			if(result == vatek_result_success)
//				result = colorbardis();
//			else{
//				error("chip restart fail %d",result);
//				return result;
//			}
//		}
//		
//		if(left_b == 0){
//			//choose modulator type first
//			mod_num -= 1;
//		}
//		
//		if(right_b == 0){
//			//choose modulator type first
//			mod_num += 1;
//				

//		}
//		
//		if(up_b == 0){
//			parm_num -= 1;

//		}
//		
//		if(down_b == 0){
//			//setting parameter depend on modulator type
//			parm_num += 1;

//		}

//		atick = vatek_system_gettick();
//		return vatek_result_success;	
//	}
//}
//#endif
