

#ifndef _HALREG_IP_R2_
#define _HALREG_IP_R2_

#include <hal/hal_register/halreg_rfmixer.h>

#define R2MIXER_CMD_LOAD_DEFAULT	0x00004000
#define R2MIXER_CMD_SAVE_DEFAULT	0x00008000
#define R2MIXER_CMD_R2_TUNE			0x00010000

/* r2 function flag define */
#define R2FLAG_I_PN_SWAP			0x00000010
#define R2FLAG_Q_PN_SWAP			0x00000001

/* r2 hal_register define */
#define HALREG_R2_PARAM_BASE        HALREG_RFMIXER_ADDR_START

#define HALREG_R2_STATUS            HALREG_RF_COM_STATUS
#define HALREG_R2_PARAM_FREQ        HALREG_RF_COM_FREQ

#define HALREG_R2_EN_CNTL			0x1208
	#define R2_EN_TAG_BASE			0xA5FF0000
	#define R2_EN_PATH_0			0xA5FF0000
	#define R2_EN_PATH_1			0xA5FF0001
	#define R2_EN_PARAM				0xA5FF0002

#define HALREG_R2_FLAGS				0x1209
	
#define HALREG_R2_PARAM_I           0x120A
#define HALREG_R2_PARAM_Q           0x120B
#define HALREG_R2_PARAM_IMAGE       0x120C
#define HALREG_R2_PARAM_PHASE       0x120D
#define HALREG_R2_PARAM_GPIO        0x120E
#define HALREG_R2_PARAM_GAIN        0x120F

#define HALREG_R2_DEF_TAG			0x1210
	#define R2_EN_DEF_TAG			0xA5FF0002
#define HALREG_R2_DEF_I             0x1211
#define HALREG_R2_DEF_Q             0x1212
#define HALREG_R2_DEF_IMAGE         0x1213
#define HALREG_R2_DEF_PHASE         0x1214
#define HALREG_R2_DEF_GPIO          0x1215
#define HALREG_R2_DEF_GAIN          0x1216

#define HALOF_R2_TUNE_I				0x00
#define HALOF_R2_TUNE_Q				0x01
#define HALOF_R2_TUNE_IMG			0x02
#define HALOF_R2_TUNE_PHASE			0x03
#define HALOF_R2_TUNE_GPIO			0x04
#define HALOF_R2_TUNE_GAIN			0x05

#endif

