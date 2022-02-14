
#if defined(PHY_EP9555E)

#include "vatek_system.h"
#include "phy/phy_ep9555e_internal.h"
#include "phy/tool_hdmi.h"

static uint8_t before_colorFormat = EP9555E_COLOR_INIT;

static audio_input_samplerate audioinfo_samplerate(uint8_t val)
{
    audio_input_samplerate samplerate = ai_samplerate_unknown;
    val &= 0x7;

    switch (val)
    {
        case EP9555E_AUDIO_SAMPLERATE_32KHZ:
            samplerate = ai_samplerate_32K;
            break;

        case EP9555E_AUDIO_SAMPLERATE_44_1KHZ:
            samplerate = ai_samplerate_44_1K;
            break;

        case EP9555E_AUDIO_SAMPLERATE_48KHZ:
            samplerate = ai_samplerate_48K;
            break;

        default:
            break;
    }

    return samplerate;
}

/*
static vatek_result is_colorformatchange(Pvatek_i2c vi2c)
{
    vatek_result result = vatek_result_unknown;
    uint8_t val = 0;
    if (vi2c == NULL)
        return vatek_result_invalidparm;
    
    if ((result = EP9555E_CHIP_RD(vi2c, EP9555E_V_STATUS0, &val, 1)) != vatek_result_success)
        return result;

    if(val != before_colorFormat && before_colorFormat != EP9555E_COLOR_INIT)
    {
        result = vatek_result_success;
		before_colorFormat = val;   
    }
    else
        result = vatek_result_negative;
    
    return result;
}
*/

static vatek_result is_interrupt(Pvatek_i2c vi2c)
{
    vatek_result result = vatek_result_unknown;
    uint8_t val = 0;
    if (vi2c == NULL)
        return vatek_result_invalidparm;

    if ((result = EP9555E_CHIP_RD(vi2c, EP9555E_INT1, &val, 1)) != vatek_result_success)
        return result;  


    if((val & (EP9555E_INT1_TIM_CH | EP9555E_INT1_ADO_CH | EP9555E_INT1_AVI_CH)) == 0)
        result = vatek_result_negative;

    return result;
}


static vatek_result is_hdcpauthenticate(Pvatek_i2c vi2c)
{
    vatek_result result = vatek_result_unknown;
    uint8_t val = 0;
    if (vi2c == NULL)
        return vatek_result_invalidparm;

    if ((result = EP9555E_CHIP_RD(vi2c, EP9555E_SYS_STATUS0, &val, 1)) != vatek_result_success)
        return result;  

    result = vatek_result_negative;
    if (val & EP9555E_SYS_STATUS0_AVMUTE_F)
    {
        EP9555E_LOG("Receive AV mute signal");
    }

    if(val & EP9555E_SYS_STATUS0_DECRYPT)
    {
//        EP9555E_LOG("source is HDCP encrypted");
        if(val & EP9555E_SYS_STATUS0_AUTHEN)
        {
//            EP9555E_LOG("HDCP Authentication has been done");  
            result = vatek_result_success;
        }
    }
    
    return result;
}

static vatek_result check_sourcevalid(Pvatek_i2c vi2c, ep9555e_status* status)
{
    vatek_result result = vatek_result_unknown;
    uint8_t status0 = 0,status1 = 0;
    if (vi2c == NULL)
        return vatek_result_invalidparm;
    
    *status = epstatus_hw_fail;
    if ((result = EP9555E_CHIP_RD(vi2c, EP9555E_SYS_STATUS0, &status0, 1)) != vatek_result_success)
        return result;
    
    if ((result = EP9555E_CHIP_RD(vi2c, EP9555E_SYS_STATUS1, &status1, 1)) != vatek_result_success)
        return result;

    *status = epstatus_idle;

    if(status1 & EP9555E_SYS_STATUS1_LINK_ON)
    {
        *status = epstatus_invalid;
        if(status1 & EP9555E_SYS_STATUS1_DE_VALID)
        {
            if(status0 & EP9555E_SYS_STATUS0_HDMI)
            {
                /*
                if (is_colorformatchange(vi2c) == vatek_result_success)
                {
                    EP9555E_LOG("color_format changed");
                    *status = epstatus_idle;
                }
                else
                */
                {
                    *status = epstatus_active;
                    if (is_interrupt(vi2c) == vatek_result_success &&  
                        is_hdcpauthenticate(vi2c) == vatek_result_success)
                    {
                        *status = epstatus_active_protect;
                    }   
                }
            }
            else 
                EP9555E_LOG("linked on but not hdmi");
        }
        else 
            EP9555E_LOG("linked on but de invalid");
    }
    else 
        before_colorFormat = EP9555E_COLOR_INIT;

    return result;
}

