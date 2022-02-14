
#if defined(RF_R2)

#include "vatek_system.h"
#include "rf/rf_r2_internal.h"

Pr2_handle g_r2_handle = NULL;

const r2_calibration r2_calibration_table[] = 
{
	{50,85,0x0063,0x10,},
	{86,141,0x0033,0x04,},
	{142,250,0x0023,0x12,},
	{251,386,0x0013,0x16,},
	{387,425,0x0023,0x16,},
	{426,506,0x0013,0x11,},
	{507,578,0x0023,0x11,},
	{579,632,0x0033,0x11,},
	{633,690,0x0043,0x11,},
	{691,806,0x0033,0x15,},
	{807,1000,0x0043,0x15,},
	{0,0,0,0,},
};

static vatek_result gpio_write(Pvatek_gpio vgpio, gpio_pin pin, uint8_t val)
{
    vatek_result result = vatek_result_unknown;

    if (vgpio == NULL)
        return vatek_result_invalidparm;

    if ((result = vgpio->write(pin, val)) != vatek_result_success)
        return vatek_result_gpiofail;
    
    return result;
}

static uint8_t gpio_read(Pvatek_gpio vgpio, gpio_pin pin)
{
    uint8_t val = 0;

    if (vgpio == NULL)
        return 0;

    if (vgpio->read(pin, &val) != vatek_result_success)
        return 0;
    
    return val;
}

#define RF_R2_CS_LO     gpio_write(g_r2_handle->vgpio, g_r2_handle->pin->r2_cs, 0)
#define RF_R2_CS_HI     gpio_write(g_r2_handle->vgpio, g_r2_handle->pin->r2_cs, 1)
#define RF_R2_CLK_LO    gpio_write(g_r2_handle->vgpio, g_r2_handle->pin->r2_clk, 0)
#define RF_R2_CLK_HI    gpio_write(g_r2_handle->vgpio, g_r2_handle->pin->r2_clk, 1)
#define RF_R2_MOSI_LO   gpio_write(g_r2_handle->vgpio, g_r2_handle->pin->r2_mosi, 0)
#define RF_R2_MOSI_HI   gpio_write(g_r2_handle->vgpio, g_r2_handle->pin->r2_mosi, 1)
#define RF_R2_RD_DATA   gpio_read(g_r2_handle->vgpio, g_r2_handle->pin->r2_miso)

static void isp_delay(uint32_t ndelay)
{
    ndelay *= 10;
    while(ndelay)
    {
        ndelay--;
    }
}

static uint16_t isp_read(uint8_t addr)
{
    uint16_t nresvalue = 0;
	uint8_t addr_bit = 0x80;			//A7
    int i = 0;
    
    //R2_LOG("isp_read addr : 0x%02X",addr);

	//RF_R2_MISO_HI;
	RF_R2_MOSI_HI;
    RF_R2_CS_HI;
    isp_delay(70);
    RF_R2_CS_LO;
    
	/* Send READ command */
    for(i = 0;i < 8;i++)
    {
		if(i == 0)
			RF_R2_MOSI_HI;			//READ command
        else if(addr & addr_bit)RF_R2_MOSI_HI;
        else RF_R2_MOSI_LO;
            
        isp_delay(5);
        RF_R2_CLK_HI;
        isp_delay(20);
        RF_R2_CLK_LO;
        if(addr_bit >= 0x02)
			addr_bit >>= 1;
    }
    
    isp_delay(100);   
    /* Start to read Data */
    for(i = 16;i > 0;i--)
    {
        RF_R2_CLK_HI;
        isp_delay(10);
        if(RF_R2_RD_DATA)nresvalue |= (1 << (i-1));
        isp_delay(10);
        RF_R2_CLK_LO;
        isp_delay(5);  

		if(i == 9 || i == 1)
			isp_delay(100);
    }
    
	RF_R2_CS_HI;
    isp_delay(100);
    RF_R2_CS_LO;
    
	//R2_LOG("[0x%02X] : 0x%04X",addr, nresvalue);
    return nresvalue;
}

