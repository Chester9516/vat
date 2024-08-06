#if defined(VATEK_V1)
#include "vatek_system.h"
#include "vatek_hms.h"
#include "vatek_encoder.h"
#include "vatek_tsmux.h"
#include "vatek_modulator.h"
#include "vatek_psitable.h"
#include "vatek_userdata.h"
#include "vatek_psispec_rule.h"
#include "vatek_psispec_default.h"

#if defined(LOG_TABLE)
#include "vatek_log_table.h"
#endif

#define BROADCAST_ERR(fmt,...)    VATEK_ERR(bc,fmt,##__VA_ARGS__)
#define BROADCAST_LOG(fmt,...)    VATEK_LOG(bc,fmt,##__VA_ARGS__)

vatek_result vatek_encoder_chipstatus(Phbroadcast handle, chip_status *status);

#if defined(LOG_TABLE)
static vatek_result broadcast_dump( Phms_handle handle)
{
    vatek_result result = vatek_result_unknown;
    
    if(handle==NULL) return vatek_result_invalidparm;
    
    uint32_t vi_type, vi_resolution, vi_framerate, vi_aspectrate, ve_type, ae_type, psi_type, mod_type, rf_freq;
    
    result = vatek_hms_read_hal_v1( handle, HALREG_ENCODER_MODE, &vi_type);
    result |= vatek_hms_read_hal_v1( handle, HALREG_VIDEO_RESOLUTION, &vi_resolution);
    result |= vatek_hms_read_hal_v1( handle, HALREG_VIDEO_FRAMERATE,  &vi_framerate);
    result |= vatek_hms_read_hal_v1( handle, HALREG_VIDEO_ASPECTRATE, &vi_aspectrate);
    result |= vatek_hms_read_hal_v1( handle, HALREG_VIDEO_CODEC, &ve_type);
    result |= vatek_hms_read_hal_v1( handle, HALREG_AUDIO_CODEC, &ae_type);
    result |= vatek_hms_read_hal_v1( handle, HALREG_MUXPSI_MODE, &psi_type);
    result |= vatek_hms_read_hal_v1( handle, HALREG_MOD_MODE, &mod_type);
    result |= vatek_hms_read_hal_v1( handle, HALREG_RF_COM_FREQ, &rf_freq);
    if( result == vatek_result_success)
    {
        BROADCAST_LOG("");
        BROADCAST_LOG("%10s : %s", "vi_type"  , CONVERT_LOG(vi_source_type , vi_type));
        BROADCAST_LOG("%10s : %s", "resolution"  , CONVERT_LOG(vi_resolution_type , vi_resolution));
        BROADCAST_LOG("%10s : %s", "framerate"   , CONVERT_LOG(vi_framerate_type  , vi_framerate));
        BROADCAST_LOG("%10s : %s", "aspectrate"  , CONVERT_LOG(vi_aspectrate_type , vi_aspectrate));
        BROADCAST_LOG("%10s : %s", "ve_type"  , CONVERT_LOG(ve_encoder_type, ve_type));
        BROADCAST_LOG("%10s : %s", "ae_type"  , CONVERT_LOG(ae_encoder_type, ae_type));
        BROADCAST_LOG("%10s : %s", "psi_type" , CONVERT_LOG(psi_mode_type  , psi_type));
        BROADCAST_LOG("%10s : %s", "mod_type" , CONVERT_LOG(mod_mode_type  , mod_type));
        BROADCAST_LOG("%10s : %d", "rf_freq"  , rf_freq);
    }
    
    return result;
}
#endif

