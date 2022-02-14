
#ifndef _VATEK_DEFINE_PERIPHRAL_
#define _VATEK_DEFINE_PERIPHRAL_

    #include <stdint.h>

#ifdef __cplusplus
    extern "C" {
#endif

    /*------------------------ phy ------------------------ */
    typedef enum _phy_type
    {
        phy_type_unknown                = 0,
        phy_type_ep9555e                = 1,
        phy_type_ep9351                 = 2,
        phy_type_adv7182a_cvbs          = 3,
        phy_type_adv7182a_ypbpr         = 4,
        phy_type_adv7180                = 5,
        phy_type_adv7611                = 6,
        phy_type_h1                     = 7,
        phy_type_min                    = phy_type_unknown,
        phy_type_max                    = phy_type_h1, 
    }phy_type;

    typedef enum _phy_status
    {
        phy_status_unknown              = 0,
        phy_status_lost                 = 1,
        phy_status_active               = 2,
        phy_status_fail                 = 3,
    }phy_status;
    
    typedef enum _phy_output_mode
    {
        phy_output_bypass               = 0,
        phy_output_i_to_p               = 1,
        phy_output_p_to_i               = 2,
    }phy_output_mode;
    
    typedef enum _phy_output_baseclock
    {
        phy_baseclk_1000                = 0,
        phy_baseclk_1001                = 1,
    }phy_output_baseclock;

    typedef struct _phy_video_info
    {
        video_input_resolution resolution;
        video_input_aspectrate aspectrate;
        uint8_t buswidth_16;
        uint8_t separated_sync;
    }phy_video_info, *Pphy_video_info;

    typedef struct _phy_audio_info
    {
        audio_input_samplerate samplerate;
    }phy_audio_info, *Pphy_audio_info;
    
    /*------------------------ rf ------------------------ */
    typedef enum _rf_type
    {
        rf_type_unknown                 = 0,
        rf_type_r2                      = 1,
        rf_type_r2_via_vatek            = 2,
        rf_type_min                     = rf_type_unknown,
        rf_type_max                     = rf_type_r2_via_vatek,    
    }rf_type;

    typedef enum _rf_status
    {
        rf_status_unknown               = 0,
        rf_status_idle                  = 1,
        rf_status_active                = 2,
        rf_status_fail                  = 3,
    }rf_status;

    typedef void* Prf_calibrate;

    /* r2 */
    typedef struct _gpio_pin
    {
        void * port;
        uint16_t index;
    }gpio_pin, *Pgpio_pin;
    
    typedef struct _r2_board_handle
    {
        gpio_pin r2_cs;
        gpio_pin r2_clk;
        gpio_pin r2_mosi;
        gpio_pin r2_miso;
    }r2_board_handle, *Pr2_board_handle;

    typedef struct _r2_calibrate
    {   
        uint32_t i_offset;
        uint32_t q_offset;
        uint32_t image_offset;
        uint32_t phase_offset;
        uint32_t gpio;
        uint32_t gain;
    }r2_calibrate, *Pr2_calibrate;



    /*------------------------ tuner ------------------------ */
    typedef enum _tuner_type
    {
        tuner_type_unknown              = 0,
        tuner_type_mxl608               = 1,
        tuner_type_mxl603               = 2,
        tuner_type_min                  = tuner_type_unknown,
        tuner_type_max                  = tuner_type_mxl603,    
    }tuner_type;

    typedef enum _tuner_status
    {
        tuner_status_unknown            = 0,
        tuner_status_lock               = 1,
        tuner_status_unlock             = 2,
    }tuner_status;

    typedef enum _tuner_modulator_type
    {
        tm_type_unknown                 = 0,
        tm_type_dvbc                    = 1,
        tm_type_dvbtx                   = 2,
        tm_type_dtmb                    = 3,
        tm_type_atsc                    = 4,
        tm_type_isdbt                   = 5,
        tm_type_j83b                    = 6,
    }tuner_modulator_type;
        
    typedef vatek_result(*fpdemod_i2cbypass)(void* hdemod, uint8_t bypasson);
    typedef struct _tuner_board_handle
    {
        Pboard_handle hboard;
        void* hdemod;
        fpdemod_i2cbypass i2cbypass;
    }tuner_board_handle, *Ptuner_board_handle;
    
    typedef struct _tuner_lockparm
    {
        tuner_modulator_type type;
        uint32_t bandwidth; //kHz
        uint32_t freq; //kHz
    }tuner_lockparm, *Ptuner_lockparm;



    /*------------------------ demod ------------------------ */
    typedef enum _demod_type
    {
        demod_type_unknown              = 0,
        demod_type_avl68xx              = 1,
        demod_type_lgdt3305             = 2,
        demod_type_min                  = demod_type_unknown,
        demod_type_max                  = demod_type_lgdt3305,    
    }demod_type;

    typedef enum _demod_modulator_type
    {
        dm_type_unknown                 = 0,
        dm_type_dvbc                    = 1,
        dm_type_dvbtx                   = 2,
        dm_type_dvbsx                   = 3,
        dm_type_isdbt                   = 4,
        dm_type_atsc                    = 5,
    }demod_modulator_type;

    typedef enum _demod_status
    {
        demod_status_unknown            = 0,
        demod_status_lock               = 1,
        demod_status_unlock             = 2,
    }demod_status;

    typedef struct _demod_lockparm
    {
        demod_modulator_type type;
        uint32_t bandwidth; //kHz
    }demod_lockparm, *Pdemod_lockparm;
        
#ifdef __cplusplus
    }
#endif

#endif

