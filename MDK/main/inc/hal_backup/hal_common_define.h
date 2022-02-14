#ifndef _HAL_COMMON_DEFINE_
#define _HAL_COMMON_DEFINE_

#include <stdint.h>

/* video codec */
#define VIDEO_MPEG2					0x00000000
#define VIDEO_H264					0x00000001
#define VIDEO_HEVC					0x00000002

/* video input bt656/1120 single config */
#define VI_HWREG_MAP_MASK           0x0000FFFF	/* map width vi_0 register */
#define VI_BUSWIDTH_16              0x00000010
#define VI_SEPARATED_SYNC           0x00000040
#define VI_CLK_INVERSE              0x00000200
#define VI_HSYNC_INVERSE            0x00000400
#define VI_VSYNC_INVERSE            0x00001000
#define VI_FIELDID_SIGNAL           0x01000000
#define VI_EXT_HALF_FPS             0x10000000
#define VI_FIELD_INVERSE			0x20000000
#define VI_DISABLE_ERRCHK           0x40000000
#define VI_PCRCLK_ENC               0x80000000

/* video resolution */
#define RESOLUTION_480I				0x00000000
#define RESOLUTION_480P				0x00000001
#define RESOLUTION_576I				0x00000002
#define RESOLUTION_576P				0x00000003
#define RESOLUTION_720P				0x00000004
#define RESOLUTION_1080I			0x00000005
#define RESOLUTION_1080P			0x00000006
#define RESOLUTION_UHD				0x00000007

/* video aspect rate */
#define ASPECTRATE_4_3				0x00000000
#define ASPECTRATE_16_9				0x00000001

/* video frame rate */
#define FRAMERATE_23_97				0x00000000
#define FRAMERATE_24				0x00000001
#define FRAMERATE_25				0x00000002
#define FRAMERATE_29_97				0x00000003
#define FRAMERATE_30				0x00000004
#define FRAMERATE_50				0x00000005
#define FRAMERATE_59_94				0x00000006
#define FRAMERATE_60				0x00000007

/* video codec rc_mode */
#define QUALITY_RC_VBR				0x00000001
#define QUALITY_FIXED_Q             0x00000002
#define QUALITY_AUTO				0x00000003

/* audio codec */
#define AUDIO_MP1_L2				0x00000000
#define AUDIO_AAC_LC_ADTS			0x00000001
#define AUDIO_AC_3					0x00000002	
#define AUDIO_AAC_LC_LATM			0x00000003

/* audio sample rate */
#define SAMPLERATE_32KHZ			0x00000000
#define SAMPLERATE_44_1KHZ			0x00000001
#define	SAMPLERATE_48KHZ			0x00000002

/* audio channel */
#define CHANNEL_MUTE				0x00000000
#define CHANNEL_STEREO				0x00000001
#define CHANNEL_MONO_L				0x00000002
#define CHANNEL_MONO_R				0x00000003
#define CHANNEL_STEREO_MONO_L		0x00000004
#define CHANNEL_STEREO_MONO_R		0x00000005

#endif
