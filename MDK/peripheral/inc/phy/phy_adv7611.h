
#ifndef _PHY_ADV7611_
#define _PHY_ADV7611_

#ifdef __cplusplus
    extern "C" {
#endif

    typedef void* Padv7611_handle;

    vatek_result adv7611_create(Pvatek_i2c vi2c, Padv7611_handle *hadv7611);
    vatek_result adv7611_destroy(Padv7611_handle hadv7611);
    vatek_result adv7611_enable(Padv7611_handle hadv7611, uint8_t enable);
    vatek_result adv7611_get_videoinfo(Padv7611_handle hadv7611, Pphy_video_info info);
    vatek_result adv7611_get_audioinfo(Padv7611_handle hadv7611, Pphy_audio_info info);
    vatek_result adv7611_get_status(Padv7611_handle hadv7611, phy_status *status);

#ifdef __cplusplus
    }
#endif

#endif