static vatek_result broadcast_ctrl(Phms_handle handle, uint32_t cmd, uint32_t *errcode)
{
    vatek_result result = vatek_result_unknown;
    uint32_t timeout = 0;

    if ((result = vatek_hms_write_hal_v1(handle, HALREG_BROADCAST_CNTL, cmd)) == vatek_result_success)
    {
//        while (timeout < 50) //5 sec
//        {
//            vatek_system_delay(100);
//            if ((result = vatek_hms_read_hal_v1(handle, HALREG_BROADCAST_CNTL, errcode)) != vatek_result_success)
//                return result;
////						printf("errcode = %d\r\n",*errcode);
//            if (*errcode == 0)
//                break;
//            timeout++;
//        }

//        if (timeout >= 50)
//            result = vatek_result_timeout;
//        else
//        {
//            if ((result = vatek_hms_read_hal_v1(handle, HALREG_SYS_ERRCODE, errcode)) != vatek_result_success)
//                return result;

//            if (*errcode != HALREG_ERR_SUCCESS)
//                result = vatek_result_hwfail;
//        }
		printf("bc start and not wait 0x600 change to 0\r\n");
    }

    return result;
}

static vatek_result broadcast_reset(Phms_handle handle)
{
    vatek_result result = vatek_result_unknown;
    uint32_t timeout = 0;

    if (handle == NULL)
        return vatek_result_invalidparm;

    BROADCAST_LOG("encoder reset");
    if ((result = vatek_hms_reset(handle)) != vatek_result_success)
        return result;

    //polling until chip ready or timeout (5 sec)
    BROADCAST_LOG("waiting chip ready");
    while (((result = vatek_hms_issystemidle_v1(handle)) != vatek_result_success) && timeout <= 50)
    {  
        vatek_system_delay(100);
        timeout++;
    }
    
    if (timeout >= 50)
    {
        BROADCAST_ERR("reset timeout");
        result = vatek_result_timeout;
    }
    
    BROADCAST_LOG("chip ready");
    
    return result;
}

static vatek_result broadcast_getstatus(Phbroadcast handle, broadcast_status *status)
{
    vatek_result result = vatek_result_unknown;
    uint32_t value = 0;

    if (handle == NULL || status == NULL)
        return vatek_result_invalidparm;

    *status = bc_status_unknown;
    if ((result = vatek_hms_read_hal_v1(handle, HALREG_BCINFO_STATUS, &value)) != vatek_result_success)
        return result;

    switch (value)
    {
        case BCSTATUS_IDLE:
            *status = bc_status_idle; 
            break;

        case BCSTATUS_WAIT_SOURCE:
            *status = bc_status_wait_source;
            break;

        case BCSTATUS_BROADCAST:
            *status = bc_status_broadcast;
            break;

        case BCSTATUS_FINISH:
            *status = bc_status_finish;
            break;
            
        case BCSTATUS_FAIL_SOURCE:
            *status = bc_status_fail_source;
            break;

        case BCSTATUS_FAIL_TIMEOUT:
            *status = bc_status_fail_timeout;
            break;

        case BCSTATUS_FAIL_CODECDROP:
            *status = bc_status_fail_codecdrop;
            break;

        case BCSTATUS_FAIL_BUFFER:
            *status = bc_status_fail_buffer;
            break;

        case BCSTATUS_FAIL_MUXER:
            *status = bc_status_fail_muxer;
            break;

        case BCSTATUS_FAIL_ENCODE:
            *status = bc_status_fail_encode;
            break;

        case BCSTATUS_FAIL_MEDIA:
            *status = bc_status_fail_media;
            break;

        case BCSTATUS_FAIL_DEMUX:
            *status = bc_status_fail_demux;
            break;

        default:
        case BCSTATUS_FAIL_UNKNOWN:
            *status = bc_status_fail_unknown;
            break;
    }

    return result;
}

