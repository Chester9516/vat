

#ifndef _HALREG_IP_STREAM_
#define _HALREG_IP_STREAM_

/* stream test mode param							*/
/*--------------------------------------------------*/
#define HALREG_TESTMODE_MODE            0x640
    #define STREAM_HW_PATTERN           0x00000000			/* _hw created test packets */
    #define STREAM_REMUX                0x00000001			/* enable extend_ts and mux_function but no pes_stream*/

/* stream usb param									*/
/*--------------------------------------------------*/
#define HALREG_USB_INPUT_MODE			0x640
	#define USB_INPUT_NORMAL			0x00000000			

#define HALREG_USB_STREAM_FLAGS			0x641

/* stream ts param									*/
/*--------------------------------------------------*/
#define HALREG_TSIN_MODE				0x640
    #define TSIN_MODE_BYPASS			0x00000000
    #define TSIN_MODE_SMOOTH			0x00000001
    #define TSIN_MODE_REMUX				0x00000002

#define HALREG_TSIN_FLAGS				0x641
	#define TS_PIN_SERIAL               0x00000001
	#define TS_PIN_NEGATIVE_CLK         0x00000002
	#define TS_PIN_NEGATIVE_VALID       0x00000004
	#define TS_PIN_NEGATIVE_SYNC        0x00000008

/* stream encoder param								*/
/*--------------------------------------------------*/
#define HALREG_ENCODER_MODE             0x640
	#define ENCMOD_COLORBAR             0x00000000
	#define ENCMOD_BOOTLOGO             0x00000001
	#define ENCMOD_VI_0                 0x00000002

#define HALREG_ENCODER_FLAGS            0x641
	#define ENC_EN_INTERLACED_FRAME		0x00000002
	#define ENC_EN_PROGRESSIVE_2_I		0x00000004
	#define ENC_EN_DISABLE_ADTS_CRC		0x00000008
    #define ENC_EN_FPS_BASECLK_1001     0x00000010
	#define ENC_EN_H264_FULLRUN			0x00000040
    #define ENC_EN_TIMECODE_CLOSE       0x00000080

#define HALREG_ENCODER_PMTPID			0x2002

#define HALREG_VIDEO_PID				0x2003
#define HALREG_VIDEO_CODEC				0x642				/* defined in hal_common_define.h */
#define HALREG_VIDEO_RESOLUTION			0x643				/* defined in hal_common_define.h */
#define HALREG_VIDEO_FRAMERATE			0x644				/* defined in hal_common_define.h */
#define HALREG_VIDEO_ASPECTRATE			0x645				/* defined in hal_common_define.h */

#define HALREG_AUDIO_PID				0x2004
#define HALREG_AUDIO_CODEC				0x646				/* defined in hal_common_define.h */
#define HALREG_AUDIO_CHANNEL			0x647				/* defined in hal_common_define.h */
#define HALREG_AUDIO_SAMPLERATE			0x648				/* defined in hal_common_define.h */

#define HALREG_ENCODER_TAG				0x649

/* quality_param */

#define HALREG_QUALITY_RCMODE			0x650				/* defined in hal_common_define.h */
#define HALREG_QUALITY_GOP				0x651
#define HALREG_QUALITY_MINQ				0x652
#define HALREG_QUALITY_MAXQ				0x653
#define HALREG_QUALITY_BITRATE			0x654
#define HALREG_QUALITY_LATENCY			0x655

/* vi_parm */

#define HALREG_VI_0_FLAGS				0x660				/* defined in hal_common_define.h */
	#define VI_FWFUN_MAP_MASK           0xFFFF0000	

#define HALREG_VI_0_PIXELCLOCK			0x661
#define HALREG_VI_0_OFFSET_X			0x662
#define HALREG_VI_0_OFFSET_Y			0x663


/* runtime encoder data : mpeg2 in section and h264 in sei_message*/

#define HALREG_ENC_EXTDATA				0x668
	#define ENC_EXTDATA_TAG				0xF8880000
	#define ENC_EXTDATA_LEN_MASK		0x0000FFFF

#define HALREG_EN_EXTDATA_START			0x670
	#define ENC_EXTDATA_MAXLEN			64 


#endif

