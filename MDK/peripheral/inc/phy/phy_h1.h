
#ifndef _PHY_H1_
#define _PHY_H1_

#ifdef __cplusplus
    extern "C" {
#endif

    

    typedef void* Ph1_handle;
	typedef struct _hdmi_video_timing
    {
        uint8_t vid;
        uint8_t represent;
        video_input_resolution resolution;
        video_input_aspectrate aspectrate;
        uint16_t pixel_per_line;
        uint16_t pixel_per_frame;
        uint16_t hfp;
        uint16_t hbp;
        uint16_t hpw;
        uint8_t vfp;
        uint8_t vbp;
        uint8_t vpw;
        uint8_t _int;
    }hdmi_video_timing,*Phdmi_video_timing;

    vatek_result h1_create(Pvatek_i2c vi2c, Ph1_handle *hh1);
    vatek_result h1_destroy(Ph1_handle hh1);
    vatek_result h1_enable(Ph1_handle hh1, uint8_t enable);
    vatek_result h1_get_videoinfo(Ph1_handle hh1, Pphy_video_info info);
    vatek_result h1_get_audioinfo(Ph1_handle hh1, Pphy_audio_info info);
    vatek_result h1_get_status(Ph1_handle hh1, phy_status * status);
    vatek_result h1_set_outputmode( Ph1_handle hh1, phy_output_mode mode);
    vatek_result h1_set_baseclock( Ph1_handle hh1, phy_output_baseclock clk);
		vatek_result get_h1_HV();
		vatek_result h1_dump_reg(Ph1_handle hh1);
		vatek_result h1_vout_check(Ph1_handle hh1);
		vatek_result h1_change_clk(Ph1_handle hh1);
		
#if defined(SELF_TEST)    
    vatek_result h1_write_reg( Ph1_handle hh1, uint32_t addr, uint8_t val);
    vatek_result h1_read_reg( Ph1_handle hh1, uint32_t addr, uint8_t *val);
#endif    


    

    
#ifdef __cplusplus
    }
#endif

#endif

