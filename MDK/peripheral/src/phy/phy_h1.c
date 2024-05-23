
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

		vatek_system_delay(1);
		
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
    }else if(hh1->output_mode == h1_output_scale_720P)
		{
			if(in_info->resolution == vi_resolution_1080p59_94 || in_info->resolution == vi_resolution_1080p60){
				in_info->resolution = vi_resolution_720p60;
			}
		}else if(hh1->output_mode == h1_output_scale_1440p)
		{
			in_info->resolution = vi_resolution_1440p60;
			in_info->aspectrate = vi_aspectrate_16_9;
		}
		else if(hh1->output_mode == h1_output_scale_1440i)
		{
			in_info->resolution = vi_resolution_1440i60;
			in_info->aspectrate = vi_aspectrate_16_9;
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
						
						case vi_resolution_1080p30:
								in_info->resolution = vi_resolution_1080p29_97;
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
						
						case vi_resolution_1440p60:
                in_info->resolution = vi_resolution_1440p59_94;
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
    
		if((result = H1_REG_RD( hh1->vi2c, H1_VOUT_CFG, &vinfo))!=vatek_result_success)
        return result;
		
		if((vinfo & H1_VOUT_CFG_16BITS) && !(vinfo & H1_VOUT_CFG_EMBEDDED)) //if use Separate 8bits, need to enable H1_OUT_FMT_422_8BIT
			vout_fmt |= H1_OUT_FMT_422_8BIT;
		
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

static uint8_t vic_change = 0;
static uint8_t vic_save = 0;

static vatek_result output_bypass(Ph1_handle hh1)
{
    vatek_result result = vatek_result_unknown;
    uint8_t v_interlaced = 0;
    uint8_t flag = H1_HDMI_FLAG_BYPASS_MODE;

    if(hh1==NULL)
        return vatek_result_invalidparm;
    
		if(vic_change != vic_save){ //check wether VIC change, if change, reset and check interlace or not
			printf("VIC change 0x%x to 0x%x----\r\n",vic_save,vic_change);
			 /** check video input interlaced */
			if((result = H1_REG_RD( hh1->vi2c, H1_VIN_VPLUSE, &v_interlaced))!=vatek_result_success)
					return result;
			
			if(v_interlaced&H1_VIN_VPLUSE_INT) flag |= H1_HDMI_FLAG_OUT_INTERLACED; //setting output format from input(i or p)
			else flag &= 0x7f;

			vic_save = vic_change;
		}
    
    if((result = H1_REG_WR( hh1->vi2c, H1_HDMI_FLAG, flag))!=vatek_result_success)
        return result;
		

		
    return result;
}

static vatek_result output_scale(Ph1_handle hh1, Pphy_video_info info)
{
		vatek_result result = vatek_result_unknown;
    hdmi_video_timing vtable = {0};
    uint8_t vid = 0;
		uint8_t h1_val = 0;
    
    if(hh1==NULL)
        return vatek_result_invalidparm;
    
		if((result = H1_REG_RD( hh1->vi2c, H1_HDMI_FLAG, &h1_val)) != vatek_result_success)
				return result;
		h1_val &= H1_HDMI_FLAG_PCLK_1001;
    if((result = H1_REG_WR( hh1->vi2c, H1_HDMI_FLAG, h1_val))!=vatek_result_success)
        return result;
		
    /*1080P to 720P (down scale)*/
		if(info->resolution == vi_resolution_1080p59_94 || info->resolution == vi_resolution_1080p60){
			info->resolution = vi_resolution_720p60;
		}
		/*480P to 480I (p2i + down scale)*/
		if(info->resolution == vi_resolution_480p60 || info->resolution == vi_resolution_480p59_94){
			info->resolution = vi_resolution_480i60;
			if((result = H1_REG_WR( hh1->vi2c, H1_HDMI_FLAG, H1_HDMI_FLAG_OUT_INTERLACED))!=vatek_result_success)
        return result;
		}
		
    /* set video timing. */
    result = tool_hdmi_get_vid( info, &vid);
    if(IS_SUCCESS(result)) result = tool_hdmi_get_videoinfo( vid, &vtable);
    if(IS_SUCCESS(result)) result = set_output_timing( hh1, &vtable);
    if(!IS_SUCCESS(result)) return result;
    
    return result;
}

static vatek_result output_scale_1440p(Ph1_handle hh1, Pphy_video_info info)
{
		vatek_result result = vatek_result_unknown;
    hdmi_video_timing vtable = {0};
    uint8_t vid = 0;
		uint8_t h1_val = 0;
    
    if(hh1==NULL)
        return vatek_result_invalidparm;
    
		if((result = H1_REG_RD( hh1->vi2c, H1_HDMI_FLAG, &h1_val)) != vatek_result_success)
				return result;
		h1_val &= H1_HDMI_FLAG_PCLK_1001;
    if((result = H1_REG_WR( hh1->vi2c, H1_HDMI_FLAG, h1_val))!=vatek_result_success)
        return result;
		
		/*any resolution to 1440P*/
//		if(h1_val & H1_HDMI_FLAG_PCLK_1001)
//			info->resolution = vi_resolution_1440p59_94;
//		else
//			info->resolution = vi_resolution_1440p60;

    /* set video timing. */
//    result = tool_hdmi_get_vid( info, &vid);
//    if(IS_SUCCESS(result)) result = tool_hdmi_get_videoinfo( vid, &vtable);
//    if(IS_SUCCESS(result)) result = set_output_timing( hh1, &vtable);
    if(!IS_SUCCESS(result)) return result;
    
    return result;
}

static vatek_result output_scale_1440i(Ph1_handle hh1, Pphy_video_info info)
{
		vatek_result result = vatek_result_unknown;
    hdmi_video_timing vtable = {0};
    uint8_t vid = 0;
		uint8_t h1_val = 0;
    
    if(hh1==NULL)
        return vatek_result_invalidparm;
    
		if((result = H1_REG_RD( hh1->vi2c, H1_HDMI_FLAG, &h1_val)) != vatek_result_success)
				return result;
		h1_val &= H1_HDMI_FLAG_PCLK_1001;
		h1_val |= H1_HDMI_FLAG_OUT_INTERLACED;
    if((result = H1_REG_WR( hh1->vi2c, H1_HDMI_FLAG, h1_val))!=vatek_result_success)
        return result;
		
		/*any resolution to 1440I*/
//		if(h1_val & H1_HDMI_FLAG_PCLK_1001)
//			info->resolution = vi_resolution_1440i59_94;
//		else
//			info->resolution = vi_resolution_1440i60;

    /* set video timing. */
//    result = tool_hdmi_get_vid( info, &vid);
//    if(IS_SUCCESS(result)) result = tool_hdmi_get_videoinfo( vid, &vtable);
//    if(IS_SUCCESS(result)) result = set_output_timing( hh1, &vtable);
    if(!IS_SUCCESS(result)) return result;
    
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
		else if(hh1->output_mode==h1_output_scale_720P)
				result = output_scale(hh1, info);
		else if(hh1->output_mode==h1_output_scale_1440p)
				result = output_scale_1440p(hh1, info);
		else if(hh1->output_mode==h1_output_scale_1440i)
				result = output_scale_1440i(hh1, info);
    if(IS_SUCCESS(result)) result = set_output_format(hh1, info);
    if(IS_SUCCESS(result)) result = enable_vout( hh1, g_vout);//g_vout
    if(!IS_SUCCESS(result)) return result;

    return result;
}

static vatek_result enable_vout(Ph1_handle hh1, uint8_t enable)
{
    vatek_result result = vatek_result_unknown;
    uint8_t hal_val = 0;
    if (IS_NEW_REV(h1_rev))
    {
        if(enable){
            result = H1_REG_WR( hh1->vi2c, H1_OUT_CNTL, H1_OUT_CNTL_EN_AOUT);//H1_OUT_CNTL_EN_AOUT
				}
        else result = H1_REG_WR( hh1->vi2c, H1_OUT_CNTL, H1_OUT_CNTL_DIS_VOUT);
    }else
    {
        if(enable == 1){
            result = H1_REG_WR( hh1->vi2c, H1_OUT_CNTL, 0x00);
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
    uint8_t reg_val = 0, pclk = 0;
    hdmi_video_timing v_info = {0};
    
    if( hh1==NULL || info==NULL)
        return vatek_result_invalidparm;

		if((result = check_source( hh1, &status))==vatek_result_success)
		{
				if( status!=h1_active && status!=h1_active_protect)
						return vatek_result_badstatus;
		}else return result;
		

    result = H1_REG_RD( hh1->vi2c, H1_VIN_VIC, &vic_change);
    if(IS_SUCCESS(result)) result = H1_REG_RD( hh1->vi2c, H1_HDMI_FLAG, &pclk);
    if(!IS_SUCCESS(result)) return result;
		

    if(pclk&H1_HDMI_FLAG_PCLK_1001) 
        hh1->baseclk = h1_baseclk_1001; //modify
		else
				hh1->baseclk = h1_baseclk_1000;

    if((result = tool_hdmi_get_videoinfo( vic_change, &v_info))!=vatek_result_success)
    {
        H1_LOG("unsupporteed video codec %d", vic_change);
        return vatek_result_unsupport;
    }
		result = H1_REG_RD(hh1->vi2c, H1_VOUT_CFG, &reg_val);
		if(IS_SUCCESS(result)){
			if(reg_val & 0x80){
				info->buswidth_16 = 0;
			}
			else{
				info->buswidth_16 = 1;
			}
			if(reg_val & 0x20){
				info->separated_sync = 0;
			}
			else{
				info->separated_sync = 1;
			}
		}
//		if(vic_change == 0x05){
//			info->separated_sync = 0;
//			info->buswidth_16 = 0;
//			result = H1_REG_WR(hh1->vi2c, H1_VOUT_CFG, reg_val | 0xa);
//		}
    
    info->resolution     = v_info.resolution;
    info->aspectrate     = v_info.aspectrate;
    
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
    #define AUDIO_SAMPLE_FREQ_32KHZ		0x00 //000
	#define AUDIO_SAMPLE_FREQ_44_1KHZ	0x01	//001
	#define AUDIO_SAMPLE_FREQ_48KHZ		0x02 //010
    
    vatek_result result = vatek_result_unknown;
    uint8_t val = 0;
	uint8_t audio_reg_val = 0;
    
    if( hh1==NULL || info==NULL)
        return vatek_result_invalidparm;
    
    if((result = H1_REG_RD( hh1->vi2c, H1_AIN_STATUS, &val)) == vatek_result_success)
    {
        val &= H1_AIN_STATUS_SAMP_FREQ;
        if( val == AUDIO_SAMPLE_FREQ_48KHZ)
            info->samplerate = ai_samplerate_48K;
        else if( val == AUDIO_SAMPLE_FREQ_44_1KHZ)
            info->samplerate = ai_samplerate_44_1K;
        else if( val == AUDIO_SAMPLE_FREQ_32KHZ)
            info->samplerate = ai_samplerate_32K;
        else info->samplerate = ai_samplerate_unknown;
    }
	
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
		printf("enable second 0x%x version\r\n",h1_rev);
        uint8_t i = 0;
        for( i=0; i< sizeof(h1_init_cmd_1)/sizeof(h1_reg); i++)
        {
            if((result = H1_REG_WR(vi2c, h1_init_cmd_1[i].addr, h1_init_cmd_1[i].val)) != vatek_result_success)
                return result;
        }
    }else
    {
		printf("enable first 0x%x version\r\n",h1_rev);
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
    newphy->output_mode = h1_output_bypass;//h1_output_bypass, h1_output_scale_720P, h1_output_progressive, h1_output_scale_1440p, h1_output_scale_1440i
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

vatek_result h1_dump_reg(Ph1_handle hh1)
{
	vatek_result result = vatek_result_unknown;
	uint32_t i = 0x3000, n = 0x3000;
	uint32_t x=0,y=0;
	uint8_t reg_val = 0;
	
	printf("---------------------H1 256B----------------------\r\n");
	printf("0x0000 00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f\r\n");
	for(x=0;x<16;x++){
		printf("0x%x ", n);
		n += 0x10;
		for(y=0;y<16;y++){
			H1_REG_RD(hh1->vi2c, i, &reg_val);
			printf("%02x ",reg_val);
			i++;
		}
		printf("\r\n");
	}
	printf("---------------------H1 End-----------------------\r\n");
	uint32_t h1_reg = 0xa0;
	for(h1_reg=0xa0;h1_reg<0xa8;h1_reg++){
		if((result = H1_REG_RD( hh1->vi2c, h1_reg, &reg_val))!=vatek_result_success){
			return result;
		}
		printf("0x%x ",reg_val);
	}
	printf("\r\n");
	if((result = H1_REG_RD( hh1->vi2c, H1_HDMI_FLAG, &reg_val))!=vatek_result_success)
    return result;
  printf("H1_HDMI_FLAG(0x104) = 0x%x\r\n",reg_val);

	if((result = H1_REG_RD( hh1->vi2c, H1_MUTE_CNTL, &reg_val))!=vatek_result_success)
    return result;
  printf("H1_MUTE_CNTL(0x107) = 0x%x\r\n",reg_val);

	if((result = H1_REG_RD( hh1->vi2c, H1_OUT_FMT, &reg_val))!=vatek_result_success)
    return result;
  printf("H1_OUT_FMT(0x201) = 0x%x\r\n",reg_val);
	
	if((result = H1_REG_RD( hh1->vi2c, H1_VOUT_CFG, &reg_val))!=vatek_result_success)
    return result;
  printf("H1_VOUT_CFG(0x202) = 0x%x\r\n",reg_val);

	if((result = H1_REG_RD( hh1->vi2c, H1_VIN_FMT, &reg_val))!=vatek_result_success)
    return result;
  printf("H1_VIN_FMT(0x203) = 0x%x\r\n",reg_val);

	if((result = H1_REG_RD( hh1->vi2c, H1_OUT_CNTL, &reg_val))!=vatek_result_success)
    return result;
  printf("H1_OUT_CNTL(0x300) = 0x%x\r\n",reg_val);
	
	if((result = H1_REG_RD( hh1->vi2c, H1_AOUT_CFG, &reg_val))!=vatek_result_success)
    return result;
  printf("H1_AOUT_CFG(0x301) = 0x%x\r\n",reg_val);
	
	if((result = H1_REG_RD( hh1->vi2c, H1_AIN_STATUS, &reg_val))!=vatek_result_success)
    return result;
  printf("H1_AIN_STATUS(0x302) = 0x%x\r\n",reg_val);

	for(uint32_t hal_addr = 0x205;hal_addr<=0x20e;hal_addr++){
		if((result = H1_REG_RD( hh1->vi2c, hal_addr, &reg_val))!=vatek_result_success)
			return result;
		printf("0x%x ",reg_val);
	}
	printf("\r\n");
}

vatek_result h1_change_clk(Ph1_handle hh1) //for 1440ver, user need to write resolution format by self
{
	vatek_result result = vatek_result_success;
	uint8_t reg_val = 0;
	if(hh1->output_mode == h1_output_scale_1440i || hh1->output_mode == h1_output_scale_1440p){
		H1_REG_WR(hh1->vi2c, H1_OUT_CNTL, H1_OUT_CNTL_DIS_ALL);
		H1_REG_WR(hh1->vi2c, 0x30b0, 0x34);//hbp 0-7
		H1_REG_WR(hh1->vi2c, 0x30b1, 0x1);//hbp 8-9
		H1_REG_WR(hh1->vi2c, 0x30b2, 0xa0);//hactive 0-7
		H1_REG_WR(hh1->vi2c, 0x30b3, 0x5);//hactive 8-9
		H1_REG_WR(hh1->vi2c, 0x30ac, 0xf8);//hfp 0-7
		H1_REG_WR(hh1->vi2c, 0x30ad, 0x0);//hfp 8-9
		H1_REG_WR(hh1->vi2c, 0x30ae, 0xcc);//hpw 0-7
		H1_REG_WR(hh1->vi2c, 0x30af, 0x0);//hpw 8-9
	}
	
	if(hh1->output_mode == h1_output_scale_1440i){
		H1_REG_WR(hh1->vi2c, 0x30b4, 0x1c);//vactive 0-7
		H1_REG_WR(hh1->vi2c, 0x30b5, 0x2);//vactive 8-9
		H1_REG_WR(hh1->vi2c, 0x30b6, 0x33);//vtotal 0-7
		H1_REG_WR(hh1->vi2c, 0x30b7, 0x2);//vtotal 8-9
		H1_REG_WR(hh1->vi2c, 0x30aa, 0x5);//v pulse
		H1_REG_WR(hh1->vi2c, 0x30ab, 0xf);//v de
	}
	if(hh1->output_mode == h1_output_scale_1440p){
		H1_REG_WR(hh1->vi2c, 0x30b4, 0x38);//vactive 0-7
		H1_REG_WR(hh1->vi2c, 0x30b5, 0x4);//vactive 8-9
		H1_REG_WR(hh1->vi2c, 0x30b6, 0x65);//vtotal 0-7
		H1_REG_WR(hh1->vi2c, 0x30b7, 0x4);//vtotal 8-9
		H1_REG_WR(hh1->vi2c, 0x30aa, 0x5);//vpw
		H1_REG_WR(hh1->vi2c, 0x30ab, 0x24);//vbp
	}
	H1_REG_WR(hh1->vi2c, 0x309d, 0xa);//YUV422 modify
		
	
	
	return result;
}
#endif