static vatek_result isp_write(uint8_t addr, uint16_t value)
{
	uint16_t check_value = 0;
	uint16_t value_bit = 0x8000;	//D15
	uint8_t addr_bit = 0x80;			//A7
    int i = 0;
  
    //R2_LOG("isp_write addr : [0x%02X] = 0x%04X",addr, value);

	//RF_R2_MOSI_HI;
    RF_R2_CS_HI;
    isp_delay(70);		//delay at least 100ns
    RF_R2_CS_LO;
    
	/* Send READ command */
    for(i = 0;i < 8;i++)
    {
		if(i == 0)
			RF_R2_MOSI_LO;			//WRITE command
        else if(addr & addr_bit)RF_R2_MOSI_HI;
        else RF_R2_MOSI_LO;
            
        isp_delay(5);
        RF_R2_CLK_HI;
        isp_delay(20);
        RF_R2_CLK_LO;
        if(addr_bit >= 0x02)
					addr_bit >>= 1;
    }
		
	isp_delay(100);
	/* Start to write Data */
    for(i = 16;i > 0;i--)
    {
        RF_R2_CLK_HI;
        isp_delay(10);
		if(value & value_bit) RF_R2_MOSI_HI;
			else RF_R2_MOSI_LO;
        isp_delay(10);
        RF_R2_CLK_LO;
        isp_delay(5); 
				
		if(value_bit >= 0x02)
			value_bit >>= 1;
				
		if(i == 9 || i == 1)
			isp_delay(100);
    }
    
	RF_R2_CS_HI;
    isp_delay(100);
    RF_R2_CS_LO;

	check_value = isp_read(addr);
	if(check_value != value && addr != R2_REG7_SOFT_RESET){
		R2_ERR("isp_write error!! [0x%02X] : 0x%04X",addr, check_value);
		return vatek_result_gpiofail;
	}
    isp_delay(100);
	return vatek_result_success;
}

static vatek_result _r2_default_setting(void)
{
    //RF_R2_VCO_DefalutSetting
    
	vatek_result result = vatek_result_success;
	uint16_t regValue = 0;

    //soft reset
    if ((result = isp_write(R2_REG7_SOFT_RESET, 0x5253)) != vatek_result_success)
        return result;

    if ((result = isp_write(0x00, 0x3000)) != vatek_result_success)
        return result;
    if ((result = isp_write(0x01, 0x3041)) != vatek_result_success)
        return result;
		
	//enble block
    if ((result = isp_write(0x00, 0x3127)) != vatek_result_success)
        return result;
    if ((result = isp_write(0x01, 0xFFF1)) != vatek_result_success)
        return result;
		
	//PA and LPF
    if ((result = isp_write(0x04, 0x0363)) != vatek_result_success)
        return result;
    if ((result = isp_write(0x06, 0x0001)) != vatek_result_success)
        return result;
		
	// pass word
    if ((result = isp_write(0x0F, 0x5344)) != vatek_result_success)
        return result;
    if ((result = isp_write(0x12, 0x4341)) != vatek_result_success)
        return result;
    if ((result = isp_write(0x1D, 0x4956)) != vatek_result_success)
        return result;
    if ((result = isp_write(0x28, 0x5356)) != vatek_result_success)
        return result;
    if ((result = isp_write(0x32, 0x4447)) != vatek_result_success)
        return result;
		
	// LO Leakage, calibrated by external tool "Auto Calibration"
	/*
    if ((result = RF_R2_WRITE(0x13, 0x0000)) != vatek_result_success)
        return result;
    if ((result = RF_R2_WRITE(0x14, 0x0000)) != vatek_result_success)
        return result;
    */
		
	// Image, calibrated by external tool "Auto Calibration"
	/*
    if ((result = RF_R2_WRITE(0x15, 0x0000)) != vatek_result_success)
        return result;
    if ((result = RF_R2_WRITE(0x16, 0x0000)) != vatek_result_success)
        return result;
    */
    if ((result = isp_write(0x2D, 0x0020)) != vatek_result_success)
        return result;

	// IB
    if ((result = isp_write(0x20, 0x5538)) != vatek_result_success)
        return result;
    if ((result = isp_write(0x21, 0x1131)) != vatek_result_success)
        return result;

	//VCO
    if ((result = isp_write(0x2A, 0x0082)) != vatek_result_success)
        return result;
    if ((result = isp_write(0x2B, 0x5213)) != vatek_result_success)
        return result;
    if ((result = isp_write(0x2C, 0x1322)) != vatek_result_success)
        return result;
    if ((result = isp_write(0x2E, 0x8678)) != vatek_result_success)
        return result;
    if ((result = isp_write(0x2F, 0x9B91)) != vatek_result_success)
        return result;
    if ((result = isp_write(0x30, 0xB4AA)) != vatek_result_success)
        return result;
    if ((result = isp_write(0x31, 0xD2CE)) != vatek_result_success)
        return result;
		
	// LO Frequency
    if ((result = isp_write(0x02, 0x0000)) != vatek_result_success)
        return result;
    if ((result = isp_write(0x03, 0x26F0)) != vatek_result_success)   //623MhZ
        return result;

	//Auto Calibration
    if ((result = isp_write(0x00, 0x3107)) != vatek_result_success)   //reset auto-calibration lock
        return result;
    if ((result = isp_write(0x00, 0x3F07)) != vatek_result_success)
        return result;
		
	// SDM 1-1-1
    if ((result = isp_write(0x11, 0x0000)) != vatek_result_success)
        return result;
    if ((result = isp_write(0x10, 0x0015)) != vatek_result_success)
        return result;
    vatek_system_delay(100);    //delay 100ms
    if ((result = isp_write(0x10, 0x0115)) != vatek_result_success)
        return result;
		
	//Auto Calibration done
    if ((result = isp_write(0x00, 0x3D07)) != vatek_result_success)
        return result;

	// pass word lock
    if ((result = isp_write(0x0F, 0x0000)) != vatek_result_success)
        return result;
    if ((result = isp_write(0x12, 0x0000)) != vatek_result_success)
        return result;
    if ((result = isp_write(0x1D, 0x0000)) != vatek_result_success)
        return result;
    if ((result = isp_write(0x28, 0x0000)) != vatek_result_success)
        return result;
    if ((result = isp_write(0x32, 0x0000)) != vatek_result_success)
        return result;
    
	//Check R2 status
    regValue = isp_read(R2_REG0E_RF_STATUS);
	if(regValue == 0x1F || regValue == 0x00){
		R2_ERR("r2 status error [%x]=%x",R2_REG0E_RF_STATUS, regValue);	
		result = vatek_result_hwfail;
	}	
    
	return result;
}

