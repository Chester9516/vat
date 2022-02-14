
#ifndef _RF_R2_INTERNAL_
#define _RF_R2_INTERNAL_

#ifdef __cplusplus
    extern "C" {
#endif
    
    #define MAX_R2_FREQ                 1000000 //1000M Hz
    #define MIN_R2_FREQ                 50000   //50M Hz
    
    #define R2_ERR(fmt,...)             VATEK_ERR(r2,fmt,##__VA_ARGS__)
    #define R2_LOG(fmt,...)             VATEK_LOG(r2,fmt,##__VA_ARGS__)
    
    #define R2_GAIN_SIGNAL		        0x2393
    #define R2_I_SIGNAL_OFFSET	        0x03
    #define R2_Q_SIGNAL_OFFSET	        0x03
    #define R2_IQ_IMAGE_OFFSET	        0x21
    #define R2_IQ_PHASE_OFFSET	        0x02
    
    //RF R2 Register Map
    #define R2_REG4_MX_PA_GAIN          0x04
    #define R2_REG5_RF_FILTER           0x05
    #define R2_REG6_DATA_SWAP           0x06
    #define R2_REG7_SOFT_RESET          0x07
    #define R2_REG0E_RF_STATUS          0x0e
    #define R2_REG13_I_CALIBRATE        0x13
    #define R2_REG14_Q_CALIBRATE        0x14
    #define R2_REG15_IMAGE_CALIBRATE    0x15
    #define R2_REG16_PHASE_CALIBRATE    0x16
    
    
    //Register control RF filter
    #define R2_REG5_MASK                0x3F
    #define R2_REG5_GPIO0               0x01
    #define R2_REG5_GPIO1               0x02
    #define R2_REG5_GPIO2               0x04
    #define R2_REG5_GPIO3               0x08
    #define R2_REG5_GPIO4               0x10
    #define R2_REG5_GPIO5               0x20
    
    #define R2_PA_MIXER_GAIN_MAX        0x33F3
    #define R2_PA_MIXER_GAIN_MIN        0x0303
    #define R2_I_OFFSET_MAX             0xFF
    #define R2_I_OFFSET_MIN             0
    #define R2_Q_OFFSET_MAX             0xFF
    #define R2_Q_OFFSET_MIN             0
    #define R2_IMAGE_OFFSET_MAX         0x3F
    #define R2_IMAGE_OFFSET_MIN         0
    #define R2_PHASE_OFFSET_MAX         0x1F
    #define R2_PHASE_OFFSET_MIN         0
    
    typedef struct _r2_handle
    {
        Pvatek_gpio vgpio;
        Pr2_board_handle pin;
        rf_status status;
        uint8_t en_calibrate;
        r2_calibrate calibrate;
    }r2_handle,*Pr2_handle;
    
    typedef struct _r2_calibration_
    {
        uint16_t min_freq;
        uint16_t max_freq;
        uint16_t pagain;
        uint16_t gpio;
    }r2_calibration,*Pr2_calibration;

#ifdef __cplusplus
    }
#endif

#endif

