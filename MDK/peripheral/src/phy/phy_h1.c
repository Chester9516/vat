
#if defined(PHY_H1)

#include "vatek_system.h"
#include "phy/phy_h1_internal.h"
#include "phy/tool_hdmi.h"
#include "main.h"
extern vatek_result h1_enable( Ph1_handle hh1, uint8_t enable);

/** previous vinfo, vout */
static uint8_t g_vout = 0;
static phy_video_info g_vinfo;
uint32_t h1_rev = 0;

static vatek_result enable_vout(Ph1_handle hh1, uint8_t enable);
vatek_result get_input_timming( Ph1_handle hh1, Phdmi_video_timing timing);
vatek_result set_output_timing( Ph1_handle hh1, Phdmi_video_timing ptiming);
static vatek_result i2c_write(Pvatek_i2c vi2c, uint8_t dev, uint16_t reg, uint8_t val)
{
	uint8_t buf[2] = {0};
    vatek_result result = vatek_result_unknown;
    
    if (vi2c == NULL)
        return vatek_result_invalidparm;

    if ((result = vi2c->start(vi2c->hboard, H1_I2C_WRITE, 0)) != vatek_result_success)
        return vatek_result_i2cfail;

    buf[0] = (uint8_t)(reg >> 8);
    buf[1] = (uint8_t)reg;
    if ((result = vi2c->write(vi2c->hboard, &buf[0], 2)) != vatek_result_success)
        return vatek_result_i2cfail;

    if ((result = vi2c->write(vi2c->hboard, &val, 1)) != vatek_result_success)
        return vatek_result_i2cfail;

    if ((result = vi2c->stop(vi2c->hboard)) != vatek_result_success)
        return vatek_result_i2cfail;
    
    vatek_system_delay(1);
    return result;
}

static vatek_result i2c_read(Pvatek_i2c vi2c, uint8_t dev, uint16_t reg, uint8_t* pbuf, int32_t len)
{
	uint8_t buf[2] = {0};
    vatek_result result = vatek_result_unknown;
    
    if (vi2c == NULL || pbuf == NULL || len == 0)
        return vatek_result_invalidparm;

    if ((result = vi2c->start(vi2c->hboard, H1_I2C_WRITE, 0)) != vatek_result_success)
        return vatek_result_i2cfail;

    buf[0] = (uint8_t)(reg >> 8);
    buf[1] = (uint8_t)reg;
    if ((result = vi2c->write(vi2c->hboard, &buf[0], 2)) != vatek_result_success)
        return vatek_result_i2cfail;

    if ((result = vi2c->start(vi2c->hboard, H1_I2C_READ, 1)) != vatek_result_success)
        return vatek_result_i2cfail;
    
    if ((result = vi2c->read(vi2c->hboard, pbuf, len)) != vatek_result_success)
        return vatek_result_i2cfail;

    if ((result = vi2c->stop(vi2c->hboard)) != vatek_result_success)
            return vatek_result_i2cfail;

    return result;
}

static vatek_result i2c_reg_write( Pvatek_i2c vi2c, uint16_t reg, uint8_t val)
{
    return i2c_write(vi2c, H1_I2C_ADDR, reg, val);
}

static vatek_result i2c_write_double_reg( Pvatek_i2c vi2c, uint16_t reg, uint16_t val)
{
    vatek_result result = vatek_result_unknown;
    uint8_t buf[2] = {0};
    
    buf[0] = (uint8_t)(val>>8);
    buf[1] = (uint8_t) val;
    
    result = H1_REG_WR( vi2c, reg, buf[0]);
    if( IS_SUCCESS(result))
        result = H1_REG_WR( vi2c, reg+1, buf[1]);
    
    return result;
}

static vatek_result get_h1_rev(Pvatek_i2c vi2c)
{
    vatek_result result = vatek_result_unknown;
    uint8_t val = 0;
    
    result = H1_REG_RD(vi2c, H1_FW_REV2, &val);
    if( IS_SUCCESS(result))
    {
        h1_rev = val;
        result = H1_REG_RD( vi2c, H1_FW_REV1, &val);
    }
    if( IS_SUCCESS(result))
    {
        h1_rev |= val<<8;
        result = H1_REG_RD( vi2c, H1_FW_REV0, &val);
    }
    if( IS_SUCCESS(result)) h1_rev |= val<<16;
    printf("h1_rev = 0x%x\r\n",h1_rev);
    
    return result;
}

static vatek_result check_source(Ph1_handle hh1, h1_status *status)
{
    vatek_result result = vatek_result_unknown;
    uint8_t val, hdcp;
    
    *status = h1_hw_fail;
    if((result = H1_REG_RD( hh1->vi2c, H1_SYS_STATUS2, &val)) != vatek_result_success)
        return result;
    
    *status = h1_idle;
    if( val&H1_SYS_STATUS2_HPD)
    {
        if(val&H1_SYS_STATUS2_LINK)
        {
            *status = h1_invailed;
            if(val&H1_SYS_STATUS2_DE)
            {
                *status = h1_active;
                if((result=H1_REG_RD(hh1->vi2c, H1_SYS_STATUS1, &hdcp))!=vatek_result_success)
                    return result;
//                H1_LOG("H1_SYS_STATUS1(0x105) = 0x%x",hdcp);
                if(hdcp&H1_SYS_STATUS1_AUTH)
                {
//                        H1_LOG("HDCP AUTH");
                    if(hdcp&H1_SYS_STATUS1_DECRYPT)
                    {
//                            H1_LOG("HDCP DECRYPT");
                        *status = h1_active_protect;
                    }
//										else H1_LOG("not HDCP DECRYPT.");
                }
//								else H1_LOG("not HDCP AUTH.");
            }else H1_LOG("linked on but de invailed.");
        }else H1_LOG("no link.");
    }
    return result;
}