static vatek_result _r2_signal_control(uint8_t flag)
{
	vatek_result result = vatek_result_success;
    
    if (flag >= 1)
        flag = 1;

	//I / Q signal
    if ((result = isp_write(R2_REG6_DATA_SWAP, flag)) != vatek_result_success)
        return result;

	R2_LOG("signal ctrl : 0x%x", isp_read(R2_REG6_DATA_SWAP));	
	return result;
}

static uint16_t _r2_get_calibration_pagain(uint32_t freq)
{
    int32_t i = 0;
    while(r2_calibration_table[i].min_freq != 0)
    {
        if(freq >= r2_calibration_table[i].min_freq  && 
           freq <= r2_calibration_table[i].max_freq)
        {
            return r2_calibration_table[i].pagain;
        }
        i++;
    }
    
    return 0x1302;
}

uint16_t _r2_get_calibration_gpio(uint32_t freq)
{
    int32_t i = 0;
    while(r2_calibration_table[i].min_freq != 0)
    {
        if(freq >= r2_calibration_table[i].min_freq  && 
           freq <= r2_calibration_table[i].max_freq)
        {
            return r2_calibration_table[i].gpio;
        }
        i++;
    }
    
    return 0x17;    
}

static vatek_result _r2_iq_calibration(uint32_t freq)
{
    //RF_R2_TuneIQ
	vatek_result result = vatek_result_success;
    uint16_t pa_gain = 0;
	uint16_t i_offset = 0;
    uint16_t q_offset = 0;
    uint16_t image_offset = 0;
    uint16_t phase_offset = 0;
    
	R2_LOG("_r2_tuneiq by DEFAULT setting");

    pa_gain = _r2_get_calibration_pagain(freq);
    i_offset = R2_I_SIGNAL_OFFSET;
    q_offset = R2_Q_SIGNAL_OFFSET;
    image_offset = R2_IQ_IMAGE_OFFSET;
    phase_offset = R2_IQ_PHASE_OFFSET;

    //calibration register unlock
    if ((result = isp_write(0x12, 0x4341)) != vatek_result_success)
        return result;

    //tune iq setting
    if ((result = isp_write(R2_REG4_MX_PA_GAIN, pa_gain)) != vatek_result_success)
        return result;
    R2_LOG("tune pa gain : 0x%x" ,isp_read(R2_REG4_MX_PA_GAIN));

    if ((result = isp_write(R2_REG13_I_CALIBRATE, i_offset)) != vatek_result_success)
        return result;
    R2_LOG("tune i : 0x%x" ,isp_read(R2_REG13_I_CALIBRATE));

    if ((result = isp_write(R2_REG14_Q_CALIBRATE, q_offset)) != vatek_result_success)
            return result;
    R2_LOG("tune q : 0x%x" ,isp_read(R2_REG14_Q_CALIBRATE));

    if ((result = isp_write(R2_REG15_IMAGE_CALIBRATE, image_offset)) != vatek_result_success)
        return result;
    R2_LOG("tune image : 0x%x" ,isp_read(R2_REG15_IMAGE_CALIBRATE));

    if ((result = isp_write(R2_REG16_PHASE_CALIBRATE, phase_offset)) != vatek_result_success)
        return result;
    R2_LOG("tune phase : 0x%x" ,isp_read(R2_REG16_PHASE_CALIBRATE));
    
    //calibration register lock
    if ((result = isp_write(0x12, 0x0000)) != vatek_result_success)
        return result;

    return result;
}

