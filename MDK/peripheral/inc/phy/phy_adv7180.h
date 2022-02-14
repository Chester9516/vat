
#ifndef _PHY_ADV7180_
#define _PHY_ADV7180_

#ifdef __cplusplus
    extern "C" {
#endif

    typedef void* Padv7180_handle;

    vatek_result adv7180_create(Pvatek_i2c vi2c,Padv7180_handle *hadv7180);
    vatek_result adv7180_destroy(Padv7180_handle hadv7180);
    vatek_result adv7180_enable(Padv7180_handle hadv7180, uint8_t enable);
    vatek_result adv7180_get_videoinfo(Padv7180_handle hadv7180, Pphy_video_info info);
    vatek_result adv7180_get_audioinfo(Padv7180_handle hadv7180, Pphy_audio_info info);    
    vatek_result adv7180_get_status(Padv7180_handle hadv7180, phy_status *status);
    vatek_result adv7180_get_ccdata(Padv7180_handle hadv7180, Puserdata_cc_parm cc_parm);

#ifdef __cplusplus
    }
#endif

#endif

