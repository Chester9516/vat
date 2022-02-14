#ifndef _HAL_REGISTER_BROADCAST_
#define _HAL_REGISTER_BROADCAST_

#include <hal/hal_register/halreg_errcode.h>
#include <hal/hal_register/halreg_broadcast_info.h>
#include <hal/hal_register/halreg_output.h>
#include <hal/hal_register/halreg_stream.h>
#include <hal/hal_register/halreg_mux.h>
#include <hal/hal_register/halreg_r2.h>

#define HALREG_BROADCAST_CNTL           0x600
    #define BC_START                    0x00000001
    #define BC_STOP                     0x00000002

    #define BC_REBOOT                   BASE_CMD_REBOOT
    #define BC_REBOOT_RESCURE           BASE_CMD_REBOOT_RESCURE

    #define BC_RFMIXER_START            RFMIXER_CMD_START
    #define BC_RFMIXER_STOP             RFMIXER_CMD_STOP

    #define BC_CALIBRATION_APPLY        BASE_CMD_CALIBRATION_APPLY
    #define BC_CALIBRATION_SAVE         BASE_CMD_CALIBRATION_SAVE
    #define BC_CALIBRATION_LOAD         BASE_CMD_CALIBRATION_LOAD
    
	#define BC_LICENSE_CMD				0x80000000

#define HALREG_BROADCAST_OUTPUT			0x601
    #define BC_OUT_MODULATOR            0x00000000
    #define BC_OUT_SINEWAVE             0x00000001
    
#define HALREG_BROADCAST_STREAM			0x602
	#define STREAM_TESTMODE             0x00000000
	#define STREAM_USB					0x00000001
	#define STREAM_TS					0x00000002
	#define STREAM_ENCODER              0x00000003

#endif