static vatek_result output_format( Ph1_handle hh1, Pphy_video_info in_info)
{    
    if( hh1==NULL || in_info==NULL)
        return vatek_result_invalidparm;
    
    if( hh1->output_mode == h1_output_progressive)
    {
        switch (in_info->resolution)
        {
            case vi_resolution_1080i60:
                in_info->resolution = vi_resolution_1080p60;
                break;
            
            case vi_resolution_1080i59_94:
                in_info->resolution = vi_resolution_1080p59_94;
                break;
            
            case vi_resolution_1080i50:
                in_info->resolution = vi_resolution_1080p50;
                break;
            
            case vi_resolution_576i50:
                in_info->resolution = vi_resolution_576p50;
                break;
            
            case vi_resolution_480i60:
                in_info->resolution = vi_resolution_480p60;
                break;
            
            case vi_resolution_480i59_94:
                in_info->resolution = vi_resolution_480p59_94;
                break;
        }
    }else if( hh1->output_mode==h1_output_interlace)
    {
        switch (in_info->resolution)
        {
            case vi_resolution_1080p60:
                in_info->resolution = vi_resolution_1080i60;
                break;
            
            case vi_resolution_1080p59_94:
                in_info->resolution = vi_resolution_1080i59_94;
                break;
            
            case vi_resolution_1080p50:
                in_info->resolution = vi_resolution_1080i50;
                break;
            
            case vi_resolution_576p50:
                in_info->resolution = vi_resolution_576i50;
                break;
            
            case vi_resolution_480p60:
                in_info->resolution = vi_resolution_480i60;
                break;
            
            case vi_resolution_480p59_94:
                in_info->resolution = vi_resolution_480i59_94;
                break;
        }
    }
    
    if( hh1->baseclk == h1_baseclk_1001)
    {
        switch( in_info->resolution)
        {
            case vi_resolution_1080p60:
                in_info->resolution = vi_resolution_1080p59_94;
                break;
            
            case vi_resolution_1080p24:
                in_info->resolution = vi_resolution_1080p23_97;
                break;
            
            case vi_resolution_1080i60:
                in_info->resolution = vi_resolution_1080i59_94;
                break;
            
            case vi_resolution_720p60:
                in_info->resolution = vi_resolution_720p59_94;
                break;
            
            case vi_resolution_480p60:
                in_info->resolution = vi_resolution_480p59_94;
                break;
            
            case vi_resolution_480i60:
                in_info->resolution = vi_resolution_480i59_94;
                break;
            
            case vi_resolution_480p30:
                in_info->resolution = vi_resolution_480p29_97;
                break;
        }
    }
        
    return vatek_result_success;
}

static vatek_result set_output_format( Ph1_handle hh1, Pphy_video_info info)
{
    vatek_result result = vatek_result_unknown;
    uint8_t vout_fmt = H1_OUT_FMT_422 | H1_OUT_FMT_YCC;
    uint8_t vinfo = 0;
    
    if( hh1==NULL)
        return vatek_result_invalidparm;
    
    if((result = H1_REG_RD( hh1->vi2c, H1_VIN_FMT, &vinfo))!=vatek_result_success)
        return result;
    
    if(vinfo&H1_VIN_FMT_CS709) vout_fmt |= H1_OUT_FMT_CS709;
    
    if(hh1->output_mode==h1_output_bypass && 
       (is_480i(info->resolution)||is_576i(info->resolution)))
        vout_fmt |= H1_OUT_FMT_PR2;
    
    if((result = H1_REG_WR( hh1->vi2c, H1_OUT_FMT, vout_fmt))!=vatek_result_success)
        return result;
    
    return result;
}

vatek_result set_output_timing( Ph1_handle hh1, Phdmi_video_timing ptiming)
{
    vatek_result result = vatek_result_unknown;
    
    if( hh1==NULL || ptiming==NULL)
        return vatek_result_invalidparm;
    
    if( ptiming->represent != 0)
    {
        ptiming->hbp >>= 1;
        ptiming->hfp >>= 1;
        ptiming->hpw >>= 1;
    }
    
    result = H1_WR_DREG( hh1->vi2c, H1_VOUT_ACTIVE_H, ptiming->pixel_per_line);
    if( IS_SUCCESS(result)) result = H1_WR_DREG( hh1->vi2c, H1_VOUT_ACTIVE_V, ptiming->pixel_per_frame);
    if( IS_SUCCESS(result)) result = H1_WR_DREG( hh1->vi2c, H1_VOUT_H_DELAY, ptiming->hfp);
    if( IS_SUCCESS(result)) result = H1_WR_DREG( hh1->vi2c, H1_VOUT_H_WIDTH, ptiming->hpw);
    if( IS_SUCCESS(result)) result = H1_REG_WR( hh1->vi2c, H1_VOUT_V_DELAY, ptiming->vfp);
    if( IS_SUCCESS(result)) result = H1_REG_WR( hh1->vi2c, H1_VOUT_V_WIDTH, ptiming->vpw);
    
    return result;    
}

