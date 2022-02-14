

#ifndef _HAL_REG_RFMIXER_
#define _HAL_REG_RFMIXER_

#define RFMIXER_CMD_START				0x00001000
#define RFMIXER_CMD_STOP				0x00002000

#define HALREG_RFMIXER_ADDR_START       0x1200			/* used with different rf_mixer define		*/
#define HALREG_RFMIXER_ADDR_END         0x1600


#define HALREG_RF_COM_STATUS            0x1200			
	#define RF_STATUS_IDLE				0x00000001
	#define RF_STATUS_ACTIVE			0x00000002
	#define RF_STATUS_FAIL				0x80000000

#define HALREG_RF_COM_FREQ              0x1202			/* param used with rfmixer_cmd_start (KHz)	*/

#endif

