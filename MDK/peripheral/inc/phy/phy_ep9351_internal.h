
#ifndef _PHY_EP9351_INTERNAL_
#define _PHY_EP9351_INTERNAL_

#ifdef __cplusplus
extern "C" {
#endif

    #define EP9351_INVALID_RESET_COUNT                  10
    
    #define EP9351_ADDRESS                              0x78
    
    #define EP9351_HDMI_HDCP_KEY_AREA				    0x00
    #define EP9351_HDMI_HDCP_BKSV					    0x28
    
    
    #define EP9351_HDMI_INT							    0x29
        #define HDMI_INT_POL_ACTIVE_HIGH	            0x80
        #define HDMI_INT_AVMC					        0x40
        #define HDMI_INT_AVMS					        0x20
        #define HDMI_INT_SEL2					        0x10
        #define HDMI_INT_SEL1					        0x08
        #define HDMI_INT_MS						        0x04
        #define HDMI_INT_ADO					        0x02
        #define HDMI_INT_AVI					        0x01
    
    #define EP9351_AVI_INFOFRAME					    0x2A	
    #define EP9351_ADO_INFOFRAME					    0x2B	
    #define EP9351_MS_INFOFRAME						    0x2C	
    #define EP9351_SELECT_PACKET_1					    0x2D	
    #define EP9351_SELECT_PACKET_2					    0x2E
        
    #if 0
    #define EP9351_M0_Register						    0x2F
    #define EP9351_BCAP_STATUS_REGISTER			        0x30
    #define EP9351_Bcap_Register					    0x00
        #define EP9351_Bstatus_Register_H				0x01
        #define EP9351_Bstatus_Register_L				0x02
    
    
    #define EP9351_V1_Register						    0x31
    
    #define EP9351_KSV0								    0x32
    #define EP9351_KSV1								    0x33
    #define EP9351_KSV2								    0x34
    #define EP9351_KSV3								    0x35
    #endif
    
    
    #define EP9351_TIMING_REGISTERS				        0x3B
    
    #define EP9351_STATUS_REGISTER_0				    0x3C
        #define STATUS_REGISTER_0_AVMUTE		        0x20
        #define STATUS_REGISTER_0_HDMI			        0x10
        #define STATUS_REGISTER_0_AUTH			        0x08
        #define STATUS_REGISTER_0_ENC_EN		        0x04
        #define STATUS_REGISTER_0_DST_double	        0x02
        #define STATUS_REGISTER_0_LAYOUT		        0x01
    
    #define EP9351_STATUS_REGISTER_1			        0x3D
        #define STATUS_REGISTER_1_LINK_ON		        0x80
        #define STATUS_REGISTER_1_DE_VALID		        0x40
        #define STATUS_REGISTER_1_A_UF			        0x10
        #define STATUS_REGISTER_1_A_OF			        0x08
    
    #define EP9351_SPDIF_CHANNEL_STATUS				    0x3E
    
    #define EP9351_EE_CHECKSUM					        0x3F
    
    #define EP9351_GENERAL_CONTROL_0                    0x40
        #define GENEROL_CONTROL_0_HDCP_RST		        0x10
        #define GENEROL_CONTROL_0_DE_RST		        0x20
        #define GENEROL_CONTROL_0_DDC_DIS		        0x40
        #define GENEROL_CONTROL_0_RST			        0x08
        #define GENEROL_CONTROL_0_PWR_DWN		        0x04
        #define GENEROL_CONTROL_0_ON_CHIP_EDID	        0x02
    
    #define EP9351_GENERAL_CONTROL_1				    0x41
        #define GENERAL_CONTROL_1_V_POL_RW              0x80
        #define GENERAL_CONTROL_1_H_POL_RW              0x40
    
    #define EP9351_GENERAL_CONTROL_2				    0x42
        #define GENERAL_CONTROL_2_OUTFORMAT_422         0x80
        #define GENERAL_CONTROL_2_INFORMAT_422          0x40
        #define GENERAL_CONTROL_2_OUTCOLOR_YCC          0x20
        #define GENERAL_CONTROL_2_INCOLOR_YCC           0x10
        #define GENERAL_CONTROL_2_FULLRANGE             0x08
        #define GENERAL_CONTROL_2_ITU709                0x04
        
        
    #define EP9351_GENERAL_CONTROL_3				    0x43
    	#define GENERAL_CONTROL_3_A_MUTE		        0x01
    	#define GENERAL_CONTROL_3_V_MUTE		        0x02
        
    #define EP9351_GENERAL_CONTROL_4				    0x44	
    	#define GENERAL_CONTROL_4_SFREQ_R	            0x07
    	#define GENERAL_CONTROL_4_LNK_ON_RST	        0x80
    
    #define EP9351_GENERAL_CONTROL_5				    0x45
    #define EP9351_GENERAL_CONTROL_6				    0x46	
    /*
    #define EP9351_General_Control_7			0x47
    */
    #define EP9351_GENERAL_CONTROL_8				    0x48
        #define GENERAL_CONTROL_8_EE_DIS		        0x04
        
    #define EP9351_RX_PHY_CONTROL_REGISTER		        0x49
    #define EP9351_EDID_DATA_REGISTER				    0xFF

    #define EP9351_WRITE_ADDR                           EP9351_ADDRESS
    #define EP9351_READ_ADDR                            (EP9351_ADDRESS | 1)
    #define EP9351_GC_1_IO_DISABLE                      0x0F
    #define EP9351_SR_1_LINKED_ON                       (STATUS_REGISTER_1_LINK_ON | STATUS_REGISTER_1_DE_VALID)
    #define EP9351_GC_4_OUTPUT                          (GENERAL_CONTROL_2_OUTFORMAT_422 | GENERAL_CONTROL_2_OUTCOLOR_YCC)

    #define EP9351_AUDIO_SAMPLERATE_32KHZ               0
    #define EP9351_AUDIO_SAMPLERATE_44_1KHZ             1
    #define EP9351_AUDIO_SAMPLERATE_48KHZ               2

    /* CEA-861-D P65 video info */
    
    #define Y_COLOR_RGB         0
    #define Y_COLOR_YCC_422     1
    #define Y_COLOR_YCC_444     2
    
    #define M_ASPECT_4_3        1
    #define M_ASPECT_16_9       2
    
    #define C_COLORSPACE_ITU601 1
    #define C_COLORSPACE_ITU709 2
    #define C_COLORSPACE_EXT    3
    
    #define Q_COLORRANGE_LIMIT  1
    #define Q_COLORRANGE_FULL   2
    
    #define EC_COLORSPACE_CC601 0
    #define EC_COLORSPACE_CC709 1
    
    /* 
    CEA-861  
        3 Format Note: For 60 Hz formats, displays respond automatically to either 60 Hz or 59.94 Hz (same format with 
        slightly different clocks). The 25.2 MHz pixel frequency value is within the ± 0.5% (allowed in the VESA DMT [10]) of 
        25.175 MHz. The 480p formats and the 480i formats are typically 59.94 Hz, and the HDTV formats are typically 60 
        Hz. 
    */

    #define EP9351_ERR(fmt,...)                     VATEK_ERR(ep9351,fmt,##__VA_ARGS__)
    #define EP9351_LOG(fmt,...)                     VATEK_LOG(ep9351,fmt,##__VA_ARGS__)
    
    typedef enum _ep9351_status
    {
        epstatus_invalid = -2,
        epstatus_hw_fail = -1,
        epstatus_idle = 0,
        epstatus_active = 1,
        epstatus_active_protect = 2,
    }ep9351_status;

    typedef enum _ep9351_baseclock
    {
        baseclock_1000 = 0,
        baseclock_1001 = 1,
    }ep9351_baseclock;

    typedef struct _ep9351_handle
    {
        Pvatek_i2c vi2c;
        ep9351_baseclock baseclk;
    }ep9351_handle,*Pep9351_handle;
    
#ifdef __cplusplus
}
#endif

#endif

