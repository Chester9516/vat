
#if defined(PHY_EP9351)

#include "vatek_system.h"
#include "phy/phy_ep9351_internal.h"
#include "phy/tool_hdmi.h"

uint8_t g_edid[256]=
{
    0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00,   //00
    0x06, 0x8F, 0x07, 0x11, 0x01, 0x00, 0x00, 0x00,   //08
    0x17, 0x11, 0x01, 0x03, 0x80, 0x0C, 0x09, 0x78,   //10
    0x0A, 0x1E, 0xAC, 0x98, 0x59, 0x56, 0x85, 0x28,   //18
    0x29, 0x52, 0x57, 0x00, 0x00, 0x00, 0x01, 0x01,   //20
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,   //28
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x8C, 0x0A,   //30
    0xD0, 0x8A, 0x20, 0xE0, 0x2D, 0x10, 0x10, 0x3E,   //38
    0x96, 0x00, 0x81, 0x60, 0x00, 0x00, 0x00, 0x18,   //40
    0x01, 0x1D, 0x80, 0x18, 0x71, 0x1C, 0x16, 0x20,   //48
    0x58, 0x2C, 0x25, 0x00, 0x81, 0x49, 0x00, 0x00,   //50
    0x00, 0x9E, 0x00, 0x00, 0x00, 0xFC, 0x00, 0x56,   //58
    0x41, 0x2D, 0x31, 0x38, 0x30, 0x39, 0x41, 0x0A,   //60
    0x20, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0xFD,   //68
    0x00, 0x17, 0x3D, 0x0D, 0x3E, 0x11, 0x00, 0x0A,   //70
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x01, 0x1C,   //78
    0x02, 0x03, 0x34, 0x71, 0x4D, 0x82, 0x05, 0x04,   //80
    0x01, 0x10, 0x11, 0x14, 0x13, 0x1F, 0x06, 0x15,   //88
    0x03, 0x12, 0x35, 0x0F, 0x7F, 0x07, 0x17, 0x1F,
    0x38, 0x1F, 0x07, 0x30, 0x2F, 0x07, 0x72, 0x3F,
    0x7F, 0x72, 0x57, 0x7F, 0x00, 0x37, 0x7F, 0x72,
    0x83, 0x4F, 0x00, 0x00, 0x67, 0x03, 0x0C, 0x00,
    0x10, 0x00, 0x88, 0x2D, 0x00, 0x00, 0x00, 0xFF,
    0x00, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00, 0x00,
    0x00, 0xFF, 0x00, 0x0A, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x00, 0x00, 0x00, 0xFF, 0x00, 0x0A, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xDA,
};

static vatek_result i2c_read(Pvatek_i2c vi2c, uint8_t reg, uint8_t* pbuf, int32_t len)
{
    vatek_result result = vatek_result_unknown;
    
    if (vi2c == NULL || pbuf == NULL || len == 0)
        return vatek_result_invalidparm;

    if ((result = vi2c->start(vi2c->hboard, EP9351_WRITE_ADDR, 0)) != vatek_result_success)
        return vatek_result_i2cfail;
    
    if ((result = vi2c->write(vi2c->hboard, &reg, 1)) != vatek_result_success)
        return vatek_result_i2cfail;

    if ((result = vi2c->stop(vi2c->hboard)) != vatek_result_success)
        return vatek_result_i2cfail;

    if ((result = vi2c->start(vi2c->hboard, EP9351_READ_ADDR, 0)) != vatek_result_success)
        return vatek_result_i2cfail;
    
    if ((result = vi2c->read(vi2c->hboard, pbuf, len)) != vatek_result_success)
        return vatek_result_i2cfail;

    if ((result = vi2c->stop(vi2c->hboard)) != vatek_result_success)
            return vatek_result_i2cfail;

    return result;
}

static vatek_result i2c_write(Pvatek_i2c vi2c, uint8_t reg, uint8_t* pbuf, int32_t len)
{
    vatek_result result = vatek_result_unknown;
    
    if (vi2c == NULL || pbuf == NULL || len == 0)
        return vatek_result_invalidparm;

    if ((result = vi2c->start(vi2c->hboard, EP9351_WRITE_ADDR, 0)) != vatek_result_success)
        return vatek_result_i2cfail;

    if ((result = vi2c->write(vi2c->hboard, &reg, 1)) != vatek_result_success)
        return vatek_result_i2cfail;

    if ((result = vi2c->write(vi2c->hboard, pbuf, len)) != vatek_result_success)
        return vatek_result_i2cfail;

    if ((result = vi2c->stop(vi2c->hboard)) != vatek_result_success)
        return vatek_result_i2cfail;
    
    vatek_system_delay(1);
    return result;
}