static vatek_result autoset_videoinfo(Pep9555e_handle hep9555e, Phdmi_video_timing epinfo)
{
    vatek_result result = vatek_result_unknown;
    phy_video_info info;
    
    if (hep9555e == NULL || epinfo == NULL)
        return vatek_result_invalidparm;
    
    info.resolution = epinfo->resolution;
    info.aspectrate = epinfo->aspectrate;

    if (hep9555e->outputmode == ep9555e_interlaced_2_p)
    {
        switch (epinfo->resolution)
        {
            case vi_resolution_1080i60:
                info.resolution = vi_resolution_1080p60;
                break;

            case vi_resolution_1080i59_94:
                info.resolution = vi_resolution_1080p59_94;
                break;

            case vi_resolution_1080i50:
                info.resolution = vi_resolution_1080p50;
                break;

            case vi_resolution_576i50:
                info.resolution = vi_resolution_576p50;
                break;

            case vi_resolution_480i60:
                info.resolution = vi_resolution_480p60;
                break;

            case vi_resolution_480i59_94:
                info.resolution = vi_resolution_480p59_94;
                break;

            default:
                info.resolution = epinfo->resolution;
                break;
        }
    }

    if (hep9555e->baseclk == baseclock_1001)
    {
        switch (info.resolution)
        {
            case vi_resolution_1080i60:
                info.resolution = vi_resolution_1080i59_94;
                break;

            case vi_resolution_1080p60:
                info.resolution = vi_resolution_1080p59_94;
                break;

            case vi_resolution_720p60:
                info.resolution = vi_resolution_720p59_94;
                break;

            case vi_resolution_480i60:
                info.resolution = vi_resolution_480i59_94;
                break;

            case vi_resolution_480p60:
                info.resolution = vi_resolution_480p59_94;
                break;
            
            default:
                break;
        }
    }
    info.aspectrate = epinfo->aspectrate;
    if ((result = ep9555e_set_videoinfo(hep9555e, &info)) != vatek_result_success)
        return result;

    epinfo->resolution = info.resolution;
    epinfo->aspectrate = info.aspectrate;
    
    return result;
}

static vatek_result set_outputformat(Pep9555e_handle hep9555e)
{
    vatek_result result = vatek_result_unknown;
    uint8_t val = 0;
    uint8_t infopr = 0;
    uint8_t ncfg = (EP9555E_MANUAL_VFMT_OUT422 | EP9555E_MANUAL_VFMT_OUT_YCC); // default value

    if (hep9555e == NULL)
        return vatek_result_invalidparm;

    if ((result =  EP9555E_CHIP_RD(hep9555e->vi2c, EP9555E_V_STATUS0, &val, 1)) != vatek_result_success)
        return result;
    
    if ((result =  EP9555E_CHIP_RD(hep9555e->vi2c, EP9555E_AVI_INFO + 5, &infopr, 1)) != vatek_result_success)
        return result;

    if(val & EP9555E_V_STATUS0_VIN_FMT_YCC)
    {
        ncfg |= EP9555E_MANUAL_VFMT_IN_YCC;
        if(val & EP9555E_V_STATUS0_VIN_FMT_422)
            ncfg |= EP9555E_MANUAL_VFMT_IN422;
    }
        
    if(val & EP9555E_V_STATUS0_VIN_FMT_Full_Range)
        ncfg |= EP9555E_MANUAL_VFMT_RGB_range;

    if(val & EP9555E_V_STATUS0_VIN_FMT_ITU709)
        ncfg |= EP9555E_MANUAL_VFMT_CS;

    if(hep9555e->outputmode == ep9555e_bypassmode)
        ncfg |= (infopr & 0xF);
    else 
        before_colorFormat = val;
    
    if ((result =  EP9555E_CHIP_WR(hep9555e->vi2c, EP9555E_MANUAL_VFMT_CNTL, &ncfg, 1)) != vatek_result_success)
        return result;

    return result;
}