static vatek_result broadcast_getinfo(Phbroadcast handle, broadcast_infotype type, uint32_t *value)
{
    vatek_result result = vatek_result_unknown;
    uint32_t reg_val = 0;
    if(handle==NULL || value==NULL)
        return vatek_result_invalidparm;
    
    switch(type)
    {
        case bc_infotype_chipid:
            result = vatek_hms_read_hal_v1(handle, HALREG_CHIP_ID, value);
        break;
        
        case bc_infotype_fw_ver:
            result = vatek_hms_read_hal_v1(handle, HALREG_FW_VER, value);
        break;
        
        case bc_infotype_sdk_ver:
            *value = SDK_VERSION;
        break;
        
        case bc_infotype_service_mode:
            result = vatek_hms_read_hal_v1(handle, HALREG_SERVICE_MODE, value);
        break;
        
        case bc_infotype_peripheral_en:
            result = vatek_hms_read_hal_v1(handle, HALREG_PERIPHERAL_EN, value);
        break;
        
        case bc_infotype_insupport:
            result = vatek_hms_read_hal_v1(handle, HALREG_INPUT_SUPPORT, value);
        break;
        
        case bc_infotype_outsupport:
            result = vatek_hms_read_hal_v1(handle, HALREG_OUTPUT_SUPPORT, value);
        break;
				
				case v1_internal_hdmi_set:
						result = vatek_hms_write_reg_v1(handle, 0x32f0, 0x30009803);
						result = vatek_hms_write_reg_v1(handle, 0x32f0, 0x30000001);
						result = vatek_hms_read_reg_v1(handle, 0x32f0, value);
//						result = vatek_hms_write_reg_v1(handle, 0x32f0, 0x30c40103);
				break;
				
				case v1_internal_h1_enable:
						result = vatek_hms_write_reg_v1(handle, 0x32f0, 0x30808103);
				break;
				
				case v1_internal_900_11:
						result = vatek_hms_write_hal_v1(handle, 0x900, 0x11);
//						result = vatek_hms_write_hal_v1(handle, 0x10, 0x0);
				break;

        default :
        break;
    }
    
    if(result!=vatek_result_success)
    {
        *value = 0xFFFFFFFF;
        return result;
    }
    
    return result;
}

vatek_result vatek_encoder_v1_create(Pboard_handle hboard, Phbroadcast *handle)
{
    vatek_result result = vatek_result_unknown;
    Phms_handle hms_handle = NULL;

    if (hboard == NULL || handle == NULL)
        return vatek_result_invalidparm;

    *handle = NULL;
    if ((result = vatek_hms_create(hboard, &hms_handle)) != vatek_result_success)
        return result;
    hms_handle->type = hms_type_broadcast;
    *handle = hms_handle;

    if ((result = broadcast_reset(*handle)) != vatek_result_success)
        return result;
  
    return result;
}

vatek_result vatek_encoder_v1_destroy(Phbroadcast handle)
{
    vatek_result result = vatek_result_unknown;

    if (handle == NULL)
        return vatek_result_invalidparm;

    result = vatek_hms_destroy((Phms_handle)handle);

    return result;
}

vatek_result vatek_encoder_v1_start(Phbroadcast handle)
{
    vatek_result result = vatek_result_unknown;
    uint32_t errcode;
    broadcast_status status = bc_status_unknown;
    uint32_t timeout = 0;
    
    if (handle == NULL)
        return vatek_result_invalidparm;

    if ((result = broadcast_ctrl(handle, BC_START, &errcode)) != vatek_result_success)
    {
        BROADCAST_ERR("BC_START fail, errcode = 0x%lX", errcode);
        return result;
    }
    
//    while (timeout < 50) // 2 sec
//    {
//        vatek_system_delay(100);
//        if ((result = broadcast_getstatus(handle, &status)) != vatek_result_success)
//            return result;

//        if (status == bc_status_broadcast)
//            break;
//        
//        timeout++;
//    }
    
    return result;
}

vatek_result vatek_encoder_v1_stop(Phbroadcast handle)
{
    vatek_result result = vatek_result_unknown;
    
    if (handle == NULL)
        return vatek_result_invalidparm;
    
    uint32_t errcode;
    if ((result = broadcast_ctrl(handle, BC_STOP, &errcode)) != vatek_result_success)
        BROADCAST_ERR("BC_STOP fail, errcode = 0x%lX", errcode);
    
    if ((result = broadcast_reset(handle)) != vatek_result_success)
        return result;

    return result;
}