static vatek_result output_bypass(Ph1_handle hh1)
{
    vatek_result result = vatek_result_unknown;
    uint8_t v_interlaced = 0;
    uint8_t flag = H1_HDMI_FLAG_BYPASS_MODE;

    if(hh1==NULL)
        return vatek_result_invalidparm;
    
    /** check video input interlaced */
    if((result = H1_REG_RD( hh1->vi2c, H1_VIN_VPLUSE, &v_interlaced))!=vatek_result_success)
        return result;
    
    if(v_interlaced&H1_VIN_VPLUSE_INT) flag |= H1_HDMI_FLAG_OUT_INTERLACED;
    
    if((result = H1_REG_WR( hh1->vi2c, H1_HDMI_FLAG, flag))!=vatek_result_success)
        return result;
    
    return result;
}

static vatek_result output_progressive(Ph1_handle hh1, Pphy_video_info info)
{
    vatek_result result = vatek_result_unknown;
    hdmi_video_timing vtable = {0};
    uint8_t vid = 0;
    
    if(hh1==NULL)
        return vatek_result_invalidparm;
    
    if((result = H1_REG_WR( hh1->vi2c, H1_HDMI_FLAG, 0x00))!=vatek_result_success)
        return result;
    
    /* set video timing. */
    result = tool_hdmi_get_vid( info, &vid);
    if(IS_SUCCESS(result)) result = tool_hdmi_get_videoinfo( vid, &vtable);
    if(IS_SUCCESS(result)) result = set_output_timing( hh1, &vtable);
    if(!IS_SUCCESS(result)) return result;
    
    return result;
}

static vatek_result output_interlace(Ph1_handle hh1, Pphy_video_info info)
{
    vatek_result result = vatek_result_unknown;
    hdmi_video_timing vtable = {0};
    uint8_t vid = 0;
    
    if(hh1==NULL)
        return vatek_result_invalidparm;
    
    if((result = H1_REG_WR(hh1->vi2c, H1_HDMI_FLAG, H1_HDMI_FLAG_OUT_INTERLACED))!=vatek_result_success)
        return result;
    
    /* set video timing. */
    result = tool_hdmi_get_vid( info, &vid);
    if(IS_SUCCESS(result)) result = tool_hdmi_get_videoinfo( vid, &vtable);
    if(IS_SUCCESS(result)) result = set_output_timing( hh1, &vtable);
    if(!IS_SUCCESS(result)) return result;
    
    return result;
}

static vatek_result set_videoinfo( Ph1_handle hh1, Pphy_video_info info)
{
    vatek_result   result = vatek_result_unknown;
    h1_status status = h1_invailed;
    
    if( hh1==NULL || info==NULL)
        return vatek_result_invalidparm;
    
    if((result = check_source( hh1, &status))!=vatek_result_success)
        return result;

    /** disable video output */
    if((result = enable_vout( hh1, 0))!=vatek_result_success)
        return result;
    
    /** set video parameter */
    if( hh1->output_mode == h1_output_bypass)
        result = output_bypass(hh1);
    else if(hh1->output_mode==h1_output_progressive)
        result = output_progressive(hh1, info);
    else if(hh1->output_mode==h1_output_interlace)
        result = output_interlace(hh1, info);
    if(IS_SUCCESS(result)) result = set_output_format(hh1, info);
    if(IS_SUCCESS(result)) result = enable_vout( hh1, g_vout);
    if(!IS_SUCCESS(result)) return result;

    return result;
}

static vatek_result enable_vout(Ph1_handle hh1, uint8_t enable)
{
    vatek_result result = vatek_result_unknown;
    
    if (IS_NEW_REV(h1_rev))
    {
        if(enable){
            result = H1_REG_WR( hh1->vi2c, H1_OUT_CNTL, H1_OUT_CNTL_EN_AOUT);
						printf("enable 1101 version\r\n");
				}
        else result = H1_REG_WR( hh1->vi2c, H1_OUT_CNTL, H1_OUT_CNTL_DIS_VOUT);
    }else
    {
        if(enable == 1){
            result = H1_REG_WR( hh1->vi2c, H1_OUT_CNTL, 0x00);//0x00
						printf("enable 1007 version\r\n");
				}
        else if(enable == 0)result = H1_REG_WR( hh1->vi2c, H1_OUT_CNTL, H1_OUT_CNTL_DIS_ALL);
				else if(enable == 2){
					result = H1_REG_WR( hh1->vi2c, H1_OUT_CNTL, 0x10);
				}
    }    
    
    return result;
}