static vatek_result get_videotiming(uint8_t* vtimebuf, Phdmi_video_timing ptiming)
{
    if (vtimebuf == NULL || ptiming == NULL)
        return vatek_result_invalidparm;

    #define TIME_APPL(a)            (((a[1]&0x0F) << 8) | a[0])		//Active Pixels Per Line
    #define TIME_ALPF(a)            (((a[9]&0x0F) << 8) | a[8])		//Active Lines Per Frame
    #define TIME_HFP(a)             (((a[3]&0x03) << 8) | a[2])		//H sync offset
    #define TIME_HBP(a)             (((a[5]&0x03) << 8) | a[4])		//H Back Porch
    #define TIME_HPW(a)             (((a[7]&0x03) << 8) | a[6])		//H sync pulse width
    #define TIME_VFP(a)             (a[10])							//V sync offset
    #define TIME_VBP(a)             (a[11])							//V Back Porch
    #define TIME_VPW(a)             (a[12] & 0x7F)					//V sync pulse width		
    #define TIME_INT(a)             (a[12] >> 7)
    
    ptiming->pixel_per_line = TIME_APPL(vtimebuf);
    ptiming->pixel_per_frame = TIME_ALPF(vtimebuf);
    ptiming->hfp = TIME_HFP(vtimebuf);
    ptiming->hbp = TIME_HFP(vtimebuf);
    ptiming->hpw = TIME_HPW(vtimebuf);
    ptiming->vfp = TIME_VFP(vtimebuf);
    ptiming->vbp = TIME_VBP(vtimebuf);
    ptiming->vpw = TIME_VPW(vtimebuf);
    ptiming->_int = TIME_INT(vtimebuf);

    return vatek_result_success;
}


static void swip_byte(uint8_t* pbuf)
{
    uint8_t tmp = pbuf[0];
    pbuf[0] = pbuf[1];
    pbuf[1] = tmp;
}

static vatek_result read_timing(Pvatek_i2c vi2c, uint8_t* pbuf)
{
    vatek_result result = vatek_result_unknown;

    if (vi2c == NULL || pbuf == NULL)
        return vatek_result_invalidparm;

    if ((result = EP9555E_CHIP_RD(vi2c, EP9555E_SYNC_TIMING_H, &pbuf[0], EP9555E_SYNC_H_LEN)) != vatek_result_success)
        return result;

    if ((result = EP9555E_CHIP_RD(vi2c, EP9555E_SYNC_TIMING_V, &pbuf[EP9555E_SYNC_H_LEN], EP9555E_SYNC_V_LEN)) != vatek_result_success)
        return result;
    
    swip_byte(&pbuf[0]);
    swip_byte(&pbuf[2]);
    swip_byte(&pbuf[4]);
    swip_byte(&pbuf[6]);
    swip_byte(&pbuf[8]);
    
    return result;
}


