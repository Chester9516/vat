
#include "vatek_system.h"
#include "vatek_hms.h"
#include "vatek_encoder.h"
#include "phy/tool_hdmi.h"

#define MAX_PID 0x1FFF
#define MIN_PID 0
#define MAX_Q   32
#define MIN_Q   0

static vatek_result encoder_vi_setparm_v1(Phms_handle handle, video_input_parm vparm)
{
    vatek_result result = vatek_result_unknown;
		printf("into external set parameter v1\r\n");
    if (vparm.buswidth_16 == 0)
    {
        if (vparm.resolution > vi_resolution_bus8_max)
        {
            ENCODER_ERR("vi resolution overrange");
            return vatek_result_overrange;
        }
    }
    else
    {
        if (vparm.resolution > vi_resolution_max)
        {
            ENCODER_ERR("vi resolution overrange");
            return vatek_result_overrange;
        }
    }

    if (vparm.aspectrate > vi_aspectrate_max)
    {
        ENCODER_ERR("vi aspectrate overrange");
        return vatek_result_overrange;
    }

    uint32_t resolution = 0, framerate = 0, clk = 0;
    switch (vparm.resolution)
    {
        case vi_resolution_1080p60:
            resolution = RESOLUTION_1080P;
            framerate = FRAMERATE_60;
            clk = 148500;
            break;

        case vi_resolution_1080p59_94:
            resolution = RESOLUTION_1080P;
            framerate = FRAMERATE_59_94;
            clk = 148351;
            break;

        case vi_resolution_1080p50:
            resolution = RESOLUTION_1080P;
            framerate = FRAMERATE_50;
            clk = 148500;
            break;

        case vi_resolution_1080p30:
            resolution = RESOLUTION_1080P;
            framerate = FRAMERATE_30;
            clk = 74250;
            break;
            
        case vi_resolution_1080p25:
            resolution = RESOLUTION_1080P;
            framerate = FRAMERATE_25;
            clk = 74250;
            break;
            
        case vi_resolution_1080p24:
            resolution = RESOLUTION_1080P;
            framerate = FRAMERATE_24;
            clk = 74250;
            break;

        case vi_resolution_1080p23_97:
            resolution = RESOLUTION_1080P;
            framerate = FRAMERATE_23_97;
            clk = 74176;
            break;

        case vi_resolution_1080i60:
            resolution = RESOLUTION_1080I;
            framerate = FRAMERATE_60;
            clk = 74250;
            break;

        case vi_resolution_1080i59_94:
            resolution = RESOLUTION_1080I;
            framerate = FRAMERATE_59_94;
            clk = 74176;
            break;

        case vi_resolution_1080i50:
            resolution = RESOLUTION_1080I;
            framerate = FRAMERATE_50;
            clk = 74250;
            break;

        case vi_resolution_720p60:
            resolution = RESOLUTION_720P;
            framerate = FRAMERATE_60;
            clk = 74250;
            break;

        case vi_resolution_720p59_94:
            resolution = RESOLUTION_720P;
            framerate = FRAMERATE_59_94;
            clk = 74176;
            break;

        case vi_resolution_720p50:
            resolution = RESOLUTION_720P;
            framerate = FRAMERATE_50;
            clk = 74250;
            break;

        case vi_resolution_576p50:
            resolution = RESOLUTION_576P;
            framerate = FRAMERATE_50;
            clk = 27000;
            break;

        case vi_resolution_576i50:
            resolution = RESOLUTION_576I;
            framerate = FRAMERATE_50;
            clk = 13500;
            break;

        case vi_resolution_480p60:
            resolution = RESOLUTION_480P;
            framerate = FRAMERATE_60;
            clk = 27000;
            break;

        case vi_resolution_480p59_94:
            resolution = RESOLUTION_480P;
            framerate = FRAMERATE_59_94;
            clk = 27000;
            break;

        case vi_resolution_480i60:
            resolution = RESOLUTION_480I;
            framerate = FRAMERATE_60;
            clk = 13500;
            break;

        case vi_resolution_480i59_94:
            resolution = RESOLUTION_480I;
            framerate = FRAMERATE_59_94;
            clk = 13500;
            break;
            
        case vi_resolution_576p25:
            resolution = RESOLUTION_576P;
            framerate = FRAMERATE_25;
            clk = 13500;
            break;

        case vi_resolution_480p30:
            resolution = RESOLUTION_480P;
            framerate = FRAMERATE_30;
            clk = 13500;
            break;

        case vi_resolution_480p29_97:
            resolution = RESOLUTION_480P;
            framerate = FRAMERATE_29_97;
            clk = 13500;
            break;

        case vi_resolution_unknown:
            resolution = RESOLUTION_1080P;
            framerate = FRAMERATE_60;
            clk = 148500;
        default:
            break;
    }

    if (vparm.buswidth_16 == 0)
        clk = clk << 1;
    

    if ((result = vatek_hms_write_hal_v1(handle, HALREG_BROADCAST_STREAM, STREAM_ENCODER)) != vatek_result_success)
        return result;

    if ((result = vatek_hms_write_hal_v1(handle, HALREG_ENCODER_MODE, vparm.input_mode)) != vatek_result_success)//vparm.input_mode, ENCMOD_VI_0
        return result;

    if ((result = vatek_hms_write_hal_v1(handle, HALREG_VIDEO_RESOLUTION, resolution)) != vatek_result_success)
        return result;

    if ((result = vatek_hms_write_hal_v1(handle, HALREG_VIDEO_FRAMERATE, framerate)) != vatek_result_success)
        return result;

    if ((result = vatek_hms_write_hal_v1(handle, HALREG_VI_0_PIXELCLOCK, clk)) != vatek_result_success)
        return result;

    uint32_t value = 0;
    switch (vparm.aspectrate)
    {
        case vi_aspectrate_16_9:
            value = ASPECTRATE_16_9;
            break;

        case vi_aspectrate_4_3:
            value = ASPECTRATE_4_3;
            break;

        case vi_aspectrate_unknown:
        default:
            value = ASPECTRATE_16_9;
            break;
    }

    if ((result = vatek_hms_write_hal_v1(handle, HALREG_VIDEO_ASPECTRATE, value)) != vatek_result_success)
        return result;

    value = 0;
    if (vparm.buswidth_16)
        value |= VI_BUSWIDTH_16;

    if (vparm.separated_sync)
        value |= VI_SEPARATED_SYNC;

    if (vparm.clk_inverse)
        value |= VI_CLK_INVERSE;

    if (vparm.hsync_inverse)
        value |= VI_HSYNC_INVERSE;

    if (vparm.vsync_inverse)
        value |= VI_VSYNC_INVERSE;

    if (vparm.field_inverse)
        value |= VI_FIELD_INVERSE;

    if (vparm.ext_half_fps)
        value |= VI_EXT_HALF_FPS;
		
		if (vparm.down_scale)
				value |= VI_DOWN_SCALER;

    if ((result = vatek_hms_write_hal_v1(handle, HALREG_VI_0_FLAGS, value)) != vatek_result_success)
        return result;

    value = vparm.offset_x;
    if ((result = vatek_hms_write_hal_v1(handle, HALREG_VI_0_OFFSET_X, value)) != vatek_result_success)
        return result;
    
    value = vparm.offset_y;
    if ((result = vatek_hms_write_hal_v1(handle, HALREG_VI_0_OFFSET_Y, value)) != vatek_result_success)
        return result;

    return result;
}

