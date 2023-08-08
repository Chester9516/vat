#ifndef _HALSERVICE_BROADCAST_
#define _HALSERVICE_BROADCAST_

#include <hal/halreg_mux.h>
#include <hal/halservice_rescure.h>

#define HALREG_BROADCAST_CNTL           HALREG_SERVICE_BASE_CNTL
    #define BC_START                    BASE_CMD_START
    #define BC_STOP                     BASE_CMD_STOP
	#define BC_START_SINE				BASE_CMD_TEST_START_SINE
	#define BC_START_TEST				BASE_CMD_TEST_START

    #define BC_REBOOT                   BASE_CMD_REBOOT
    #define BC_REBOOT_RESCURE           BASE_CMD_REBOOT_RESCURE

    #define BC_RFMIXER_START            RFMIXER_CMD_START
    #define BC_RFMIXER_STOP             RFMIXER_CMD_STOP
			
	#define BC_CALIBRATION_SAVE         BASE_CMD_CALIBRATION_SAVE

#define HALREG_BROADCAST_MODE			0x601
	#define RUNMODE_BROADCAST			0x00000003

#define HALREG_BROADCAST_STREAM			HALREG_SERVICE_INPUT

/**
 * @brief Encoder Extend Data Interface
 * 
 */
#define HALREG_ENC_EXTDATA				0x668
	#define ENC_EXTDATA_TAG				0xF8880000
	#define ENC_EXTLDATA_TAG				0xF8890000
	#define ENC_EXTDATA_LEN_MASK		0x0000FFFF

#define HALREG_EN_EXTDATA_START			0x670
	#define ENC_EXTDATA_MAXLEN			64 
	
#define HALREG_EN_EXTLDATA_START			0x6C0
	#define ENC_EXTLDATA_MAXLEN			256 

#endif