static vatek_result initialize(Pvatek_i2c vi2c, uint8_t *edid)
{
    vatek_result result = vatek_result_unknown;
    uint8_t val = 0;

    if (vi2c == NULL || edid == NULL)
        return vatek_result_invalidparm;

    //reset    
    val = GENEROL_CONTROL_0_HDCP_RST | GENEROL_CONTROL_0_DDC_DIS | GENEROL_CONTROL_0_RST;
    if ((result = i2c_write(vi2c, EP9351_GENERAL_CONTROL_0, &val, 1)) != vatek_result_success)
        return result;
    //normal
    val = GENEROL_CONTROL_0_DDC_DIS;
    if ((result = i2c_write(vi2c, EP9351_GENERAL_CONTROL_0, &val, 1)) != vatek_result_success)
        return result;
    //powerdown
    val = GENEROL_CONTROL_0_DDC_DIS | GENEROL_CONTROL_0_PWR_DWN | GENEROL_CONTROL_0_DE_RST;
    if ((result = i2c_write(vi2c, EP9351_GENERAL_CONTROL_0, &val, 1)) != vatek_result_success)
        return result;
    
    //edid
    if ((result = i2c_write(vi2c, EP9351_EDID_DATA_REGISTER, edid, 256)) != vatek_result_success)
        return result;
    val = GENEROL_CONTROL_0_ON_CHIP_EDID;
    if ((result = i2c_write(vi2c, EP9351_GENERAL_CONTROL_0, &val, 1)) != vatek_result_success)
        return result;

    val = GENERAL_CONTROL_4_LNK_ON_RST;
    if ((result = i2c_write(vi2c, EP9351_GENERAL_CONTROL_4, &val, 1)) != vatek_result_success)
        return result;

    val = EP9351_GC_1_IO_DISABLE;
    if ((result = i2c_write(vi2c, EP9351_GENERAL_CONTROL_1, &val, 1)) != vatek_result_success)
        return result;
    
    return result;
}

static vatek_result check_sourcevalid(Pvatek_i2c vi2c, ep9351_status* status)
{
    vatek_result result = vatek_result_unknown;
    uint8_t status0 = 0;
    uint8_t status1 = 0;

    if (vi2c == NULL || status == NULL)
        return vatek_result_invalidparm;

    *status = epstatus_hw_fail;
    
    if ((result = i2c_read(vi2c, EP9351_STATUS_REGISTER_0, &status0, 1)) != vatek_result_success)
        return result;
    if ((result = i2c_read(vi2c, EP9351_STATUS_REGISTER_1, &status1, 1)) != vatek_result_success)
        return result;

    *status = epstatus_idle;
    if (status1 & STATUS_REGISTER_1_LINK_ON)
    {
        if((status1 & EP9351_SR_1_LINKED_ON) == EP9351_SR_1_LINKED_ON)
        {
            *status = epstatus_invalid;
            if(status0 & STATUS_REGISTER_0_HDMI)
            {
                *status = epstatus_active;
            }
            else
                EP9351_LOG("linked but not hdmi");
        }
        else
            EP9351_LOG("linked but invalid");
    }

    return result;
}

static audio_input_samplerate audioinfo_samplerate(uint8_t val)
{
    audio_input_samplerate samplerate = ai_samplerate_unknown;
    val &= 0x7;

    switch (val)
    {
        case EP9351_AUDIO_SAMPLERATE_32KHZ:
            samplerate = ai_samplerate_32K;
            break;

        case EP9351_AUDIO_SAMPLERATE_44_1KHZ:
            samplerate = ai_samplerate_44_1K;
            break;

        case EP9351_AUDIO_SAMPLERATE_48KHZ:
            samplerate = ai_samplerate_48K;
            break;

        default:
            break;
    }

    return samplerate;
}

