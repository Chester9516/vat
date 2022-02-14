
#ifndef _PHY_EP9555E_
#define _PHY_EP9555E_

#ifdef __cplusplus
    extern "C" {
#endif

    typedef void* Pep9555e_handle;

    vatek_result ep9555e_create(Pvatek_i2c vi2c, Pep9555e_handle *hep9555e);
    vatek_result ep9555e_destroy(Pep9555e_handle hep9555e);
    vatek_result ep9555e_enable(Pep9555e_handle hep9555e, uint8_t enable);
    vatek_result ep9555e_get_audioinfo(Pep9555e_handle hep9555e, Pphy_audio_info info);
    vatek_result ep9555e_get_videoinfo(Pep9555e_handle hep9555e, Pphy_video_info info);
    vatek_result ep9555e_set_videoinfo(Pep9555e_handle hep9555e, Pphy_video_info info);
    vatek_result ep9555e_get_status(Pep9555e_handle hep9555e, phy_status *status);
    vatek_result ep9555e_set_baseclock(Pep9555e_handle hep9555e, phy_output_baseclock clk);
    
#ifdef __cplusplus
    }
#endif

#endif