static vatek_result encoder_vi_setparm_internal_v1(Phms_handle handle, video_input_parm vparm)
{
    vatek_result result = vatek_result_unknown;
		printf("into internal set parameter v1\r\n");

    if ((result = vatek_hms_write_hal_v1(handle, HALREG_BROADCAST_STREAM, STREAM_ENCODER)) != vatek_result_success)
        return result;

    if ((result = vatek_hms_write_hal_v1(handle, HALREG_ENCODER_MODE, vparm.input_mode)) != vatek_result_success)//vparm.input_mode, ENCMOD_VI_0
        return result;

    uint32_t value = 0;
		value = 0;
    if (vparm.buswidth_16)
        value |= VI_BUSWIDTH_16;

    if (vparm.separated_sync)
        value |= VI_SEPARATED_SYNC;

    if (vparm.clk_inverse)
        value |= VI_CLK_INVERSE;

    if (vparm.hsync_inverse)
        value |= VI_HSYNC_INVERSE;

    if (vparm.vsync_inverse)
        value |= VI_VSYNC_INVERSE;

    if (vparm.field_inverse)
        value |= VI_FIELD_INVERSE;

    if (vparm.ext_half_fps)
        value |= VI_EXT_HALF_FPS;
		
		if (vparm.down_scale)
				value |= VI_DOWN_SCALER;

    if ((result = vatek_hms_write_hal_v1(handle, HALREG_VI_0_FLAGS, value)) != vatek_result_success)
        return result;

    return result;
}