static vatek_result autoset_videoinfo(Pep9351_handle hep9351, Phdmi_video_timing epinfo)
{
    vatek_result result = vatek_result_unknown;
    phy_video_info info;
    
    if (hep9351 == NULL || epinfo == NULL)
        return vatek_result_invalidparm;
    
    info.resolution = epinfo->resolution;
    info.aspectrate = epinfo->aspectrate;

    if (hep9351->baseclk == baseclock_1001)
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
    
    epinfo->resolution = info.resolution;
    epinfo->aspectrate = info.aspectrate;
    
    return result;
}

static vatek_result set_ouputformat(Pvatek_i2c vi2c)
{
    vatek_result result = vatek_result_unknown;
    uint8_t vinfo[EP9XXX_VINFO_SIZE];
    uint8_t val = 0;
    uint8_t flag = 0;
        
    if (vi2c == NULL)
        return vatek_result_invalidparm;

    if ((result = i2c_read(vi2c, EP9351_AVI_INFOFRAME, vinfo, EP9XXX_VINFO_SIZE)) != vatek_result_success)
        return result;

    val = EP9351_GC_4_OUTPUT;
    flag = VINFO_COLORSPACE(vinfo);
    if(flag != Y_COLOR_RGB)
    {
        val |= GENERAL_CONTROL_2_INCOLOR_YCC;
        if(flag == Y_COLOR_YCC_422)
            val |= GENERAL_CONTROL_2_INFORMAT_422;
    }
                    
    flag = VINFO_PIXEL_REPEAT(vinfo) & 0xF;
    if(flag != 0)
        val |= ((flag) & 0x3);
    
    flag = VINFO_COLORIMETRY(vinfo);
    if(flag == C_COLORSPACE_ITU709)
        val |= GENERAL_CONTROL_2_ITU709;
    else if(flag == C_COLORSPACE_EXT)
    {
        flag = VINFO_EX_COLORIMETRY(vinfo);
        if(flag == EC_COLORSPACE_CC709)
            val |= GENERAL_CONTROL_2_ITU709;
    }
                
    flag = VINFO_QUANT_RANGE(vinfo);
    if(flag == Q_COLORRANGE_FULL)
        val |= GENERAL_CONTROL_2_FULLRANGE;

    if ((result = i2c_write(vi2c, EP9351_GENERAL_CONTROL_2, &val, 1)) != vatek_result_success)
        return result;

    return result;
}   

vatek_result ep9351_create(Pvatek_i2c vi2c, Pep9351_handle *hep9351)
{
   
    vatek_result result = vatek_result_unknown;
    Pep9351_handle newphy = NULL;
    uint8_t val = 0;

    if (vi2c == NULL || hep9351 == NULL)
        return vatek_result_invalidparm;
    
    EP9351_LOG("reset ep9351");
    if ((result = vatek_system_reset(sys_dev_type_phy_ep9351, vi2c->hboard)) != vatek_result_success)
        return result;
    
    vatek_system_delay(1000); // wait ep9351 reboot

    EP9351_LOG("init ep9351");
    
    if ((result = i2c_read(vi2c, EP9351_STATUS_REGISTER_0, &val, 1)) != vatek_result_success)
        return result;

    newphy = (Pep9351_handle)malloc(sizeof(ep9351_handle));
    if (newphy == NULL)
        return vatek_result_memfail;

    newphy->vi2c = vi2c;
    newphy->baseclk = baseclock_1001;
    
    if ((result = initialize(newphy->vi2c, g_edid)) != vatek_result_success)
        return result;

    *hep9351 = newphy;
    vatek_system_delay(200); // wait ep9351 initialize
    
    return result;
}

vatek_result ep9351_destroy(Pep9351_handle hep9351)
{
    vatek_result result = vatek_result_success;

    if (hep9351 == NULL)
        return vatek_result_invalidparm;
    
    free(hep9351);
    hep9351 = NULL;

    return result;
}

