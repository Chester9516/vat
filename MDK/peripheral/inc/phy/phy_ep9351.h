
#ifndef _PHY_EP9351_
#define _PHY_EP9351_

#ifdef __cplusplus
extern "C" {
#endif

    typedef void* Pep9351_handle;

    vatek_result ep9351_create(Pvatek_i2c vi2c, Pep9351_handle *hep9351);
    vatek_result ep9351_destroy(Pep9351_handle hep9351);
    vatek_result ep9351_enable(Pep9351_handle hep9351, uint8_t enable);
    vatek_result ep9351_get_audioinfo(Pep9351_handle hep9351, Pphy_audio_info info);
    vatek_result ep9351_get_videoinfo(Pep9351_handle hep9351, Pphy_video_info info);
    vatek_result ep9351_get_status(Pep9351_handle hep9351, phy_status *status);
    
#ifdef __cplusplus
}
#endif

#endif

