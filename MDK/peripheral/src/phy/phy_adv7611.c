
#if defined(PHY_ADV7611)

#include "vatek_system.h"
#include "phy/phy_adv7611_internal.h"

static vatek_result i2c_write(Pvatek_i2c vi2c, uint8_t dev, uint8_t reg, uint8_t val)
{
    vatek_result result = vatek_result_unknown;
    
    if (vi2c == NULL)
        return vatek_result_invalidparm;

    if ((result = vi2c->start(vi2c->hboard, dev, 0)) != vatek_result_success)
        return vatek_result_i2cfail;

    if ((result = vi2c->write(vi2c->hboard, &reg, 1)) != vatek_result_success)
        return vatek_result_i2cfail;

    if ((result = vi2c->write(vi2c->hboard, &val, 1)) != vatek_result_success)
        return vatek_result_i2cfail;

    if ((result = vi2c->stop(vi2c->hboard)) != vatek_result_success)
        return vatek_result_i2cfail;
    
    return result;
}

static vatek_result i2c_read(Pvatek_i2c vi2c, uint8_t dev, uint8_t reg, uint8_t* val)
{
    vatek_result result = vatek_result_unknown;
    
    if (vi2c == NULL || val == NULL)
        return vatek_result_invalidparm;

    uint8_t dev_read = dev | 0x1;
    
    if ((result = vi2c->start(vi2c->hboard, dev, 0)) != vatek_result_success)
        return vatek_result_i2cfail;
    
    if ((result = vi2c->write(vi2c->hboard, &reg, 1)) != vatek_result_success)
        return vatek_result_i2cfail;

    if ((result = vi2c->start(vi2c->hboard, dev_read, 1)) != vatek_result_success)
        return vatek_result_i2cfail;

    if ((result = vi2c->read(vi2c->hboard, val, 1)) != vatek_result_success)
        return vatek_result_i2cfail;

    if ((result = vi2c->stop(vi2c->hboard)) != vatek_result_success)
        return vatek_result_i2cfail;
    
    return result;
}

static vatek_result write_regtable(Pvatek_i2c vi2c, Padv7611_reg table, uint32_t num)
{
    vatek_result result = vatek_result_unknown;
    uint32_t idx = 0;
    uint8_t val = 0;

    if (table == NULL || vi2c == NULL || num == 0)
        return vatek_result_invalidparm;
    
    for (idx = 0; idx < num; idx++)
    {
        if ((result = i2c_write(vi2c, table[idx].dev, table[idx].addr, table[idx].val)) != vatek_result_success)
            return vatek_result_i2cfail;

        if ((result = i2c_read(vi2c, table[idx].dev, table[idx].addr, &val)) != vatek_result_success)
            return vatek_result_i2cfail;

        if (val != table[idx].val)
        {
            ADV7611_ERR("write regtable fail!, [0x%02x][0x%02x] = 0x%02x", table[idx].dev, table[idx].addr, table[idx].val);
            return vatek_result_unknown;
            
        }
    }
    
    return result;
}   

static vatek_result initialize(Pvatek_i2c vi2c)
{
    vatek_result result = vatek_result_unknown;
    uint32_t num = 0;

    if (vi2c == NULL)
        return vatek_result_invalidparm;

    //MAIN_RESET , IO, 0xFF [7]
    // Main reset where everything, all I2C registers will be reset to their default values.
    // 0 - Normal Operation.
    // 1 - Apply Main I2C reset.
    if ((result = i2c_write(vi2c, IIC_ADV7611_BASE_ADDR, 0xFF, 0x80)) != vatek_result_success)
        return result;
    
    vatek_system_delay(15); //main reset need delay 15 ms
    
    num = sizeof(adv7611_initial_reg) / sizeof(adv7611_reg);
    if ((result = write_regtable(vi2c, adv7611_initial_reg, num) != vatek_result_success))
        return result;

    return vatek_result_success;
}