static vatek_result _r2_switchrffilter(uint32_t freq)
{
    //RF_R2_SwitchFilterReg
	vatek_result result = vatek_result_success;
    uint16_t filtergpio = 0;

    filtergpio = _r2_get_calibration_gpio(freq);
    
    if ((result = isp_write(R2_REG5_RF_FILTER, filtergpio)) != vatek_result_success)
        return result;

    vatek_system_delay(50);
    
	R2_LOG("filter GPIO value : 0x%x", isp_read(R2_REG5_RF_FILTER));
    
	return result;
}

static vatek_result _r2_start(uint32_t freq)
{
    //RF_R2_ChangeFreq
    vatek_result result = vatek_result_success;

    if (freq >= MAX_R2_FREQ || freq <= MIN_R2_FREQ)
        freq = 473;
    else
        freq /= 1000;

    R2_LOG("R2 start freq : %ld Mhz",freq);

    freq = 0xFFFF & freq;		
	// LO Frequency
    if ((result = isp_write(0x03, freq<<4)) != vatek_result_success)
        return result;
	
	//PA and LPF
	/*
    if ((result = RF_R2_WRITE(0x04, 0x2393)) != vatek_result_success)
            return result;
    */
    
	// IQ Calibration
    if ((result = _r2_iq_calibration(freq)) != vatek_result_success)
        return result;
	
	//Auto Calibration
    if ((result = isp_write(0x00, 0x3107)) != vatek_result_success)   //reset auto-calibration lock
        return result;
    if ((result = isp_write(0x00, 0x3F07)) != vatek_result_success)
        return result;
    
	// SDM 1-1-1 restart
    if ((result = isp_write(0x0F, 0x5344)) != vatek_result_success)   //unlock SDM
        return result;
    if ((result = isp_write(0x11, 0x0000)) != vatek_result_success)
        return result;
    if ((result = isp_write(0x10, 0x0015)) != vatek_result_success)
        return result;
    vatek_system_delay(100);    //delay 100ms
    if ((result = isp_write(0x10, 0x0115)) != vatek_result_success)
        return result;
    if ((result = isp_write(0x0F, 0x0000)) != vatek_result_success)   //lock SDM
        return result;
	
	//Auto Calibration done
    if ((result = isp_write(0x00, 0x3D07)) != vatek_result_success)
        return result;	
	
	//Switch RF filter
    if ((result = _r2_switchrffilter(freq)) != vatek_result_success)
        return result;

	//Switch signal path
    if ((result = _r2_signal_control(0x1)) != vatek_result_success)
        return result;

    return result;
}

static vatek_result _r2_start_calibrate(uint32_t freq, r2_calibrate parm)
{
    //RF_R2_ChangeFreq
    vatek_result result = vatek_result_success;

    if (freq >= MAX_R2_FREQ || freq <= MIN_R2_FREQ)
        freq = 473;
    else
        freq /= 1000;

    R2_LOG("R2 start freq : %ld Mhz",freq);

    freq = 0xFFFF & freq;		
	// LO Frequency
    if ((result = isp_write(0x03, freq<<4)) != vatek_result_success)
        return result;
	
	//PA and LPF
	/*
    if ((result = RF_R2_WRITE(0x04, 0x2393)) != vatek_result_success)
            return result;
    */
    
	// IQ Calibration
    //calibration register unlock
    if ((result = isp_write(0x12, 0x4341)) != vatek_result_success)
        return result;

    //tune iq setting
    if ((result = isp_write(R2_REG4_MX_PA_GAIN, parm.gain)) != vatek_result_success)
        return result;
    if ((result = isp_write(R2_REG13_I_CALIBRATE, parm.i_offset)) != vatek_result_success)
        return result;
    if ((result = isp_write(R2_REG14_Q_CALIBRATE, parm.q_offset)) != vatek_result_success)
        return result;
    if ((result = isp_write(R2_REG15_IMAGE_CALIBRATE, parm.image_offset)) != vatek_result_success)
        return result;
    if ((result = isp_write(R2_REG16_PHASE_CALIBRATE, parm.phase_offset)) != vatek_result_success)
        return result;

    //calibration register lock
    if ((result = isp_write(0x12, 0x0000)) != vatek_result_success)
        return result;

	
	//Auto Calibration
    if ((result = isp_write(0x00, 0x3107)) != vatek_result_success)   //reset auto-calibration lock
        return result;
    if ((result = isp_write(0x00, 0x3F07)) != vatek_result_success)
        return result;
    
	// SDM 1-1-1 restart
    if ((result = isp_write(0x0F, 0x5344)) != vatek_result_success)   //unlock SDM
        return result;
    if ((result = isp_write(0x11, 0x0000)) != vatek_result_success)
        return result;
    if ((result = isp_write(0x10, 0x0015)) != vatek_result_success)
        return result;
    vatek_system_delay(100);    //delay 100ms
    if ((result = isp_write(0x10, 0x0115)) != vatek_result_success)
        return result;
    if ((result = isp_write(0x0F, 0x0000)) != vatek_result_success)   //lock SDM
        return result;
	
	//Auto Calibration done
    if ((result = isp_write(0x00, 0x3D07)) != vatek_result_success)
        return result;	
	
	//Switch RF filter
    if ((result = isp_write(R2_REG5_RF_FILTER, parm.gpio)) != vatek_result_success)
        return result;

    vatek_system_delay(50);

	//Switch signal path
    if ((result = _r2_signal_control(0x1)) != vatek_result_success)
        return result;

    return result;
}