static vatek_result get_scaletiming(video_input_resolution resolution, Phdmi_video_timing vtiming)
{
    typedef struct _ep9555e_timingmap
    {
        video_input_resolution resolution;
        uint16_t framewidth;
        uint16_t frameheight;
        uint16_t hfp;
        uint16_t hpw;
        uint8_t vfp;
        uint8_t vpw;
    }ep9555e_timingmap;

    const ep9555e_timingmap ep9555e_timing_table[] = 
    {
        {vi_resolution_480p59_94,720,480,16,62,9,6,},
        {vi_resolution_480p60,720,480,16,62,9,6,},
        {vi_resolution_576p50,720,576,12,64,5,5,},
        {vi_resolution_720p50,1280,720,440,40,5,5,},
        {vi_resolution_720p59_94,1280,720,110,40,5,5,},
        {vi_resolution_720p60,1280,720,110,40,5,5,},
        //{vi_resolution_1080i59_94,1920,540,88,44,2,5},
        //{vi_resolution_1080i60,1920,540,88,44,2,5},
        {vi_resolution_1080p50,1920,1080,528,44,4,5,},
        {vi_resolution_1080p59_94,1920,1080,88,44,4,5,},
        {vi_resolution_1080p60,1920,1080,88,44,4,5,},
    /*
        vi_resolution_1080p30          = 4,
        vi_resolution_1080p25          = 5,
        vi_resolution_1080p24          = 6,
        vi_resolution_1080p23_97       = 7,
        vi_resolution_1080i50          = 10,
        vi_resolution_576i50           = 15,
        vi_resolution_480i60           = 18,
        vi_resolution_480i59_94        = 19,
    */
    };

    uint16_t timetable_nums = sizeof(ep9555e_timing_table)/sizeof(ep9555e_timingmap);
    uint16_t idx = 0;
    vatek_result result = vatek_result_unsupport;

    for (idx = 0; idx < timetable_nums; idx++)
    {
        if (ep9555e_timing_table[idx].resolution == resolution)
        {
            vtiming->pixel_per_line = ep9555e_timing_table[idx].framewidth;
            vtiming->pixel_per_frame = ep9555e_timing_table[idx].frameheight;
            vtiming->hfp = ep9555e_timing_table[idx].hfp;
            vtiming->hpw = ep9555e_timing_table[idx].hpw;
            vtiming->vfp = ep9555e_timing_table[idx].vfp;
            vtiming->vpw = ep9555e_timing_table[idx].vpw;
            result = vatek_result_success;
            break;
        }
    }

    return result;
}

static vatek_result set_outputtiming(Pep9555e_handle hep9555e, Pphy_video_info info)
{
    vatek_result result = vatek_result_unknown;
    uint8_t buf[EP9XXX_VTIME_SIZE];
//    hdmi_video_id vtable = {0};
    hdmi_video_timing vtiming;
    uint8_t w_buf[2] = {0};
    uint8_t vid = 0;
    
    if (hep9555e == NULL)
        return vatek_result_invalidparm;

    //get original timing  or want to set
    if (hep9555e->outputmode == ep9555e_bypassmode)
    {
        if ((result = read_timing(hep9555e->vi2c, buf)) != vatek_result_success)
        {
            EP9555E_ERR("read timing fail");
            return result;
        }

        if ((result = get_videotiming(buf, &vtiming)) != vatek_result_success)
        {
            EP9555E_ERR("transfer timing fail");
            return result;
        }
    }
    else
    {
//        result = tool_hdmi_get_vid( info, &vid);
//        if(result==vatek_result_success) result = tool_hdmi_get_videoinfo(vid, &vtiming);
//        if(result!=vatek_result_success)
//        {
//            EP9555E_ERR("unsupport scale timing");
//            return result;
//        }
        if ((result = get_scaletiming(info->resolution, &vtiming)) != vatek_result_success)
        if( result != vatek_result_success)
        {
            EP9555E_ERR("unsupport scale timing");
            return result;
        }
    }
    //tool_ep9xxx_print_videotiming(&vtiming);
    //set scale timing
    w_buf[0] = vtiming.pixel_per_line >> 8;
    w_buf[1] = vtiming.pixel_per_line & 0xFF;
    if ((result = EP9555E_CHIP_WR(hep9555e->vi2c, EP9555E_SCALE_ACTIVE_H, w_buf, 2)) != vatek_result_success)
        return result;

    w_buf[0] = vtiming.pixel_per_frame >> 8;
    w_buf[1] = vtiming.pixel_per_frame & 0xFF;
    if ((result = EP9555E_CHIP_WR(hep9555e->vi2c, EP9555E_SCALE_ACTIVE_V, w_buf, 2)) != vatek_result_success)
        return result;

    w_buf[0] = vtiming.hfp >> 8;
    w_buf[1] = vtiming.hfp & 0xFF;
    if ((result = EP9555E_CHIP_WR(hep9555e->vi2c, EP9555E_SCALE_H_DELAY, w_buf, 2)) != vatek_result_success)
        return result;

    w_buf[0] = vtiming.hpw >> 8;
    w_buf[1] = vtiming.hpw & 0xFF;
    if ((result = EP9555E_CHIP_WR(hep9555e->vi2c, EP9555E_SCALE_H_WIDTH, w_buf, 2)) != vatek_result_success)
        return result;

    if ((result = EP9555E_CHIP_WR(hep9555e->vi2c, EP9555E_SCALE_V_DELAY, &vtiming.vfp, 1)) != vatek_result_success)
        return result;
    
    if ((result = EP9555E_CHIP_WR(hep9555e->vi2c, EP9555E_SCALE_V_WIDTH, &vtiming.vpw, 1)) != vatek_result_success)
            return result;
    
    return result;
}