static vatek_result signalisdetect(Pvatek_i2c vi2c)
{
    vatek_result result = vatek_result_success;
    uint8_t val = 0;
    
    if (vi2c == NULL)
        return vatek_result_invalidparm;
    
    //detect cable
    if ((result = i2c_read(vi2c, IIC_ADV7611_BASE_ADDR, 0x6F, &val)) != vatek_result_success)
        return result;

    //CABLE_DET_A_RAW : IO, 0x6F [0]
    // Raw status of Port A +5 V cable detection signal.
    // 0 - No cable detected on Port A
    // 1 - Cable detected on Port A (High level on RXA_5V)
    if (!(val & 0x01))
    {
        ADV7611_LOG("No cable detected on Port A, 0x%X", val);
        return vatek_result_negative;
    }

    //HDMI_ENCRPT_A_RAW : IO, 0x6F [2]
    // Raw status of Port A Encryption detection signal.
    // 0 - Current frame in port A is not encrypted
    // 1 - Current frame in port A is encrypted
    if (val & 0x04)
    {
        //ADV7611_LOG("Current frame in port A is encrypted, 0x%X", val);
    }

    if ((result = i2c_read(vi2c, IIC_ADV7611_BASE_ADDR, 0x6A, &val)) != vatek_result_success)
        return result;

    //TMDSPLL_LCK_A_RAW : 0x6A [6]
    // A readback to indicate the raw status of the port A TMDS PLL lock signal.
    // 0 - TMDS PLL on port A is not locked
    // 1 - TMDS PLL on port A is locked to the incoming clock
    if (!(val & 0x40))
    {
        ADV7611_LOG("TMDS PLL on port A is not locked, 0x%X", val);
        return vatek_result_negative;
    }
    
    //TMDS_CLK_A_RAW : 0x6A [4]
    // Raw status of Port A TMDS Clock detection signal.
    // 0 - No TMDS clock detected on port A
    // 1 - TMDS clock detected on port A
    if (!(val & 0x10))
    {
        ADV7611_LOG("No TMDS clock detected on port A, 0x%X", val);
        return vatek_result_negative;
    }
    
    //V_LOCKED_RAW : 0x6A [1]
    // Raw status of the Vertical Sync Filter Locked signal.
    // 0 - Vertical sync filter has not locked and vertical sync parameters are not valid
    // 1 - Vertical sync filter has locked and vertical sync parameters are valid
    if (!(val & 0x02))
    {
        ADV7611_LOG("Vertical sync filter has not locked and vertical sync parameters are not valid, 0x%X", val);
        return vatek_result_negative;
    }
    
    //DE_REGEN_LCK_RAW : 0x6A [0]
    // Raw status of the DE regeneration lock signal.
    // 0 - DE regeneration block has not been locked
    // 1 - DE regeneration block has been locked to the incoming DE signal
    if (!(val & 0x01))
    {
        ADV7611_LOG("DE regeneration block has not been locked, 0x%X", val);
        return vatek_result_negative;
    }
    
    return result;
}

