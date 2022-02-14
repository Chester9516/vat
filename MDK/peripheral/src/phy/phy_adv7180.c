
#if defined(PHY_ADV7180)

#include "vatek_system.h"
#include "phy/phy_adv7180_internal.h"


typedef struct _adv7180_handle
{
    Pvatek_i2c vi2c;
}adv7180_handle,*Padv7180_handle;

typedef struct _adv7180_regs
{
    uint8_t reg;
    uint8_t val;
}adv7180_regs,*Padv7180_regs;

typedef enum _adv7180_encoding
{
    encoding_unknown = 0,
    encoding_ntsc_m = 1,
    encoding_pal_bdghi = 2,
    encoding_secam = 3,
    encoding_ntsc_4_3 = 4,
    encoding_pal_m = 5,
    encoding_pal_cn = 6,
    encoding_pal_60 = 7,
    encoding_secm_525 = 8,
}adv7180_encoding;


#define ADV7180_ERR(fmt,...)                VATEK_ERR(adv7180,fmt,##__VA_ARGS__)
#define ADV7180_LOG(fmt,...)                VATEK_LOG(adv7180,fmt,##__VA_ARGS__)

static vatek_result i2c_write(Pvatek_i2c vi2c, uint8_t reg, uint8_t val)
{
    uint8_t buf[2] = {0};
	vatek_result result = vatek_result_unknown;
    
    if (vi2c == NULL)
        return vatek_result_invalidparm;
    
    buf[0] = reg;
    buf[1] = val;

    if ((result = vi2c->start(vi2c->hboard, ADV7180_SLAVE_WRITE, 0)) != vatek_result_success)
        return vatek_result_i2cfail;

    if ((result = vi2c->write(vi2c->hboard, &buf[0], 2)) != vatek_result_success)
        return vatek_result_i2cfail;

    if ((result = vi2c->stop(vi2c->hboard)) != vatek_result_success)
        return vatek_result_i2cfail;

    return result;
}

static vatek_result i2c_read(Pvatek_i2c vi2c, uint8_t reg, uint8_t* val)
{
    vatek_result result = vatek_result_unknown;
    
    if (vi2c == NULL || val == NULL)
        return vatek_result_invalidparm;

    if ((result = vi2c->start(vi2c->hboard, ADV7180_SLAVE_WRITE, 0)) != vatek_result_success)
        return vatek_result_i2cfail;
    
    if ((result = vi2c->write(vi2c->hboard, &reg, 1)) != vatek_result_success)
        return vatek_result_i2cfail;

    if ((result = vi2c->start(vi2c->hboard, ADV7180_SLAVE_READ, 1)) != vatek_result_success)
        return vatek_result_i2cfail;

    if ((result = vi2c->read(vi2c->hboard, val, 1)) != vatek_result_success)
        return vatek_result_i2cfail;

    if ((result = vi2c->stop(vi2c->hboard)) != vatek_result_success)
        return vatek_result_i2cfail;

    return result;
}

static vatek_result write_table(Pvatek_i2c vi2c, Padv7180_regs table, uint32_t table_num)
{
    vatek_result result = vatek_result_unknown;
    uint32_t idx = 0;
    uint8_t r_val = 0;
    
    if (vi2c == NULL || table == NULL || table_num == 0)
        return vatek_result_invalidparm;

    for (idx = 0; idx < table_num; idx++)
    {
        if ((result = i2c_write(vi2c, table[idx].reg, table[idx].val)) != vatek_result_success)
            return result;

        if ((result = i2c_read(vi2c, table[idx].reg, &r_val)) != vatek_result_success)
            return result;

        if (table[idx].val != r_val)
        {
            ADV7180_ERR("write table fail, 0x%X[0x%X]", table[idx].reg, table[idx].val);
            return vatek_result_hwfail;
        }
    }

    return result;
}

static vatek_result initialize(Pvatek_i2c vi2c)
{
    vatek_result result = vatek_result_unknown;
    static adv7180_regs cvbs_init[] = {
        {0x0F, 0x00,},  //power up
        {0x00, 0x10,},  //INSEL = CVBS in with pedestal
        {0xC4, 0x80,},  //ADC Switch
        {0xC3, 0x01,},  //on A IN 1
        {0x03, 0x0C,},  //AV codes to suit 8-bit interleaved data output
                        //8-bit LLC1 4:2:2 ITU-R BT.656
        {0x04, 0x77,},  //Enable SFL ITU-R BT.656
        {0x17, 0x41,},  //Select SH1
        {0x31, 0x02,},  //Clear NEWAV_MODE, SAV/EAV to suit ADV video encoders
        {0x3A, 0x17,},  
        {0x3D, 0xA2,},  //MWE enable manual window, color kill threshold to 2
        {0x3E, 0x6A,},  //BLM optimization
        {0x3F, 0xA0,},  //BGB optimization
        {0x0E, 0x80,},  //Hidden space
        {0x55, 0x81,},  //ADC configuration
        {0x0E, 0x00,},  //User space
    };
    uint32_t table_num = 0;

    if (vi2c == NULL)
        return vatek_result_invalidparm;

    table_num = sizeof(cvbs_init) / sizeof(adv7180_regs);
    if ((result = write_table(vi2c, cvbs_init, table_num)) != vatek_result_success)
        return result;

    //enable interrupt map
    if ((result = i2c_write(vi2c, ADV7180_REG_ADI_CONTROL1, ADV7180_VALUE_SUB_USR_EN)) != vatek_result_success)
        return result;

    if ((result = i2c_write(vi2c, ADV7180_REG_INTERRUPT_Mask4, ADV7180_VALUE_INTERRUPT_Mask4)) != vatek_result_success)
        return result;
        
    return result;
}

