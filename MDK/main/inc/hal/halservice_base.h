#ifndef _HALSERVICE_BASE_
#define _HALSERVICE_BASE_

#include <hal/halreg_define.h>
#include <hal/halreg_stream.h>
#include <hal/halreg_output.h>

#define HALREG_SYS_STATUS_0             0x20
    #define SYS_STATUS_TAG              0xFF000000
    #define SYS_STATUS_IDLE             0xFF000001
    #define SYS_STATUS_RUN              0xFF000002

	#define SYS_STATUS_ERRTAG			0xFF000080
	#define SYS_STATUS_LOADER_FAIL		0xFF000085
	#define SYS_STATUS_SERVICE_FAIL		0xFF000086
	#define SYS_STATUS_EXCEPTION_FAIL	0xFF000088
	#define SYS_STATUS_BADSTATUS		0xFF00008E
	#define SYS_STATUS_UNKNOWN_FAIL		0xFF00008F

#define HALREG_SYS_STATUS_1             0x21
#define HALREG_SYS_STATUS_2             0x22

#define HALREG_SYS_ERRCODE              0x23
    #define SYS_ERRCODE_INIT            0x80000001

    #define LOADER_ERRCODE_NOAPP        0x80010001
    #define LOADER_ERRCODE_CRC32        0x80010002
    #define LOADER_ERRCODE_HW           0x80010003
    
    #define SERVICE_INIT_FAIL           0x80020001
	#define SERVICE_HW_FAIL				0x80020002

#define HALREG_CHIP_ID                  0x24
    #define HAL_CHIPID_A1               0x00010100
	#define HAL_CHIPID_A3               0x00010300
    #define HAL_CHIPID_B1               0x00020100
    #define HAL_CHIPID_B2               0x00020200
	#define HAL_CHIPID_B2_PLUS			0x00020201
    #define HAL_CHIPID_B3               0x00020300
	#define HAL_CHIPID_B3_PLUS			0x00020301
	#define HAL_CHIPID_E1				0x00040300

	#define CHIPID_MAIN_MASK			0x000FFF00
	#define CHIPID_VER_MASK				0x000000FF
	#define CHIPID_AX_BASE				0x00010000
	#define CHIPID_BX_BASE				0x00020000

#define HALREG_FW_VER                   0x25
#define HALREG_SERVICE_MODE             0x26
    #define SERVICE_TAG_RESCUE          0xFF000001
    #define SERVICE_TAG_BROADCAST       0xF8000001
	#define SERVICE_TAG_TRANSFORM		0xF8000002

#define HALREG_PERIPHERAL_EN            0x27
    #define PERIPHERAL_RF_MASK          0x000000FF
    #define PERIPHERAL_FINTEKR2         0x00000002
    #define PERIPHERAL_VIDEO_MASK       0x00FF0000
    #define PERIPHERAL_EP9555E          0x00010000 

/*v1 status register*/
#define HALREG_HDMI_STATUS						0x1020
		#define HDMI_CONNECT								0x00000001
		#define HDMI_HPD_EN									0x00000002
		#define HDMI_DE_VLD									0x00000004

#define HALREG_HDMI_VIDEO_STS					0x1022
		#define VIDEO_VIC_CHG								0x00000001

#define HALREG_HDMI_AUDIO_STS						0x1024
		#define AUDIO_SF_CHG								0x00000001
		
//VIDEO INFO BLOCK
#define HALREG_HDMI_VIDEO_INFO				0x1040
		#define HDMI_VID_VIC_MASK						0x0000FFFF
		#define HDMI_HDCP_AI								0x00010000
		#define HDMI_HDCP_DP								0x00020000
		
#define HALREG_HDMI_VID_RESOLUTION		0x1041
		#define HDMI_VID_RESOLUTION_480I		0x00000000
		#define HDMI_VID_RESOLUTION_480P		0x00000001
		#define HDMI_VID_RESOLUTION_576I		0x00000002
		#define HDMI_VID_RESOLUTION_576P		0x00000003
		#define HDMI_VID_RESOLUTION_720P		0x00000004
		#define HDMI_VID_RESOLUTION_1080I		0x00000005
		#define HDMI_VID_RESOLUTION_1080P		0x00000006

#define HALREG_HDMI_VID_FRAMERATE			0x1042
		#define	HDMI_VID_FR_23_97						0x00000000
		#define	HDMI_VID_FR_24							0x00000001
		#define	HDMI_VID_FR_25							0x00000002
		#define	HDMI_VID_FR_29_97						0x00000003
		#define	HDMI_VID_FR_30							0x00000004
		#define	HDMI_VID_FR_50							0x00000005
		#define	HDMI_VID_FR_59_94						0x00000006
		#define	HDMI_VID_FR_60							0x00000007