vatek_result vatek_encoder_v1_reset(Phbroadcast handle)
{
    vatek_result result = vatek_result_unknown;
    
    if( handle == NULL)
        return vatek_result_invalidparm;
    
    if( ( result = broadcast_reset( handle)) != vatek_result_success)
        return result;
    
    return result;
}


vatek_result vatek_encoder_v1_bcstatus(Phbroadcast handle, broadcast_status *status)
{
    vatek_result result = vatek_result_unknown;

    if (handle == NULL || status == NULL)
        return vatek_result_invalidparm;

    if ((result = broadcast_getstatus(handle, status)) != vatek_result_success)
        return result;
    
    return result;
}

vatek_result vatek_encoder_v1_chipstatus(Pboard_handle hboard, chip_status *status)
{
    vatek_result result = vatek_result_unknown;
    uint32_t value = 0;

    if (hboard == NULL || status == NULL)
        return vatek_result_invalidparm;

    *status = chip_status_unknown;
    if ((result = vatek_hms_read_hal_v1(hboard, HALREG_SYS_STATUS_0, &value)) != vatek_result_success)
        return result;

    if (value == 0xFFFFFFFF || value == 0)
    {
        *status = chip_status_idle;
        return result;
    }

    if((value & SYS_STATUS_TAG) == SYS_STATUS_TAG)
    {
        switch (value)
        {
            case SYS_STATUS_IDLE:
                *status = chip_status_wait_command;
                break;

            case SYS_STATUS_RUN:
                *status = chip_status_running;
                break;

            default:
                *status = chip_status_fail;
                break;
        }
    }
    else
        *status = chip_status_badfw;
    
    return result;
}

/*check internal HDMI status*/
static vatek_result vatek_encoder_v1_check_source(Pboard_handle handle, internal_hdmi_status *hdmi_status)
{
	vatek_result result = vatek_result_unknown;
	uint32_t hdmi_check = 0;
	if((result = vatek_hms_read_hal_v1(handle, HALREG_HDMI_STATUS, &hdmi_check)) != vatek_result_success){
		return result;
	}
	printf("hdmi status = 0x%x\r\n",hdmi_check);
	if(hdmi_check & HDMI_CONNECT){
//		printf("hdmi conncet\r\n");
		if(hdmi_check & HDMI_HPD_EN){
//			printf("hdmi hot plug in enable\r\n");
//			if(hdmi_check & HDMI_DE_VLD){
//				printf("hdmi data enable valid\r\n");
				//do something, maybe check status
				*hdmi_status = internal_hdmi_lock;
				return result;
//			}else{
//				*hdmi_status = internal_hdmi_node_vld;
//				printf("hdmi data not enable\r\n");
//			}
		}else{
			*hdmi_status = internal_hdmi_node_vld;
			printf("hdmi hot plug in not enable\r\n");
		}
	}else{
		*hdmi_status = internal_hdmi_noconnect;
		printf("hdmi not connect\r\n");
	}
	
	return result;
}

