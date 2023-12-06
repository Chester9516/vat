#include "stdio.h"
#include "vatek_define.h"
#include "vatek_sample_modulator.h"
#include "hal_backup\hal_register\halreg_base.h"
#include "vatek_api.h"

	static Phphy phy_handle = NULL;
	static Phrf rf_handle = NULL;
	static uint32_t rffreq = 473000;
	static Pboard_handle handle = NULL;
	static modulator_type mod_type = m_type_dvb_t;//set modulator value
	static uint32_t b_width = 6;//set bandwidth value
//	static uint32_t default_table = mux_default_psip;//select psi table in default mode(old version)
//	static uint32_t default_table = psispec_default_dvb; //new version usage


static vatek_result vatek_default_table_choose(Phbroadcast bc_handle, psispec_default_type def_type){
	vatek_result result = vatek_result_unknown;
	if(def_type == psispec_default_psip){
		static const uint8_t _psip_short_name[] = {0x00,'V',0x00,'A',0x00,'T',0x00,'E',0x00,'K'};
		static const uint8_t _psip_long_name[] = {0x01,'e','n','g',0x01,0x00,0x00,0x04,'V','A','T','V'};
		vatek_string short_name = {
			.len = sizeof(_psip_short_name),
			.text = (uint8_t *)_psip_short_name,
		};
		vatek_string long_name = {
			.len = sizeof(_psip_long_name),
			.text = (uint8_t *)_psip_long_name,
		};
		
		result = vatek_broadcast_psispec_default_init(bc_handle,psispec_default_psip,atsc_usa);
		if(result != vatek_result_success){
			SELF_ERR("psip default init fail %d",result);
			return result;
		}
		psispec_default_psip_channel ch = {0};
		psispec_default_psip_program pr = {0};
		ch.cc_mode = cc_mode_disabble;
		ch.daylight_saving = 0;
		ch.gps_utc_offset = 0;
		ch.psip_flags = 0;
		ch.transport_stream_id = 2;
		ch.short_name = &short_name;

		pr.channel_major = 31;
		pr.channel_minor = 1;
		pr.long_name = &long_name;
		pr.program_number = 0x10;
		pr.source_id = 13;
		result = vatek_broadcast_psispec_default_config(&ch,&pr);
		if(result != vatek_result_success){
			SELF_ERR("psip default config fail %d",result);
			return result;
		}
		result = vatek_broadcast_psispec_default_start();
		if(result != vatek_result_success){
			SELF_ERR("psip default start fail %d",result);
			return result;
		}
	}
	else if(def_type == psispec_default_abnt){
		#define _AC(ch) (ch+0x80)
		static const uint8_t _abnt_network_name[] = {0x1b, 0x7e, _AC('V'),_AC('A'),_AC('T'),_AC('E'),_AC('K')};
		static const uint8_t _abnt_service_name[] = {0x1b, 0x7e, _AC('v'),_AC('a'),_AC('t'),_AC('e'),_AC('k')};
		static const uint8_t _ts_name[] = {0x1b, 0x7e, _AC('T'),_AC('S')};
		vatek_string network_name = {
			.len = sizeof(_abnt_network_name),
			.text = (uint8_t *)_abnt_network_name,
		};
		vatek_string service_name = {
			.len = sizeof(_abnt_service_name),
			.text = (uint8_t *)_abnt_service_name,
		};
		vatek_string ts_name = {
			.len = sizeof(_ts_name),
			.text = (uint8_t *)_ts_name,
		};
		psispec_default_abnt_channel ch = {0};
		psispec_default_abnt_program pr = {0};
		ch.area_code = 1;
		ch.network_name = &network_name;
		ch.original_network_id = 1;
		ch.remote_control_key_id = 1;
		
		pr.service_name = &service_name;
		pr.service_no = 1;
		pr.copy_flag = arib_abnt_free;
		pr.ts_name = &ts_name;
		
		result = vatek_broadcast_psispec_default_init(bc_handle,psispec_default_abnt,psispec_country_undefined);
		if(result != vatek_result_success){
			SELF_ERR("abnt default init fail %d",result);
			return result;
		}
		
		result = vatek_broadcast_psispec_default_config(&ch,&pr);
		if(result != vatek_result_success){
			SELF_ERR("abnt default config fail %d",result);
			return result;
		}
		
		result = vatek_broadcast_psispec_default_start();
		if(result != vatek_result_success){
			SELF_ERR("abnt default start fail %d",result);
			return result;
		}
		
	}
	else if(def_type == psispec_default_arib){
		//#define _AC(ch) (ch + 0x80)
		static const uint8_t _arib_network_name[] = {0x1b, 0x7e, _AC('V'),_AC('A'),_AC('T'),_AC('E'),_AC('K')};
		static const uint8_t _arib_service_name[] = {0x1b, 0x7e, _AC('T'),_AC('V')};
		static const uint8_t _arib_ts_name[] = {0x1b, 0x7e, _AC('T'),_AC('S')};
			
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
		
		psispec_default_arib_channel ch = {0};
		psispec_default_arib_program pr = {0};
		ch.region_id = 1;
		ch.network_name = &network_name;
		ch.broadcaster_id = 1;
		ch.remote_control_key_id = 1;
		pr.copy_flag = arib_abnt_free;
		
		pr.service_name = &service_name;
		pr.service_no = 1;
		pr.ts_name = &ts_name;
		
		result = vatek_broadcast_psispec_default_init(bc_handle,psispec_default_arib,psispec_country_undefined);
		if(result != vatek_result_success){
			SELF_ERR("arib default init fail %d",result);
			return result;
		}
		
		result = vatek_broadcast_psispec_default_config(&ch,&pr);
		if(result != vatek_result_success){
			SELF_ERR("arib default config fail %d",result);
			return result;
		}
		
		result = vatek_broadcast_psispec_default_start();
		if(result != vatek_result_success){
			SELF_ERR("airb default start fail %d",result);
			return result;
		}
	
	}
	else if(def_type == psispec_default_dvb){
		static const uint8_t _dvb_service_name[] = "VATEK";
		static const uint8_t _dvb_network_name[] = "DEMO";
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
		ch.transport_stream_id = 0x2;
		pr.channel_no = 1;
		pr.original_network_id = 1;
		pr.program_number = 0x20;
		pr.service_name = &service_name;
		
		result = vatek_broadcast_psispec_default_init(bc_handle,psispec_default_dvb,dvb_taiwan);
		if(result != vatek_result_success){
			SELF_ERR("dvb default init fail %d",result);
			return result;
		}
		
		result = vatek_broadcast_psispec_default_config(&ch,&pr);
		if(result != vatek_result_success){
			SELF_ERR("dvb default config fail %d",result);
			return result;
		}
		
		result = vatek_broadcast_psispec_default_start();
		if(result != vatek_result_success){
			SELF_ERR("dvb default start fail %d",result);
			return result;
		}
	
	}
	else if(def_type == psispec_default_iso){
		psispec_default_iso13818_channel ch = {0};
		psispec_default_iso13818_program pr ={0};
		ch.transport_stream_id = 0x2;
		pr.program_number = 0x20;
		
		result = vatek_broadcast_psispec_default_init(bc_handle,psispec_default_iso,psispec_country_undefined);
		if(result != vatek_result_success){
			SELF_ERR("iso default init fail %d",result);
			return result;
		}
		
		result = vatek_broadcast_psispec_default_config(&ch,&pr);
		if(result != vatek_result_success){
			SELF_ERR("iso default config fail %d",result);
			return result;
		}
		
		result = vatek_broadcast_psispec_default_start();
		if(result != vatek_result_success){
			SELF_ERR("iso default start fail %d",result);
			return result;
		}
	
	}
}