static vatek_result encoder_ai_setparm_v1(Phms_handle handle, audio_input_parm aparm)
{
    vatek_result result = vatek_result_unknown;
    
    if (aparm.samplerate > ai_samplerate_max)
    {
        ENCODER_ERR("ai samplerate overrange");
        return vatek_result_overrange;
    }

    uint32_t value = 0;
    switch (aparm.samplerate)
    {
        case ai_samplerate_48K:
            value = SAMPLERATE_48KHZ;
            break;

        case ai_samplerate_44_1K:
            value = SAMPLERATE_44_1KHZ;
            break;

        case ai_samplerate_32K:
            value = SAMPLERATE_32KHZ;
            break;

        case ai_samplerate_unknown:
        default:
            value = SAMPLERATE_48KHZ;
            break;
    }   

    if ((result = vatek_hms_write_hal_v1(handle, HALREG_AUDIO_SAMPLERATE, value)) != vatek_result_success)
        return result;

    return result;
}

static vatek_result encoder_logo_setparm_v1(Phms_handle handle, logo_input_parm parm)
{
    vatek_result result = vatek_result_unknown;
    
    if (parm.type > logo_type_max)
    {
        ENCODER_ERR("logo type overrange");
        return vatek_result_overrange;
    }

    uint32_t value = 0;
    switch (parm.type)
    {
        case logo_type_colorbar:
            value = ENCMOD_COLORBAR;
            break;

        case logo_type_bootlogo:
            value = ENCMOD_BOOTLOGO;
            break;
        
        case logo_type_unknown:
        default:
            value = ENCMOD_COLORBAR;
            break;
    }   
    
    if ((result = vatek_hms_write_hal_v1(handle, HALREG_BROADCAST_STREAM, STREAM_ENCODER)) != vatek_result_success)
            return result;

    if ((result = vatek_hms_write_hal_v1(handle, HALREG_ENCODER_MODE, value)) != vatek_result_success)
        return result;

    if (parm.type == logo_type_bootlogo)
    {
        if ((result = vatek_hms_write_hal_v1(handle, HALREG_ENCODER_TAG, parm.logoidx)) != vatek_result_success)
            return result;
    }
    
    return result;
}

static vatek_result encoder_ve_setparm_v1(Phms_handle handle, video_encode_v1_parm vparm)
{
    vatek_result result = vatek_result_unknown;
    
    if (vparm.type > ve_type_max)
    {
        ENCODER_ERR("ve type overrange");
        return vatek_result_overrange;
    }

    uint32_t value = 0;
    switch (vparm.type)
    {
        case ve_type_mpeg2:
            value = VIDEO_MPEG2;
            break;

        case ve_type_h264:
            value = VIDEO_H264;
            break;
        
        case ve_type_unknown:
        default:
            value = VIDEO_MPEG2;
            break;
    } 

    if ((result = vatek_hms_write_hal_v1(handle, HALREG_VIDEO_CODEC, value)) != vatek_result_success)
        return result;

    value = 0;
    if ((result = vatek_hms_read_hal_v1(handle, HALREG_ENCODER_FLAGS, &value)) != vatek_result_success)
        return result;
    
    if (vparm.en_interlaced)//vparm.en_interlaced
        value |= ENC_EN_INTERLACED;

    if (vparm.progressive_2_i)
        value |= ENC_EN_PROGRESSIVE_2_I;
		
//		if(vparm.en_drop_frame)
//				value |= ENC_EN_DROP_FRAME;
    
//    if (vparm.fixed_rc_threshold)
//        value |= ENC_EN_FIXED_RC_THR;
//    
//    if(vparm.en_sw_clk)
//        value |= ENC_EN_SW_CLK;

    if ((result = vatek_hms_write_hal_v1(handle, HALREG_ENCODER_FLAGS, value)) != vatek_result_success)
        return result;

		if((result = vatek_hms_write_hal_v1(handle, HALREG_MUX_BITRATE, vparm.output_bitrate)) != vatek_result_success)//set output mux bitrate
				return result;
		
    return result;
}