static vatek_result i2c_write(Pvatek_i2c vi2c, uint8_t dev, uint16_t reg, uint8_t* pbuf, int32_t len)
{
	uint8_t buf[2] = {0};
    vatek_result result = vatek_result_unknown;
    
    if (vi2c == NULL || pbuf == NULL || len == 0)
        return vatek_result_invalidparm;

    if ((result = vi2c->start(vi2c->hboard, EP9555E_ADDR_W(dev), 0)) != vatek_result_success)
        return vatek_result_i2cfail;

    buf[0] = (uint8_t)(reg >> 8);
    buf[1] = (uint8_t)reg;
    if ((result = vi2c->write(vi2c->hboard, &buf[0], 2)) != vatek_result_success)
        return vatek_result_i2cfail;

    if ((result = vi2c->write(vi2c->hboard, pbuf, len)) != vatek_result_success)
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

    if ((result = vi2c->start(vi2c->hboard, EP9555E_ADDR_W(dev), 0)) != vatek_result_success)
        return vatek_result_i2cfail;

    buf[0] = (uint8_t)(reg >> 8);
    buf[1] = (uint8_t)reg;
    if ((result = vi2c->write(vi2c->hboard, &buf[0], 2)) != vatek_result_success)
        return vatek_result_i2cfail;

    if ((result = vi2c->stop(vi2c->hboard)) != vatek_result_success)
        return vatek_result_i2cfail;

    if ((result = vi2c->start(vi2c->hboard, EP9555E_ADDR_R(dev), 0)) != vatek_result_success)
        return vatek_result_i2cfail;
    
    if ((result = vi2c->read(vi2c->hboard, pbuf, len)) != vatek_result_success)
        return vatek_result_i2cfail;

    if ((result = vi2c->stop(vi2c->hboard)) != vatek_result_success)
            return vatek_result_i2cfail;

    return result;
}