#if defined(VATEK_B3_PLUS)
vatek_result b_modulator_select(Phbroadcast bc_handle, modulator_type m_type,uint32_t bandwidth){//we can choose which mod we want to use and use
		vatek_result result = vatek_result_unknown;
		
		if(m_type == m_type_atsc){
				modulator_base_parm m_base_p = {0};
				m_base_p.bw_sb = bandwidth;
				m_base_p.dacgain = 0;
				m_base_p.iffreq = 0;
				m_base_p.ifmode = m_ifmode_disable;
				m_base_p.type = m_type_atsc;
				modulator_atsc_parm atsc_parm = {0};
				atsc_parm.constellation = atsc_constellation_8vsb;
				result = vatek_broadcast_modulator_setparm(bc_handle,m_base_p, (Pmodulator_advanced_parm)&atsc_parm);
			}
		else if(m_type == m_type_j83b){
				modulator_base_parm m_base_p = {0};
				m_base_p.bw_sb = bandwidth;
				m_base_p.dacgain = 0;
				m_base_p.iffreq = 0;
				m_base_p.ifmode = m_ifmode_disable;
				m_base_p.type = m_type_j83b;
				modulator_j83b_parm j83b_parm = {0};
				j83b_parm.constellation = j83b_constellation_qam64;
				result = vatek_broadcast_modulator_setparm(bc_handle,m_base_p,(Pmodulator_advanced_parm)&j83b_parm);
			}
		else if(m_type == m_type_j83a){
				modulator_base_parm m_base_p = {0};
				m_base_p.bw_sb = 6875;
				m_base_p.dacgain = 0;
				m_base_p.iffreq = 0;
				m_base_p.ifmode = m_ifmode_disable;
				m_base_p.type = m_type_j83a;
				modulator_j83a_parm j83a_parm = {0};
				j83a_parm.constellation = j83a_constellation_qam64;
				result = vatek_broadcast_modulator_setparm(bc_handle,m_base_p,(Pmodulator_advanced_parm)&j83a_parm);
			}
		else if(m_type == m_type_j83c){
				modulator_base_parm m_base_p = {0};
				m_base_p.bw_sb = bandwidth;
				m_base_p.dacgain = 0;
				m_base_p.iffreq = 0;
				m_base_p.ifmode = m_ifmode_disable;
				m_base_p.type = m_type_j83c;
				modulator_j83c_parm j83c_parm = {0};
				j83c_parm.constellation = j83c_constellation_qam64;
				result = vatek_broadcast_modulator_setparm(bc_handle,m_base_p,(Pmodulator_advanced_parm)&j83c_parm);
			}
		else if(m_type == m_type_dvb_t){
				modulator_base_parm m_base_p = {0};
				m_base_p.bw_sb = bandwidth;
				m_base_p.dacgain = 0;
				m_base_p.iffreq = 0;
				m_base_p.ifmode = m_ifmode_disable;
				m_base_p.type = m_type_dvb_t;
				modulator_dvbt_parm dvbt_parm = {0};
				dvbt_parm.coderate = dvbt_coderate_3_4;
				dvbt_parm.guardinterval = dvbt_guardinterval_1_16;
				dvbt_parm.fft = dvbt_fft_2k;
				dvbt_parm.constellation = dvbt_constellation_qam64;
				result = vatek_broadcast_modulator_setparm(bc_handle,m_base_p,(Pmodulator_advanced_parm)&dvbt_parm);
			}
		else if(m_type == m_type_dtmb){
				modulator_base_parm m_base_p = {0};
				m_base_p.bw_sb = bandwidth;
				m_base_p.dacgain = 0;
				m_base_p.iffreq = 0;
				m_base_p.ifmode = m_ifmode_disable;
				m_base_p.type = m_type_dtmb;
				modulator_dtmb_parm dtmb_parm = {0};
				dtmb_parm.carriermode = dtmb_carriermode_3780;
				dtmb_parm.coderate = dtmb_coderate_0_6;
				dtmb_parm.syncframe = dtmb_syncframe_595;
				dtmb_parm.ti = dtmb_time_interleaved_240;
				dtmb_parm.constellation = dtmb_constellation_qam64;
				result = vatek_broadcast_modulator_setparm(bc_handle,m_base_p,(Pmodulator_advanced_parm)&dtmb_parm);
			}
		else if(m_type == m_type_isdb_t){
				modulator_base_parm m_base_p = {0};
				m_base_p.bw_sb = bandwidth;
				m_base_p.dacgain = 0;
				m_base_p.iffreq = 0;
				m_base_p.ifmode = m_ifmode_disable;
				m_base_p.type = m_type_isdb_t;
				modulator_isdbt_parm isdbt_parm = {0};
				isdbt_parm.coderate = isdbt_coderate_3_4;
				isdbt_parm.fft = isdbt_fft_2k;
				isdbt_parm.guardinterval = isdbt_guardinterval_1_16;
				isdbt_parm.ti = isdbt_time_interleaved_mode1;
				isdbt_parm.constellation = isdbt_constellation_qam64;
				result = vatek_broadcast_modulator_setparm(bc_handle,m_base_p,(Pmodulator_advanced_parm)&isdbt_parm);
			}
		else if(m_type == m_type_dvb_t2){
				modulator_base_parm m_base_p = {0};
				m_base_p.bw_sb = 0;
				m_base_p.dacgain = 0;
				m_base_p.iffreq = 0;
				m_base_p.ifmode = m_ifmode_disable;
				m_base_p.type = m_type_dvb_t2;
				modulator_dvbt2_parm dvbt2_parm = {0};
				dvbt2_parm.coderate = dvbt2_coderate_4_5;
				dvbt2_parm.constellation_rotation_en = 1;
				dvbt2_parm.delete_null_packet_en = 0;
				dvbt2_parm.extend_carrier_mode_en = 0;
				dvbt2_parm.fec = dvbt2_fec_16200;
				dvbt2_parm.fecbn = 0;
				dvbt2_parm.fft = dvbt2_fft_8k;
				dvbt2_parm.guardinterval = dvbt2_guardinterval_1_32;
				dvbt2_parm.input_ts_hem_en = 1;
				dvbt2_parm.issy = dvbt2_issy_disable;
				dvbt2_parm.l1_constellation = dvbt2_l1_constellation_qam16;
				dvbt2_parm.nid = 0;
				dvbt2_parm.nti = 0;
				dvbt2_parm.pilotpattern = dvbt2_pilotpattern_7;
				dvbt2_parm.plp_constellation = dvbt2_plp_constellation_qam256;
				dvbt2_parm.sbn = 0;
				dvbt2_parm.sid = 0;
				dvbt2_parm.time_interval_en = 0;
				dvbt2_parm.vbr_coding_en = 0;
				dvbt2_parm.version = dvbt2_version_1_3_1;
				
				result = vatek_broadcast_modulator_setparm(bc_handle,m_base_p,(Pmodulator_advanced_parm)&dvbt2_parm);
				if(result != vatek_result_success){
					printf("dvbt2 set fail %d\r\n",result);
					return result;
				}
		}
		
		if(result != vatek_result_success){
			SELF_ERR("modulator choose error %d",result);
			return result;
		}
		return result;
	
}
#endif