#define HALREG_HDMI_VID_ASPECTRATE		0x1043
		#define HALREG_ASPECTRATE_4_3				0x00000000
		#define HALREG_ASPECTRATE_16_9			0x00000001

//H SYNC, V SYNC TIMING
#define	HALREG_HDMI_VID_HACT					0x1045 /*H active*/
#define	HALREG_HDMI_VID_HFB					  0x1046 /*H front blank*/
#define	HALREG_HDMI_VID_HPLS					0x1047 /*H pulse*/
#define	HALREG_HDMI_VID_HBB						0x1048 /*H back blank*/

#define HALREG_HDMI_VID_VACT					0x1049 /*V active*/
#define	HALREG_HDMI_VID_VFB						0x104A /*V front blank*/
#define	HALREG_HDMI_VID_VPLS					0x104B /*V pulse*/
#define	HALREG_HDMI_VID_VBB						0x104C /*V back blank*/

#define HALREG_HDMI_AUD_SAMPLERATE		0x1060
		#define	HDMI_AUD_SR_32KHZ						0x00000000
		#define HDMI_AUD_SR_44_1KHZ					0x00000001
		#define HDMI_AUD_SR_48KHZ						0x00000002
		
/*end*/

	#define PERIPHERAL_USBBULK			0x00000100
	#define PERIPHERAL_AUXSTREAM_USB	0x00000200
	#define PERIPHERAL_AUXSTREAM_FLASH	0x00000400
	#define PERIPHERAL_AUXSTREAM_TSIN	0x00000800
	#define PERIPHERAL_CALIBRATION		0x01000000
	#define PERIPHERAL_LICENSED			0x80000000
	#define PERIPHERAL_HWID_MASK		0x70000000

#define HALREG_INPUT_SUPPORT            0x28
	#define INPUT_EN_TEST				0x00000001
	#define INPUT_EN_USB                0x00000002
    #define INPUT_EN_TS                 0x00000004    
	#define INPUT_EN_ENC				0x00000008

    #define VENC_EN_MPEG2				0x00000010
    #define VENC_EN_H264				0x00000020
	#define VENC_EN_HEVC				0x00000040
	#define VENC_EN_FULLHD				0x00000100

	#define AENC_EN_MP1_L2				0x00001000
	#define AENC_EN_AAC_LC_ADTS			0x00002000
	#define AENC_EN_AC_3				0x00004000
	#define AENC_EN_AAC_LC_LATM			0x00008000
	
	#define TSIN_EN_BYPASS				0x00010000
	#define TSIN_EN_TSSMOOTH			0x00020000
    #define TSIN_EN_TSREMUX				0x00040000
	#define TSIN_EN_ALL					0x00070000

	#define TEST_EN_HW					0x00100000
	#define TEST_EN_REMUX				0x00200000

	#define TEST_EN_ALL					0x00300000

	#define ABASE_EN_IN					(INPUT_EN_TEST | INPUT_EN_USB | INPUT_EN_TS | TSIN_EN_BYPASS | TEST_EN_HW)
	#define BBASE_EN_IN					(INPUT_EN_TEST | INPUT_EN_ENC | AENC_EN_MP1_L2 | TEST_EN_ALL)

	#define A1_EN_IN					ABASE_EN_IN
	#define A3_EN_IN					(A1_EN_IN | TSIN_EN_ALL | TEST_EN_ALL)

	#define BX_EN_AUDIO_ALL				(AENC_EN_AAC_LC_ADTS | AENC_EN_AAC_LC_LATM | AENC_EN_AC_3)

	#define B1_EN_IN					(BBASE_EN_IN | VENC_EN_MPEG2)
	#define B2_EN_IN					(B1_EN_IN | VENC_EN_FULLHD | BX_EN_AUDIO_ALL)
	#define B3_EN_IN					(BBASE_EN_IN | VENC_EN_H264 | VENC_EN_FULLHD | BX_EN_AUDIO_ALL)
	#define B3PLUS_EN_IN				(B3_EN_IN | VENC_EN_MPEG2)

