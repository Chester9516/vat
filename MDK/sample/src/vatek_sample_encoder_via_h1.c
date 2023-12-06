#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "vatek_api.h"
#include "vatek_sample_encoder_via_h1.h"
#include "vatek_sample_modulator.h"

//#define PHY_EXTERNAL 1//external HDMI
#define V1_USE 1
#define A3_USE 1

#if defined(DEBUG)
extern vatek_result vatek_phy_write(Phphy handle, uint32_t addr, uint8_t val);
extern vatek_result vatek_phy_read(Phphy handle, uint32_t addr, uint8_t* val);
extern vatek_result vatek_encoder_reg_write_v1(Phbroadcast handle, uint32_t addr, uint32_t val);
extern vatek_result vatek_encoder_reg_read_v1(Phbroadcast handle, uint32_t addr, uint32_t* val);
extern vatek_result vatek_encoder_hal_write_v1(Phbroadcast handle, uint32_t addr, uint32_t val);
extern vatek_result vatek_encoder_hal_read_v1(Phbroadcast handle, uint32_t addr, uint32_t* val);
#endif

static vatek_result init_process();
static vatek_result v1_set_logo();
static vatek_result v1_set_phy();
static vatek_result v1_process();
static vatek_result a3_process();
extern vatek_result vatek_phy_dump_reg(Phphy phy_handle);
extern vatek_result vatek_h1_change_clk(Phphy phy_handle);

static Phphy phy_handle = NULL;
static Phrf rf_handle = NULL;
static Phbroadcast enc_handle = NULL;
static Phtransform tf_handle = NULL;

static encoder_input enc_input_mode = input_mode_external;//decide internal or external HDMI
static modulator_type a3_mod_type = m_type_dvb_t;//m_type_dvb_t, m_type_atsc
static uint32_t a3_mod_bandwidth = 8; //6, 8
static uint32_t rf_freq = 474000; //473000, 474000

static phy_audio_info phy_a_info;
static phy_video_info phy_v_info;

vatek_result vatek_encoder_v1_polling()
{
	vatek_result result = vatek_result_unknown;
	
	
#if 1
	phy_status h1_status = phy_status_unknown;
	broadcast_status encoder_status = bc_status_unknown;
	chip_status v1_status = chip_status_unknown;
	static uint32_t tick = 0;
	
	/*polling every 3 second*/
	if(vatek_system_gettick() - tick < 2000)
		return vatek_result_success;
	tick = vatek_system_gettick();

	/*get chip and encoder status*/
	if((result = vatek_encoder_v1_bcstatus(enc_handle, &encoder_status)) != vatek_result_success){
		errlog("get Encoder status fail, %d",result);
		return result;
	}
	if((result = vatek_encoder_v1_chipstatus(enc_handle, &v1_status)) != vatek_result_success){
		errlog("get V1 status fail, %d",result);
		return result;
	}
	if(encoder_status != bc_status_broadcast || v1_status != chip_status_running){
		result = vatek_encoder_v1_reset(enc_handle);
		result = v1_set_phy();
		result = v1_process();
	}
#endif
	
#if 0 //wait for color bar
	/*check phy status and hdmi input information*/
	if((result = vatek_phy_status(phy_handle, &h1_status)) != vatek_result_success){
		errlog("get H1 status fail, %d",result);
		return result;
	}
	/*check chip status and do different thing by status*/
	if(encoder_status != bc_status_broadcast || v1_status != chip_status_running){
		if(h1_status != phy_status_active){
			if(encoder_status != bc_status_idle)
				result = vatek_encoder_v1_reset(enc_handle);
//			result = v1_set_logo();
//			result = v1_process();
		}
		else{
			result = vatek_phy_getaudioinfo(phy_handle, &phy_a);
			result = vatek_phy_getvideoinfo(phy_handle, &phy_v);
			if((memcmp(&phy_a_info, &phy_a, sizeof(phy_audio_info)) != 0) || (memcmp(&phy_v, &phy_v_info, sizeof(phy_video_info)) != 0)){
				log("hdmi input source change, reset chip");
				result = vatek_encoder_v1_reset(enc_handle);
				result = v1_set_phy();
				result = v1_process();
			}
			else{
				log("source not change");
			}
			if(encoder_status == bc_status_idle){
				result = v1_set_phy();
				result = v1_process();
			}
		}
	}
	else{ //broadcasting
		
	}
#endif
	
	
	return result;
}

vatek_result vatek_sample_encoder_start(Pboard_handle handle)
{
	vatek_result result = vatek_result_unknown;
	result = init_process(handle);
	if(result != vatek_result_success)
		return result;
	
#if A3_USE
	result = a3_process();
	if(result != vatek_result_success){
		log("a3 process fail, %d",result);
	}
#endif

#if V1_USE
	result = v1_set_phy();//v1_set_logo, v1_set_phy
	if(result != vatek_result_success){
		log("v1 set PHY fail, %d",result);
		return result;
	}
	result = v1_process();
	if(result != vatek_result_success){
		log("v1 process fail, %d",result);
		return result;
	}
	
	log("v1 process success");
#endif

	return result;
}