vatek_result ep9351_enable(Pep9351_handle hep9351, uint8_t enable)
{
    vatek_result result = vatek_result_unknown;
    uint8_t val = 0;
    uint8_t vcfg = 0;
    ep9351_status epstatus = epstatus_idle; 
        
    if (hep9351 == NULL)
        return vatek_result_invalidparm;

    if (!enable)
    {
        val = EP9351_GC_1_IO_DISABLE;
        if ((result = i2c_write(hep9351->vi2c, EP9351_GENERAL_CONTROL_1, &val, 1)) != vatek_result_success)
            return result;
    }
    else
    {
        if ((result = check_sourcevalid(hep9351->vi2c, &epstatus)) != vatek_result_success)
            return result;

        if (epstatus != epstatus_active && epstatus != epstatus_active_protect)
            return vatek_result_badstatus;
        
        if ((result = set_ouputformat(hep9351->vi2c)) != vatek_result_success)
            return result;

        if ((result = i2c_read(hep9351->vi2c, EP9351_GENERAL_CONTROL_1, &val, 1)) != vatek_result_success)
            return result;

        if((val & GENERAL_CONTROL_1_V_POL_RW) == 0)
            vcfg |= GENERAL_CONTROL_1_V_POL_RW;
        if(val & GENERAL_CONTROL_1_H_POL_RW)
            vcfg |= GENERAL_CONTROL_1_H_POL_RW;
        
        if ((result = i2c_write(hep9351->vi2c, EP9351_GENERAL_CONTROL_1, &vcfg, 1)) != vatek_result_success)
            return result;
    }

    return result;
}

vatek_result ep9351_get_audioinfo(Pep9351_handle hep9351, Pphy_audio_info info)
{
    vatek_result result = vatek_result_unknown;
    ep9351_status epstatus = epstatus_idle;
    uint8_t val = 0;

    if (hep9351 == NULL || info == NULL)
        return vatek_result_invalidparm;

    if ((result = check_sourcevalid(hep9351->vi2c, &epstatus)) != vatek_result_success)
        return result;

    if (epstatus != epstatus_active && epstatus != epstatus_active_protect)
        return vatek_result_badstatus;

    if ((result = i2c_read(hep9351->vi2c, EP9351_GENERAL_CONTROL_4, &val, 1)) != vatek_result_success)
        return result;

    info->samplerate = audioinfo_samplerate(val);
    if (info->samplerate == ai_samplerate_unknown)
    {
        EP9351_ERR("unsupport audio sample rate : %d", val);
        result = vatek_result_unsupport;
    }
    
    return result;
}

vatek_result ep9351_get_videoinfo(Pep9351_handle hep9351, Pphy_video_info info)
{
    vatek_result result = vatek_result_unknown;
    ep9351_status epstatus = epstatus_idle;
    uint8_t vbuf[EP9XXX_VINFO_SIZE];
    uint8_t vcode = 0;    
    //ep9xxx_video_info epinfo = {0};
    hdmi_video_timing epinfo = {0};

    if (hep9351 == NULL || info == NULL)
        return vatek_result_invalidparm;

    if ((result = check_sourcevalid(hep9351->vi2c, &epstatus)) != vatek_result_success)
        return result;

    if ((result = i2c_read(hep9351->vi2c, EP9351_AVI_INFOFRAME, vbuf, EP9XXX_VINFO_SIZE)) != vatek_result_success)
        return result;

    vcode = VINFO_VIDEOCODE(vbuf);

    //if ((result = tool_ep9xxx_get_videoinfo(vcode, &epinfo)) != vatek_result_success)
    if ((result = tool_hdmi_get_videoinfo(vcode, &epinfo)) != vatek_result_success)
    {
        EP9351_ERR("unsupported video code : %d", vcode);
        return vatek_result_unsupport;
    }
    
    if(hep9351->baseclk==baseclock_1001)
    {
        switch (epinfo.resolution)
        {
            case vi_resolution_1080i60:
                epinfo.resolution = vi_resolution_1080i59_94;
                break;

            case vi_resolution_1080p60:
                epinfo.resolution = vi_resolution_1080p59_94;
                break;

            case vi_resolution_720p60:
                epinfo.resolution = vi_resolution_720p59_94;
                break;

            case vi_resolution_480i60:
                epinfo.resolution = vi_resolution_480i59_94;
                break;

            case vi_resolution_480p60:
                epinfo.resolution = vi_resolution_480p59_94;
                break;
            
            default:
                break;
        }
    }
    
    info->aspectrate = epinfo.aspectrate;
    info->resolution = epinfo.resolution;
    info->buswidth_16 = 1;
    info->separated_sync = 1;

    return result;
}

vatek_result ep9351_get_status(Pep9351_handle hep9351, phy_status *status)
{
    vatek_result result = vatek_result_unknown;
    ep9351_status epstatus = epstatus_invalid;
    
    if (hep9351 == NULL || status == NULL)
        return vatek_result_invalidparm;

    if ((result = check_sourcevalid(hep9351->vi2c, &epstatus)) != vatek_result_success)
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

#endif

