#include "internal/sx1804a_internal.h"

typedef struct _sx1804a_handle
{
    sx1804_param param;
}sx1804a_handle,*Psx1804a_handle;

extern vatek_result sx1804a_write(uint16_t reg,uint8_t val);
extern vatek_result sx1804a_write_uint16_t(uint16_t reg,uint16_t val);
extern vatek_result sx1804a_read(uint16_t reg,uint8_t* val);
extern vatek_result sx1804a_check_link_on(bridge_status* bstatus);
extern void sa1804a_set_video_info(Phdmi_video_id phdmiinfo,Pbridge_video_info psource,int32_t isclk1001);
extern vatek_result sx1804a_write_output_timing(Phdmi_video_timing ptiming,Phdmi_video_id pvid);
extern vatek_result sx1804a_write_output(Psx1804_param param,video_resolution vresolution);

vatek_result sx1804a_check_support()
{
    return hal_i2c_check_device(SX1804A_I2C_ADDR);
}

vatek_result sx1804a_open(hbridge_source* hsource)
{
    Psx1804a_handle newdrv = (Psx1804a_handle)malloc(sizeof(sx1804a_handle));
    vatek_result nres = vatek_memfail;
    if(newdrv)
    {
        memset(newdrv,0,sizeof(sx1804a_handle));
        newdrv->param.scale = sscale_bypass;
        *hsource = newdrv;
        nres = sx1804a_set_output((hbridge_source)newdrv,0);
        if(is_vatek_success(nres))*hsource = newdrv;
    }
    return nres;
}

vatek_result sx1804a_config(hbridge_source hsource,Pbavsource_param param)
{
    return vatek_unsupport;
}

vatek_result sx1804a_get_status(hbridge_source hsource,Pbridge_source pbsourcesttaus)
{
    vatek_result nres = vatek_success;
    memset(pbsourcesttaus,0,sizeof(bridge_source));

    nres = sx1804a_check_link_on(&pbsourcesttaus->status);
    if(is_vatek_success(nres))
    {
        if(pbsourcesttaus->status >= bstatus_active)
        {
            uint8_t vstatus = 0;
            uint8_t vhdmi = 0;
            nres = sx1804a_read(SX1804A_V_STATUS,&vstatus);
            if(is_vatek_success(nres))
                nres = sx1804a_read(SX1804A_HDMI_FLAG,&vhdmi);
            
            if(is_vatek_success(nres))
            {
                Phdmi_video_id ptime = hdmi_tool_get_video_id(vstatus);
                nres = vatek_unsupport;
                if(ptime == NULL)_HAL_LOG(sx1804a,"unknown video code : %d",vstatus);
                else sa1804a_set_video_info(ptime,&pbsourcesttaus->video_info,(vhdmi & SX1804A_HDMI_FLAG_PCLK_1001));
            }
            if(is_vatek_success(nres))
            {
                #define AUDIO_SAMPLE_FREQ_32KHZ		0x00
                #define AUDIO_SAMPLE_FREQ_44_1KHZ	0x01
                #define AUDIO_SAMPLE_FREQ_48KHZ		0x02
                uint8_t val = 0;
                nres = sx1804a_read(SX1804A_A_STATUS,&val);
                if(is_vatek_success(nres))
                {
                    val &= SX1804A_A_STATUS_SAMP_FREQ;
                    if(val == AUDIO_SAMPLE_FREQ_32KHZ)pbsourcesttaus->audio_info.samplerate = sample_rate_32;
                    else if(val == AUDIO_SAMPLE_FREQ_44_1KHZ)pbsourcesttaus->audio_info.samplerate = sample_rate_44_1;
                    else pbsourcesttaus->audio_info.samplerate = sample_rate_48;
                    pbsourcesttaus->audio_info.channel = channel_stereo;
                }
            }
            if(!is_vatek_success(nres))pbsourcesttaus->status = bstatus_invalid;
        }
    }

    return vatek_unsupport;
}

