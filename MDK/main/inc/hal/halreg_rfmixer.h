#ifndef _HALREG_RFMIXER_
#define _HALREG_RFMIXER_

#define RFMIXER_CMD_START				0x00001000
#define RFMIXER_CMD_STOP				0x00002000

#define HALREG_RFMIXER_ADDR_START       0x1200			/* used with different rf_mixer define		*/
#define HALREG_RFMIXER_ADDR_END         0x1600


#define HALREG_RF_COM_STATUS            0x1200			
	#define RF_STATUS_IDLE				0x00000001
	#define RF_STATUS_ACTIVE			0x00000002
	#define RF_STATUS_FAIL				0x80000000

#define HALREG_RF_COM_FREQ              0x1202			/* param used with rfmixer_cmd_start (KHz)	*/

#define R2MIXER_CMD_R2_TUNE			    0x00010000

/* r2 function flag define */
#define R2FLAG_I_PN_SWAP			    0x00000010
#define R2FLAG_Q_PN_SWAP			    0x00000001
#define R2FLAG_EN_CALIBRATION			0x08000000

#define HALREG_R2_EN_CNTL			    0x1208
	#define R2_EN_TAG_BASE			    0xA5FF0000
	#define R2_EN_PATH_0			    0xA5FF0000		/* used default param with path_0	*/
	#define R2_EN_PATH_1			    0xA5FF0001		/* used default param with path_1	*/
	#define R2_EN_PARAM				    0xA5FF0002		/* used customized param			*/

#define HALREG_R2_FLAGS				    0x1209
	
#define HALREG_R2_PARAM_I               0x120A
#define HALREG_R2_PARAM_Q               0x120B
#define HALREG_R2_PARAM_IMAGE           0x120C
#define HALREG_R2_PARAM_PHASE           0x120D
#define HALREG_R2_PARAM_GPIO            0x120E
#define HALREG_R2_PARAM_GAIN            0x120F

#define HALREG_CALIBRATION_CNTL         0x1220
    #define CALIBRATION_EN_TAG              0x78122900
    #define CALIBRATION_APPLY               0x00000001
    
    
#define HALREG_CALIBRATION_CLOCK        0x1228
#define HALREG_CALIBRATION_DAC          0x1229

#define HALREG_CALIBRATION_R2_0_I       0x1280
#define HALREG_CALIBRATION_R2_0_Q       0x1281
#define HALREG_CALIBRATION_R2_0_IMAGE   0x1282
#define HALREG_CALIBRATION_R2_0_PHASE   0x1283

#define HALREG_EXT_R2_GAIN							0x1290
		#define	EXT_R2_GAIN_EN_TAG							0x45585400					
		#define EXT_R2_GAIN_MASK								0x0000000F
		#define EXT_R2_GAIN_EN_READ							0x45585800

#endif