static vatek_result get_lockstatus(Padv7180_handle hadv7180, adv7180_encoding *encoding)
{
    vatek_result result = vatek_result_success;
    uint8_t status1 = 0, status3 = 0;
    uint8_t val = 0;
    
    if (hadv7180 == NULL || encoding == NULL)
        return vatek_result_invalidparm;

    if ((result = i2c_read(hadv7180->vi2c, ADV7180_REG_IDENT, &val)) != vatek_result_success)
        return result; 
    if (val != ADV7180_VALUE_IDENT)
    {
        ADV7180_ERR("can't found adv7180");
        return vatek_result_hwfail;     
    }

    if ((result = i2c_read(hadv7180->vi2c, ADV7180_REG_STATUS1, &status1)) != vatek_result_success)
        return result;
    if ((result = i2c_read(hadv7180->vi2c, ADV7180_REG_STATUS3, &status3)) != vatek_result_success)
        return result;

    *encoding = encoding_unknown;

    //CVBS signal detected
    if (!(status3 & 0x08))
    {
        ADV7180_LOG("cable not detected");
        return vatek_result_badstatus;
    }

    //CVBS locked
    if (!(status1 & ADV7180_STATUS1_IN_LOCK))
    {
        ADV7180_LOG("cvbs not locked");
        return vatek_result_badstatus;
    }

    val = (status1 & 0x70) >> 4;

    switch(val)
    {   
        case 0x00:
            *encoding = encoding_ntsc_m;  
            break;
        
        case 0x01:
            *encoding = encoding_ntsc_4_3;  
            break;
        
        case 0x02:
            *encoding = encoding_pal_m;
            break;
        
        case 0x03:
            *encoding = encoding_pal_60;
            break;
        
        case 0x04:
            *encoding = encoding_pal_bdghi;
            break;
        
        case 0x05:
            *encoding = encoding_secam;
            break;
        
        case 0x06:
            *encoding = encoding_pal_cn;
            break;
        
        case 0x07:
            *encoding = encoding_secm_525;
            break;
        
        default:
            *encoding = encoding_unknown;
            ADV7180_LOG("ADV7180 INPUT : Unknown (%x)", val);
            result = vatek_result_unsupport;
            break;  
    }

    return result;
}

vatek_result adv7180_create(Pvatek_i2c vi2c, Padv7180_handle *hadv7180)
{
    vatek_result result = vatek_result_unknown;
    Padv7180_handle newphy = NULL;

    if (vi2c == NULL || hadv7180 == NULL)
        return vatek_result_invalidparm;

    ADV7180_LOG("reset adv7180");
    if ((result = vatek_system_reset(sys_dev_type_phy_adv7180, vi2c->hboard)) != vatek_result_success)
        return result;
    
    vatek_system_delay(1000); // wait adv7180 reboot

    ADV7180_LOG("init adv7180");

    newphy = (Padv7180_handle)malloc(sizeof(adv7180_handle));
    if (newphy == NULL)
        return vatek_result_memfail;
    
    if ((result = initialize(vi2c)) != vatek_result_success)
    {
        free(newphy);
        return result;
    }
    ADV7180_LOG("init adv7180 done");
    
    newphy->vi2c = vi2c;
    *hadv7180 = newphy;

    vatek_system_delay(100); // wait adv7180 intialize
    
    return result;
}

vatek_result adv7180_destroy(Padv7180_handle hadv7180)
{
    vatek_result result = vatek_result_success;
    if (hadv7180 == NULL)
        return vatek_result_invalidparm;
    
    free(hadv7180);
    hadv7180 = NULL;

    return result;
}

vatek_result adv7180_enable(Padv7180_handle hadv7180, uint8_t enable)
{
    vatek_result result = vatek_result_unknown;
    uint8_t output_ctrl = 0, adi_cnt2 = 0;

    if (hadv7180 == NULL)
        return vatek_result_invalidparm;

    if (enable)
    {
        output_ctrl = 0x0C;     //pixel outputs, HS and VS/FIELD/SFL enable
        adi_cnt2 = 0x40;        //LLC pin active
    }
    else
    {
        output_ctrl = 0x4C;         //pixel outputs, HS and VS/FIELD/SFL tristated  
        adi_cnt2 = 0xC0;            //LLC pin tristated
    }

    if ((result = i2c_write(hadv7180->vi2c, ADV7180_OUTPUT_CONTROL, output_ctrl)) != vatek_result_success)
        return result;
        
    if ((result = i2c_write(hadv7180->vi2c, ADV7180_ADI_CNT2, adi_cnt2)) != vatek_result_success)
        return result;

    return result;

}

