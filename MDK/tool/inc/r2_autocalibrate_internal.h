
#ifndef _R2_AUTOCALIBRATE_INTERNAL_
#define _R2_AUTOCALIBRATE_INTERNAL_

#ifdef __cplusplus
    extern "C" {
#endif

    #define R2_TUNE_BUF_LENGTH              64

    #define FC_READ_R2_TUNE                 0xBA
    #define FC_READ_R2_TUNE_ACK             0xBB
    #define R2_TUNE_MODULATION				0x20
    	#define R2_TUNE_MODULATION_GLOBAL   1
    	#define R2_TUNE_MODULATION_J83B		2
    #define R2_TUNE_R2_REG					0x21
    #define R2_TUNE_START_TABLE				0x22
    #define R2_TUNE_TABLE_RULE				0x23
    #define R2_TUNE_END_TABLE				0x24
    #define R2_TUNE_FINISH					0x2F

    typedef enum _r2_tune_status
    {
        r2_tune_status_idle = 0,
        r2_tune_status_setfreq,
        r2_tune_status_setcalibrate,
        r2_tune_status_startsavetable,
        r2_tune_status_puttable,
        r2_tune_status_endsavetable,
        r2_tune_status_finish,
    }r2_tune_status;

    typedef struct _r2_tune_parm
    {
        r2_tune_status  status;
        uint8_t path_tag;
        uint32_t frequency;
        uint16_t dac_gain;
        uint32_t i_offset;
        uint32_t q_offset;
        uint32_t image_offset;
        uint32_t phase_offset;
        uint32_t gpio;
        uint32_t gain;
    }r2_tune_parm, *Pr2_tune_parm;

    #define R2AUTOK_ERR(fmt,...)    printf("error [%s-%d]"fmt"\r\n",__func__ ,__LINE__,##__VA_ARGS__)
    #define R2AUTOK_LOG(fmt,...)    printf(fmt"\r\n",##__VA_ARGS__)
    

#ifdef __cplusplus
    }
#endif

#endif