#if defined(VATEK_A3)
vatek_result a_modulator_select(Phtransform tf_handle, modulator_type m_type,uint32_t bandwidth){//we can choose which mod we want to use and use
		vatek_result result = vatek_result_unknown;
		
		if(m_type == m_type_atsc){
				modulator_base_parm m_base_p = {0};
				m_base_p.bw_sb = bandwidth;
				m_base_p.dacgain = 0;
				m_base_p.iffreq = 0;
				m_base_p.ifmode = m_ifmode_disable;
				m_base_p.type = m_type_atsc;
				modulator_atsc_parm atsc_parm = {0};
				atsc_parm.constellation = atsc_constellation_8vsb;
				result = vatek_transform_modulator_setparm(tf_handle,m_base_p, (Pmodulator_advanced_parm)&atsc_parm);
			}
		else if(m_type == m_type_j83b){
				modulator_base_parm m_base_p = {0};
				m_base_p.bw_sb = bandwidth;
				m_base_p.dacgain = 0;
				m_base_p.iffreq = 0;
				m_base_p.ifmode = m_ifmode_disable;
				m_base_p.type = m_type_j83b;
				modulator_j83b_parm j83b_parm = {0};
				j83b_parm.constellation = j83b_constellation_qam64;
				result = vatek_transform_modulator_setparm(tf_handle,m_base_p,(Pmodulator_advanced_parm)&j83b_parm);
			}
		else if(m_type == m_type_j83a){
				modulator_base_parm m_base_p = {0};
				m_base_p.bw_sb = 6875;
				m_base_p.dacgain = 0;
				m_base_p.iffreq = 0;
				m_base_p.ifmode = m_ifmode_disable;
				m_base_p.type = m_type_j83a;
				modulator_j83a_parm j83a_parm = {0};
				j83a_parm.constellation = j83a_constellation_qam64;
				result = vatek_transform_modulator_setparm(tf_handle,m_base_p,(Pmodulator_advanced_parm)&j83a_parm);
			}
		else if(m_type == m_type_j83c){
				modulator_base_parm m_base_p = {0};
				m_base_p.bw_sb = bandwidth;
				m_base_p.dacgain = 0;
				m_base_p.iffreq = 0;
				m_base_p.ifmode = m_ifmode_disable;
				m_base_p.type = m_type_j83c;
				modulator_j83c_parm j83c_parm = {0};
				j83c_parm.constellation = j83c_constellation_qam64;
				result = vatek_transform_modulator_setparm(tf_handle,m_base_p,(Pmodulator_advanced_parm)&j83c_parm);
			}
		else if(m_type == m_type_dvb_t){
				modulator_base_parm m_base_p = {0};
				m_base_p.bw_sb = bandwidth;
				m_base_p.dacgain = 0;
				m_base_p.iffreq = 0;
				m_base_p.ifmode = m_ifmode_disable;
				m_base_p.type = m_type_dvb_t;
				modulator_dvbt_parm dvbt_parm = {0};
				dvbt_parm.coderate = dvbt_coderate_5_6;
				dvbt_parm.guardinterval = dvbt_guardinterval_1_16;
				dvbt_parm.fft = dvbt_fft_8k;
				dvbt_parm.constellation = dvbt_constellation_qam64;
				result = vatek_transform_modulator_setparm(tf_handle,m_base_p,(Pmodulator_advanced_parm)&dvbt_parm);
			}
		else if(m_type == m_type_dtmb){
				modulator_base_parm m_base_p = {0};
				m_base_p.bw_sb = bandwidth;
				m_base_p.dacgain = 0;
				m_base_p.iffreq = 0;
				m_base_p.ifmode = m_ifmode_disable;
				m_base_p.type = m_type_dtmb;
				modulator_dtmb_parm dtmb_parm = {0};
				dtmb_parm.carriermode = dtmb_carriermode_3780;
				dtmb_parm.coderate = dtmb_coderate_0_6;
				dtmb_parm.syncframe = dtmb_syncframe_595;
				dtmb_parm.ti = dtmb_time_interleaved_240;
				dtmb_parm.constellation = dtmb_constellation_qam64;
				result = vatek_transform_modulator_setparm(tf_handle,m_base_p,(Pmodulator_advanced_parm)&dtmb_parm);
			}
		else if(m_type == m_type_isdb_t){
				modulator_base_parm m_base_p = {0};
				m_base_p.bw_sb = bandwidth;
				m_base_p.dacgain = 0;
				m_base_p.iffreq = 0;
				m_base_p.ifmode = m_ifmode_disable;
				m_base_p.type = m_type_isdb_t;
				modulator_isdbt_parm isdbt_parm = {0};
				isdbt_parm.coderate = isdbt_coderate_3_4;
				isdbt_parm.fft = isdbt_fft_2k;
				isdbt_parm.guardinterval = isdbt_guardinterval_1_16;
				isdbt_parm.ti = isdbt_time_interleaved_mode1;
				isdbt_parm.constellation = isdbt_constellation_qam64;
				result = vatek_transform_modulator_setparm(tf_handle,m_base_p,(Pmodulator_advanced_parm)&isdbt_parm);
			}
		else if(m_type == m_type_dvb_t2){
				modulator_base_parm m_base_p = {0};
				m_base_p.bw_sb = 0;
				m_base_p.dacgain = 0;
				m_base_p.iffreq = 0;
				m_base_p.ifmode = m_ifmode_disable;
				m_base_p.type = m_type_dvb_t2;
				modulator_dvbt2_parm dvbt2_parm = {0};
				dvbt2_parm.coderate = dvbt2_coderate_4_5;
				dvbt2_parm.constellation_rotation_en = 1;
				dvbt2_parm.delete_null_packet_en = 0;
				dvbt2_parm.extend_carrier_mode_en = 0;
				dvbt2_parm.fec = dvbt2_fec_16200;
				dvbt2_parm.fecbn = 0;
				dvbt2_parm.fft = dvbt2_fft_8k;
				dvbt2_parm.guardinterval = dvbt2_guardinterval_1_32;
				dvbt2_parm.input_ts_hem_en = 1;
				dvbt2_parm.issy = dvbt2_issy_disable;
				dvbt2_parm.l1_constellation = dvbt2_l1_constellation_qam16;
				dvbt2_parm.nid = 0;
				dvbt2_parm.nti = 0;
				dvbt2_parm.pilotpattern = dvbt2_pilotpattern_7;
				dvbt2_parm.plp_constellation = dvbt2_plp_constellation_qam256;
				dvbt2_parm.sbn = 0;
				dvbt2_parm.sid = 0;
				dvbt2_parm.time_interval_en = 0;
				dvbt2_parm.vbr_coding_en = 0;
				dvbt2_parm.version = dvbt2_version_1_3_1;
				
				result = vatek_transform_modulator_setparm(tf_handle,m_base_p,(Pmodulator_advanced_parm)&dvbt2_parm);
				if(result != vatek_result_success){
					printf("dvbt2 set fail %d\r\n",result);
					return result;
				}
		}
		
		if(result != vatek_result_success){
			SELF_ERR("modulator choose error %d",result);
			return result;
		}
		return result;
	
}
#endif