vatek_result vatek_encoder_v1_hdmichange(Pboard_handle handle)
{
	vatek_result result = vatek_result_unknown;
	uint32_t video_val = 0, audio_val = 0, resolution = 0,frameraate = 0, samplerate = 0;
	internal_hdmi_status hdmi_status = internal_hdmi_unknown;
	uint32_t vic_val = 0;
	static internal_hdmi_status hdmi_temp_status = internal_hdmi_unknown;
	
	if((result = vatek_hms_read_hal_v1(handle, HALREG_HDMI_VIN_VIC, &vic_val)) != vatek_result_success){
		printf("get vic val error\r\n");
		return result;
	}
	
	if((result = vatek_encoder_v1_check_source(handle, &hdmi_status)) != vatek_result_success){
		return result;
	}
	printf("internal HDMI status = %d\r\n", hdmi_status);
	
	if(hdmi_status != internal_hdmi_lock){
		printf("no HDMI source\r\n");
		hdmi_temp_status = hdmi_status;
		return vatek_result_idle; //no HDMI source, so return idle status.
	}
	
	if(hdmi_temp_status == internal_hdmi_lock)
		return vatek_result_samesource;
	
	hdmi_temp_status = hdmi_status;
	
	if((result = vatek_hms_read_hal_v1(handle, HALREG_HDMI_VIDEO_STS, &video_val)) != vatek_result_success){
		return result;
	}
	if((result = vatek_hms_read_hal_v1(handle, HALREG_HDMI_VID_RESOLUTION, &resolution)) != vatek_result_success){
		return result;
	}
	if((result = vatek_hms_read_hal_v1(handle, HALREG_HDMI_VID_FRAMERATE, &frameraate)) != vatek_result_success){
		return result;
	}
	if((result = vatek_hms_read_hal_v1(handle, HALREG_HDMI_AUDIO_STS, &audio_val)) != vatek_result_success){
		return result;
	}
	if(video_val == 1){
		printf("VIC change, means resolution change, %d, %d, %d\r\n",video_val, resolution, frameraate);
	}else{
//		printf("VIC not change, means resolution not change, %d, %d, %d\r\n",video_val, resolution, frameraate);
	}
	if(audio_val == 1){
		if((result = vatek_hms_read_hal_v1(handle, HALREG_HDMI_AUD_SAMPLERATE, &samplerate)) != vatek_result_success){
			return result;
		}
		printf("sample rate change 0x%x, please reset\r\n",samplerate);
	}

	return result;
	
}

/**
* @brief null null
* @details 
*
* @param null null
* @param null null
* 
* @return 
*   @retval vatek_result_success 
*   @retval vatek_result_unknown
*   @retval vatek_result_invalidparm
*   @retval vatek_result_timeout
*   @retval vatek_result_memfail
*   @retval vatek_result_fromat
*   @retval vatek_result_badstatus 
*   @retval vatek_result_busy
*   @retval vatek_result_idle
*   @retval vatek_result_overrange
*   @retval vatek_result_hwfail
*   @retval vatek_result_unsupport
*   @retval vatek_result_negative
*   @retval vatek_result_bufoverflow
*   @retval vatek_result_i2cfail
*   @retval vatek_result_gpiofail
*   @retval vatek_result_uartfail
*/
vatek_result vatek_encoder_v1_getinfo(Phbroadcast handle, broadcast_infotype type, uint32_t *value)
{
    vatek_result result = vatek_result_unknown;
    
    if (handle == NULL || value == NULL)
        return vatek_result_invalidparm;
    
    result = broadcast_getinfo(handle, type, value);

    return result;
}

vatek_result vatek_encoder_v1_setinputparm_phy(Phbroadcast handle, video_input_parm vparm, audio_input_parm aparm)
{
    vatek_result result = vatek_result_unknown;
//		video_input_parm vparm = {0};
//		audio_input_parm aparm = {0};

    if (handle == NULL)
        return vatek_result_invalidparm;

    result = vatek_encoder_setinputparm_phy_v1((Phms_handle)handle, vparm, aparm);
    
    return result;
}

vatek_result vatek_encoder_v1_setinputparm_logo(Phbroadcast handle, logo_input_parm parm)
{
    vatek_result result = vatek_result_unknown;

    if (handle == NULL)
        return vatek_result_invalidparm;

    result = vatek_encoder_setinputparm_logo_v1((Phms_handle)handle, parm);
    
    return result;
}

vatek_result vatek_encoder_v1_setencodeparm(Phbroadcast handle, video_encode_v1_parm vparm, audio_encode_parm aparm)
{
    vatek_result result = vatek_result_unknown;

    if (handle == NULL)
        return vatek_result_invalidparm;

    if ((result = vatek_userdata_setvideocodec(vparm.type)) != vatek_result_success)
        BROADCAST_ERR("set userdata's video enc type fail");
    
    result = vatek_encoder_setencodeparm_v1((Phms_handle)handle, vparm, aparm);
    
    return result;
}

vatek_result vatek_encoder_v1_setmuxparm(Phbroadcast handle, encoder_mux_parm parm)
{
    vatek_result result = vatek_result_unknown;

    if (handle == NULL)
        return vatek_result_invalidparm;

    result = vatek_encoder_setmuxparm_v1((Phms_handle)handle, parm);
    
    return result;
}