static vatek_result get_videoinfo(Pvatek_i2c vi2c, Pphy_video_info info)
{
    vatek_result result = vatek_result_unknown;
    uint8_t val = 0, val_2 = 0;
    //uint16_t ch1_bl = 0;
    //uint8_t ch1_lcvs = 0;
    uint16_t ch1_fcl = 0;
    uint16_t ch1_lcf = 0;
    uint8_t ch1_stdi_intlcd = 0; 

    if (vi2c == NULL || info == NULL)
        return vatek_result_invalidparm;

    //default value
    info->resolution = vi_resolution_unknown;
    info->aspectrate = vi_aspectrate_16_9;
    info->separated_sync = 1;
    info->buswidth_16 = 1;

    //CH1_STDI_DVALID : CP, 0xB1 [7]
    // This bit is set when the measurements performed by sync channel 1 STDI are completed. 
    // High level signals validity for CH1_BL, CH1_LCF, CH1_LCVS, CH1_FCL, and CH1_STDI_INTLCD parameters. 
    // To prevent false readouts, especially during signal acquisition, CH1_SDTI_DVALID only goes high after four fields with 
    // same length are recorded. As a result, STDI measurements can take up to five fields to finish.
    // 0 - Sync channel 1 STDI measurement are not valid
    // 1 - Sync channel 1 STDI measurement are valid
    if ((result = i2c_read(vi2c, IIC_ADV7611_CP_ADDR, 0xB1, &val)) != vatek_result_success)
        return result;
    if (!(val & 0x80))
        return vatek_result_badstatus;

    //CH1_BL[13:0] : CP, 0xB1 [5:0], 0xB2 [7:0]
    // A readback for the Block Length for sync channel 1. Number of crystal cycle cycles in a block 
    // of eight lines of incoming video. This readback is valid if CH1_STDI_DVALID is high.
    //CH1_STDI_INTLCD : CP, 0xB1[6]
    // Interlaced vs. progressive mode detected by sync channel 1 STDI. 
    // The readback from this register is valid if CH1_STDI_DVALID is high.
    // 0 - Indicates a video signal on sync channel 1 with non interlaced timing.
    // 1 - Indicates a signal on sync channel 1 with interlaced timing.
    if ((result = i2c_read(vi2c, IIC_ADV7611_CP_ADDR, 0xB1, &val)) != vatek_result_success)
        return result;
    if ((result = i2c_read(vi2c, IIC_ADV7611_CP_ADDR, 0xB2, &val_2)) != vatek_result_success)
        return result;
    //ch1_bl = ((val & 0x3F) << 8) + val_2;
    ch1_stdi_intlcd = (val & 0x40) >> 6;

    //CH1_LCVS[4:0] : CP, 0xB3 [7:3]
    // A readback for the sync channel 1 Line Count in a VSync. Number of lines in a VSync 
    // period measured on sync channel 1. The readback from this field is valid if CH1_STDI_DVALID is high.
    if ((result = i2c_read(vi2c, IIC_ADV7611_CP_ADDR, 0xB3, &val)) != vatek_result_success)
        return result;
    //ch1_lcvs = val >> 3;

    //CH1_FCL[12:0] : CP, 0xB8 [4:0], 0xB9 [7:0]
    // A readback for the sync channel 1 Field Count Length Number of crystal clock cycles between successive 
    // VSyncs measured by sync channel 1 STDI or in 1/256th of a field. 
    // The readback from this field is valid if CH1_STDI_DVALID is high.
    if ((result = i2c_read(vi2c, IIC_ADV7611_CP_ADDR, 0xB8, &val)) != vatek_result_success)
        return result;
    if ((result = i2c_read(vi2c, IIC_ADV7611_CP_ADDR, 0xB9, &val_2)) != vatek_result_success)
        return result;
    ch1_fcl = ((val & 0x1F) << 8) + val_2;

    //CH1_LCF[11:0] : CP, 0xA3 [3:0], 0xA4 [7:0]
    // A readback for the sync channel 1 Line Count in a Field Number of lines between two VSyncs measured on sync channel 1. 
    // The readback from this field is valid if CH1_STDI_DVALID is high.
    if ((result = i2c_read(vi2c, IIC_ADV7611_CP_ADDR, 0xA3, &val)) != vatek_result_success)
        return result;
    if ((result = i2c_read(vi2c, IIC_ADV7611_CP_ADDR, 0xA4, &val_2)) != vatek_result_success)
        return result;
    ch1_lcf = ((val & 0x0F) << 8) + val_2;

    //reference
    // 1080P60
    //[adv7611  : 00005b28] : ch1_bl = 3385, ch1_lcf = 1124, ch1_lcvs = 5, ch1_fcl = 1864, ch1_stdi_intlcd = 0
    // 1080I60
    //[adv7611  : 00327a6f] : ch1_bl = 6779, ch1_lcf = 561, ch1_lcvs = 5, ch1_fcl = 1864, ch1_stdi_intlcd = 1 
    // 1080I50
    //[adv7611  : 00349a95] : ch1_bl = 8137, ch1_lcf = 561, ch1_lcvs = 5, ch1_fcl = 2237, ch1_stdi_intlcd = 1
    // 720P60
    //[adv7611  : 000155b9] : ch1_bl = 5082, ch1_lcf = 749, ch1_lcvs = 5, ch1_fcl = 1864, ch1_stdi_intlcd = 0
    // 720P50
    //[adv7611  : 0001d8eb] : ch1_bl = 6100, ch1_lcf = 749, ch1_lcvs = 5, ch1_fcl = 2237, ch1_stdi_intlcd = 0
    // 576P50
    //[adv7611  : 000103a3] : ch1_bl = 7322, ch1_lcf = 624, ch1_lcvs = 5, ch1_fcl = 2237, ch1_stdi_intlcd = 0
    // 480P60
    //[adv7611  : 00034731] : ch1_bl = 7271, ch1_lcf = 524, ch1_lcvs = 6, ch1_fcl = 1866, ch1_stdi_intlcd = 0
    
    //ADV7611_LOG("ch1_bl = %d, ch1_lcf = %d, ch1_lcvs = %d, ch1_fcl = %d, ch1_stdi_intlcd = %d", ch1_bl, ch1_lcf, ch1_lcvs, ch1_fcl, ch1_stdi_intlcd);

    if (ch1_stdi_intlcd)
    {   
        //interlaced,  because adv7611 field & V-Sync same pin -> embedded sync
        info->separated_sync = 0;
    }

    if (CHECK_RANGE(ch1_lcf, 1125))
    {
        if (CHECK_RANGE(ch1_fcl, 1868))
            info->resolution = vi_resolution_1080p60;
        else if (CHECK_RANGE(ch1_fcl, 2237))
            info->resolution = vi_resolution_1080p50;
    }
    else if (CHECK_RANGE(ch1_lcf, 750))
    {
        if (CHECK_RANGE(ch1_fcl, 1868))
            info->resolution = vi_resolution_720p60;
        else if (CHECK_RANGE(ch1_fcl, 2237))
            info->resolution = vi_resolution_720p50;
    }
    else if (CHECK_RANGE(ch1_lcf, 625) && CHECK_RANGE(ch1_fcl, 2237))
    {
        info->resolution = vi_resolution_576p50;
        info->aspectrate = vi_aspectrate_4_3;
    }
    else if (CHECK_RANGE(ch1_lcf, 562) && ch1_stdi_intlcd)
    {
        if (CHECK_RANGE(ch1_fcl, 1868))
            info->resolution = vi_resolution_1080i60;
        else if (CHECK_RANGE(ch1_fcl, 2237))
            info->resolution = vi_resolution_1080i50;
    }
    else if (CHECK_RANGE(ch1_lcf, 525) && CHECK_RANGE(ch1_fcl, 1868))
    {
        info->resolution = vi_resolution_480p60;
        info->aspectrate = vi_aspectrate_4_3;
    }
    else if (CHECK_RANGE(ch1_lcf, 312) && ch1_stdi_intlcd)
    {
        info->resolution = vi_resolution_576i50;
        info->aspectrate = vi_aspectrate_4_3;
    }
    else if (CHECK_RANGE(ch1_lcf, 262) && ch1_stdi_intlcd)
    {
        info->resolution = vi_resolution_480i60;
        info->aspectrate = vi_aspectrate_4_3;
    }

    return result;
}