static vatek_result init_process(Pboard_handle handle){
	vatek_result result = vatek_result_unknown;
	chip_status v1_status = chip_status_unknown;
#if V1_USE
	result = vatek_encoder_v1_create(handle, &enc_handle);
	if(result != vatek_result_success){
		errlog("V1 initail fail, %d",result);
		return result;
	}
	
	broadcast_infotype bc_info = bc_infotype_chipid;
	uint32_t v1_info = 0;
	result = vatek_encoder_v1_getinfo(enc_handle,bc_info, &v1_info);
	log("v1 chip ID = 0x%x",v1_info);

	while(v1_status != chip_status_wait_command){
		result = vatek_encoder_v1_chipstatus(enc_handle, &v1_status);
		log("V1 status = %d",v1_status);
		if(result != vatek_result_success){
			errlog("get V1 status fail, %d",result);
			return result;
		}
	}
	
#endif
	
if(enc_input_mode == input_mode_external){
	result = vatek_phy_create(handle,phy_type_h1, &phy_handle);//phy_type_ep9555e, phy_type_h1
	if(result != vatek_result_success){
		errlog("peripheral PHY initail fail, %d",result);
		return result;
	}
}

#if A3_USE
	if(tf_handle == NULL){
		result = vatek_transform_create(handle, &tf_handle);
		if(result != vatek_result_success){
			errlog("A3 chip initail fail, %d",result);
		}
		
		result = vatek_rf_create(handle, rf_type_r2_via_vatek, &rf_handle);
		if(result != vatek_result_success){
			errlog("peripheral R2 initail fail, %d",result);
			return result;
		}
	}
#endif
	
	log("peripheral initial success");
	return result;
	
}

/*not yet*/
static vatek_result v1_set_logo()
{
	vatek_result result = vatek_result_unknown;
	logo_input_parm logo_in = {0};
	logo_in.type = logo_type_colorbar;
	logo_in.logoidx = 0;
	result = vatek_encoder_v1_setinputparm_logo(enc_handle, logo_in);
	if(result != vatek_result_success){
		log("set logo parameter fail, %d",result);
		return result;
	}
	return result;
}

static vatek_result v1_set_phy()
{
	vatek_result result = vatek_result_unknown;
	phy_status h1_status = phy_status_unknown;
	chip_status v1_status = chip_status_unknown;
	
	video_input_parm vi_parm = {0};
	audio_input_parm ai_parm = {0};
	
	while(v1_status != chip_status_wait_command){
		result = vatek_encoder_v1_chipstatus(enc_handle, &v1_status);
		if(result != vatek_result_success){
			errlog("get V1 status fail, %d",result);
			return result;
		}
//		else{
//			log("wait V1 idle, %d",v1_status); //debug
//		}
	}
	
	if(enc_input_mode == input_mode_external){
		while(h1_status != phy_status_active){
			if((result = vatek_phy_status(phy_handle, &h1_status)) != vatek_result_success){
				log("get phy status fail, %d",result);
			}
		}
		phy_audio_info phy_ai = {0};
		phy_video_info phy_vi = {0};
		if((result = vatek_phy_getaudioinfo(phy_handle, &phy_ai)) != vatek_result_success){
			log("get phy audio information fail, %d",result);
			return result;
		}
		if((result = vatek_phy_getvideoinfo(phy_handle, &phy_vi)) != vatek_result_success){
			log("get phy video information fail, %d",result);
			return result;
		}
		
		if(enc_input_mode == input_mode_external){
			result = vatek_h1_change_clk(phy_handle);
			
			if((result = vatek_phy_enable(phy_handle, 1)) != vatek_result_success){
				printf("phy enable fail, %d\r\n",result);
				return result;
			}
		//	vatek_phy_dump_reg(phy_handle);
		}
		
		memcpy(&phy_a_info, &phy_ai, sizeof(phy_audio_info));
		memcpy(&phy_v_info, &phy_vi, sizeof(phy_video_info));
		
		vi_parm.clk_inverse = 0;
		vi_parm.aspectrate = vi_aspectrate_16_9;//phy_vi.aspectrate;
		vi_parm.buswidth_16 = phy_vi.buswidth_16;//0;
		vi_parm.separated_sync = phy_vi.separated_sync;//1;
		vi_parm.resolution = phy_vi.resolution;
		vi_parm.vsync_inverse = 0;
		vi_parm.input_mode = enc_input_mode;

		ai_parm.samplerate = phy_ai.samplerate;

	}
	
	if(enc_input_mode == input_mode_internal){
		vi_parm.input_mode = input_mode_internal;
	}
	
	if((result = vatek_encoder_v1_setinputparm_phy(enc_handle, vi_parm, ai_parm)) != vatek_result_success){
		log("set input parameter fail, %d",result);
		return result;
	}
	
	return result;
}

