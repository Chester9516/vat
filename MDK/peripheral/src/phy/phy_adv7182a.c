
#if defined(PHY_ADV7182A)

#include "vatek_system.h"
#include "phy/phy_adv7182a_internal.h"

static vatek_result i2c_write(Pvatek_i2c vi2c, uint8_t reg, uint8_t val)
{
    uint8_t buf[2] = {0};
	vatek_result result = vatek_result_unknown;
    
    if (vi2c == NULL)
        return vatek_result_invalidparm;
    
    buf[0] = reg;
    buf[1] = val;

    if ((result = vi2c->start(vi2c->hboard, ADV7182A_SLAVE_WRITE, 0)) != vatek_result_success)
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

    if ((result = vi2c->start(vi2c->hboard, ADV7182A_SLAVE_WRITE, 0)) != vatek_result_success)
        return vatek_result_i2cfail;
    
    if ((result = vi2c->write(vi2c->hboard, &reg, 1)) != vatek_result_success)
        return vatek_result_i2cfail;

    if ((result = vi2c->start(vi2c->hboard, ADV7182A_SLAVE_READ, 1)) != vatek_result_success)
        return vatek_result_i2cfail;

    if ((result = vi2c->read(vi2c->hboard, val, 1)) != vatek_result_success)
        return vatek_result_i2cfail;

    if ((result = vi2c->stop(vi2c->hboard)) != vatek_result_success)
        return vatek_result_i2cfail;

    return result;
}

static vatek_result write_table(Pvatek_i2c vi2c, Padv7182a_regs table, uint32_t table_num)
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

        //some registers unnecessary valid value
        if (table[idx].reg == ADV7182A_STATUS3 || table[idx].reg == ADV7182A_LB_LCM)
            continue;

        if (table[idx].val != r_val)
        {
            ADV7182A_ERR("write table fail, 0x%X", table[idx].reg);
            return vatek_result_hwfail;
        }
    }

    return result;
}

static vatek_result print_identificationcode(Pvatek_i2c vi2c)
{
    vatek_result result = vatek_result_unknown;
    uint8_t val = 0;
    
    if ((result = i2c_read(vi2c, ADV7182A_IDENTIFICATION, &val)) != vatek_result_success)
        return result;

    switch(val)
    {
    	case 0x40:
    		ADV7182A_LOG("ADV7182A ID : [Prerelease silicon]"); 
            break;		
    	case 0x41:
    		ADV7182A_LOG("ADV7182A ID : [Released silicon]");
            break;
    	default:
    		ADV7182A_LOG("ADV7182A ID : [Unknown model-%x]",val);
            break;
    }
    
    return result;
}

static vatek_result get_lockstatus(Padv7182a_handle hadv7182a, adv7182a_encoding *encoding)
{
    vatek_result result = vatek_result_success;
    uint8_t status1 = 0, status2 = 0, status3 = 0;
    uint8_t val = 0;
    
    if (hadv7182a == NULL || encoding == NULL)
        return vatek_result_invalidparm;

    if ((result = i2c_read(hadv7182a->vi2c, ADV7182A_STATUS1, &status1)) != vatek_result_success)
        return result;
    if ((result = i2c_read(hadv7182a->vi2c, ADV7182A_STATUS2, &status2)) != vatek_result_success)
        return result;
    if ((result = i2c_read(hadv7182a->vi2c, ADV7182A_STATUS3, &status3)) != vatek_result_success)
        return result;

    if (status2 & ADV7182A_STATUS2_LL_NSTD)
        ADV7182A_LOG("Nonstandard line length");
    if (status2 & ADV7182A_STATUS2_FSC_NSTD)
        ADV7182A_LOG("fSC frequency nonstandard");

    *encoding = encoding_unknown;
    
    if (hadv7182a->type == si_type_cvbs && (!(status1 & ADV7182A_IS_LOCK_SIGNAL_CVBS)))
        return vatek_result_badstatus;
    else if (hadv7182a->type == si_type_ypbpr && 
        (!((status1 & ADV7182A_IS_LOCK_SIGNAL_YPBPR) && (status3 & ADV7182A_IS_LOCK_HLOCK_YPBPR))))
        return vatek_result_badstatus;

    val = (status1 & ADV7182A_STATUS1_AD_RESULT) >> 4;
    
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
            ADV7182A_LOG("ADV7182A INPUT : Unknown (%x)", val);
            result = vatek_result_unsupport;
            break;	
	}

    return result;
}


