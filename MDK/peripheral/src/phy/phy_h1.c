
#if defined(PHY_H1)

#include "vatek_system.h"
#include "phy/phy_h1_internal.h"
#include "phy/tool_hdmi.h"

vatek_result h1_enable( Ph1_handle hh1, uint8_t enable);

/** previous vinfo, vout */
static uint8_t g_vout = 0;
static phy_video_info g_vinfo;

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
                
                if(hdcp&H1_SYS_STATUS1_AUTH)
                {
//                        H1_LOG("HDCP AUTH");
                    if(hdcp&H1_SYS_STATUS1_DECRYPT)
                    {
//                            H1_LOG("HDCP DECRYPT");
                        *status = h1_active_protect;
                    }
                }
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

static vatek_result set_output_timing( Ph1_handle hh1, Phdmi_video_timing ptiming)
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
    if((result = h1_enable( hh1, 0))!=vatek_result_success)
        return result;
    
    /** set video parameter */
    if( hh1->output_mode == h1_output_bypass)
        result = output_bypass(hh1);
    else if(hh1->output_mode==h1_output_progressive)
        result = output_progressive(hh1, info);
    else if(hh1->output_mode==h1_output_interlace)
        result = output_interlace(hh1, info);
    if(IS_SUCCESS(result)) result = set_output_format(hh1, info);
    if(IS_SUCCESS(result) && g_vout) result = h1_enable( hh1, 1);
    if(!IS_SUCCESS(result)) return result;

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
    
    if(enable)
        result = H1_REG_WR( hh1->vi2c, H1_OUT_CNTL, 0x00);
    else result = H1_REG_WR( hh1->vi2c, H1_OUT_CNTL, H1_OUT_CNTL_DIS_ALL);
    
    if(IS_SUCCESS(result)) g_vout = enable;
    
    return result;    
}

vatek_result h1_set_baseclock( Ph1_handle hh1, phy_output_baseclock clk)
{
    if( hh1==NULL)
        return vatek_result_invalidparm;
    
    hh1->baseclk = (h1_baseclk)clk;
    
    return vatek_result_success;
}

vatek_result h1_set_outputmode( Ph1_handle hh1, phy_output_mode mode)
{    
    if( hh1==NULL)
        return vatek_result_invalidparm;
    
    if( mode < h1_output_min || mode > h1_output_max)
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

    if(pclk&H1_HDMI_FLAG_PCLK_1001) hh1->baseclk = h1_baseclk_1001;
    
    

    if((result = tool_hdmi_get_videoinfo( vid, &v_info))!=vatek_result_success)
    {
        H1_LOG("unsupporteed video codec %d", vid);
        return vatek_result_unsupport;
    }
    
    info->resolution     = v_info.resolution;
    info->aspectrate     = v_info.aspectrate;
    info->buswidth_16    = 1;
    info->separated_sync = 1;
    
    
    if((result = output_format( hh1, info))!=vatek_result_success)
        return result;
    
    if( g_vinfo.resolution==info->resolution && g_vinfo.aspectrate==info->aspectrate)
        return vatek_result_success;

    if((result = set_videoinfo( hh1, info))!=vatek_result_success)
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
    
    if( hh1==NULL || info==NULL)
        return vatek_result_invalidparm;
    
    if((result = H1_REG_RD( hh1->vi2c, H1_AIN_STATUS, &val)) != vatek_result_success)
        return result;
    
    val &= H1_AIN_STATUS_SAMP_FREQ;
    if( val == AUDIO_SAMPLE_FREQ_48KHZ)
        info->samplerate = ai_samplerate_48K;
    else if( val == AUDIO_SAMPLE_FREQ_44_1KHZ)
        info->samplerate = ai_samplerate_44_1K;
    else if( val == AUDIO_SAMPLE_FREQ_32KHZ)
        info->samplerate = ai_samplerate_32K;
    else info->samplerate = ai_samplerate_unknown;
    
    return result;   
}

vatek_result h1_get_status(Ph1_handle hh1, phy_status * status)
{
    vatek_result result = vatek_result_unknown;
    h1_status h1_status = h1_invailed;
    
    if( hh1==NULL || status==NULL)
        return vatek_result_invalidparm;
    
    result = check_source( hh1, &h1_status);
    if( result != vatek_result_success) return result;
    
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
    result = vatek_system_reset( sys_dev_type_phy_h1, vi2c->hboard);
    if( !IS_SUCCESS(result)) return result;
    
    vatek_system_delay(1000);
    while(!power_ready)
    {
        if((result = H1_REG_RD(vi2c, H1_POWER, &power_ready)) != vatek_result_success)
            return result;
    }
    
    // To-Do. read chip id.

    const h1_reg h1_init_cmd[] = 
    {
        { H1_OUT_CNTL   , 0xD0},        /** SPDIF, I2S, VOUT disable */
        { H1_INT_EN     , 0x00},        /** INT disable */
        { H1_INT_CNTL   , 0x02},        /** VSYNC low pulse */
        { H1_HDMI_FLAG  , 0x00},        /** Vout Progressive, Scalar, Baseclk_1001 */
        { H1_OUT_FMT    , 0xA4},        /** YUV422, BT709*/
        { H1_AOUT_CFG   , 0x03},        /** Audio 512 sampling */
        { H1_VOUT_CFG   , 0x40},         /** Vout Y/Cb/Y/Cr */
    };

    uint8_t i = 0;
    for( i=0; i< sizeof(h1_init_cmd)/sizeof(h1_reg); i++)
    {
        if((result = H1_REG_WR(vi2c, h1_init_cmd[i].addr, h1_init_cmd[i].val)) != vatek_result_success)
            return result;
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

#if defined(SELF_TEST)
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