vatek_result adv7180_get_videoinfo(Padv7180_handle hadv7180, Pphy_video_info info)
{
    vatek_result result = vatek_result_unknown;
    adv7180_encoding encoding = encoding_unknown;
    uint8_t status3 = 0;
    uint8_t isinterlace = 0;
    
    if (hadv7180 == NULL || info == NULL)
        return vatek_result_invalidparm;

    info->resolution = vi_resolution_unknown;
    info->aspectrate = vi_aspectrate_16_9;
    info->separated_sync = 0;
    info->buswidth_16 = 0;
    
    if((result = get_lockstatus(hadv7180, &encoding)) != vatek_result_success)
        return result;

    if (encoding == encoding_unknown)
        return vatek_result_unknown;

    if ((result = i2c_read(hadv7180->vi2c, ADV7180_REG_STATUS3, &status3)) != vatek_result_success)
        return result;

    if (status3 & ADV7180_STATUS3_Interlace)
        isinterlace = 1;

    switch (encoding)
    {
        case encoding_ntsc_m:
            if (isinterlace)
                info->resolution = vi_resolution_480i59_94;
            else
                info->resolution = vi_resolution_480p29_97;
            break;

        case encoding_pal_bdghi:
        case encoding_secam:
        case encoding_pal_cn:
            if (isinterlace)
                info->resolution = vi_resolution_576i50;
            else
                info->resolution = vi_resolution_576p25;
            break;

        case encoding_ntsc_4_3:
        case encoding_pal_m:
        case encoding_pal_60:
        case encoding_secm_525:
            if (isinterlace)
                info->resolution = vi_resolution_480i60;
            else
                info->resolution = vi_resolution_480p30;
            break;

        default:
            break;
    }
    
    return result;
}

vatek_result adv7180_get_audioinfo(Padv7180_handle hadv7180, Pphy_audio_info info)
{
    vatek_result result = vatek_result_success;

    if (hadv7180 == NULL || info == NULL)
        return vatek_result_invalidparm;

    //adv7180 not support audio decode
    info->samplerate = ai_samplerate_48K;
    
    return result;
}

vatek_result adv7180_get_status(Padv7180_handle hadv7180, phy_status *status)
{
    vatek_result result = vatek_result_success;
    adv7180_encoding encoding;
    uint8_t status1 = 0; 
        
    if (hadv7180 == NULL || status == NULL)
        return vatek_result_invalidparm;

    if ((result = i2c_read(hadv7180->vi2c, ADV7180_REG_STATUS1, &status1)) != vatek_result_success)
        return result;

    if (status1 & ADV7180_STATUS1_LOST_LOCK)
    {
        *status = phy_status_lost;
        return vatek_result_success;
    }

    *status = phy_status_unknown;
    result = get_lockstatus(hadv7180, &encoding);
    switch (result)
    {
        case vatek_result_badstatus:
            *status = phy_status_lost;
            break;

        case vatek_result_success:
            if (encoding != encoding_unknown)
                *status = phy_status_active;
            break;

        default:
            *status = phy_status_fail;
            break;
    }
    
    return vatek_result_success;
}

vatek_result adv7180_get_ccdata(Padv7180_handle hadv7180, Puserdata_cc_parm cc_parm)
{
    vatek_result result = vatek_result_success;
    uint8_t status = 0;
    uint8_t data_0 = 0, data_1 = 0;

    if (hadv7180 == NULL || cc_parm == NULL)
        return vatek_result_invalidparm;

    if ((result = i2c_read(hadv7180->vi2c, ADV7180_REG_VDP_STATUS, &status)) != vatek_result_success)
        return result;

    if (!(status & ADV7180_VALUE_CCAP_DETECT))
        return vatek_result_negative;

    if ((result = i2c_read(hadv7180->vi2c, ADV7180_REG_CCAP_DATA0, &data_0)) != vatek_result_success)
        return result;
    
    if ((result = i2c_read(hadv7180->vi2c, ADV7180_REG_CCAP_DATA1, &data_1)) != vatek_result_success)
        return result;

    if (status & ADV7180_VALUE_CCAP_FIELD)
        cc_parm->attr = 0x01;
    else
        cc_parm->attr = 0x00;

    cc_parm->ccdata1 = data_0;
    cc_parm->ccdata2 = data_1;

    if ((result = i2c_write(hadv7180->vi2c, ADV7180_REG_VDP_STATUS, 0x01)) != vatek_result_success)
        return result;
    
    return result;
}

#endif