vatek_result adv7182a_create(Pvatek_i2c vi2c, adv7182a_si_type type, Padv7182a_handle *hadv7182a)
{
    vatek_result result = vatek_result_unknown;
    Padv7182a_handle newphy = NULL;
    static adv7182a_regs cvbs_phy_init[]={
        {0x52, 0xCD},   //SE_CVBS AFE IBIAS(p.s.register isn't defined in spec.)
        {0x00, 0x03},   //CVBS in on AIN4
        {0x0E, 0x80},   //ADI Required Write. Enter Main register
        {0x9C, 0x00},   //Reset Current Clamp Circuitry [step1]
        {0x9C, 0xFF},   //Reset Current Clamp Circuitry [step2]                                                 
        {0x0E, 0x00},   //Enter User Sub Map
        {0x0E, 0x80},   //ADI Required Write [Fast Switch]
        {0xD9, 0x44},   //ADI Required Write [Fast Switch]
        {0x0E, 0x40},   //Enter User Sub Map 2 [Fast Switch]
        {0xE0, 0x01},   //Enable Fast Switch Mode [Fast Switch]
        {0x0E, 0x00},   //Enter User Sub Map
        {0x17, 0x41},   //Enable SH1
        {0x03, 0x0C},   //Enable Pixel & Sync output drivers
        {0x04, 0x07},   //Power-up INTRQ, HS & VS pads
        {0x13, 0x00},   //Enable ADV7182A for 28_63636MHz crystal 
        {0x1D, 0x40},   //Enable LLC output driver [ADV7182A writes finished]
    };
    static adv7182a_regs cvbs_sys_init[]={       
        {ADV7182A_ADIControl1,          ADV7182A_REG_MAP_MAIN},     //Enter Main Map
        {ADV7182A_AFE_CNT1,             ADV7182A_AFE_CNT1_MAN_OVR|ADV7182A_AFE_CNT1_FILTER4},   //default antialiasing Filter Ain4
        {ADV7182A_EXT_OUTPUT_CONTROL,   0xB0},      //BT656-4 / HS, VS, FIELD forced active or tristated by TOD 
        {ADV7182A_V_F_Control1,         0x02},      //EAV/SAV codes generated to suit Analog Devices encoders
        {ADV7182A_VIDEO_SELECT2,        0x04},      //AutoDetect Video Standard 
        {ADV7182A_OUTPUT_CONTROL,       0x4C},      //TOD bit low : output pins three-stated ; 8-bit interleaved data output
        {ADV7182A_ADI_CNT2,             0xC0},      //LLC pin tristated
    }; 
    static adv7182a_regs ypbpr_phy_init[]={
        {0x54, 0xc0},   //YPbPr AFE IBIAS
        {0x00, 0x0c},   //INSEL = YPbPr, Y on AIn1, Pb on Ain2, Pr on Ain3
        {0x0e, 0x80},   //ADI Required Write
        {0x9c, 0x00},   //Reset Current Clamp Circuitry [step1] 
        {0x9c, 0xff},   //Reset Current Clamp Circuitry [step2] 
        {0x0e, 0x00},   //Enter User Sub Map
        {0x03, 0x0c},   //Enable Pixel & Sync output drivers
        {0x04, 0x07},   //Power-up INTRQ pad, Enable SFL & VS pin
        {0x13, 0x00},   //Enable ADV7182A for 28_63636MHz crystal 
        {0x1d, 0x40},   //Enable LLC output driver [All ADV7182A Writes Finished]
    };  
    static adv7182a_regs ypbpr_sys_init[]={       
        {ADV7182A_ADIControl1,          ADV7182A_REG_MAP_MAIN}, //Enter Main Map
        {ADV7182A_AFE_CNT1,             ADV7182A_AFE_CNT1_MAN_OVR|ADV7182A_AFE_CNT1_FILTER1|ADV7182A_AFE_CNT1_FILTER2|ADV7182A_AFE_CNT1_FILTER3},   //default antialiasing Filter Ain1-3
        {ADV7182A_EXT_OUTPUT_CONTROL,   0xB0},      //BT656-4 / HS, VS, FIELD forced active or tristated by TOD
        {ADV7182A_V_F_Control1,         0x02},      //EAV/SAV codes generated to suit Analog Devices encoders
        {ADV7182A_VIDEO_SELECT2,        0x04},      //AutoDetect Video Standard 
        {ADV7182A_OUTPUT_CONTROL,       0x4C},      //TOD bit low : output pins three-stated ; 8-bit interleaved data output
        {ADV7182A_ADI_CNT2,             0xC0},      //LLC pin tristated     
    };
    Padv7182a_regs phy_init = NULL, sys_init = NULL;
    uint8_t phy_size = 0, sys_size = 0;
    system_device_type dev_type = sys_dev_type_phy_adv7182a_cvbs;
    
    if (vi2c == NULL || hadv7182a == NULL)
        return vatek_result_invalidparm;

    if (type < si_type_min || type > si_type_max)
        return vatek_result_overrange;
    
    newphy = (Padv7182a_handle)malloc(sizeof(adv7182a_handle));
    if (newphy == NULL)
        return vatek_result_memfail;

    switch (type)
    {
        case si_type_ypbpr:
            phy_init = ypbpr_phy_init;
            phy_size = sizeof(ypbpr_phy_init)/sizeof(adv7182a_regs);
            sys_init = ypbpr_sys_init;
            sys_size = sizeof(ypbpr_sys_init)/sizeof(adv7182a_regs);
            dev_type = sys_dev_type_phy_adv7182a_ypbpr;
            break;
                
        case si_type_cvbs:
        default:
            phy_init = cvbs_phy_init;
            phy_size = sizeof(cvbs_phy_init)/sizeof(adv7182a_regs);
            sys_init = cvbs_sys_init;
            sys_size = sizeof(cvbs_sys_init)/sizeof(adv7182a_regs);
            dev_type = sys_dev_type_phy_adv7182a_cvbs;
            break;
    }
    
    ADV7182A_LOG("reset adv7182");
    if ((result = vatek_system_reset(dev_type, vi2c->hboard)) != vatek_result_success)
        return result;
    
    vatek_system_delay(1000); // wait adv7182 reboot

    ADV7182A_LOG("init adv7182");

    //Reset ADV7182
    if ((result = i2c_write(vi2c, ADV7182A_POWER_MANAGEMENT, ADV7182A_POWER_MANAGEMENT_RST)) != vatek_result_success)
        return result;
    vatek_system_delay(10);
    if ((result = i2c_write(vi2c, ADV7182A_POWER_MANAGEMENT, 0)) != vatek_result_success)
        return result;
    vatek_system_delay(10);

    if ((result = write_table(vi2c, phy_init, phy_size)) != vatek_result_success)
    {
        ADV7182A_ERR("Initial official script unsuccessfully, %d", result);
        return result;
    }

    if ((result = print_identificationcode(vi2c)) != vatek_result_success)
    {
        ADV7182A_ERR("Couldn't identify device, %d", result);
        return result;
    }

    if ((result = write_table(vi2c, sys_init, sys_size)) != vatek_result_success)
        return result;

    newphy->vi2c = vi2c;
    newphy->type = type;
    *hadv7182a = newphy;

    vatek_system_delay(1000); // wait adv7182 intialize
    
    return result;
}