static vatek_result encoder_ae_setparm_v1(Phms_handle handle, audio_encode_parm aparm)
{
    vatek_result result = vatek_result_unknown;
    
    if (aparm.type > ae_type_max)
    {
        ENCODER_ERR("ae type overrange");
        return vatek_result_overrange;
    }

    if (aparm.channel > ae_channel_max)
    {
        ENCODER_ERR("ae channel overrange");
        return vatek_result_overrange;
    }

    uint32_t value = 0;
    switch (aparm.type)
    {
        case ae_type_mp1_l2:
            value = AUDIO_MP1_L2;
            break;

        case ae_type_aac_lc_adts:
            value = AUDIO_AAC_LC_ADTS;
            break;
        
        case ae_type_ac_3:
            value = AUDIO_AC_3;
            break;
        
        case ae_type_aac_lc_latm:
            value = AUDIO_AAC_LC_LATM;
            break;
        
        case ae_type_unknown:
        default:
            value = AUDIO_MP1_L2;
            break;
    } 
    
    if ((result = vatek_hms_write_hal_v1(handle, HALREG_AUDIO_CODEC, value)) != vatek_result_success)
        return result;

    switch (aparm.channel)
    {
        case ae_channel_mute:
            value = CHANNEL_MUTE;
            break;

        case ae_channel_stereo:
            value = CHANNEL_STEREO;
            break;
        
        case ae_channel_mono_l:
            value = CHANNEL_MONO_L;
            break;
        
        case ae_channel_mono_r:
            value = CHANNEL_MONO_R;
            break;
        
        case ae_channel_stereo_mono_l:
            value = CHANNEL_STEREO_MONO_L;
            break;
        
        case ae_channel_stereo_mono_r:
            value = CHANNEL_STEREO_MONO_R;
            break;
        
        case ae_channel_unknown:
        default:
            value = CHANNEL_STEREO;
            break;
    } 

    if ((result = vatek_hms_write_hal_v1(handle, HALREG_AUDIO_CHANNEL, value)) != vatek_result_success)
        return result;
    
    value = 0;
    if ((result = vatek_hms_read_hal_v1(handle, HALREG_ENCODER_FLAGS, &value)) != vatek_result_success)
        return result;
    
    if (aparm.sw_i2s)
        value |= ENC_EN_SW_I2S;
    
    if ((result = vatek_hms_write_hal_v1(handle, HALREG_ENCODER_FLAGS, value)) != vatek_result_success)
        return result;
    

    return result;
}

static vatek_result encoder_mux_setparm_v1(Phms_handle handle, encoder_mux_parm parm)
{
    vatek_result result = vatek_result_unknown;

    if ((result = vatek_hms_write_hal_v1(handle, HALREG_VIDEO_PID, parm.video_pid)) != vatek_result_success)
        return result;

    if ((result = vatek_hms_write_hal_v1(handle, HALREG_AUDIO_PID, parm.audio_pid)) != vatek_result_success)
        return result;

    return result;
}

static vatek_result encoder_quality_setparm_v1(Phms_handle handle, encoder_quality_parm parm)
{
    vatek_result result = vatek_result_unknown;
    
    if (parm.rcmode > q_rcmode_max)
    {
        ENCODER_ERR("quality rcmode overrange");
        return vatek_result_overrange;
    }
    
    uint32_t value = 0;
    switch (parm.rcmode)
    {
        case q_rcmode_vbr:
            value = QUALITY_RC_VBR;
            break;

				case q_rcmode_fixed_q:
            value = QUALITY_FIXED_Q;
            break;
				
				case q_rcmode_auto:
            value = QUALITY_AUTO;
            break;
				
        case q_rcmode_unknown:
        default:
            value = QUALITY_RC_VBR;
            break;
    } 
    
    if ((result = vatek_hms_write_hal_v1(handle, HALREG_QUALITY_RCMODE, value)) != vatek_result_success)
        return result;

    if ((result = vatek_hms_write_hal_v1(handle, HALREG_QUALITY_GOP, parm.gop)) != vatek_result_success)
        return result;
    
    if ((result = vatek_hms_write_hal_v1(handle, HALREG_QUALITY_MINQ, parm.minq)) != vatek_result_success)
            return result;
    
    if ((result = vatek_hms_write_hal_v1(handle, HALREG_QUALITY_MAXQ, parm.maxq)) != vatek_result_success)
            return result;
    
    if ((result = vatek_hms_write_hal_v1(handle, HALREG_QUALITY_BITRATE, parm.bitrate)) != vatek_result_success)
            return result;
    
    if ((result = vatek_hms_write_hal_v1(handle, HALREG_QUALITY_LATENCY, parm.latency)) != vatek_result_success)
            return result;
		
		switch (parm.q_flag)
    {
        case q_flag_user:
            value = QUALITY_EN_USER;
            break;

				case q_flag_en_hf:
            value = QUALITY_EN_HF;
            break;
				
				case q_flag_en_hq:
            value = QUALITY_EN_HQ;
            break;
				
        case q_flag_unknown:
        default:
            value = QUALITY_EN_USER;
            break;
    } 
		if ((result = vatek_hms_write_hal_v1(handle, HALREG_QUALITY_FLAGS, value)) != vatek_result_success)
            return result;

    return result;
}