static vatek_result set_videoinfo(Pvatek_i2c vi2c, Pphy_video_info info)
{
    vatek_result result = vatek_result_unknown;
    uint8_t val_resolution = 0;
    uint8_t val_framerate = 0;

    if (vi2c == NULL || info == NULL)
        return vatek_result_invalidparm;

    switch (info->resolution)
    {
        case vi_resolution_1080p60:
        case vi_resolution_1080p59_94:
            val_resolution = ADV7611_VALUE_1920_1080P;
            val_framerate = ADV7611_VALUE_FREQ60HZ;
            break;
        
        case vi_resolution_1080p50:
            val_resolution = ADV7611_VALUE_1920_1080P;
            val_framerate = ADV7611_VALUE_FREQ50HZ;
            break;
        
        case vi_resolution_1080p30:
            val_resolution = ADV7611_VALUE_1920_1080P;
            val_framerate = ADV7611_VALUE_FREQ30HZ;
            break;
        
        case vi_resolution_1080p25:
            val_resolution = ADV7611_VALUE_1920_1080P;
            val_framerate = ADV7611_VALUE_FREQ25HZ;
            break;
        
        case vi_resolution_1080p24:
            val_resolution = ADV7611_VALUE_1920_1080P;
            val_framerate = ADV7611_VALUE_FREQ24HZ;
            break;
            
        case vi_resolution_1080i60:
        case vi_resolution_1080i59_94:
            val_resolution = ADV7611_VALUE_1920_1080I;
            val_framerate = ADV7611_VALUE_FREQ60HZ;
            break;
        
        case vi_resolution_1080i50:
            val_resolution = ADV7611_VALUE_1920_1080I;
            val_framerate = ADV7611_VALUE_FREQ50HZ;
            break;

        case vi_resolution_720p60:
        case vi_resolution_720p59_94:
            val_resolution = ADV7611_VALUE_1280_720;
            val_framerate = ADV7611_VALUE_FREQ60HZ;
            break;
        
        case vi_resolution_720p50:
            val_resolution = ADV7611_VALUE_1280_720;
            val_framerate = ADV7611_VALUE_FREQ50HZ;
            break;

        case vi_resolution_576p50:
            val_resolution = ADV7611_VALUE_720_576;
            val_framerate = ADV7611_VALUE_FREQ50HZ;
            break;
            
        case vi_resolution_576i50:
            val_resolution = ADV7611_VALUE_720_576;
            val_framerate = ADV7611_VALUE_FREQ50HZ;
            break;

        case vi_resolution_480p60:
        case vi_resolution_480p59_94:
            val_resolution = ADV7611_VALUE_720_480;
            val_framerate = ADV7611_VALUE_FREQ60HZ;
            break;
            
        case vi_resolution_480i60:
        case vi_resolution_480i59_94:
            val_resolution = ADV7611_VALUE_720_480;
            val_framerate = ADV7611_VALUE_FREQ60HZ;
            break;
        
        default:
            return vatek_result_overrange;
    }

    if ((result = i2c_write(vi2c, IIC_ADV7611_BASE_ADDR, ADV7611_REG_VIDEO_STANDARD, val_resolution)) != vatek_result_success)
        return result;
    
    if ((result = i2c_write(vi2c, IIC_ADV7611_BASE_ADDR, ADV7611_REG_PRIMARY_MODE, val_framerate)) != vatek_result_success)
        return result;
        
    return result;
}