#if 0
static vatek_result get_timing( Ph1_handle hh1)
{
    vatek_result result = vatek_result_unknown;
    uint8_t  hval=0, lval=0;
    uint16_t start_addr = 0x400;
    
    if(hh1==NULL) return vatek_result_invalidparm;
    
    hdmi_video_timing v_timing;
    
    result = H1_REG_RD( hh1->vi2c, start_addr++, &hval);
    if(IS_SUCCESS(result)) result = H1_REG_RD( hh1->vi2c, start_addr++, &lval);
    if(IS_SUCCESS(result)) v_timing.pixel_per_line = (hval<<8)|(lval);
    
    result = H1_REG_RD( hh1->vi2c, start_addr++, &hval);
    if(IS_SUCCESS(result)) result = H1_REG_RD( hh1->vi2c, start_addr++, &lval);
    if(IS_SUCCESS(result)) v_timing.hfp = (hval<<8)|(lval);
    
    result = H1_REG_RD( hh1->vi2c, start_addr++, &hval);
    if(IS_SUCCESS(result)) result = H1_REG_RD( hh1->vi2c, start_addr++, &lval);
    if(IS_SUCCESS(result)) v_timing.hbp = (hval<<8)|(lval);
    
    result = H1_REG_RD( hh1->vi2c, start_addr++, &hval);
    if(IS_SUCCESS(result)) result = H1_REG_RD( hh1->vi2c, start_addr++, &lval);
    if(IS_SUCCESS(result)) v_timing.hpw = (hval<<8)|(lval);
    
    
    start_addr = 0x410;
    result = H1_REG_RD( hh1->vi2c, start_addr++, &hval);
    if(IS_SUCCESS(result)) result = H1_REG_RD( hh1->vi2c, start_addr++, &lval);
    if(IS_SUCCESS(result)) v_timing.pixel_per_frame = (hval<<8)|(lval);
    
    result = H1_REG_RD( hh1->vi2c, start_addr++, &lval);
    if(IS_SUCCESS(result)) v_timing.vfp = lval;
    
    result = H1_REG_RD( hh1->vi2c, start_addr++, &lval);
    if(IS_SUCCESS(result)) v_timing.vbp = lval;
    
    result = H1_REG_RD( hh1->vi2c, start_addr, &lval);
    if(IS_SUCCESS(result)) v_timing.vpw = lval;

    return result;
}
#endif

vatek_result h1_enable( Ph1_handle hh1, uint8_t enable)
{
    vatek_result result = vatek_result_unknown;
    
    if( hh1 == NULL)
        return vatek_result_invalidparm;
    
    if( g_vout == enable)
        return vatek_result_success;
    
    if((result = enable_vout(hh1, enable)) == vatek_result_success)
        g_vout = enable;
    
    return result;    
}

vatek_result h1_set_baseclock( Ph1_handle hh1, phy_output_baseclock clk)
{
    if(hh1==NULL)
        return vatek_result_invalidparm;
    
    hh1->baseclk = (h1_baseclk)clk;
    
    return vatek_result_success;
}

vatek_result h1_set_outputmode( Ph1_handle hh1, phy_output_mode mode)
{    
    if(hh1==NULL)
        return vatek_result_invalidparm;
    
    if(mode > h1_output_max)
        return vatek_result_unsupport;
    
    hh1->output_mode = (h1_output_mode)mode;
    
    return vatek_result_success;
}

vatek_result h1_get_videoinfo(Ph1_handle hh1, Pphy_video_info info)
{
    vatek_result result = vatek_result_unknown;
    h1_status status = h1_invailed;
    uint8_t vid = 0, pclk = 0;
    hdmi_video_timing v_info = {0};
    
    if( hh1==NULL || info==NULL)
        return vatek_result_invalidparm;
    
    if((result = check_source( hh1, &status))==vatek_result_success)
    {
        if( status!=h1_active && status!=h1_active_protect)
            return vatek_result_badstatus;
    }else return result;

    result = H1_REG_RD( hh1->vi2c, H1_VIN_VIC, &vid);
    if(IS_SUCCESS(result)) result = H1_REG_RD( hh1->vi2c, H1_HDMI_FLAG, &pclk);
    if(!IS_SUCCESS(result)) return result;
		

    if(pclk&H1_HDMI_FLAG_PCLK_1001) 
        hh1->baseclk = h1_baseclk_1001; //modify

    if((result = tool_hdmi_get_videoinfo( vid, &v_info))!=vatek_result_success)
    {
        H1_LOG("unsupporteed video codec %d", vid);
        return vatek_result_unsupport;
    }
    
    info->resolution     = v_info.resolution;
    info->aspectrate     = v_info.aspectrate;
    info->buswidth_16    = 1;
    info->separated_sync = 1;
    
    
    if((result = output_format( hh1, info)) != vatek_result_success)
        return result;
    
    if( g_vinfo.resolution==info->resolution &&
        g_vinfo.aspectrate==info->aspectrate)
        return vatek_result_success;

    if((result = set_videoinfo( hh1, info)) != vatek_result_success)
        return result;
    
    g_vinfo.resolution = info->resolution;
    g_vinfo.aspectrate = info->aspectrate;
    
    H1_LOG("set vout parameter");
    
    return result;        
}