#define HALREG_OUTPUT_SUPPORT           0x29
	#define OUTPUT_EN_MOD				0x00000001
	#define OUTPUT_EN_SINEWAVE			0x00000002
	#define OUTPUT_EN_TS				0x00000004
	#define OUTPUT_EN_USB				0x00000008
	#define OUTPUT_EN_PCIE				0x00000010
	#define OUTPUT_EN_BASE				0x00000003
	
    #define MOD_EN_DVB_T				0x00000100
    #define MOD_EN_J83_A				0x00000200
    #define MOD_EN_ATSC					0x00000400
    #define MOD_EN_J83_B				0x00000800
    #define MOD_EN_DTMB					0x00001000
    #define MOD_EN_ISDB_T				0x00002000
	#define MOD_EN_J83_C				0x00004000
    #define MOD_EN_DVB_T2				0x00008000
	#define MOD_EN_BASE					0x00005F00

	#define DVB_T2_EN_FFT32K			0x01000000

	#define A1_EN_OUT					(OUTPUT_EN_BASE | MOD_EN_BASE)
	#define A3_EN_OUT					(A1_EN_OUT | MOD_EN_ISDB_T | MOD_EN_DVB_T2)

	#define B1_EN_OUT					(OUTPUT_EN_BASE | MOD_EN_BASE)
	#define B2_EN_OUT					(B1_EN_OUT | MOD_EN_ISDB_T)
	#define B3_EN_OUT					(B2_EN_OUT)
	#define B3PLUS_EN_OUT				(B2_EN_OUT | MOD_EN_DVB_T2)

	#define E1_EN_OUT					(OUTPUT_EN_USB | OUTPUT_EN_TS | OUTPUT_EN_PCIE)

/* service base control register	*/

#define HALREG_SERVICE_BASE_CNTL		0x600
	#define BASE_CMD_START				0x00000001			/* start service with different params						*/	
	#define BASE_CMD_STOP				0x00000002			/* stop service												*/
	#define BASE_CMD_TEST_START_SINE	0x00000004			/* DAC test output 1MHz sine wave							*/	
	#define BASE_CMD_TEST_START			0x00000008			/* base chip functions test									*/

	#define BASE_CMD_REBOOT				0x00000100			/* reboot chip normal										*/
	#define BASE_CMD_REBOOT_RESCURE		0x00000200			/* reboot chip and into rescue mode							*/

	#define BASE_CMD_CALIBRATION_RESET	0x10000000
	#define BASE_CMD_CALIBRATION_SAVE	0x20000000

#define HALREG_SERVICE_INPUT			0x602
	#define STREAM_NULL					0x00000000
	#define STREAM_USB					0x00000001
	#define STREAM_TSIN					0x00000002
	#define STREAM_ENCODER				0x00000003
	#define STREAM_SINE					0x00001000
	#define STREAM_TEST					0x00001001

/* broadcast_information_ip			*/

#define HALREG_BCINFO_STREAM			0x620				/* enabled stream mode										*/	
#define HALREG_BCINFO_OUTPUT			0x621				/* enabled output mode										*/	
#define HALREG_BCINFO_MODRATE			0x622				/* modulator output bitrate									*/	
#define HALREG_BCINFO_MUXRATE			0x623				/* current mux bitrate										*/
#define HALREG_BCINFO_STATUS			0x624				/* current broadcast status									*/
	#define BCSTATUS_IDLE				0x00000000			/* - broadcast not enabled									*/
	#define BCSTATUS_WAIT_SOURCE		0x00000001			/* - broadcast not start output wait source					*/
	#define BCSTATUS_BROADCAST			0x00000002			/* - broadcast start output data							*/
	#define BCSTATUS_FINISH				0x00000003			/* - broadcast operation finish	(used with transform)		*/	

	#define BCSTATUS_FAIL_UNKNOWN       0x80000000
	#define BCSTATUS_FAIL_SOURCE        0x80000001
	#define BCSTATUS_FAIL_TIMEOUT       0x80000002
	#define BCSTATUS_FAIL_CODECDROP     0x80000003
	#define BCSTATUS_FAIL_BUFFER        0x80000004
	#define BCSTATUS_FAIL_MUXER         0x80000005
	#define BCSTATUS_FAIL_ENCODE        0x80000006
	#define BCSTATUS_FAIL_MEDIA         0x80000007
	#define BCSTATUS_FAIL_DEMUX			0x80000008

#define HALREG_BCINFO_CURRATE			0x625				/* current mux valid data bitrate							*/	
#define HALREG_BCINFO_DATARATE			0x626				/* current main PES data bitrate							*/
															/* - broadcast : mean video encode bitrate					*/
															/* - transform : mean _demux bitrate						*/

#define HALREG_BCINFO_QUERYBUF          0x627               /* buffer query_buffer len									*/
															/* - broadcast : video result buffer remain size			*/
															/* - transform : _demux query buffer remain size			*/	

#define HALREG_BCINFO_LAST				0x62F

#define HALRANGE_PLAYLOAD_START			0x2100
#define HALRANGE_PLAYLOAD_END			0x3000



#endif