vatek_result r2_create(Pr2_board_handle hboard, Pr2_handle *hr2)
{
    vatek_result result = vatek_result_unknown;
    Pr2_handle newr2 = NULL; 

    if (hboard == NULL || hr2 == NULL)
        return vatek_result_invalidparm;

    R2_LOG("reset R2");
    if ((result = vatek_system_reset(sys_dev_type_rf_r2, hboard)) != vatek_result_success)
        return result;
    
    vatek_system_delay(300); // wait r2 reboot

    newr2 = (Pr2_handle)malloc(sizeof(r2_handle));
    if (newr2 == NULL)
        return vatek_result_memfail;

    R2_LOG("R2 init");
    
    if ((result = vatek_system_gpio_create(&newr2->vgpio)) != vatek_result_success)
        return result;
    
    newr2->pin = hboard;
    newr2->status = rf_status_unknown;
    newr2->en_calibrate = 0;
    
    g_r2_handle = newr2;

    RF_R2_CS_LO;
    RF_R2_CLK_LO;
    
    if ((result = _r2_default_setting()) != vatek_result_success)
        return result;
    
    newr2->status = rf_status_idle;
    *hr2 = newr2;
    return result;
}

vatek_result r2_destroy(Pr2_handle hr2)
{
    vatek_result result = vatek_result_unknown;

    if (hr2 == NULL)
        return vatek_result_invalidparm;

    if ((result = vatek_system_gpio_destroy(hr2->vgpio)) != vatek_result_success)
        return result;

    free(hr2);
    hr2 = NULL;
    g_r2_handle = NULL;

    return result;
}

vatek_result r2_start(Pr2_handle hr2, uint32_t freq)
{
    vatek_result result = vatek_result_unknown;
    
    if (hr2 == NULL)
        return vatek_result_invalidparm;

    if (hr2->status != rf_status_idle)
        return vatek_result_badstatus;

    hr2->status = rf_status_fail;
    if (hr2->en_calibrate)
    {
        if ((result = _r2_start_calibrate(freq, hr2->calibrate)) != vatek_result_success)
            return result;
    }
    else
    {
        if ((result = _r2_start(freq)) != vatek_result_success)
            return result;
    }
    
    hr2->status = rf_status_active;

    return result;
}

vatek_result r2_stop(Pr2_handle hr2)
{
    vatek_result result = vatek_result_unknown;

    if (hr2->status != rf_status_active)
        return vatek_result_badstatus;

    if ((result = _r2_default_setting()) != vatek_result_success)
        return result;

    hr2->status = rf_status_idle;
    hr2->en_calibrate = 0;
    return result;
}

vatek_result r2_getstatus(Pr2_handle hr2, rf_status *status)
{
    vatek_result result = vatek_result_unknown;

    if (hr2 == NULL || status == NULL)
        return vatek_result_invalidparm;
    
    *status = hr2->status;
    
    return result;
}

vatek_result r2_setcalibrate(Pr2_handle hr2, Pr2_calibrate parm)
{
    vatek_result result = vatek_result_success;
    
    if (hr2 == NULL)
        return vatek_result_invalidparm;

    if (hr2->status != rf_status_idle)
        return vatek_result_badstatus;

    hr2->en_calibrate = 1;
    memcpy(&hr2->calibrate, parm, sizeof(r2_calibrate));
    
    return result;
}

#endif

