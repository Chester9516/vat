
#ifndef _PHY_ADV7182A_
#define _PHY_ADV7182A_

#ifdef __cplusplus
    extern "C" {
#endif

    typedef void* Padv7182a_handle;

    typedef enum
    {
        si_type_cvbs = 0,
        si_type_ypbpr = 1,
        si_type_min = si_type_cvbs,
        si_type_max = si_type_ypbpr,
    }adv7182a_si_type;

    vatek_result adv7182a_create(Pvatek_i2c vi2c, adv7182a_si_type type, Padv7182a_handle *hadv7182a);
    vatek_result adv7182a_destroy(Padv7182a_handle hadv7182a);
    vatek_result adv7182a_enable(Padv7182a_handle hadv7182a, uint8_t enable);
    vatek_result adv7182a_get_videoinfo(Padv7182a_handle hadv7182a, Pphy_video_info info);
    vatek_result adv7182a_get_audioinfo(Padv7182a_handle hadv7182a, Pphy_audio_info info);
    vatek_result adv7182a_get_status(Padv7182a_handle hadv7182a, phy_status *status);
    vatek_result adv7182a_get_ccdata(Padv7182a_handle hadv7182a, Puserdata_cc_parm cc_parm);

#ifdef __cplusplus
    }
#endif


#endif