vatek_result h1_get_audioinfo(Ph1_handle hh1, Pphy_audio_info info)
{
    #define AUDIO_SAMPLE_FREQ_32KHZ		0x00
	#define AUDIO_SAMPLE_FREQ_44_1KHZ	0x01
	#define AUDIO_SAMPLE_FREQ_48KHZ		0x02
    
    vatek_result result = vatek_result_unknown;
    uint8_t val = 0;
	uint8_t audio_reg_val = 0;
    
    if( hh1==NULL || info==NULL)
        return vatek_result_invalidparm;
    
    if((result = H1_REG_RD( hh1->vi2c, H1_AIN_STATUS, &val)) == vatek_result_success)
    {
//			printf("H1_AIN_STATUS(0x302) = 0x%x\r\n",val);
        val &= H1_AIN_STATUS_SAMP_FREQ;
        if( val == AUDIO_SAMPLE_FREQ_48KHZ)
            info->samplerate = ai_samplerate_48K;
        else if( val == AUDIO_SAMPLE_FREQ_44_1KHZ)
            info->samplerate = ai_samplerate_44_1K;
        else if( val == AUDIO_SAMPLE_FREQ_32KHZ)
            info->samplerate = ai_samplerate_32K;
        else info->samplerate = ai_samplerate_unknown;
    }

//	printf("\n\r------------------------------------------\n\r");
//	result = H1_REG_RD(hh1->vi2c,0x301c,&audio_reg_val);
//	if(result == vatek_result_success){
//		printf("h1 reg 0x301c = 0x%x\r\n",audio_reg_val);
//	}
//	result = H1_REG_RD(hh1->vi2c,0x301d,&audio_reg_val);
//	if(result == vatek_result_success){
//		printf("h1 reg 0x301d = 0x%x\r\n",audio_reg_val);
//	}
//	result = H1_REG_RD(hh1->vi2c,0x3061,&audio_reg_val);
//	if(result == vatek_result_success){
//		printf("h1 reg 0x3061 = 0x%x\r\n",audio_reg_val);
//	}
//	
//	for(int32_t temp_i = 0x3065; temp_i<= 0x306a; temp_i++){
//	  	result = H1_REG_RD(hh1->vi2c,temp_i,&audio_reg_val);
//		if(result == vatek_result_success){
//			printf("h1 reg 0x%x = 0x%x\r\n",temp_i, audio_reg_val);
//		}
//	}
//	printf("\n\r------------------------------------------\n\r");
//	for(int32_t temp_x = 0xa0; temp_x<= 0xab; temp_x++){
//	  	result = H1_REG_RD(hh1->vi2c,temp_x,&audio_reg_val);
//		if(result == vatek_result_success){
//			printf("h1 reg 0x%x = 0x%x\r\n",temp_x, audio_reg_val);
//		}
//	}
//	
//	for(int32_t temp_n = 0x10; temp_n<= 0x13; temp_n++){
//	  	result = H1_REG_RD(hh1->vi2c,temp_n,&audio_reg_val);
//		if(result == vatek_result_success){
//			printf("h1 reg 0x%x = 0x%x\r\n",temp_n, audio_reg_val);
//		}
//	}
	
    return result;   
}

vatek_result h1_get_status(Ph1_handle hh1, phy_status * status)
{
    vatek_result result = vatek_result_unknown;
    h1_status h1_status = h1_invailed;
    
    if( hh1==NULL || status==NULL)
        return vatek_result_invalidparm;
    
    if((result = check_source( hh1, &h1_status)) != vatek_result_success)
        return result;
    
    switch(h1_status)
    {
        case h1_hw_fail:
        case h1_invailed:
            *status = phy_status_fail;
            break;
        
        case h1_idle:
            *status = phy_status_lost;
            break;
        
        case h1_active:
        case h1_active_protect:
            *status = phy_status_active;
            break;
        
        default:
            *status = phy_status_unknown;
            break;
    }
    
    return vatek_result_success;
}

vatek_result h1_create(Pvatek_i2c vi2c, Ph1_handle *hh1)
{
    vatek_result result = vatek_result_unknown;
    uint8_t power_ready = 0;

    if( vi2c == NULL || hh1 == NULL)
        return vatek_result_invalidparm;
    
    H1_LOG("reset h1");
    
    if((result = vatek_system_reset( sys_dev_type_phy_h1, vi2c->hboard)) != vatek_result_success)
        return result;
    
    vatek_system_delay(1000);
    while(!power_ready)
    {

        if((result = H1_REG_RD(vi2c, H1_POWER, &power_ready)) != vatek_result_success)
            return result;
    }
    
    // get H1 revision.
    if ((result = get_h1_rev(vi2c)) != vatek_result_success)
        return result;

    if (IS_NEW_REV(h1_rev))
    {
		printf("enable 1101 version\r\n");
        uint8_t i = 0;
        for( i=0; i< sizeof(h1_init_cmd_1)/sizeof(h1_reg); i++)
        {
            if((result = H1_REG_WR(vi2c, h1_init_cmd_1[i].addr, h1_init_cmd_1[i].val)) != vatek_result_success)
                return result;
        }
    }else
    {
		printf("enable 1007 version\r\n");
        uint8_t i = 0;
        for( i=0; i< sizeof(h1_init_cmd_0)/sizeof(h1_reg); i++)
        {
            if((result = H1_REG_WR(vi2c, h1_init_cmd_0[i].addr, h1_init_cmd_0[i].val)) != vatek_result_success)
                return result;
        }
    }
        
    Ph1_handle newphy = (Ph1_handle)malloc(sizeof(h1_handle));
    if(newphy == NULL) return vatek_result_memfail;

    newphy->vi2c        = vi2c;
    newphy->output_mode = h1_output_bypass;
    newphy->baseclk     = h1_baseclk_1000;
    
    *hh1 = newphy;
    
    return result;
}

vatek_result h1_destroy(Ph1_handle hh1)
{
    vatek_result result = vatek_result_success;
    if (hh1 == NULL)
        return vatek_result_invalidparm;
    
    free(hh1);
    hh1 = NULL;

    return result;
}

#if defined(DEBUG)
vatek_result h1_write_reg(Ph1_handle hh1, uint32_t addr, uint8_t val)
{
    vatek_result result = vatek_result_success;
    if (hh1 == NULL)
        return vatek_result_invalidparm;
    
    H1_REG_WR(hh1->vi2c, addr, val);

    return result;
}

vatek_result h1_read_reg(Ph1_handle hh1, uint32_t addr, uint8_t* val)
{
    vatek_result result = vatek_result_success;
    if (hh1 == NULL)
        return vatek_result_invalidparm;
    
    H1_REG_RD(hh1->vi2c, addr, val);

    return result;
}
#endif