vatek_result sx1804a_set_output(hbridge_source hsource,int32_t isoutput)
{
    vatek_result nres = vatek_success;
    Psx1804a_handle pdrv = (Psx1804a_handle)hsource;
    if(isoutput)
    {
        bridge_status status = bstatus_idle;
        nres = sx1804a_check_link_on(&status);
        if(is_vatek_success(nres))
        {
            if(status >= bstatus_active)
            {
                uint8_t vstatus = 0;
                uint8_t vhdmi = 0;
                nres = sx1804a_read(SX1804A_V_STATUS,&vstatus);
                if(is_vatek_success(nres))
                    nres = sx1804a_read(SX1804A_HDMI_FLAG,&vhdmi);
            
                if(is_vatek_success(nres))
                {
                    Phdmi_video_id pvid = hdmi_tool_get_video_id(vstatus);
                    if(pvid == NULL)nres = vatek_badstatus;
                    else 
                    {
                        hdmi_video_timing vtime;
                        hdmi_tool_get_timing(pvid,&vtime);
                        nres = sx1804a_write_output_timing(&vtime,pvid);
                        if(is_vatek_success(nres))
                            nres = sx1804a_write_output(&pdrv->param,pvid->resolution);
                    }
                }
            }
        }
    }else nres = sx1804a_write(SX1804A_V_MODE,0x00); /* disable output */
   
    return nres;
}

void sx1804a_close(hbridge_source hsource)
{
    Psx1804a_handle psx = (Psx1804a_handle)hsource;
    free(psx);
}

vatek_result sx1804a_write(uint16_t reg,uint8_t val)
{
    vatek_result nres = hal_i2c_start(SX1804A_I2C_WRITE,0);
    if(is_vatek_success(nres))
    {
        uint8_t buf[2];
        buf[0] = (uint8_t)(reg >> 8);
        buf[1] = (uint8_t)reg;
        nres = hal_i2c_write(&buf[0],2);
         
        if(is_vatek_success(nres))
        {
            nres = hal_i2c_write(&val,1);
            if(is_vatek_success(nres))
                nres = hal_i2c_stop();
        }
    }
    return nres;
}

vatek_result sx1804a_write_uint16_t(uint16_t reg,uint16_t val)
{
    vatek_result nres = sx1804a_write(reg++,(uint8_t)val);
    if(is_vatek_success(nres))
        nres = sx1804a_write(reg,(uint8_t)(val >> 8));
    return nres;
}

vatek_result sx1804a_read(uint16_t reg,uint8_t* val)
{
    vatek_result nres = hal_i2c_start(SX1804A_I2C_WRITE,0);
    if(is_vatek_success(nres))
    {
        uint8_t buf[2];
        buf[0] = (uint8_t)(reg >> 8);
        buf[1] = (uint8_t)reg;
        nres = hal_i2c_write(&buf[0],2);
        if(is_vatek_success(nres))
        {
            nres = hal_i2c_start(SX1804A_I2C_READ,1);
            if(is_vatek_success(nres))
                nres = hal_i2c_read(val,1);
            if(is_vatek_success(nres))
                nres = hal_i2c_stop();
        }
    }
    return nres;
}

vatek_result sx1804a_check_link_on(bridge_status* bstatus)
{
    #define SA1804A_HDMI_VALID  (SX1804A_SYS_STATUS2_DE | SX1804A_SYS_STATUS2_LINK)
    uint8_t val = 0;
    vatek_result nres = sx1804a_read(SX1804A_SYS_STATUS2,&val);
    *bstatus = bstatus_idle;
    if(is_vatek_success(nres))
    {
        if(val & SX1804A_SYS_STATUS2_PWR5V)
        {
            if((val & SA1804A_HDMI_VALID) == SA1804A_HDMI_VALID)
            {
                *bstatus = bstatus_active;
                /* check is hdcp enable ?? bstatus_active_protect */
            }else *bstatus = bstatus_invalid;
        }
    }
    return nres;
}

