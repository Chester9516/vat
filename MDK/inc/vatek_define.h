

#ifndef _VATEK_DEFINE_
#define _VATEK_DEFINE_

    #include <stdint.h>

#ifdef __cplusplus
    extern "C" {
#endif

    #define SDK_VERSION                 0x2103

    #define MAX_PSITABLE_NUM            16
    #define PSI_TSPACKET_LEN            188
    #define PSI_TSPACKET_WLEN           (PSI_TSPACKET_LEN >> 2)
    #define MAX_USERDATA_CC_NUM         12
    #define MAX_USERDATA_RAW_NUM        64
    #define MAX_TSP_FILTER_NUM          16

    /*------------------------ common ------------------------*/
    typedef void* Pboard_handle;
    typedef void* Phbroadcast;
    typedef void* Phtransform;
    
    /* status */
    typedef enum _broadcast_status
    {
        bc_status_unknown               = 0,
        bc_status_idle                  = 1,
        bc_status_wait_source           = 2,
        bc_status_broadcast             = 3,
        bc_status_finish                = 4,
        bc_status_fail_unknown          = 11,
        bc_status_fail_source           = 12,
        bc_status_fail_timeout          = 13,
        bc_status_fail_codecdrop        = 14,
        bc_status_fail_buffer           = 15,
        bc_status_fail_muxer            = 16,
        bc_status_fail_encode           = 17,
        bc_status_fail_media            = 18,
        bc_status_fail_demux            = 19,
    }broadcast_status;

    typedef enum _chip_status
    {
        chip_status_unknown             = 0,
        chip_status_idle                = 1,
        chip_status_wait_command        = 2,
        chip_status_running             = 3,
        chip_status_fail                = 4,
        chip_status_badfw               = 5,
    }chip_status;
        
    /* infomation type*/
    typedef enum
    {
        bc_infotype_unknown             = 0,
        bc_infotype_chipid              = 1,
        bc_infotype_fw_ver              = 2,
        bc_infotype_sdk_ver             = 3,
        bc_infotype_service_mode        = 4,
        bc_infotype_peripheral_en       = 5,
        bc_infotype_insupport           = 6,
        bc_infotype_outsupport          = 7,
        bc_infotype_min                 = bc_infotype_unknown,
        bc_infotype_max                 = bc_infotype_outsupport,
    }broadcast_infotype;

    /* vatek result*/
    typedef enum _vatek_result
    {
        vatek_result_success            = 0,
        vatek_result_unknown            = 1,
        vatek_result_invalidparm        = 2,
        vatek_result_timeout            = 3,
        vatek_result_memfail            = 4,
        vatek_result_format             = 5,
        vatek_result_badstatus          = 6,
        vatek_result_busy               = 7,
        vatek_result_idle               = 8,
        vatek_result_overrange          = 9,
        vatek_result_hwfail             = 10,
        vatek_result_unsupport          = 11,
        vatek_result_negative           = 12,
        vatek_result_bufoverflow        = 13,
        vatek_result_i2cfail            = 14,
        vatek_result_gpiofail           = 15,
        vatek_result_uartfail           = 16,
    }vatek_result;



    /*------------------------ encoder (broadcast only) ------------------------*/
    /**
    * @brief video output type of logo mode.
    */
    typedef enum _logo_type
    {
        logo_type_unknown              = 0,     ///< unknow logo type.
        logo_type_colorbar             = 1,     ///< broadcast colorbar image.
        logo_type_bootlogo             = 2,     ///< broadcast logo image.
        logo_type_min                  = logo_type_unknown,
        logo_type_max                  = logo_type_bootlogo,
    }logo_type;

    /**
    * @brief logo output index of logo type of logo mode.
    */
    typedef struct _logo_input_parm
    {
        logo_type type;      ///< see logo_type
        uint32_t logoidx;    ///< logo index. logo images is stored in the flash.
    }logo_input_parm, *Plogo_input_parm;
    
    /**
    * @brief video input resolution.
    */
    typedef enum _video_input_resolution
    {
        vi_resolution_unknown          = 0,     ///< resolution unknown.
        vi_resolution_1080p60          = 1,     ///< resolution 1080P, fps 60Hz.
        vi_resolution_1080p59_94       = 2,     ///< resolution 1080P, fps 59.94Hz.
        vi_resolution_1080p50          = 3,     ///< resolution 1080P, fps 50Hz.
        vi_resolution_1080p30          = 4,     ///< resolution 1080P, fps 30Hz.
        vi_resolution_1080p25          = 5,     ///< resolution 1080P, fps 20Hz.
        vi_resolution_1080p24          = 6,     ///< resolution 1080P, fps 24Hz.
        vi_resolution_1080p23_97       = 7,     ///< resolution 1080P, fps 23.97Hz.
        vi_resolution_1080i60          = 8,     ///< resolution 1080i, fps 60Hz.
        vi_resolution_1080i59_94       = 9,     ///< resolution 1080i, fps 50.94Hz.
        vi_resolution_1080i50          = 10,    ///< resolution 1080i, fps 50Hz.
        vi_resolution_720p60           = 11,    ///< resolution 720P, fps 60Hz.
        vi_resolution_720p59_94        = 12,    ///< resolution 720P, fps 59.94Hz.
        vi_resolution_720p50           = 13,    ///< resolution 720P, fps 50Hz.
        vi_resolution_576p50           = 14,    ///< resolution 576P, fps 50Hz.
        vi_resolution_480p60           = 15,    ///< resolution 480P, fps 60Hz.
        vi_resolution_480p59_94        = 16,    ///< resolution 480P, fps 59.94Hz.
        vi_resolution_576i50           = 17,    ///< resolution 576i, fps 50Hz.
        vi_resolution_576p25           = 18,    ///< resolution 576P, fps 25Hz.
        vi_resolution_480i60           = 19,    ///< resolution 480i, fps 60Hz.
        vi_resolution_480i59_94        = 20,    ///< resolution 480i, fps 59.94Hz.
        vi_resolution_480p30           = 21,    ///< resolution 480P, fps 30Hz.
        vi_resolution_480p29_97        = 22,    ///< resolution 480P, fps 29.97Hz.
        vi_resolution_bus8_min         = vi_resolution_576i50,    
        vi_resolution_bus8_max         = vi_resolution_480p29_97,
        vi_resolution_min              = vi_resolution_unknown,
        vi_resolution_max              = vi_resolution_480p29_97,
    }video_input_resolution;
        
    /**
    * @brief video input aspectrate.
    */
    typedef enum _video_input_aspectrate
    {
        vi_aspectrate_unknown          = 0,     ///< unknown aspectrate.
        vi_aspectrate_16_9             = 1,     ///< 16:9 aspectrate.
        vi_aspectrate_4_3              = 2,     ///< 4:3 aspectrate.
        vi_aspectrate_min              = vi_aspectrate_unknown,
        vi_aspectrate_max              = vi_aspectrate_4_3,
    }video_input_aspectrate;

    /**
    * @brief video input parameter in chip. input parameter must be as same as phy output parameter.
    */
    typedef struct _video_input_parm
    {
        video_input_resolution resolution;      ///< video input source resolution.
        video_input_aspectrate aspectrate;      ///< video input source aspectrate.
        uint32_t offset_x;                      ///< image display x axis offset.
        uint32_t offset_y;                      ///< image display y axis offset.
        uint8_t buswidth_16;                    ///< phy to chip data bus number. buswidth need to study phy specification.
        uint8_t separated_sync;                 ///< phy to chip spearated sync pin. separated sync need to study phy specification.
        uint8_t clk_inverse;                    ///< phy to chip clk pin. clk signal inverse bool.
        uint8_t hsync_inverse;                  ///< phy to chip hsync pin. hsync signal inverse bool.
        uint8_t vsync_inverse;                  ///< phy to chip vsync pin. vsync signal inverse bool.
        uint8_t field_inverse;                  ///< phy to chip FID pin. FID signal inverse bool.
        uint8_t ext_half_fps;                   ///< output fps half. output fps = input fps/2.
    }video_input_parm, *Pvideo_input_parm;

    /**
    * @brief audio input sample rate.
    */
    typedef enum _audio_input_samplerate
    {
        ai_samplerate_unknown          = 0,     ///< audio input sample rate unknown.
        ai_samplerate_48K              = 1,     ///< audio input sample rate 48K.
        ai_samplerate_44_1K            = 2,     ///< audio input sample rate 44.1K.
        ai_samplerate_32K              = 3,     ///< audio input sample rate 32K.
        ai_samplerate_min              = ai_samplerate_unknown,
        ai_samplerate_max              = ai_samplerate_32K,
    }audio_input_samplerate;

    /**
    * @brief audio input parameter in chip. input parameter must be as same as phy output parameter. 
    */
    typedef struct _audio_input_parm
    {
        audio_input_samplerate samplerate;      ///< audio input source sample rate.
    }audio_input_parm, *Paudio_input_parm;

    /**
    * @brief video encoder type.
    */
    typedef enum _video_encode_type
    {
        ve_type_unknown                = 0,     ///< unknown type.
        ve_type_mpeg2                  = 1,     ///< mpeg-2 type of encoder.
        ve_type_h264                   = 2,     ///< h.264 type of encoder.
        ve_type_min                    = ve_type_unknown,
        ve_type_max                    = ve_type_h264,
    }video_encode_type;

    /**
    * @brief video encoder parameter of encoder.
    */
    typedef struct _video_encoder_parm
    {
        video_encode_type type;                  ///< video encoder type.
#if 0        
        uint8_t interlaced_frame;                ///< merge two field into a frame.
#endif        
        uint8_t progressive_2_i;                 ///< video progressive change to interlace.
        uint8_t latency_q_close;                 ///< stop the latency of the encode parm affect the q(quantization).
        uint8_t disable_deinterlaced;            ///< disable deinterlaced via b3+.
    }video_encode_parm, *Pvideo_encode_parm;

    /**
    * @brief audio encoder type.
    */
    typedef enum _audio_encode_type
    {
        ae_type_unknown                = 0,      ///< unknown type.
        ae_type_mp1_l2                 = 1,      ///< mp1_l2 type.
        ae_type_aac_lc_adts            = 2,      ///< aac_lc_adts type.
        ae_type_ac_3                   = 3,      ///< ac3 type.
        ae_type_aac_lc_latm            = 4,      ///< aac_lc_latm type.
        ae_type_min                    = ae_type_unknown,
        ae_type_max                    = ae_type_aac_lc_latm,
    }audio_encode_type;

    /**
    * @brief audio channel of encoder.
    */
    typedef enum _audio_encode_channel
    {
        ae_channel_unknown             = 0,      ///< unknown channel.
        ae_channel_mute                = 1,      ///< audio mute.
        ae_channel_stereo              = 2,      ///< audio stereo channel.
        ae_channel_mono_l              = 3,      ///< audio only left channel.
        ae_channel_mono_r              = 4,      ///< audio only right channel.
        ae_channel_stereo_mono_l       = 5,      ///< audio left channel in stereo channel.
        ae_channel_stereo_mono_r       = 6,      ///< audio right channel in stereo channel.
        ae_channel_min                 = ae_channel_unknown,
        ae_channel_max                 = ae_channel_stereo_mono_r,
    }audio_encode_channel;

    /**
    * @brief audio encoder parameter of encoder.
    */
    typedef struct _audio_encoder_parm
    {
        audio_encode_type type;                  ///< audio encoder type.
        audio_encode_channel channel;            ///< audio channel type.
    }audio_encode_parm, *Paudio_encode_parm;

    /**
    * @brief assign PID to video/audio in PES.
    */
    typedef struct _encoder_mux_parm
    {
        uint32_t video_pid;                      ///< video PID.
        uint32_t audio_pid;                      ///< audio PID.
    }encoder_mux_parm, *Pencoder_mux_parm;

    /**
    * @brief rate control type of encoder.
    */
    typedef enum _quality_rcmode
    {
        q_rcmode_unknown                = 0,     ///< unknown type.
        q_rcmode_vbr                    = 1,     ///< rate control can exceed the threshold in a short time in vbr mode.
#if 0        
        q_rcmode_fixedq                 = 2,
        q_rcmode_auto                   = 3,     ///< rate control must be not exceed the thresholdin vbr mode.
#endif        
        q_rcmode_min                    = q_rcmode_unknown,
#if 0        
        q_rcmode_max                    = q_rcmode_auto,
#else
        q_rcmode_max                    = q_rcmode_vbr,
#endif        
    }quality_rcmode;

    /**
    * @brief quality parameter of encoder.
    */
    typedef struct _encoder_quality_parm
    {
        quality_rcmode  rcmode;                  ///< rate control type.
        uint32_t minq;                           ///< minimum quantization value.
        uint32_t maxq;                           ///< maximum quantization value.
        uint32_t gop;                            ///< group of picture value.
        uint32_t latency;                        ///< latency value. the latency unit is ms.
        uint32_t bitrate;                        ///< bitrate that is used to quantify the quality of the video.
    }encoder_quality_parm, *Pencoder_quality_parm;


    
    /*------------------------ userdata (broadcast only) ------------------------*/
    /**
    * @brief closed caption parameter in cea-608 of user data.
    */
    typedef struct 
    {
        uint8_t attr;                            ///< closed caption attribute.
        uint8_t ccdata1;                         ///< closed caption first byte.
        uint8_t ccdata2;                         ///< closed caption second byte.
    }userdata_cc_parm, *Puserdata_cc_parm;


    
    /*------------------------ tsmux (both) ------------------------*/
    /**
    * @brief psi creater type of the muxer.
    */
#if 0    
    typedef enum
    {
        tsmux_type_unknown              = 0,     ///< unknown type.
        tsmux_type_pure                 = 1,     ///< pure mode of psi create type.
        tsmux_type_iso13818             = 2,     ///< iso mode of psi create type.
        tsmux_type_rule                 = 3,     ///< license mode of psi create type.
        tsmux_type_default              = 4,     ///< default mode of psi create type.
        tsmux_type_min                  = tsmux_type_unknown,
        tsmux_type_max                  = tsmux_type_default,
    }tsmux_type;
#else
    typedef enum
    {
        tsmux_type_unknown              = 0,     ///< unknown type.
        tsmux_type_pure                 = 1,     ///< pure mode of psi create type.
        tsmux_type_rule                 = 2,     ///< license mode of psi create type.
        tsmux_type_default              = 3,     ///< default mode of psi create type.
        tsmux_type_min                  = tsmux_type_unknown,
        tsmux_type_max                  = tsmux_type_default,
    }tsmux_type;
#endif    

    typedef void* Ptsmux_parm;

    /**
    * @brief pure mode parameter.
    */
    typedef struct _tsmux_pure_parm
    {
        uint32_t pcr_pid;                         ///< pcr pid.
        uint32_t padding_pid;                     ///< padding pid.
        uint32_t en_function;                     ///< option function.
    }tsmux_pure_parm, *Ptsmux_pure_parm;
#if 0
    /**
    * @brief iso mode parameter.
    */
    typedef struct _tsmux_iso13818_parm
    {
        uint32_t pcr_pid;                         ///< pcr pid.
        uint32_t padding_pid;                     ///< padding pid.
        uint32_t pmtpid;                          ///< PMT table pid.
        uint32_t tsid;                            ///< TS id.
        uint32_t program_num;                     ///< program number.
    }tsmux_iso13818_parm, *Ptsmux_iso13818_parm;
#endif
    /**
    * @brief license mode parameter.
    */
    typedef struct _tsmux_rule_parm
    {
        uint32_t pcr_pid;                         ///< pcr pid.
        uint32_t padding_pid;                     ///< padding pid.
        uint32_t en_function;                     ///< option function.
    }tsmux_rule_parm, *Ptsmux_rule_parm;

    /**
    * @brief default mode parameter.
    */
    typedef struct _tsmux_default_parm
    {
        uint32_t pcr_pid;                         ///< pcr pid.
        uint32_t pmt_pid;                         ///< PMT table pid.
        uint32_t padding_pid;                     ///< padding pid.
        uint32_t en_function;                     ///< option function.
    }tsmux_default_parm, *Ptsmux_default_parm;


    /*------------------------ psitable (both) ------------------------*/
    /**
    * @brief psi table parameter in pure mode..
    */
    typedef struct _psitable_parm
    {
        uint16_t interval_ms;                     ///< psi table transmit interval.
        uint16_t tspacket_num;                    ///< packet total of number in once psi table.
        uint8_t *tspackets;                       ///< pointer to the ts data.
    }psitable_parm, *Ppsitable_parm;
    
    /**
    * @brief psi table list parameter in pure mode.
    */
    typedef struct _psitablelist_parm
    {
        uint32_t table_num;                       ///< table total of number in psi table.
        psitable_parm table[MAX_PSITABLE_NUM];    ///< psi table parameter.
        uint8_t psip_en;                          ///< using psip table msut be enabled psip flag.
    }psitablelist_parm, *Ppsitablelist_parm;


    
    /*------------------------ modulator (both) ------------------------*/
    /**
    * @brief modulator type of the modulator.
    */
    typedef enum _modulator_type
    {
        m_type_unknown                  = 0,      ///< unknown type.
        m_type_dvb_t                    = 1,      ///< dvb-t type.
        m_type_j83a                     = 2,      ///< j83a type.
        m_type_atsc                     = 3,      ///< atsc type.
        m_type_j83b                     = 4,      ///< j83b type.
        m_type_dtmb                     = 5,      ///< dtmb type.
        m_type_isdb_t                   = 6,      ///< isdb-t type.
        m_type_j83c                     = 7,      ///< j83c type.
        m_type_dvb_t2                   = 8,      ///< dvb-t2 type.
//        m_type_sinewave                 = 9,      ///< sinewave type. sinewave type only used in test mode.
        m_type_min                      = m_type_unknown,
        m_type_max                      = m_type_dvb_t2,
    }modulator_type;

    /**
    * @brief DAC advanced mode of the modulator.
    */
    typedef enum _modulator_ifmode
    {
        m_ifmode_unknown                = 0,      ///< unknown mode.
        m_ifmode_disable                = 1,      ///< disable mode.
        m_ifmode_if_freq                = 2,      ///< if frequency mode. debug used.
        m_ifmode_iq_offset              = 3,      ///< IQ offset mode. used to the frequency has a decimal point status.
        m_ifmode_min                    = m_ifmode_unknown,
        m_ifmode_max                    = m_ifmode_iq_offset,
    }modulator_ifmode;

    /**
    * @brief modulator basic parameter of the modulator.
    */
    typedef struct _modulator_base_parm
    {
        modulator_type type;                      ///< modulator type.
        modulator_ifmode ifmode;                  ///< DAC advanced mode.
        uint32_t iffreq;                          ///< offset frequency when modulator_ifmode isn't m_ifmode_disable.
        uint32_t dacgain;                         ///< DAC output gain value when modulator_ifmode is m_ifmode_disable.
        uint32_t bw_sb;                           ///< bandwidth of the modulator.
    }modulator_base_parm, *Pmodulator_base_parm;
    
    /* modulator (advanced) */
    typedef void* Pmodulator_advanced_parm;
    
    /**
    * @brief dvb-t constellation parameter.
    */
    typedef enum _dvbt_constellation
    {
        dvbt_constellation_unknown      = 0,      ///< unknown modulation.
        dvbt_constellation_qpsk         = 1,      ///< QPSK modulation.
        dvbt_constellation_qam16        = 2,      ///< QAM16 modulation.
        dvbt_constellation_qam64        = 3,      ///< QAM64 modulation.
        dvbt_constellation_min          = dvbt_constellation_unknown,
        dvbt_constellation_max          = dvbt_constellation_qam64,
    }dvbt_constellation;

    /**
    * @brief dvb-t fast fourier transform(FFT) parameter.
    */
    typedef enum _dvbt_fft
    {
        dvbt_fft_unknown                = 0,      ///< unknown FFT.
        dvbt_fft_2k                     = 1,      ///< 2K FFT.
        dvbt_fft_4k                     = 2,      ///< 4K FFT.
        dvbt_fft_8k                     = 3,      ///< 8K FFT.
        dvbt_fft_min                    = dvbt_fft_unknown,
        dvbt_fft_max                    = dvbt_fft_8k,
    }dvbt_fft;

    /**
    * @brief dvb-t guard interval parameter.
    */
    typedef enum _dvbt_guardinterval
    {
        dvbt_guardinterval_unknown      = 0,      ///< unknown GI.
        dvbt_guardinterval_1_32         = 1,      ///< 1/32 GI.
        dvbt_guardinterval_1_16         = 2,      ///< 1/16 GI.
        dvbt_guardinterval_1_8          = 3,      ///< 1/8 GI.
        dvbt_guardinterval_1_4          = 4,      ///< 1/4 GI.
        dvbt_guardinterval_min          = dvbt_guardinterval_unknown,
        dvbt_guardinterval_max          = dvbt_guardinterval_1_4,
    }dvbt_guardinterval;

    /**
    * @brief dvb-t code rate parameter.
    */
    typedef enum _dvbt_coderate
    {
        dvbt_coderate_unknown           = 0,      ///< unknown code rate.
        dvbt_coderate_1_2               = 1,      ///< each 2 bits have a redundant bit.
        dvbt_coderate_2_3               = 2,      ///< each 3 bits have a redundant bit.
        dvbt_coderate_3_4               = 3,      ///< each 4 bits have a redundant bit.
        dvbt_coderate_5_6               = 4,      ///< each 6 bits have a redundant bit.
        dvbt_coderate_7_8               = 5,      ///< each 8 bits have a redundant bit.
        dvbt_coderate_min               = dvbt_coderate_unknown,
        dvbt_coderate_max               = dvbt_coderate_7_8,
    }dvbt_coderate;
        
    /**
    * @brief dvb-t parameter of the modulator.
    */
    typedef struct _modulator_dvbt_parm
    {
        dvbt_constellation constellation;         ///< dvb-t constellation parameter.
        dvbt_fft fft;                             ///< dvb-t FFT parameter.
        dvbt_guardinterval guardinterval;         ///< dvb-t GI parameter.
        dvbt_coderate coderate;                   ///< dvb-t code rate parameter.
    }modulator_dvbt_parm, *Pmodulator_dvbt_parm;

    /**
    * @brief j83a constellation parameter.
    */
    typedef enum _j83a_constellation
    {
        j83a_constellation_unknown      = 0,      ///< unknown modulation.
        j83a_constellation_qam16        = 1,      ///< QAM16 modulation.
        j83a_constellation_qam32        = 2,      ///< QAM32 modulation.
        j83a_constellation_qam64        = 3,      ///< QAM64 modulation.
        j83a_constellation_qam128       = 4,      ///< QAM128 modulation.
        j83a_constellation_qam256       = 5,      ///< QAM256 modulation.
        j83a_constellation_min          = j83a_constellation_unknown,
        j83a_constellation_max          = j83a_constellation_qam256,
    }j83a_constellation;

    /**
    * @brief j83a parameter of the modulator.
    */
    typedef struct _modulator_j83a_parm
    {
        j83a_constellation constellation;
    }modulator_j83a_parm, *Pmodulator_j83a_parm;

    /**
    * @brief atsc constellation parameter.
    */
    typedef enum _atsc_constellation
    {
        atsc_constellation_unknown      = 0,      ///< unknown modulation.
        atsc_constellation_8vsb         = 1,      ///< 8vsb modulation.
        atsc_constellation_min          = atsc_constellation_unknown,
        atsc_constellation_max          = atsc_constellation_8vsb,
    }atsc_constellation;
        
    /**
    * @brief atsc parameter of the modulator.
    */
    typedef struct _modulator_atsc_parm
    {
        atsc_constellation constellation;         ///< atsc constellation parameter.
    }modulator_atsc_parm, *Pmodulator_atsc_parm;

    /**
    * @brief j83b constellation parameter.
    */
    typedef enum _j83b_constellation
    {
        j83b_constellation_unknown      = 0,      ///< unknown modulation.
        j83b_constellation_qam64        = 1,      ///< QAM64 modulation.
        j83b_constellation_qam256       = 2,      ///< QAM256 modulation.
        j83b_constellation_min          = j83b_constellation_unknown,
        j83b_constellation_max          = j83b_constellation_qam256,
    }j83b_constellation;

    /**
    * @brief j83b parameter of the modulator.
    */
    typedef struct _modulator_j83b_parm
    {
        j83b_constellation constellation;         ///< j83b constellation parameter.
    }modulator_j83b_parm, *Pmodulator_j83b_parm;

    /**
    * @brief dtmb constellation parameter.
    */
    typedef enum _dtmb_constellation
    {
        dtmb_constellation_unknown      = 0,      ///< unknown modulation.
        dtmb_constellation_qpsk         = 1,      ///< QPSK modulation.
        dtmb_constellation_qam4_nr      = 2,      ///< QAM4-NR modulation.
        dtmb_constellation_qam16        = 3,      ///< QAM16 modulation.
        dtmb_constellation_qam32        = 4,      ///< QAM32 modulation.
        dtmb_constellation_qam64        = 5,      ///< QAM64 modulation.
        dtmb_constellation_min          = dtmb_constellation_unknown,
        dtmb_constellation_max          = dtmb_constellation_qam64,
    }dtmb_constellation;

    /**
    * @brief dtmb time interleaced parameter.
    */
    typedef enum _dtmb_time_interleaved
    {
        dtmb_time_interleaved_unknown   = 0,      ///< unknown TI.
        dtmb_time_interleaved_disable   = 1,      ///< disable TI.
        dtmb_time_interleaved_240       = 2,      ///< 240 ms.
        dtmb_time_interleaved_720       = 3,      ///< 720 ms.
        dtmb_time_interleaved_min       = dtmb_time_interleaved_unknown,
        dtmb_time_interleaved_max       = dtmb_time_interleaved_720,
    }dtmb_time_interleaved;

    /**
    * @brief dtmb code rate parameter.
    */
    typedef enum _dtmb_coderate
    {
        dtmb_coderate_unknown           = 0,      ///< unknown code rate.
        dtmb_coderate_0_4               = 1,      ///< each 5 bits have 3 redundant bit.
        dtmb_coderate_0_6               = 2,      ///< each 5 bits have 2 redundant bit.
        dtmb_coderate_0_8               = 3,      ///< each 8 bits have a redundant bit.
        dtmb_coderate_min               = dtmb_coderate_unknown,
        dtmb_coderate_max               = dtmb_coderate_0_8,
    }dtmb_coderate;

    /**
    * @brief dtmb carrier mode parameter.
    */
    typedef enum _dtmb_carriermode
    {
        dtmb_carriermode_unknown        = 0,      ///< unknown mode.
        dtmb_carriermode_3780           = 1,      ///< 3780 mode.
        dtmb_carriermode_1              = 2,      ///< sigle mode.
        dtmb_carriermode_min            = dtmb_carriermode_unknown,
        dtmb_carriermode_max            = dtmb_carriermode_1,
    }dtmb_carriermode;

    /**
    * @brief dtmb sync frame parameter.
    */
    typedef enum _dtmb_syncframe
    {
        dtmb_syncframe_unknown          = 0,      ///< unknown length.
        dtmb_syncframe_420              = 1,      ///< 420 symbol.
        dtmb_syncframe_595              = 2,      ///< 595 symbol.
        dtmb_syncframe_945              = 3,      ///< 945 symbol.
        dtmb_syncframe_min              = dtmb_syncframe_unknown,
        dtmb_syncframe_max              = dtmb_syncframe_945,
    }dtmb_syncframe;
    
    /**
    * @brief dtmb parameter of the modulator.
    */
    typedef struct _modulator_dtmb_parm
    {
        dtmb_constellation constellation;         ///< dtmb constellation parameter.
        dtmb_time_interleaved ti;                 ///< dtmb TI parameter.
        dtmb_coderate coderate;                   ///< dtmb code rate parameter.
        dtmb_carriermode carriermode;             ///< dtmb carrier mode parameter.
        dtmb_syncframe syncframe;                 ///< dtmb sync frame parameter.
    }modulator_dtmb_parm, *Pmodulator_dtmb_parm;

    /**
    * @brief isdb-t consellation parameter.
    */
    typedef enum _isdbt_constellation
    {
        isdbt_constellation_unknown     = 0,      ///< unknown modulation.
        isdbt_constellation_dqpsk       = 1,      ///< DQPSK modulation.
        isdbt_constellation_qpsk        = 2,      ///< QPSK modulation.
        isdbt_constellation_qam16       = 3,      ///< QAM16 modulation.
        isdbt_constellation_qam64       = 4,      ///< QAM64 modulation.
        isdbt_constellation_min         = isdbt_constellation_unknown,
        isdbt_constellation_max         = isdbt_constellation_qam64,
    }isdbt_constellation;

    /**
    * @brief isdb-t fast fourier transform(FFT) parameter.
    */
    typedef enum _isdbt_fft
    {
        isdbt_fft_unknown               = 0,      ///< unknown FFT.
        isdbt_fft_2k                    = 1,      ///< 2K FFT.
        isdbt_fft_4k                    = 2,      ///< 4K FFT.
        isdbt_fft_8k                    = 3,      ///< 8K FFT.
        isdbt_fft_min                   = isdbt_fft_unknown,
        isdbt_fft_max                   = isdbt_fft_8k,
    }isdbt_fft;

    /**
    * @brief isdb-t guard interval parameter.
    */
    typedef enum _isdbt_guardinterval
    {
        isdbt_guardinterval_unknown     = 0,     ///< unknown GI.
        isdbt_guardinterval_1_32        = 1,     ///< 1/32 GI.
        isdbt_guardinterval_1_16        = 2,     ///< 1/16 GI.
        isdbt_guardinterval_1_8         = 3,     ///< 1/8 GI.
        isdbt_guardinterval_1_4         = 4,     ///< 1/4 GI.
        isdbt_guardinterval_min         = isdbt_guardinterval_unknown,
        isdbt_guardinterval_max         = isdbt_guardinterval_1_4,
    }isdbt_guardinterval;

    /**
    * @brief isdb-t code rate parameter.
    */
    typedef enum _isdbt_coderate
    {
        isdbt_coderate_unknown          = 0,     ///< unknown code rate.
        isdbt_coderate_1_2              = 1,     ///< each 2 bits have a redundant bit.
        isdbt_coderate_2_3              = 2,     ///< each 3 bits have a redundant bit.
        isdbt_coderate_3_4              = 3,     ///< each 4 bits have a redundant bit.
        isdbt_coderate_5_6              = 4,     ///< each 6 bits have a redundant bit.
        isdbt_coderate_7_8              = 5,     ///< each 8 bits have a redundant bit.
        isdbt_coderate_min              = isdbt_coderate_unknown,
        isdbt_coderate_max              = isdbt_coderate_7_8,
    }isdbt_coderate;

    /**
    * @brief isdb-t time interleaved parameter.
    */
    typedef enum _isdbt_time_interleaved
    {
        isdbt_time_interleaved_unknown  = 0,     ///< unknown mode.
        isdbt_time_interleaved_disable  = 1,     ///< disable mode.
        isdbt_time_interleaved_mode1    = 2,     ///< mode 1.
        isdbt_time_interleaved_mode2    = 3,     ///< mode 2.
        isdbt_time_interleaved_mode3    = 4,     ///< mode 3.
        isdbt_time_interleaved_min      = isdbt_time_interleaved_unknown,
        isdbt_time_interleaved_max      = isdbt_time_interleaved_mode3,
    }isdbt_time_interleaved;

    /**
    * @brief isdb-t parameter of the modulator.
    */
    typedef struct _modulator_isdbt_parm
    {
        isdbt_constellation constellation;       ///< isdb-t constellation parameter.
        isdbt_fft fft;                           ///< isdb-t fft parameter.
        isdbt_guardinterval guardinterval;       ///< isdb-t guard interval parameter.
        isdbt_coderate coderate;                 ///< isdb-t code rate parameter.
        isdbt_time_interleaved ti;               ///< isdb-t time interleaved parameter.
        uint8_t ac1_en;                          ///< audio encoder AC1 flag.
        uint8_t ac2_en;                          ///< audio encoder AC2 flag.
        uint8_t emergency_en;                    ///< emergency broadcast flag.
    }modulator_isdbt_parm, *Pmodulator_isdbt_parm;

    /**
    * @brief j83c constellation parameter.
    */
    typedef enum _j83c_constellation
    {
        j83c_constellation_unknown      = 0,     ///< unknown modulation.
        j83c_constellation_qam64        = 1,     ///< QAM64 modulation.
        j83c_constellation_qam256       = 2,     ///< QAM256 modulation.
        j83c_constellation_min          = j83c_constellation_unknown,
        j83c_constellation_max          = j83c_constellation_qam256,
    }j83c_constellation;

    /**
    * @brief j832c parameter of the modulator.
    */
    typedef struct _modulator_j83c_parm          ///< j83c constellation parameter.
    {
        j83c_constellation constellation;
    }modulator_j83c_parm, *Pmodulator_j83c_parm;

    /* dvbt2 parm */
    typedef enum _dvbt2_version
    {
        dvbt2_version_unknown           = 0,
        dvbt2_version_1_1_1             = 1,
        dvbt2_version_1_1_1_lite        = 2,
        dvbt2_version_1_2_1             = 3,
        dvbt2_version_1_2_1_lite        = 4,
        dvbt2_version_1_3_1             = 5,
        dvbt2_version_1_3_1_lite        = 6,
        dvbt2_version_min               = dvbt2_version_unknown,
        dvbt2_version_max               = dvbt2_version_1_3_1_lite,
    }dvbt2_version;

    typedef enum _dvbt2_issy
    {
        dvbt2_issy_unknown              = 0,
        dvbt2_issy_disable              = 1,
        dvbt2_issy_short                = 2,
        dvbt2_issy_long                 = 3,
        dvbt2_issy_min                  = dvbt2_issy_unknown,
        dvbt2_issy_max                  = dvbt2_issy_long,
    }dvbt2_issy;

    typedef enum _dvbt2_nti
    {
        dvbt2_nti_unknown               = 0,
        dvbt2_nti_disable               = 1,
        dvbt2_nti_1                     = 2,
        dvbt2_nti_2                     = 3,
        dvbt2_nti_3                     = 4,
        dvbt2_nti_min                   = dvbt2_nti_unknown,
        dvbt2_nti_max                   = dvbt2_nti_3,
    }dvbt2_nti;

    typedef enum _dvbt2_l1_constellation
    {
        dvbt2_l1_constellation_unknown  = 0,
        dvbt2_l1_constellation_bpsk     = 1,
        dvbt2_l1_constellation_qpsk     = 2,
        dvbt2_l1_constellation_qam16    = 3,
        dvbt2_l1_constellation_qam64    = 4,
        dvbt2_l1_constellation_min      = dvbt2_l1_constellation_unknown,
        dvbt2_l1_constellation_max      = dvbt2_l1_constellation_qam64,
    }dvbt2_l1_constellation;

    typedef enum _dvbt2_plp_constellation
    {
        dvbt2_plp_constellation_unknown = 0,
        dvbt2_plp_constellation_qpsk    = 1,
        dvbt2_plp_constellation_qam16   = 2,
        dvbt2_plp_constellation_qam64   = 3,
        dvbt2_plp_constellation_qam256  = 4,
        dvbt2_plp_constellation_min     = dvbt2_plp_constellation_unknown,
        dvbt2_plp_constellation_max     = dvbt2_plp_constellation_qam256,
    }dvbt2_plp_constellation;

    typedef enum _dvbt2_fft
    {
        dvbt2_fft_unknown               = 0,
        dvbt2_fft_1k                    = 1,
        dvbt2_fft_2k                    = 2,
        dvbt2_fft_4k                    = 3,
        dvbt2_fft_8k                    = 4,
        dvbt2_fft_16k                   = 5,
        dvbt2_fft_32k                   = 6,
        dvbt2_fft_min                   = dvbt2_fft_unknown,
        dvbt2_fft_max                   = dvbt2_fft_32k,
    }dvbt2_fft;

    typedef enum _dvbt2_coderate
    {
        dvbt2_coderate_unknown          = 0,
        dvbt2_coderate_1_3              = 1,
        dvbt2_coderate_2_5              = 2,
        dvbt2_coderate_1_2              = 3,
        dvbt2_coderate_3_5              = 4,
        dvbt2_coderate_2_3              = 5,
        dvbt2_coderate_3_4              = 6,
        dvbt2_coderate_4_5              = 7,
        dvbt2_coderate_5_6              = 8,
        dvbt2_coderate_min              = dvbt2_coderate_unknown,
        dvbt2_coderate_max              = dvbt2_coderate_5_6,
    }dvbt2_coderate;

    typedef enum _dvbt2_guardinterval
    {
        dvbt2_guardinterval_unknown     = 0,
        dvbt2_guardinterval_1_128       = 1,
        dvbt2_guardinterval_1_32        = 2,
        dvbt2_guardinterval_1_16        = 3,
        dvbt2_guardinterval_19_256      = 4,
        dvbt2_guardinterval_19_128      = 5,
        dvbt2_guardinterval_1_8         = 6,
        dvbt2_guardinterval_1_4         = 7,
        dvbt2_guardinterval_min         = dvbt2_guardinterval_unknown,
        dvbt2_guardinterval_max         = dvbt2_guardinterval_1_4,
    }dvbt2_guardinterval;

    typedef enum _dvbt2_pilotpattern
    {
        dvbt2_pilotpattern_unknown      = 0,
        dvbt2_pilotpattern_1            = 1,
        dvbt2_pilotpattern_2            = 2,
        dvbt2_pilotpattern_3            = 3,
        dvbt2_pilotpattern_4            = 4,
        dvbt2_pilotpattern_5            = 5,
        dvbt2_pilotpattern_6            = 6,
        dvbt2_pilotpattern_7            = 7,
        dvbt2_pilotpattern_8            = 8,
        dvbt2_pilotpattern_min          = dvbt2_pilotpattern_unknown,
        dvbt2_pilotpattern_max          = dvbt2_pilotpattern_8,
    }dvbt2_pilotpattern;

    typedef enum _dvbt2_fec
    {
        dvbt2_fec_unknown               = 0,
        dvbt2_fec_16200                 = 1,
        dvbt2_fec_64800                 = 2,
        dvbt2_fec_min                   = dvbt2_fec_unknown,
        dvbt2_fec_max                   = dvbt2_fec_64800,
    }dvbt2_fec;

    typedef struct _modulator_dvbt2_parm
    {
        dvbt2_version version;
        dvbt2_issy issy;
        dvbt2_nti nti;
        dvbt2_l1_constellation l1_constellation;
        dvbt2_plp_constellation plp_constellation;
        dvbt2_fft fft;
        dvbt2_coderate coderate;
        dvbt2_guardinterval guardinterval;
        dvbt2_pilotpattern pilotpattern;
        dvbt2_fec fec;
        uint32_t nid;
        uint32_t sid;
        uint32_t fecbn;
        uint32_t sbn;
        uint8_t extend_carrier_mode_en;
        uint8_t constellation_rotation_en;
        uint8_t input_ts_hem_en;
        uint8_t delete_null_packet_en;
        uint8_t vbr_coding_en;
        uint8_t time_interval_en;
    }modulator_dvbt2_parm, *Pmodulator_dvbt2_parm;

    typedef struct _modulator_sinewave_parm
    {
        uint32_t freq; //kHz
    }modulator_sinewave_parm, *Pmodulator_sinewave_parm;



    /*------------------------ tsp (transform only) ------------------------*/
    /* tsp parm */
    typedef enum _tsp_pcr_mode
    {
        pcr_disable     = 0,        ///< no modify ts stream.
        pcr_adjust      = 1,        ///< insert null packet to align pcr tick.
        pcr_retag       = 2,        ///< remux ts stream to align modulator.
    }tsp_pcr_mode;
    
    typedef struct _ts_input_parm
    {
        uint8_t     serial;
        uint8_t     clk_inverse;
        uint8_t     valid_inverse;
        uint8_t     sync_inverse;
        tsp_pcr_mode    pcr_mode;
    }ts_input_parm, *Pts_input_parm;

    typedef struct _tsp_filter
    {
        uint16_t    pid;
        uint8_t     replace;
        uint16_t    replace_pid;
    }tsp_filter, *Ptsp_filter;
    
    typedef struct _tsp_filter_parm
    {
        uint16_t filter_num;
        tsp_filter filter[MAX_TSP_FILTER_NUM];
    }tsp_filter_parm, *Ptsp_filter_parm;



    /*------------------------ enum (transform only) ------------------------*/
    typedef enum _stream_type
    {
        stream_type_unknown             = 0,
        stream_type_video               = 1,
        stream_type_audio               = 2,
        stream_type_data                = 3,
        stream_type_min                 = stream_type_unknown,
        stream_type_max                 = stream_type_data,
    }stream_type;
    
    typedef struct _enum_stream
    {
        stream_type type;
        uint32_t encode_type; // video_encode_type or audio_encode_type
        uint32_t stream_pid;
        uint32_t stream_type;
        uint32_t esinfo_len;
        uint8_t *esinfo;
    }enum_stream, *Penum_stream;
    
    typedef struct _enum_program
    {
        uint32_t pcr_pid;
        uint32_t pmt_pid;
        uint32_t stream_num;
        Penum_stream stream;
    }enum_program, *Penum_program;
    
    typedef struct _enum_list
    {
        uint32_t ch_spec;
        uint32_t ch_country;
        uint32_t program_num;
        Penum_program program;
    }enum_list, *Penum_list;



    /*------------------------ psispec_rule (both) ------------------------*/
    typedef struct _licensed_table
    {
    	const char* name;
    	uint16_t offset;
    }licensed_table,*Plicensed_table;
    
    typedef struct _licensed_prop
    {
    	const char* name;
    	uint16_t type;
    	uint16_t offset;
    }licensed_prop,*Plicensed_prop;

    typedef struct _psispec_rule
    {
        const licensed_table *rule_tables;
        const licensed_prop *channel_props;
        const licensed_prop *program_props;
        const uint8_t *rule_block;
        uint32_t len_rule_block;
        const uint8_t *program_block;
        uint32_t len_program_block;        
    }psispec_rule, *Ppsispec_rule;

    typedef struct _psip_props
    {
        uint16_t program_number;
        uint16_t source_id;
        uint16_t channel_major;
        uint16_t channel_minor;
        uint8_t  channel_name[32];
        uint32_t mode;
    }psip_props, *Ppsip_props   ;

	/*------------------------ psispec_default (both) ------------------------*/
    typedef void* Ppsispec_default_channel;
    typedef void* Ppsispec_default_program;
    typedef void* Ppsispec_default_stream;
    
	typedef enum _psispec_default_type
	{
		psispec_default_undefine = 0,
		psispec_default_iso         = 0x00010000,
		psispec_default_dvb         = 0x00020000,
		psispec_default_dvb_nordig  = 0x00021000,
		psispec_default_dtmb        = 0x00030000,
		psispec_default_arib        = 0x00040000,
		psispec_default_abnt        = 0x00050000,
		psispec_default_psip        = 0x00060000,
		psispec_default_common      = 0x08000000,
	}psispec_default_type;
	
	typedef enum _psispec_country_code
	{
		psispec_country_undefined = 0,
		dvb_taiwan = psispec_default_dvb,
		dvb_asean,
		dvb_uk,
		dvb_spanish,
		dvb_france,
		dvb_italian,
		dvb_colombia,
		dvb_india,
		dvb_ghana,
		dvb_singapore,
		dvb_georgin,
		nordig_finland = psispec_default_dvb_nordig,
		dtmb_china = psispec_default_dtmb,
		dtmb_hk,
		dtmb_cuba,
		arib_japan = psispec_default_arib,
		abnt_brazil = psispec_default_abnt,
		abnt_argentina,
		abnt_bolivia,
		abnt_botswana,
		abnt_costa_rica,
		abnt_chile,
		abnt_ecuador,
		abnt_paraguay,
		abnt_philippines,
		abnt_peru,
		abnt_uruguay,
		abnt_venezuela,
		atsc_usa = psispec_default_psip,
		atsc_korean,
		atsc_canada,
		atsc_mexico,
		atsc_dominican,
	}psispec_country_code;
    
    typedef union _mux_iso_lang
    {
        uint8_t  raw[4];
        uint32_t val;
    }mux_iso_lang;

    typedef struct _psispec_default_iso13818_channel
    {
        uint16_t    transport_stream_id;
    }psispec_default_iso13818_channel, *Ppsispec_default_iso13818_channel;

    typedef struct _psispec_default_iso13818_program
    {
        uint16_t    program_number;
    }psispec_default_iso13818_program, *Ppsispec_default_iso13818_program;

    typedef struct _vatek_string
    {
        uint32_t len;
        uint8_t* text;
    }vatek_string, *Pvatek_string;

    typedef struct _psispec_default_dvb_channel
    {
        uint16_t    transport_stream_id;
        uint16_t    network_id;
        Pvatek_string network_name;
    }psispec_default_dvb_channel, *Ppsispec_default_dvb_channel;

    typedef struct _psispec_default_dvb_program
    {
        uint16_t    original_network_id;
        uint16_t    program_number;
        uint16_t    channel_no;
        Pvatek_string service_name;
    }psispec_default_dvb_program, *Ppsispec_default_dvb_program;

    typedef enum _arib_abnt_copy
    {
        arib_abnt_free = 0,
        arib_abnt_copyonce = 1,
        arib_abnt_forbidden = 2,
    }arib_abnt_copy;
    
	typedef struct _psispec_default_arib_channel
    {
        uint8_t region_id;
        uint8_t broadcaster_id;
        uint8_t remote_control_key_id;
        Pvatek_string network_name;
    }psispec_default_arib_channel, *Ppsispec_default_arib_channel;

    typedef struct _psispec_default_arib_program
    {
        uint8_t         service_no;
        mux_iso_lang    main_lang;
        mux_iso_lang    sub_lang;
        arib_abnt_copy  copy_flag;
        Pvatek_string   service_name;
        Pvatek_string   ts_name;
    }psispec_default_arib_program, *Ppsispec_default_arib_program;

    typedef struct _psispec_default_abnt_channel
    {
        uint8_t  remote_control_key_id;
        uint8_t  original_network_id;
        uint16_t area_code;
        Pvatek_string network_name;
    }psispec_default_abnt_channel, *Ppsispec_default_abnt_channel;

    typedef struct _psispec_default_abnt_program
    {
        uint8_t         service_no;
        mux_iso_lang    main_lang;
        mux_iso_lang    sub_lang;
        arib_abnt_copy  copy_flag;
        Pvatek_string   service_name;
        Pvatek_string   ts_name;
    }psispec_default_abnt_program, *Ppsispec_default_abnt_program;

    typedef enum _psip_cc_mode
    {
        cc_mode_disabble = 0,
        cc_mode_608 = 1,
    }psip_cc_mode;

    typedef struct _psispec_default_psip_channel
    {
        uint32_t psip_flags;
        uint16_t transport_stream_id;
        uint8_t  gps_utc_offset;
        uint16_t daylight_saving;
        psip_cc_mode  cc_mode;
        Pvatek_string short_name;
    }psispec_default_psip_channel, *Ppsispec_default_psip_channel;

    typedef struct _psispec_default_psip_program
    {
        uint16_t program_number;
        uint16_t source_id;
        uint16_t channel_major;
        uint16_t channel_minor;
        Pvatek_string long_name;
    }psispec_default_psip_program, *Ppsispec_default_psip_program;

    /*------------------------ psispec_default program(transform) ------------------------*/
    typedef struct _psispec_default_program_tr
    {
        uint16_t pcr_pid;
        uint16_t pmt_pid;
        union
        {
            psispec_default_iso13818_program iso_prog;
            psispec_default_dvb_program      dvbt_prog;
            psispec_default_arib_program     arib_prog;
            psispec_default_abnt_program     abnt_prog;
            psispec_default_psip_program     psip_prog;
        }_prog;
    }psispec_default_program_tr, *Ppsispec_default_program_tr;
    
    /*------------------------ psispec_default stream(transform) ------------------------*/
    typedef enum _psispec_default_stream_type
    {
        psi_def_stream_type_video = 0,
        psi_def_stream_type_audio = 1,
        psi_def_stream_type_data  = 2,
    }psispec_default_stream_type;
    
    typedef struct _video_param
    {
        video_encode_type         type;
        video_input_resolution    resolution;
        video_input_aspectrate    aspectrate;
    }video_param, *Pvideo_param;
    
    typedef struct _audio_param
    {
        audio_encode_type         type;
        audio_input_samplerate    samplerate;
        audio_encode_channel      channel;
    }audio_param, *Paudio_param;
    
    typedef struct _data_param
    {
        uint32_t    resv;
    }data_param, *Pdata_param;
    
    typedef struct _psispec_default_stream_tr
    {
        psispec_default_stream_type type;
        uint32_t    stream_pid;
        uint32_t    es_type;
        uint32_t    es_len;
        union 
        {
            Pvideo_param video_param;
            Paudio_param audio_param;
            Pdata_param  data_param;
        }_stream;
        uint8_t*    es;
    }psispec_default_stream_tr, *Ppsispec_default_stream_tr;
	

#ifdef __cplusplus
    }
#endif

#endif