vatek_result vatek_encoder_setinputparm_phy_v1(Phms_handle handle, video_input_parm vparm, audio_input_parm aparm)
{
    vatek_result result = vatek_result_unknown;

    if (handle == NULL)
        return vatek_result_invalidparm;
    
    if ((result = vatek_hms_issystemidle_v1(handle)) != vatek_result_success)
        return result;
		
		if(vparm.input_mode == input_mode_external){
			if ((result = encoder_vi_setparm_v1(handle, vparm)) != vatek_result_success)
			{
					ENCODER_ERR("vi setparm fail");
					return result;
			}
			
		}else if(vparm.input_mode == input_mode_internal){
			if((result = encoder_vi_setparm_internal_v1(handle, vparm)) != vatek_result_success)
				{
					ENCODER_ERR("vi setparm fail");
					return result;
				}
		}
		
		if ((result = encoder_ai_setparm_v1(handle, aparm)) != vatek_result_success)
			{
					ENCODER_ERR("ai setparm fail");
					return result;
			}
		

    return result;
}

vatek_result vatek_encoder_setinputparm_logo_v1(Phms_handle handle, logo_input_parm parm)
{
    vatek_result result = vatek_result_unknown;

    if (handle == NULL)
        return vatek_result_invalidparm;

    if ((result = vatek_hms_issystemidle_v1(handle)) != vatek_result_success)
        return result;

    if ((result = encoder_logo_setparm_v1(handle, parm)) != vatek_result_success)
    {
        ENCODER_ERR("logo setparm fail");
        return result;
    }

    return result;
}

vatek_result vatek_encoder_setencodeparm_v1(Phms_handle handle, video_encode_v1_parm vparm, audio_encode_parm aparm)
{
    vatek_result result = vatek_result_unknown;

    if (handle == NULL)
        return vatek_result_invalidparm;

    if ((result = vatek_hms_issystemidle_v1(handle)) != vatek_result_success)
        return result;

    if ((result = encoder_ve_setparm_v1(handle, vparm)) != vatek_result_success)
    {
        ENCODER_ERR("ve setparm fail");
        return result;
    }

    if ((result = encoder_ae_setparm_v1(handle, aparm)) != vatek_result_success)
    {
        ENCODER_ERR("ae setparm fail");
        return result;
    }
    
    return result;
}

vatek_result vatek_encoder_setmuxparm_v1(Phms_handle handle, encoder_mux_parm parm)
{
    vatek_result result = vatek_result_unknown;

    if (handle == NULL)
        return vatek_result_invalidparm;

    if (parm.video_pid > MAX_PID)
    {
        ENCODER_ERR("video pid overrange");
        return vatek_result_overrange;
    }

    if (parm.audio_pid > MAX_PID)
    {
        ENCODER_ERR("audio pid overrange");
        return vatek_result_overrange;
    }  

    if ((result = vatek_hms_issystemidle_v1(handle)) != vatek_result_success)
        return result;
    
    if ((result = encoder_mux_setparm_v1(handle, parm)) != vatek_result_success)
    {
        ENCODER_ERR("mux setparm fail");
        return result;
    }

    return result;
}

vatek_result vatek_encoder_setqualityparm_v1(Phms_handle handle, encoder_quality_parm parm)
{
    vatek_result result = vatek_result_unknown;

    if (handle == NULL)
        return vatek_result_invalidparm;

    if (parm.minq > MAX_Q)
    {
        ENCODER_ERR("min q overrange");
        return vatek_result_overrange;
    }

    if (parm.maxq > MAX_Q)
    {
        ENCODER_ERR("max q overrange");
        return vatek_result_overrange;
    }

    if ((result = vatek_hms_issystemidle_v1(handle)) != vatek_result_success)
        return result;
    
    if ((result = encoder_quality_setparm_v1(handle, parm)) != vatek_result_success)
    {
        ENCODER_ERR("quality setparm fail");
        return result;
    }
    
    return result;
}

vatek_result vatek_encoder_getvideoinfo_v1(Phms_handle handle, Pvideo_info_parm v_info_parm)
{
	vatek_result result = vatek_result_unknown;
	uint32_t vic_val = 0;
	hdmi_video_timing hdmi_v_timing = {0};
	if((result = vatek_hms_read_hal_v1(handle, HALREG_HDMI_VIN_VIC, &vic_val)) != vatek_result_success){
		ENCODER_ERR("get vic val error");
		return result;
	}
	printf("vic = 0x%x\r\n",vic_val);
	if((result = tool_hdmi_get_videoinfo(vic_val, &hdmi_v_timing)) != vatek_result_success){
		ENCODER_ERR("get video info error");
		return result;
	}
	v_info_parm->resolution = hdmi_v_timing.resolution;
	v_info_parm->aspectrate = hdmi_v_timing.aspectrate;
	
	return result;
}