vatek_result vatek_encoder_v1_setqualityparm(Phbroadcast handle, encoder_quality_parm parm)
{
    vatek_result result = vatek_result_unknown;

    if (handle == NULL)
        return vatek_result_invalidparm;

    result = vatek_encoder_setqualityparm_v1((Phms_handle)handle, parm);
    
    return result;
}

vatek_result vatek_encoder_v1_tsmux_setparm(Phbroadcast handle, tsmux_type type, Ptsmux_parm parm)
{
    vatek_result result = vatek_result_unknown;

    if (handle == NULL || parm == NULL)
        return vatek_result_invalidparm;

    result = vatek_tsmux_setparm_v1((Phms_handle)handle, type, parm);
    
    return result;
}

vatek_result vatek_encoder_v1_getvideoinfoparm(Phbroadcast handle, Pvideo_info_parm parm)
{
    vatek_result result = vatek_result_unknown;

    if (handle == NULL)
        return vatek_result_invalidparm;

    result = vatek_encoder_getvideoinfo_v1((Phms_handle)handle, parm);
    
    return result;
}

#if defined (DEBUG)
vatek_result vatek_encoder_hal_write_v1(Phbroadcast handle, uint32_t addr, uint32_t val)
{
    vatek_result result = vatek_result_unknown;
    
    if((result = vatek_hms_write_hal_v1(handle, addr, val)) != vatek_result_success)
        return result;
    
    return result;
}

vatek_result vatek_encoder_hal_read_v1(Phbroadcast handle, uint32_t addr, uint32_t* val)
{
    vatek_result result = vatek_result_unknown;
    
    if((result = vatek_hms_read_hal_v1(handle, addr, val)) != vatek_result_success)
        return result;
    
    return result;
}

vatek_result vatek_encoder_reg_write_v1(Phbroadcast handle, uint32_t addr, uint32_t val)
{
    vatek_result result = vatek_result_unknown;
    
    if((result = vatek_hms_write_reg_v1(handle, addr, val)) != vatek_result_success)
        return result;
    
    return result;
}

vatek_result vatek_encoder_reg_read_v1(Phbroadcast handle, uint32_t addr, uint32_t* val)
{
    vatek_result result = vatek_result_unknown;
    
    if((result = vatek_hms_read_reg_v1(handle, addr, val)) != vatek_result_success)
        return result;
    
    return result;
}
#endif

vatek_result vatek_encoder_v1_down_scale(Phbroadcast handle, video_input_parm v_parm, video_scale_resolution vi_res)
{
	vatek_result result = vatek_result_unknown;
	audio_input_parm aparm = {0};
	if(!v_parm.down_scale){
		printf("no scaler mode, please enable scaler mode\r\n");
		return result;
	}
	uint32_t resolution = 0;
    switch (vi_res)
    {
        case scale_resolution_480i:
            resolution = RESOLUTION_480I;
            break;

        case scale_resolution_480p:
            resolution = RESOLUTION_480P;
            break;

        case scale_resolution_576i:
            resolution = RESOLUTION_576I;
            break;

        case scale_resolution_576p:
            resolution = RESOLUTION_576P;
            break;
            
        case scale_resolution_720p:
            resolution = RESOLUTION_720P;
            break;
				
				case scale_resolution_1080i:
            resolution = RESOLUTION_1080I;
            break;
				
				case scale_resolution_1080p:
            resolution = RESOLUTION_1080P;
            break;
            
        default:
            break;
    }

    if ((result = vatek_hms_write_hal_v1((Phms_handle)handle, HALREG_SCALER_RESOLUTION, resolution)) != vatek_result_success)
        return result;
}

#if defined (LOG_TABLE)
vatek_result vatek_encoder_dump( Phbroadcast handle)
{
    if( handle == NULL)
        return vatek_result_invalidparm;
    
    return broadcast_dump(handle);
}
#endif
#endif