vatek_result ep9555e_create(Pvatek_i2c vi2c, Pep9555e_handle *hep9555e)
{
    vatek_result result = vatek_result_unknown;
    Pep9555e_handle newphy = NULL; 
    const ep9555e_reg ep9555e_init_regs[] =
    {
        { EP9555E_INT_CNTL              ,0x00 },
        { EP9555E_POL_CNTL              ,0x2A },    /* VSYNC_LOW */
        { EP9555E_VOUT_CNTL             ,0x00 },
        { EP9555E_AOUT_CNTL             ,0x03 },
        { EP9555E_AOUT_CHMAP_CNTL       ,0x00 },
        { EP9555E_SYS_CNTL              ,0x10 },
        { EP9555E_MUTE_CNTL             ,0x08 },
        { EP9555E_OUTFMT_CNTL           ,0x49 },    //Disable AUTO video ouput format
        { EP9555E_MANUAL_VFMT_CNTL      ,0xA4 },    //YCC422 output, ITU-R BT709
        { EP9555E_OUTPUT_DISABLE_CNTL   ,0xD1 },    //SPDIF, IIS, DE, SYNC, DCLK, D0-2 in tri-state
    };
    uint32_t init_reg_num = sizeof(ep9555e_init_regs)/sizeof(ep9555e_reg);
    uint8_t val = 0, val_r = 0;
    uint32_t idx = 0;

    if (vi2c == NULL || hep9555e == NULL)
        return vatek_result_invalidparm;
    
    EP9555E_LOG("reset ep9555e");
    if ((result = vatek_system_reset(sys_dev_type_phy_ep9555e, vi2c->hboard)) != vatek_result_success)
        return result;
    
    vatek_system_delay(1000); // wait ep9555e reboot

    EP9555E_LOG("init ep9555e");
    if ((result = EP9555E_CHIP_RD(vi2c, EP9555E_SYS_STATUS1, &val, 1)) != vatek_result_success)
        return result;
    
    newphy = (Pep9555e_handle)malloc(sizeof(ep9555e_handle));
    if (newphy == NULL)
        return vatek_result_memfail;

    for (idx = 0; idx < init_reg_num; idx++)    
    {
        val = ep9555e_init_regs[idx].val;
        if ((result = EP9555E_CHIP_WR(vi2c, ep9555e_init_regs[idx].addr, &val, 1)) != vatek_result_success)
            return result;
        
        if ((result = EP9555E_CHIP_RD(vi2c, ep9555e_init_regs[idx].addr, &val_r, 1)) != vatek_result_success)
            return result;
        
        if (val != val_r)
            return vatek_result_badstatus;
     
    }
    
    newphy->vi2c = vi2c;    
    newphy->outputmode = ep9555e_interlaced_2_p;
    newphy->baseclk = baseclock_1001;
    *hep9555e = newphy;

    vatek_system_delay(200); // wait ep9555e initialize
    
    return result;
}

vatek_result ep9555e_destroy(Pep9555e_handle hep9555e)
{
    vatek_result result = vatek_result_success;
    if (hep9555e == NULL)
        return vatek_result_invalidparm;
    
    free(hep9555e);
    hep9555e = NULL;

    return result;
}

vatek_result ep9555e_set_outputmode(Pep9555e_handle hep9555e, phy_output_mode mode)
{
    vatek_result result = vatek_result_success;
    if (hep9555e == NULL)
        return vatek_result_invalidparm;
    
    hep9555e->outputmode = (ep9555e_outputmode)mode;
    
    return result;
}

vatek_result ep9555e_set_baseclock(Pep9555e_handle hep9555e, phy_output_baseclock clk)
{
    vatek_result result = vatek_result_success;
    if (hep9555e == NULL)
        return vatek_result_invalidparm;
    
    hep9555e->baseclk = (ep9555e_baseclock)clk;
    
    return result;
}

vatek_result ep9555e_get_videoinfo(Pep9555e_handle hep9555e, Pphy_video_info info)
{
    vatek_result result = vatek_result_unknown;
    ep9555e_status epstatus = epstatus_invalid;
    uint8_t val = 0;
    hdmi_video_timing epinfo = {0};

    if (hep9555e == NULL || info == NULL)
        return vatek_result_invalidparm;

    if ((result = check_sourcevalid(hep9555e->vi2c, &epstatus)) != vatek_result_success)
        return result;
    
    if (epstatus != epstatus_active && epstatus != epstatus_active_protect)
        return vatek_result_badstatus;

    if ((result = EP9555E_CHIP_RD(hep9555e->vi2c, EP9555E_V_TIMING, &val, 1)) != vatek_result_success)
        return result;

    //Search Timing Table.
    if ((result = tool_hdmi_get_videoinfo(val, &epinfo)) != vatek_result_success)
    {
        EP9555E_ERR("unsupported video code : %d", val);
        return vatek_result_unsupport;
    }

    if (hep9555e->outputmode != ep9555e_manual)
    {
        if ((result = autoset_videoinfo(hep9555e, &epinfo)) != vatek_result_success)
        {
            EP9555E_ERR("auto set video fail, result = %d ", result);
        }
    }

    info->aspectrate = epinfo.aspectrate;
    info->resolution = epinfo.resolution;
    info->buswidth_16 = 1;
    info->separated_sync = 1;
    
    return result;
}

