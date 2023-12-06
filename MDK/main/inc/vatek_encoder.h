
#ifndef _VATEK_ENCODER_
#define _VATEK_ENCODER_

#ifdef __cplusplus
    extern "C" {
#endif

    vatek_result vatek_encoder_setinputparm_phy(Phms_handle handle, video_input_parm vparm, audio_input_parm aparm);
    vatek_result vatek_encoder_setinputparm_logo(Phms_handle handle, logo_input_parm parm);
    vatek_result vatek_encoder_setencodeparm(Phms_handle handle, video_encode_parm vparm, audio_encode_parm aparm);
    vatek_result vatek_encoder_setmuxparm(Phms_handle handle, encoder_mux_parm parm);
    vatek_result vatek_encoder_setqualityparm(Phms_handle handle, encoder_quality_parm parm);

/*V1 API*/
		vatek_result vatek_encoder_setinputparm_phy_v1(Phms_handle handle, video_input_parm vparm, audio_input_parm aparm);
    vatek_result vatek_encoder_setinputparm_logo_v1(Phms_handle handle, logo_input_parm parm);
    vatek_result vatek_encoder_setencodeparm_v1(Phms_handle handle, video_encode_v1_parm vparm, audio_encode_parm aparm);
    vatek_result vatek_encoder_setmuxparm_v1(Phms_handle handle, encoder_mux_parm parm);
    vatek_result vatek_encoder_setqualityparm_v1(Phms_handle handle, encoder_quality_parm parm);

    #define ENCODER_ERR(fmt,...)    VATEK_ERR(encoder,fmt,##__VA_ARGS__)
    #define ENCODER_LOG(fmt,...)    VATEK_LOG(encoder,fmt,##__VA_ARGS__)

#ifdef __cplusplus
    }
#endif

#endif