static vatek_result v1_process()
{
	vatek_result result = vatek_result_unknown;
	static uint32_t start_tick = 0, count_i = 0;
	broadcast_status enc_s = bc_status_unknown;
	
	video_encode_v1_parm ve_parm = {0};
	audio_encode_parm ae_parm = {0};
	
	ve_parm.type = ve_type_mpeg2;//ve_type_mpeg2, ve_type_h264
	ve_parm.output_bitrate = 9000000; //set pure encoder output (important value)
	
	ae_parm.type = ae_type_mp1_l2;//ae_type_mp1_l2, ae_type_ac_3
	ae_parm.channel = ae_channel_stereo;
	result = vatek_encoder_v1_setencodeparm(enc_handle, ve_parm, ae_parm);
	if(result != vatek_result_success){
		log("set encode parameter fail, %d",result);
		return result;
	}
	
	encoder_quality_parm eq_parm = {0};
	eq_parm.bitrate = 9000000;
	eq_parm.gop = 16;
	eq_parm.latency = 500;
	eq_parm.minq = 5;
	eq_parm.maxq = 12;
	eq_parm.rcmode = q_rcmode_vbr;
	if((result = vatek_encoder_v1_setqualityparm(enc_handle, eq_parm)) != vatek_result_success){
		log("set quality parameter fail, %d",result);
		return result;
	}
	encoder_mux_parm em_p = {0};
	em_p.audio_pid = 0x1003;
	em_p.video_pid = 0x1002;
	if((result = vatek_encoder_v1_setmuxparm(enc_handle, em_p)) != vatek_result_success){
		log("set encoder mux parameter fail, %d",result);
		return result;
	}
		
	tsmux_iso13818_parm iso_p = {0};
	iso_p.padding_pid = 0x1FF0;
	iso_p.pcr_pid = 0x100;
	iso_p.pmtpid = 0x1001;
	iso_p.program_num = 0x10;
	iso_p.tsid = 0x2;
	if((result = vatek_encoder_v1_tsmux_setparm(enc_handle, tsmux_type_iso13818, &iso_p)) != vatek_result_success){
		log("set tsmux parameter fail, %d",result);
		return result;
	}
	
	log("final H1 256 Bytes------------------------");
	result = vatek_phy_dump_reg(phy_handle);
	uint32_t hal_val = 0;
	vatek_encoder_reg_read_v1(enc_handle, 0x60b, &hal_val);
	log("reg 0x60b = 0x%x", hal_val);
	vatek_encoder_reg_read_v1(enc_handle, 0x60c, &hal_val);
	log("reg 0x60c = 0x%x", hal_val);
	
	result = vatek_encoder_v1_start(enc_handle);
	if(result != vatek_result_success){
		log("encoder start fail, %d",result);
		return result;
	}
	
	if(enc_input_mode == input_mode_internal){
		uint32_t v1_info_600 = 0;
		broadcast_infotype h1_info = bc_reg_600;
		h1_info = h1_set_0x11;
		result = vatek_encoder_v1_getinfo(enc_handle, h1_set_0x11, &v1_info_600);
		log("0x600 = 0x%x", v1_info_600);
		if(result == vatek_result_success)
			log("use external H1 success");
	}
	
	while(enc_s != bc_status_broadcast){
		if((result = vatek_encoder_v1_bcstatus(enc_handle, &enc_s)) != vatek_result_success){
			errlog("get encoder status fail, %d",result);
			return result;
		}
	}
	
	return result;
}

static vatek_result a3_process(){
	vatek_result result = vatek_result_unknown;
	chip_status a3_status = chip_status_unknown;
	broadcast_status a3_bcstatus = bc_status_unknown;
	phy_status ep_status = phy_status_unknown;
	log("Start A3 process");

	result = vatek_transform_chipstatus(tf_handle,&a3_status);
		if(result == vatek_result_success)
			if(a3_status == chip_status_running)//chip_status_running, chip_status_idle
				return vatek_result_success;
	
	ts_input_parm a3_input = {0};	
	a3_input.tsin_mode = tsin_smooth;
	a3_input.pcr_mode = pcr_adjust;
	a3_input.serial = 1;
	result = vatek_transform_tsp_setinputparm_ts(tf_handle,a3_input);
	if(result != vatek_result_success){
		errlog("A3 set input parameter fail, %d",result);
		return result;
	}
	
	result = a_modulator_select(tf_handle, a3_mod_type, a3_mod_bandwidth);
	if(result != vatek_result_success){
		errlog("modulator setting fail, %d",result);
		return result;
	}
	
	result = vatek_transform_start(tf_handle);
	if(result != vatek_result_success){
		errlog("transform start fail, %d",result);
		return result;
	}
	
	result = vatek_rf_start(rf_handle,rf_freq);
	if(result != vatek_result_success){
		errlog("RF start fail, %d",result);
		return result;
	}
	while(a3_bcstatus != bc_status_wait_source){
		result = vatek_transform_bcstatus(tf_handle,&a3_bcstatus);
		if(result != vatek_result_success){
			errlog("get A3 status fail, %d",result);
		}
	}

	log("A3 process run success");
	return result;
}