static audio_input_samplerate audioinfo_samplerate(uint8_t val)
{
    audio_input_samplerate samplerate = ai_samplerate_unknown;
    val &= 0x0F;

    switch (val)
    {
        case ADV7611_AUDIOSAMPLE_32000HZ:
            samplerate = ai_samplerate_32K;
            break;

        case ADV7611_AUDIOSAMPLE_44100HZ:
            samplerate = ai_samplerate_44_1K;
            break;

        case ADV7611_AUDIOSAMPLE_48000HZ:
            samplerate = ai_samplerate_48K;
            break;

        default:
            break;
    }

    return samplerate;
}

vatek_result adv7611_create(Pvatek_i2c vi2c, Padv7611_handle *hadv7611)
{
    vatek_result result = vatek_result_unknown;
    Padv7611_handle newphy = NULL; 

    if (vi2c == NULL || hadv7611 == NULL)
        return vatek_result_invalidparm;

    ADV7611_LOG("reset adv7611");
    if ((result = vatek_system_reset(sys_dev_type_phy_adv7611, vi2c->hboard)) != vatek_result_success)
        return result;
    
    vatek_system_delay(2000); // wait adv7611 reboot
    
    ADV7611_LOG("init adv7611");

    newphy = (Padv7611_handle)malloc(sizeof(adv7611_handle));
    if (newphy == NULL)
        return vatek_result_memfail;

    if ((result = initialize(vi2c)) != vatek_result_success)
        return result;

    ADV7611_LOG("init adv7611 done");

    newphy->vi2c = vi2c;    
    *hadv7611 = newphy;

    vatek_system_delay(100); // wait adv7611 initialize
    
    return result;
}