void sa1804a_set_video_info(Phdmi_video_id phdmiinfo,Pbridge_video_info psource,int32_t isclk1001)
{
    psource->resolution = phdmiinfo->resolution;
    psource->aspectrate = phdmiinfo->aspectratio;
    psource->framerate = phdmiinfo->framerate;
    psource->offset_x = 0;
    psource->offset_y = 0;
    psource->pixelclk = phdmiinfo->pixelclk;
    
    if(phdmiinfo->represent != 0)
        psource->pixelclk >>= 1;
    
    psource->signal_flag = VI_BUSWIDTH_16 | VI_SEPARATED_SYNC;
    if(isclk1001)hdmi_tool_change_base_clock(sbaseclk_1001,psource);
}

vatek_result sx1804a_write_output_timing(Phdmi_video_timing ptiming,Phdmi_video_id pvid)
{
    uint16_t addr = SX1804A_V_SCALE;
    uint16_t vtotal = ptiming->pixel_per_frame + ptiming->vbp + ptiming->vfp + ptiming->vpw;
    vatek_result nres = vatek_success;
    
    if(pvid->represent != 0)
    {
        ptiming->hbp >>= 1;
        ptiming->hfp >>= 1;
        ptiming->hpw >>= 1;
    }
    
    nres = sx1804a_write(addr++,ptiming->vbp);
    if(is_vatek_success(nres))
        nres = sx1804a_write(addr++,ptiming->vpw);
    
    if(is_vatek_success(nres))
        nres = sx1804a_write_uint16_t(addr,ptiming->hfp);
    addr += 2;

    if(is_vatek_success(nres))
        nres = sx1804a_write_uint16_t(addr,ptiming->hpw);
    addr += 2;
    
    if(is_vatek_success(nres))
        nres = sx1804a_write_uint16_t(addr,ptiming->hbp);
    addr += 2;
    
    if(is_vatek_success(nres))
        nres = sx1804a_write_uint16_t(addr,ptiming->pixel_per_line);
    addr += 2;

    if(is_vatek_success(nres))
        nres = sx1804a_write_uint16_t(addr,ptiming->pixel_per_frame);
    addr += 2;
    
    if(is_vatek_success(nres))
        nres = sx1804a_write_uint16_t(addr,vtotal);
        
    return nres;
}

vatek_result sx1804a_write_output(Psx1804_param param,video_resolution vresolution)
{
    #define SX1804A_DEF_OUTPUT  (SX1804A_V_MODE_VOUT_EN | SX1804A_V_MODE_COLOR_YCRCB_422)
    
    vatek_result nres = vatek_success;
    
    if(param->scale == sscale_bypass)
    {
        if(is_resolution_interlaced(vresolution))
        {
            nres = sx1804a_write(SX1804A_V_IN_DEF,0x00);                               /* enable scale         */
    
            if(is_vatek_success(nres))
            { /* pixel cnt bypass     */
                 if(vresolution == resolution_1080i)                           /* interlaced mode      */   
                     nres = sx1804a_write(0x30a1,0x80);
                 else nres = sx1804a_write(0x30a1,0x00);
            }
             
            if(is_vatek_success(nres))nres = sx1804a_write(0x30a6,0x08);            /* line cnt bypass      */
            if(is_vatek_success(nres))nres = sx1804a_write(0x30c0,0x00);  
            if(is_vatek_success(nres))nres = sx1804a_write(0x30c1,0x00);
            if(is_vatek_success(nres))nres = sx1804a_write(0x30c2,0x01);
            if(is_vatek_success(nres))nres = sx1804a_write(0x30c3,0x00);
            
            if(is_vatek_success(nres))
            {
                if(vresolution == resolution_1080i)
                    nres = sx1804a_write(0x30c4,0x01);
                else nres = sx1804a_write(0x30c4,0x03);                                /* remove repetition */
            }
            if(is_vatek_success(nres))nres = sx1804a_write(SX1804A_V_BT656,0x22);   /* interlaced mode      */   
        }else nres = sx1804a_write(SX1804A_V_IN_DEF,SX1804A_V_IN_DEF_SCALE_BYPASS);
    }
    
    if(is_vatek_success(nres))
    {
        nres = sx1804a_write(SX1804A_POL_CNT,SX1804A_POL_CNT_HS | SX1804A_POL_CNT_VS);
        if(is_vatek_success(nres))
            nres = sx1804a_write(SX1804A_V_MODE,SX1804A_DEF_OUTPUT);
    }
    return nres;
}