#endif

vatek_result get_h1_HV(Ph1_handle hh1){
	vatek_result result = vatek_result_unknown;
	uint8_t h1_val = 0;
	//uint8_t audio_reg_val = 0;

#if 0	
	printf("------------\r\n");
	
	result = H1_REG_RD(hh1->vi2c,0x301c,&audio_reg_val);
	if(result == vatek_result_success){
		printf("h1 reg 0x301c = 0x%x\r\n",audio_reg_val);
	}
	result = H1_REG_RD(hh1->vi2c,0x301d,&audio_reg_val);
	if(result == vatek_result_success){
		printf("h1 reg 0x301d = 0x%x\r\n",audio_reg_val);
	}
	result = H1_REG_RD(hh1->vi2c,0x3061,&audio_reg_val);
	if(result == vatek_result_success){
		printf("h1 reg 0x3061 = 0x%x\r\n",audio_reg_val);
	}
	
	for(int32_t temp_i = 0x3065; temp_i<= 0x306a; temp_i++){
	  	result = H1_REG_RD(hh1->vi2c,temp_i,&audio_reg_val);
		if(result == vatek_result_success){
			printf("h1 reg 0x%x = 0x%x\r\n",temp_i, audio_reg_val);
		}
	}
#endif	
	
//	uint32_t reg_addr = 0x3000;
//	for(;reg_addr<0x3100;reg_addr++){
//		result = H1_REG_RD(hh1->vi2c, reg_addr, &h1_val);
//		printf("h1 reg 0x%x = 0x%x\r\n",reg_addr,h1_val);
//	}
//	printf("----------input source format-----------\r\n");
//	result = H1_REG_RD(hh1->vi2c,0x400,&h1_val);
//	if(result == vatek_result_success)
//		printf("h1 reg 0x400 = %x, ",h1_val);
//	result = H1_REG_RD(hh1->vi2c,0x401,&h1_val);
//	if(result == vatek_result_success)
//		printf("h1 reg 0x401 = %x\r\n",h1_val);
//	result = H1_REG_RD(hh1->vi2c,0x402,&h1_val);
//	if(result == vatek_result_success)
//		printf("h1 reg 0x402 = %x, ",h1_val);
//	result = H1_REG_RD(hh1->vi2c,0x403,&h1_val);
//	if(result == vatek_result_success)
//		printf("h1 reg 0x403 = %x\r\n",h1_val);
//	result = H1_REG_RD(hh1->vi2c,0x404,&h1_val);
//	if(result == vatek_result_success)
//		printf("h1 reg 0x404 = %x, ",h1_val);
//	result = H1_REG_RD(hh1->vi2c,0x405,&h1_val);
//	if(result == vatek_result_success)
//		printf("h1 reg 0x405 = %x\r\n",h1_val);
//	result = H1_REG_RD(hh1->vi2c,0x406,&h1_val);
//	if(result == vatek_result_success)
//		printf("h1 reg 0x406 = %x, ",h1_val);
//	result = H1_REG_RD(hh1->vi2c,0x407,&h1_val);
//	if(result == vatek_result_success)
//		printf("h1 reg 0x407 = %x\r\n",h1_val);

//	result = H1_REG_RD(hh1->vi2c,0x410,&h1_val);
//	if(result == vatek_result_success)
//		printf("h1 reg 0x410 = %x, ",h1_val);
//	result = H1_REG_RD(hh1->vi2c,0x411,&h1_val);
//	if(result == vatek_result_success)
//		printf("h1 reg 0x411 = %x\r\n",h1_val);
//	result = H1_REG_RD(hh1->vi2c,0x412,&h1_val);
//	if(result == vatek_result_success)
//		printf("h1 reg 0x412 = %x\r\n",h1_val);
//	result = H1_REG_RD(hh1->vi2c,0x413,&h1_val);
//	if(result == vatek_result_success)
//		printf("h1 reg 0x413 = %x\r\n",h1_val);
//	result = H1_REG_RD(hh1->vi2c,0x414,&h1_val);
//	if(result == vatek_result_success)
//		printf("h1 reg 0x414 = %x\r\n",h1_val);
	
	result = H1_REG_RD(hh1->vi2c,H1_HDMI_FLAG,&h1_val);
	if(result == vatek_result_success)
		printf("h1 reg 0x104 = 0x%x\r\n",h1_val);
	result = H1_REG_RD(hh1->vi2c,H1_VOUT_CFG,&h1_val);
	if(result == vatek_result_success)
		printf("h1 reg 0x202 = 0x%x\r\n",h1_val);
	
	result = H1_REG_RD(hh1->vi2c,H1_INT_EN,&h1_val);
	if(result == vatek_result_success)
		printf("h1 reg 0x101 = 0x%x\r\n",h1_val);
	result = H1_REG_RD(hh1->vi2c,H1_INT_CNTL,&h1_val);
	if(result == vatek_result_success)
		printf("h1 reg 0x103 = 0x%x\r\n",h1_val);
	result = H1_REG_RD(hh1->vi2c,H1_OUT_FMT,&h1_val);
	if(result == vatek_result_success)
		printf("h1 reg 0x201 = 0x%x\r\n",h1_val);
	result = H1_REG_RD(hh1->vi2c,H1_AOUT_CFG,&h1_val);
	if(result == vatek_result_success)
		printf("h1 reg 0x301 = 0x%x\r\n",h1_val);
	result = H1_REG_RD(hh1->vi2c,0x300,&h1_val);
	if(result == vatek_result_success)
		printf("h1 reg 0x300 = 0x%x\r\n",h1_val);
	result = H1_REG_RD(hh1->vi2c,0xf00,&h1_val);
	if(result == vatek_result_success)
		printf("h1 reg 0xf00 = 0x%x\r\n",h1_val);
	result = H1_REG_RD(hh1->vi2c,0xf01,&h1_val);
	if(result == vatek_result_success)
		printf("h1 reg 0xf01 = 0x%x\r\n",h1_val);
	result = H1_REG_RD(hh1->vi2c,0xf02,&h1_val);
	if(result == vatek_result_success)
		printf("h1 reg 0xf02 = 0x%x\r\n",h1_val);
	
//	printf("----------output source format-----------\r\n");
//	result = H1_REG_RD(hh1->vi2c,0x205,&h1_val);
//	if(result == vatek_result_success)
//		printf("h1 reg 0x205 = %x, ",h1_val);
//	result = H1_REG_RD(hh1->vi2c,0x206,&h1_val);
//	if(result == vatek_result_success)
//		printf("h1 reg 0x206 = %x\r\n",h1_val);
//	result = H1_REG_RD(hh1->vi2c,0x207,&h1_val);
//	if(result == vatek_result_success)
//		printf("h1 reg 0x207 = %x, ",h1_val);
//	result = H1_REG_RD(hh1->vi2c,0x208,&h1_val);
//	if(result == vatek_result_success)
//		printf("h1 reg 0x208 = %x\r\n",h1_val);
//	result = H1_REG_RD(hh1->vi2c,0x209,&h1_val);
//	if(result == vatek_result_success)
//		printf("h1 reg 0x209 = %x, ",h1_val);
//	result = H1_REG_RD(hh1->vi2c,0x20a,&h1_val);
//	if(result == vatek_result_success)
//		printf("h1 reg 0x20a = %x\r\n",h1_val);
//	result = H1_REG_RD(hh1->vi2c,0x20b,&h1_val);
//	if(result == vatek_result_success)
//		printf("h1 reg 0x20b = %x, ",h1_val);
//	result = H1_REG_RD(hh1->vi2c,0x20c,&h1_val);
//	if(result == vatek_result_success)
//		printf("h1 reg 0x20c = %x\r\n",h1_val);
//	result = H1_REG_RD(hh1->vi2c,0x20d,&h1_val);
//	if(result == vatek_result_success)
//		printf("h1 reg 0x20d = %x\r\n",h1_val);
//	result = H1_REG_RD(hh1->vi2c,0x20e,&h1_val);
//	if(result == vatek_result_success)
//		printf("h1 reg 0x20e = %x\r\n",h1_val);
//	result = H1_REG_RD(hh1->vi2c,0x204,&h1_val);
//	if(result == vatek_result_success)
//		printf("h1 reg 0x204 = %x\r\n",h1_val);
	
	return result;	
} 