vatek_result adv7611_destroy(Padv7611_handle hadv7611)
{
    vatek_result result = vatek_result_success;
    if (hadv7611 == NULL)
        return vatek_result_invalidparm;
    
    free(hadv7611);
    hadv7611 = NULL;

    return result;
}

vatek_result adv7611_get_videoinfo(Padv7611_handle hadv7611, Pphy_video_info info)
{
    vatek_result result = vatek_result_unknown;

    if (hadv7611 == NULL || info == NULL)
        return vatek_result_invalidparm;

    if ((result = signalisdetect(hadv7611->vi2c)) != vatek_result_success)
        return vatek_result_badstatus;

    if ((result = get_videoinfo(hadv7611->vi2c, info)) != vatek_result_success)
        return result;

    if ((result = set_videoinfo(hadv7611->vi2c, info)) != vatek_result_success)
        return result;
    
    return result;
}

vatek_result adv7611_get_audioinfo(Padv7611_handle hadv7611, Pphy_audio_info info)
{
    vatek_result result = vatek_result_unknown;
    uint8_t val = 0;

    if (hadv7611 == NULL || info == NULL)
        return vatek_result_invalidparm;

    //CS_DATA[39:0] : HDMI, 0x36, 0x37, 0x38, 0x39, 0x3A
    // Readback registers for the Channel Status data bits collected from audio channel 0. 
    // Refer to the Hardware Manual for more details on the Channel Status data readbacks.
    if ((result = i2c_read(hadv7611->vi2c, IIC_ADV7611_HDMI_ADDR, 0x39, &val)) != vatek_result_success)
        return result;

    info->samplerate = audioinfo_samplerate(val);
    if (info->samplerate == ai_samplerate_unknown)
    {
        ADV7611_ERR("unsupport audio sample rate : %d", val);
        result = vatek_result_unsupport;
    }
    
    return result;
}

vatek_result adv7611_get_status(Padv7611_handle hadv7611, phy_status *status)
{
    vatek_result result = vatek_result_unknown;
    
    if (hadv7611 == NULL || status == NULL)
        return vatek_result_invalidparm;

    result = signalisdetect(hadv7611->vi2c);
    switch (result)
    {
        case vatek_result_i2cfail:
        case vatek_result_invalidparm:
            *status = phy_status_fail;
            break;

        case vatek_result_negative:
            *status = phy_status_lost;
            result = vatek_result_success;
            break;

        case vatek_result_success:
            *status = phy_status_active;
            result = vatek_result_success;
            break;

        default:
            *status = phy_status_unknown;
            break;
    }
    
    return result;
}

vatek_result adv7611_enable(Padv7611_handle hadv7611, uint8_t enable)
{
    vatek_result result = vatek_result_unknown;
    uint8_t val = 0; 

    if (hadv7611 == NULL)
        return vatek_result_invalidparm;

    
    if (enable)
        val = 0x80;
    else
        val = 0x00;

    //LLC_DLL_EN : IO, 0x19 [7]
    // A control to enable the Delay Locked Loop for output pixel clock.
    // 1 - Enable LLC DLL
    // 0 - Disable LLC DLL
    if ((result = i2c_write(hadv7611->vi2c, IIC_ADV7611_BASE_ADDR, 0x19, val)) != vatek_result_success)
        return result;

    return result;
}

#endif