vatek_result ep9555e_set_videoinfo(Pep9555e_handle hep9555e, Pphy_video_info info)
{
    vatek_result result = vatek_result_unknown;
    ep9555e_status epstatus = epstatus_invalid;

    if (hep9555e == NULL || info == NULL)
        return vatek_result_invalidparm;

    if ((result = check_sourcevalid(hep9555e->vi2c, &epstatus)) != vatek_result_success)
        return result;
    
    if (epstatus != epstatus_active)
        return vatek_result_badstatus;

    if ((result = set_outputformat(hep9555e)) != vatek_result_success)
        return result;

    if ((result = set_outputtiming(hep9555e, info)) != vatek_result_success)
         return result;
    
    return result;
}

vatek_result ep9555e_get_audioinfo(Pep9555e_handle hep9555e, Pphy_audio_info info)
{
    vatek_result result = vatek_result_unknown;
    uint8_t val = 0;
    if (hep9555e == NULL || info == NULL)
        return vatek_result_invalidparm;
    
    if ((result = EP9555E_CHIP_RD(hep9555e->vi2c, EP9555E_SYS_STATUS1, &val, 1)) != vatek_result_success)
        return result;

    if(!is_ep9555e_source_valid(val))
        return vatek_result_badstatus;

    if ((result = EP9555E_CHIP_RD(hep9555e->vi2c, EP9555E_A_STATUS0, &val, 1)) != vatek_result_success)
        return result;

    info->samplerate = audioinfo_samplerate(val);
    if (info->samplerate == ai_samplerate_unknown)
    {
        EP9555E_ERR("unsupport audio sample rate : %d", val);
        result = vatek_result_unsupport;
    }

    return result;
}

vatek_result ep9555e_get_status(Pep9555e_handle hep9555e, phy_status *status)
{
    vatek_result result = vatek_result_unknown;
    ep9555e_status epstatus = epstatus_invalid;
    
    if (hep9555e == NULL || status == NULL)
        return vatek_result_invalidparm;

    if ((result = check_sourcevalid(hep9555e->vi2c, &epstatus)) != vatek_result_success)
        return result;
    
    switch (epstatus)
    {
        case epstatus_invalid:
        case epstatus_hw_fail:
            *status = phy_status_fail;
            break;

        case epstatus_idle:
            *status = phy_status_lost;
            break;

        case epstatus_active:
        case epstatus_active_protect:
            *status = phy_status_active;
            break;

        default:
            *status = phy_status_unknown;
            break;
    }
    
    return result;
}

vatek_result ep9555e_enable(Pep9555e_handle hep9555e, uint8_t enable)
{
    vatek_result result = vatek_result_unknown;
    uint8_t val = 0, r_val = 0; 

    if (hep9555e == NULL)
        return vatek_result_invalidparm;

    if (enable)
        val = OUTPUT_DISABLE_CNTL_I2S_PCM;
    else
        val = OUTPUT_DISABLE_CNTL_SPDIF_DIS | OUTPUT_DISABLE_CNTL_IIS_DIS | OUTPUT_DISABLE_CNTL_VOUT_DIS | OUTPUT_DISABLE_CNTL_I2S_PCM;
    
    if ((result = EP9555E_CHIP_WR(hep9555e->vi2c, EP9555E_OUTPUT_DISABLE_CNTL, &val, 1)) != vatek_result_success)
        return result;

    if ((result = EP9555E_CHIP_RD(hep9555e->vi2c, EP9555E_OUTPUT_DISABLE_CNTL, &r_val, 1)) != vatek_result_success)
        return result;

    if (val != r_val)
        return vatek_result_badstatus;

    return result;
}

#endif