hdmi_video_timing set_timing;
vatek_result get_input_timming( Ph1_handle hh1, Phdmi_video_timing timing)
{
    vatek_result result = vatek_result_unknown;
    uint8_t  hval=0, lval=0;
    uint16_t start_addr = 0x400;
    
    if(hh1==NULL) return vatek_result_invalidparm;
    
    //hdmi_video_timing v_timing;
    
    result = H1_REG_RD( hh1->vi2c, start_addr++, &hval);
    if(IS_SUCCESS(result)) result = H1_REG_RD( hh1->vi2c, start_addr++, &lval);
    if(IS_SUCCESS(result)) {
		timing->pixel_per_line = (hval<<8)|(lval);
		printf("h1 reg 0x400 = 0x%x, reg0x401 = 0x%x\r\n",hval,lval);
    }
    
    result = H1_REG_RD( hh1->vi2c, start_addr++, &hval);
    if(IS_SUCCESS(result)) result = H1_REG_RD( hh1->vi2c, start_addr++, &lval);
    if(IS_SUCCESS(result)) {
		timing->hfp = (hval<<8)|(lval);
		printf("h1 reg 0x402 = 0x%x, reg0x403 = 0x%x\r\n",hval,lval);
    }
    
    result = H1_REG_RD( hh1->vi2c, start_addr++, &hval);
    if(IS_SUCCESS(result)) result = H1_REG_RD( hh1->vi2c, start_addr++, &lval);
    if(IS_SUCCESS(result)) {
		timing->hbp = (hval<<8)|(lval);
		printf("h1 reg 0x404 = 0x%x, reg0x405 = 0x%x\r\n",hval,lval);
    }
    
    result = H1_REG_RD( hh1->vi2c, start_addr++, &hval);
    if(IS_SUCCESS(result)) result = H1_REG_RD( hh1->vi2c, start_addr++, &lval);
    if(IS_SUCCESS(result)) {
		timing->hpw = (hval<<8)|(lval);
		printf("h1 reg 0x406 = 0x%x, reg0x407 = 0x%x\r\n",hval,lval);
    }
    
    
    start_addr = 0x410;
    result = H1_REG_RD( hh1->vi2c, start_addr++, &hval);
    if(IS_SUCCESS(result)) result = H1_REG_RD( hh1->vi2c, start_addr++, &lval);
    if(IS_SUCCESS(result)) {
		timing->pixel_per_frame = (hval<<8)|(lval);
		printf("h1 reg 0x410 = 0x%x, reg0x411 = 0x%x\r\n",hval,lval);
    }
    
    result = H1_REG_RD( hh1->vi2c, start_addr++, &lval);
    if(IS_SUCCESS(result)) {
		timing->vfp = lval;
		printf("h1 reg 0x412 = 0x%x\r\n",lval);
    }
    
    result = H1_REG_RD( hh1->vi2c, start_addr++, &lval);
    if(IS_SUCCESS(result)) {
		timing->vbp = lval;
		printf("h1 reg 0x413 = 0x%x\r\n",lval);
    }
    
    result = H1_REG_RD( hh1->vi2c, start_addr, &lval);
    if(IS_SUCCESS(result)) {
		timing->vpw = lval;
		printf("h1 reg 0x414 = 0x%x\r\n",lval);
    }
	
#if 1 //setting value readback
	
	printf("\r\n");
	start_addr = 0x205;    
    
    result = H1_REG_RD( hh1->vi2c, start_addr++, &hval);
    if(IS_SUCCESS(result)) result = H1_REG_RD( hh1->vi2c, start_addr++, &lval);
    if(IS_SUCCESS(result)) {
		set_timing.pixel_per_line = (hval<<8)|(lval);
		printf("h1 reg 0x205 = 0x%x, reg 0x206 = 0x%x\r\n",hval,lval);
    }
    
    result = H1_REG_RD( hh1->vi2c, start_addr++, &hval);
    if(IS_SUCCESS(result)) result = H1_REG_RD( hh1->vi2c, start_addr++, &lval);
    if(IS_SUCCESS(result)) {
		set_timing.pixel_per_frame = (hval<<8)|(lval);
		printf("h1 reg 0x207 = 0x%x, reg 0x208 = 0x%x\r\n",hval,lval);
    }
    
    result = H1_REG_RD( hh1->vi2c, start_addr++, &hval);
    if(IS_SUCCESS(result)) result = H1_REG_RD( hh1->vi2c, start_addr++, &lval);
    if(IS_SUCCESS(result)) {
		set_timing.hfp = (hval<<8)|(lval);
		printf("h1 reg 0x209 = 0x%x, reg 0x20a = 0x%x\r\n",hval,lval);
    }
    
    result = H1_REG_RD( hh1->vi2c, start_addr++, &hval);
    if(IS_SUCCESS(result)) result = H1_REG_RD( hh1->vi2c, start_addr++, &lval);
    if(IS_SUCCESS(result)) {
		set_timing.hpw = (hval<<8)|(lval);
		printf("h1 reg 0x20b = 0x%x, reg 0x20c = 0x%x\r\n",hval,lval);
    }    
    
    result = H1_REG_RD( hh1->vi2c, start_addr++, &hval);
    if(IS_SUCCESS(result)) result = H1_REG_RD( hh1->vi2c, start_addr++, &lval);
    if(IS_SUCCESS(result)) {
		set_timing.vfp = (hval<<8)|(lval);
		printf("h1 reg 0x20d = 0x%x, reg 0x411 = 0x%x\r\n",hval,lval);
    }
    
    result = H1_REG_RD( hh1->vi2c, start_addr++, &lval);
    if(IS_SUCCESS(result)) {
		set_timing.vpw = lval;
		printf("h1 reg 0x20e = 0x%x\r\n",lval);
    } 
	printf("\r\n");
	printf("\r\n");
#endif

    return result;
}

