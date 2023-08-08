
#ifndef _PHY_ADV7182A_INTERNAL_
#define _PHY_ADV7182A_INTERNAL_

#ifdef __cplusplus
        extern "C" {
#endif

    //if ALSB pin low
    /*
    #define ADV7182A_SLAVE_WRITE      0x40
    #define ADV7182A_SLAVE_READ       (ADV7182A_SLAVE_WRITE | 1)
    */
    //if ALSB pin high
    #define ADV7182A_SLAVE_WRITE      0x42 //!!!
    #define ADV7182A_SLAVE_READ       (ADV7182A_SLAVE_WRITE | 1)
    
    /* Main Register Map Details(User Map) */
    #define ADV7182A_INPUT_CONTROL			0x00
    #define ADV7182A_VIDEO_SELECT1			0x01
    #define ADV7182A_VIDEO_SELECT2			0x02
    #define ADV7182A_OUTPUT_CONTROL			0x03
    #define ADV7182A_OUTPUT_CONTROL_SEL				0x40		//output pins drivers 0:enable / 1:tristated
    #define ADV7182A_EXT_OUTPUT_CONTROL		0x04
        #define ADV7182A_EXT_OUTPUT_CONTROL_RANGE	0x01		//0:ITU-R BT.656 / 1:Extended range
        #define ADV7182A_EXT_OUTPUT_CONTROL_TIM_OE	0x08		//HS, VS, FIELD 0:tristated / 1:forced active
        #define ADV7182A_EXT_OUTPUT_CONTROL_BT656	0x80 		//ITU-R 0:BT.656-3 / 1:656-4
    #define ADV7182A_AUTODETECT_EN			0x07
    #define ADV7182A_CONTRAST				0x08
    #define ADV7182A_BRIGHTNESS_ADJ			0x0A
    #define ADV7182A_HUE_ADJ				0x0B
    #define ADV7182A_DEF_VALUE_Y			0x0C
    	#define ADV7182A_DEF_VALUE_Y_FREERUN		0x02		//0:Disables freerun mode / 1:Enables automatic freerun mode
    #define ADV7182A_DEF_VALUE_C			0x0D
    #define ADV7182A_ADIControl1			0x0E
    	#define ADV7182A_ADIControl1_SUB_USR_EN		0x60		//0:main register / 1:User Sub Map-interrupt VDP register / 2:User Sub Map2
    #define ADV7182A_POWER_MANAGEMENT		0x0F
    	#define ADV7182A_POWER_MANAGEMENT_PWRDWN	0x20		//0:System functional / 1:Powered down
    	#define ADV7182A_POWER_MANAGEMENT_RST		0x80		//0:normal / 1:reset chip
    #define ADV7182A_STATUS1				0x10
    	#define ADV7182A_STATUS1_IN_LOCK			0x01	
    	#define ADV7182A_STATUS1_LOST_LOCK			0x02
    	#define ADV7182A_STATUS1_FSC_LOCK			0x04
    	#define ADV7182A_STATUS1_FOLLOW_PW			0x08
    	#define ADV7182A_STATUS1_AD_RESULT			0x70
    	#define ADV7182A_STATUS1_COLOR_KILL			0x80
    #define ADV7182A_IDENTIFICATION 		0x11				//RO (Read only)
    #define ADV7182A_STATUS2				0x12
    	#define ADV7182A_STATUS2_LL_NSTD			0x10
    	#define ADV7182A_STATUS2_FSC_NSTD			0x20
    #define ADV7182A_STATUS3				0x13
    	#define ADV7182A_STATUS3_INST_HLOCK			0x01
    	#define ADV7182A_STATUS3_SD_OP_50Hz			0x04		//0:SD 60Hz / 1:SD 50Hz
    	#define ADV7182A_STATUS3_FreeRunACT			0x10
    	#define ADV7182A_STATUS3_STD_FLD_LEN		0x20
    	#define ADV7182A_STATUS3_Interlace			0x40
    #define ADV7182A_ANALOG_CLMP_CNT		0x14
    	#define ADV7182A_ANALOG_CLMP_CNT_CCLEN		0x10		//0: Current sources switched off / 1:Current sources enabled
    #define ADV7182A_DIGITAL_CLMP_CNT1		0x15
    #define ADV7182A_SHAPING_FILTER_CNT1	0x17
    #define ADV7182A_SHAPING_FILTER_CNT2	0x18
    #define ADV7182A_COMB_FILTER_CNT		0x19
    #define ADV7182A_ADI_CNT2				0x1D
    	#define ADV7182A_ADI_CNT2_LLC				0x80		//0:LLC pin active / 1:LLC pin tristated
    #define ADV7182A_PIXEL_DELAY_CNT		0x27
    #define ADV7182A_MISC_GAIN_CNT			0x2B
    #define ADV7182A_AGC_GAIN_CNT			0x2C
    #define ADV7182A_CHROMA_GAIN_CNT1		0x2D
    #define ADV7182A_CHROMA_GAIN_CNT2		0x2E
    #define ADV7182A_LUMA_GAIN_CNT1			0x2F
    #define ADV7182A_LUMA_GAIN_CNT2			0x30
    #define ADV7182A_V_F_Control1			0x31
    	#define ADV7182A_V_F_Control1_HVSTIM		0x08
    	#define ADV7182A_V_F_Control1_NEWAVMODE		0x10
    #define ADV7182A_V_F_Control2			0x32
    #define ADV7182A_V_F_Control3			0x33
    #define ADV7182A_HS_PosControl1			0x34
    #define ADV7182A_HS_PosControl2			0x35
    #define ADV7182A_HS_PosControl3			0x36
    #define ADV7182A_Polarity				0x37
    #define ADV7182A_NTSC_COMB_CNT			0x38
    #define ADV7182A_PAL_COMB_CNT			0x39
    #define ADV7182A_ADC_CNT				0x3A
    #define ADV7182A_MANUAL					0x3D
    #define ADV7182A_RESAMPLE_CNT			0x41
    #define ADV7182A_CTIDNR_CNT1			0x4D
    #define ADV7182A_CTIDNR_CNT2			0x4E
    #define ADV7182A_DNR_NOISE_THOLD1		0x50
    #define ADV7182A_LOCK_COUNT				0x51
    	#define ADV7182A_LOCK_COUNT_CIL				0x07
    	#define ADV7182A_LOCK_COUNT_COL				0x31
    	#define ADV7182A_LOCK_COUNT_SRLS			0x40
    	#define ADV7182A_LOCK_COUNT_FSCLE			0x80
    #define ADV7182A_ADC_SWITCH3			0x60
    #define ADV7182A_OUTPUT_SYNC_SELE1		0x6A
    	#define ADV7182A_OUTPUT_SYNC_SELE1_HS_PIN	0x07		// 0:HSYNC / 1:VSYNC / 2:FIELD / 3:DE / 4:SFL
    #define ADV7182A_OUTPUT_SYNC_SELE2		0x6B
    	#define ADV7182A_OUTPUT_SYNC_SELE2_FLD_PIN	0x07		// 0:HSYNC / 1:VSYNC / 2:FIELD / 3:DE / 4:SFL	
    #define ADV7182A_FREERUN_LINE_LENG1		0x8F
    	#define ADV7182A_FREERUN_LINE_LENG1_CLOCK_LLC	0x70	// 0:LLC 27MHz / 5:LLC 13.5MHz
    #define ADV7182A_CCAP1					0x99
    #define ADV7182A_CCAP2					0x9A
    #define ADV7182A_LB_LCT					0x9B
    #define ADV7182A_LB_LCM					0x9C
    #define ADV7182A_LB_LCB					0x9D
    #define ADV7182A_CRC_EN					0xB2
    #define ADV7182A_ADC_SWITCH1			0xC3
    	#define ADV7182A_ADC_SWITCH1_MUX0			0x07		// 0:no connect / 1:Ain1 / 2:Ain2 / 3:Ain3 / 4:Ain4
    	#define ADV7182A_ADC_SWITCH1_MUX1			0x70		// 0:no connect / 1:no connect / 2:Ain2 / 3:no connect / 4:Ain4
    #define ADV7182A_ADC_SWITCH2			0xC4	
    	#define ADV7182A_ADC_SWITCH2_MUX2			0x07		// 0:no connect / 1:no connect / 2:Ain2 / 3:Ain3 / 4:no connect
    	#define ADV7182A_ADC_SWITCH2_MUX_EN			0x80		// 0:disable / 1:enable
    #define ADV7182A_LB_CNT1				0xDC
    #define ADV7182A_LB_CNT2				0xDD
    #define ADV7182A_ST_NOISE_RD1			0xDE
    #define ADV7182A_ST_NOISE_RD2			0xDF
    #define ADV7182A_SD_OFFSET_Cb			0xE1
    #define ADV7182A_SD_OFFSET_Cr			0xE2
    #define ADV7182A_SD_SATURA_Cb			0xE3
    #define ADV7182A_SD_SATURA_Cr			0xE4
    #define ADV7182A_NTSC_V_Begin			0xE5
    #define ADV7182A_NTSC_V_End				0xE6
    #define ADV7182A_NTSC_F_Toggle			0xE7
    #define ADV7182A_PAL_V_Begin			0xE8
    #define ADV7182A_PAL_V_End				0xE9
    #define ADV7182A_PAL_F_Toggle			0xEA
    #define ADV7182A_VBlankControl1			0xEB
    #define ADV7182A_VBlankControl2			0xEC
    #define ADV7182A_AFE_CNT1				0xF3
    	#define ADV7182A_AFE_CNT1_FILTER1			0x01
    	#define ADV7182A_AFE_CNT1_FILTER2			0x02
    	#define ADV7182A_AFE_CNT1_FILTER3			0x04
    	#define ADV7182A_AFE_CNT1_FILTER4			0x08
    	#define ADV7182A_AFE_CNT1_MAN_OVR			0x10		//0:disable override / 1:enable override
    
    /* User Sub Map - VDP System Register Map Details */
    //NOTE : Need to enable register(0x0E) bit 0 for accessing this register map
    #define ADV7182A_INT_CONF1				0x40
    	#define ADV7182A_INT_CONF1_OP_SEL			0x03
    #define ADV7182A_INT_Status4			0x4E
    	#define ADV7182A_INT_Status4_CCAPD_Q		0x01		//0: CC not detected / 1:CC detected
    #define ADV7182A_INT_Clear4				0x4F
    	#define ADV7182A_INT_Status4_CCAPD_CLR		0x01		//0:do not clear / 1:clear VDP_CCAPD_Q
    #define ADV7182A_INT_Mask4				0x50
    	#define ADV7182A_INT_Mask4_CCAPD_Mask		0x01
    	#define ADV7182A_INT_Mask4_CGMS_WSS_MSK		0x04
    #define ADV7182A_VDP_STATUS				0x78				//R : Read Status, W : Clear status
    	#define ADV7182A_VDP_STATUS_CC_AVL			0x01		//R
    	#define ADV7182A_VDP_STATUS_CC_CLEAR		0x01		//W
    	#define ADV7182A_VDP_STATUS_CC_FIELD		0x02		//0:CC detected from ODD field / 1:CC detected from EVEN field
    #define ADV7182A_VDP_DATA0				0x79
    #define ADV7182A_VDP_DATA1				0x7A
    
    /* User Sub Map2 Register Details */
    #define ADV7182A_FL_CNT					0xE0
    	#define ADV7182A_FL_CNT_FAST_LOCK			0x01		//0:disable / 1:enable

    #define ADRF_I2C_CHECK_TIMES 10
    #define ADV7182A_REG_MAP_MAIN				0x00
    #define ADV7182A_REG_MAP_USER_SUB1_INT_VDP	0x20
    #define ADV7182A_REG_MAP_USER_SUB2			0x40
        
        
    #define DEFAULT_ADV7182A_REGS_NUMS   sizeof(g_DEFAULT_REG)/sizeof(adv7182a_regs)
        
    typedef enum _adv7182a_encoding
    {
        encoding_unknown = 0,
        encoding_ntsc_m = 1,
        encoding_pal_bdghi = 2,
        encoding_secam = 3,
        encoding_ntsc_4_3 = 4,
        encoding_pal_m = 5,
        encoding_pal_cn = 6,
        encoding_pal_60 = 7,
        encoding_secm_525 = 8,
    }adv7182a_encoding;
    
    typedef enum
    {
        si_type_cvbs = 0,
        si_type_ypbpr = 1,
        si_type_min = si_type_cvbs,
        si_type_max = si_type_ypbpr,
    }adv7182a_si_type;
    
    typedef struct _adv7182a_regs
    {
        uint8_t reg;
        uint8_t val;
    }adv7182a_regs,*Padv7182a_regs;
    
    typedef struct _adv7182a_handle
    {
        Pvatek_i2c vi2c;
        adv7182a_si_type type;
    }adv7182a_handle,*Padv7182a_handle;
        
    #define ADV7182A_ERR(fmt,...)                VATEK_ERR(adv7182a,fmt,##__VA_ARGS__)
    #define ADV7182A_LOG(fmt,...)                VATEK_LOG(adv7182a,fmt,##__VA_ARGS__)
        
    #define ADV7182A_IS_LOCK_SIGNAL_CVBS    (ADV7182A_STATUS1_IN_LOCK|ADV7182A_STATUS1_FSC_LOCK/*|ADV7182A_STATUS1_FOLLOW_PW*/)
    #define ADV7182A_IS_LOCK_SIGNAL_YPBPR   (ADV7182A_STATUS1_IN_LOCK)
    #define ADV7182A_IS_LOCK_HLOCK_YPBPR    (ADV7182A_STATUS3_INST_HLOCK)


#ifdef __cplusplus
    }
#endif

#endif