vatek_result adv7182a_destroy(Padv7182a_handle hadv7182a)
{
    vatek_result result = vatek_result_success;
    if (hadv7182a == NULL)
        return vatek_result_invalidparm;
    
    free(hadv7182a);
    hadv7182a = NULL;

    return result;
}

vatek_result adv7182a_enable(Padv7182a_handle hadv7182a, uint8_t enable)
{
    vatek_result result = vatek_result_unknown;
    uint8_t output_ctrl = 0, adi_cnt2 = 0;

    if (hadv7182a == NULL)
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

    if ((result = i2c_write(hadv7182a->vi2c, ADV7182A_OUTPUT_CONTROL, output_ctrl)) != vatek_result_success)
        return result;
        
    if ((result = i2c_write(hadv7182a->vi2c, ADV7182A_ADI_CNT2, adi_cnt2)) != vatek_result_success)
        return result;

    vatek_system_delay(50); //waiting adv7182 enable
    
    return result;
}

vatek_result adv7182a_get_videoinfo(Padv7182a_handle hadv7182a, Pphy_video_info info)
{
    vatek_result result = vatek_result_unknown;
    adv7182a_encoding encoding = encoding_unknown;
    uint8_t status3 = 0;
    uint8_t isinterlace = 0;
    
    if (hadv7182a == NULL || info == NULL)
        return vatek_result_invalidparm;

    info->resolution = vi_resolution_unknown;
    info->aspectrate = vi_aspectrate_16_9;
    info->separated_sync = 0;
    info->buswidth_16 = 0;
    
    if((result = get_lockstatus(hadv7182a, &encoding)) != vatek_result_success)
        return result;

    if (encoding == encoding_unknown)
        return vatek_result_unknown;

    if ((result = i2c_read(hadv7182a->vi2c, ADV7182A_STATUS3, &status3)) != vatek_result_success)
        return result;

    if (status3 & ADV7182A_STATUS3_Interlace)
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

vatek_result adv7182a_get_audioinfo(Padv7182a_handle hadv7182a, Pphy_audio_info info)
{
    vatek_result result = vatek_result_success;

    if (hadv7182a == NULL || info == NULL)
        return vatek_result_invalidparm;

    //adv7182a not support audio decode
    info->samplerate = ai_samplerate_48K;
    
    return result;
}

vatek_result adv7182a_get_status(Padv7182a_handle hadv7182a, phy_status *status)
{
    vatek_result result = vatek_result_success;
    adv7182a_encoding encoding;
    uint8_t status1 = 0; 
        
    if (hadv7182a == NULL || status == NULL)
        return vatek_result_invalidparm;

    if ((result = i2c_read(hadv7182a->vi2c, ADV7182A_STATUS1, &status1)) != vatek_result_success)
        return result;

    if (status1 & ADV7182A_STATUS1_LOST_LOCK)
    {
        *status = phy_status_lost;
        return vatek_result_success;
    }

    *status = phy_status_unknown;
    result = get_lockstatus(hadv7182a, &encoding);
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

static vatek_result ccdata_isdetect(Pvatek_i2c vi2c)
{
    vatek_result result = vatek_result_unknown;
    uint8_t status_int = 0, status_vdp = 0;

    if (vi2c == NULL)
        return vatek_result_invalidparm;

    if ((result = i2c_write(vi2c, ADV7182A_ADIControl1, ADV7182A_REG_MAP_USER_SUB1_INT_VDP)) != vatek_result_success)
        return result;

    if ((result = i2c_write(vi2c, ADV7182A_INT_Mask4, ADV7182A_INT_Mask4_CCAPD_Mask)) != vatek_result_success)
        return result;
    
    if ((result = i2c_read(vi2c, ADV7182A_INT_Status4, &status_int)) != vatek_result_success)
        return result;
    
    if ((result = i2c_read(vi2c, ADV7182A_VDP_STATUS, &status_vdp)) != vatek_result_success)
        return result;

    if (!(status_int & ADV7182A_INT_Status4_CCAPD_Q))
        return vatek_result_negative;

    if (!(status_vdp & ADV7182A_VDP_STATUS_CC_AVL))
        return vatek_result_negative;

    //clear interrupt pin - INTRQ
    if ((result = i2c_write(vi2c, ADV7182A_INT_Clear4, ADV7182A_INT_Status4_CCAPD_CLR)) != vatek_result_success)
        return result;
    
    return result;
}

vatek_result adv7182a_get_ccdata(Padv7182a_handle hadv7182a, Puserdata_cc_parm cc_parm)
{
    vatek_result result = vatek_result_success;
    uint8_t data_0 = 0, data_1 = 0, status = 0;

    if (hadv7182a == NULL || cc_parm == NULL)
        return vatek_result_invalidparm;

    if ((result = ccdata_isdetect(hadv7182a->vi2c)) != vatek_result_success)
        return result;

    if ((result = i2c_write(hadv7182a->vi2c, ADV7182A_ADIControl1, ADV7182A_REG_MAP_USER_SUB1_INT_VDP)) != vatek_result_success)
        return result;

    if ((result = i2c_read(hadv7182a->vi2c, ADV7182A_VDP_DATA0, &data_0)) != vatek_result_success)
        return result;
    
    if ((result = i2c_read(hadv7182a->vi2c, ADV7182A_VDP_DATA1, &data_1)) != vatek_result_success)
        return result;

    if ((result = i2c_read(hadv7182a->vi2c, ADV7182A_VDP_STATUS, &status)) != vatek_result_success)
        return result;

    if (status & ADV7182A_VDP_STATUS_CC_FIELD)
        cc_parm->attr = 0x01;
    else
        cc_parm->attr = 0x00;
    
    cc_parm->ccdata1 = data_0;
    cc_parm->ccdata2 = data_1;


    if ((result = i2c_write(hadv7182a->vi2c, ADV7182A_VDP_STATUS, ADV7182A_VDP_STATUS_CC_CLEAR)) != vatek_result_success)
        return result;
    if ((result = i2c_write(hadv7182a->vi2c, ADV7182A_ADIControl1, ADV7182A_REG_MAP_MAIN)) != vatek_result_success)
        return result;

    return result;
}

#endif