vatek_result h1_dump_reg(Ph1_handle hh1)
{
	uint8_t h1_reg = 0;
	uint16_t h1_add = 0x3000;
	vatek_result result = vatek_result_unknown;
	for(uint32_t m = 1;m <= 16;m++){
		for(uint32_t n = 1; n <= 16;n++){
			result = H1_REG_RD(hh1->vi2c, h1_add++, &h1_reg);
			printf("%02x ",h1_reg);
		}
		printf("\r\n");
	}
	return result;
}

vatek_result h1_vout_check(Ph1_handle hh1)
{
	vatek_result result = vatek_result_unknown;
	static uint8_t h_active1_cmp = 0, h_active2_cmp = 0, v_active1_cmp = 0, v_active2_cmp = 0;
	uint8_t h_active1 = 0, h_active2 = 0, v_active1 =0, v_active2 = 0;
	uint8_t check_vout_disable = 0;
	printf("test test test\r\n");
	if(!h_active1_cmp){
		H1_REG_RD(hh1->vi2c, 0x3018, &h_active1_cmp);
		H1_REG_RD(hh1->vi2c, 0x3019, &h_active2_cmp);
		H1_REG_RD(hh1->vi2c, 0x301a, &v_active1_cmp);
		H1_REG_RD(hh1->vi2c, 0x301b, &v_active2_cmp);
	}

	H1_REG_RD(hh1->vi2c, 0x3080, &check_vout_disable);
	if(!(check_vout_disable & 0x80)){
		H1_REG_RD(hh1->vi2c, 0x3018, &h_active1);
		H1_REG_RD(hh1->vi2c, 0x3019, &h_active2);
		H1_REG_RD(hh1->vi2c, 0x301a, &v_active1);
		H1_REG_RD(hh1->vi2c, 0x301b, &v_active2);
		if((h_active1 == h_active1_cmp) && (h_active2 == h_active2_cmp) && (v_active1 == v_active1_cmp) && (v_active2 == v_active2_cmp)){
			H1_REG_WR(hh1->vi2c, 0x3022, 0x1c);
			H1_REG_WR(hh1->vi2c, 0x3080, check_vout_disable | 0x80);
		}
	}
	
	return result;
}