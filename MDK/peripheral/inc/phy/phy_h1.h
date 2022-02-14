
#ifndef _PHY_H1_
#define _PHY_H1_

#ifdef __cplusplus
    extern "C" {
#endif

    

    typedef void* Ph1_handle;

    vatek_result h1_create(Pvatek_i2c vi2c, Ph1_handle *hh1);
    vatek_result h1_destroy(Ph1_handle hh1);
    vatek_result h1_enable(Ph1_handle hh1, uint8_t enable);
    vatek_result h1_get_videoinfo(Ph1_handle hh1, Pphy_video_info info);
//    vatek_result h1_set_videoinfo(Ph1_handle hh1, Pphy_video_info info);
    vatek_result h1_get_audioinfo(Ph1_handle hh1, Pphy_audio_info info);
    vatek_result h1_get_status(Ph1_handle hh1, phy_status * status);
    vatek_result h1_set_outputmode( Ph1_handle hh1, phy_output_mode mode);
    vatek_result h1_set_baseclock( Ph1_handle hh1, phy_output_baseclock clk);
#if defined(SELF_TEST)    
    vatek_result h1_write_reg( Ph1_handle hh1, uint32_t addr, uint8_t val);
    vatek_result h1_read_reg( Ph1_handle hh1, uint32_t addr, uint8_t *val);
#endif    


    

    
#ifdef __cplusplus
    }
#endif

#endif

