#ifndef _MEDIA_AUDIO_
#define _MEDIA_AUDIO_

#include <stdint.h>
#include <core/hal/halreg_stream.h>

#define AUDIO_RC_AUTO					0x00000000
#define AUDIO_RC_CBR					0x00000001
#define AUDIO_RC_VBR					0x00000002

#define AUDIO_QUALITY_NORMAL			0x00000000
#define AUDIO_QUALITY_MIDDLE			0x00000001
#define AUDIO_QUALITY_GOOD				0x00000002

#define AUDIO_EN_ADTS_NO_CRC			0x00000001
#define AUDIO_EN_ADTS_FULLNESS			0x00000002

/**
 * @ingroup media_props 
 * audio codec 
 */
typedef enum _audio_codec{
	encaudio_unknown = -1,
	encaudio_mp2 = AUDIO_MP1_L2,				/*!< MP1 LayoutII (ISO-13818-3)*/
	encaudio_aac_lc_adts = AUDIO_AAC_LC_ADTS,	/*!< AAC-LC ADTS */
	encaudio_ac3 = AUDIO_AC_3,					/*!< AC-3 (PSIP A52) */
	encaudio_aac_lc_latm = AUDIO_AAC_LC_LATM,	/*!< AAC-LC LATM */
}audio_codec;

#define is_current_acodec(val)	(val >= encaudio_mp2 && val <= encaudio_aac_lc_latm)

/**
 * @ingroup media_props
 * audio sample rate 
 */
typedef enum _audio_samplerate{
	sample_rate_unknown = -1,
	sample_rate_32 = SAMPLERATE_32KHZ,		/*!< 32KHz (32000) */
	sample_rate_44_1 = SAMPLERATE_44_1KHZ,	/*!< 44.1KHz (44100) */
	sample_rate_48 = SAMPLERATE_48KHZ,		/*!< 48KHz (48000) */
}audio_samplerate;

#define is_current_samplerate(val)	(val >= sample_rate_32 && val <= sample_rate_48)

/**
 * @ingroup media_props
 * audio channel define 
 */
typedef enum _audio_channel{
	channel_unknow = -1,
	channel_mute = CHANNEL_MUTE,					/*!< mute */
	channel_stereo = CHANNEL_STEREO,				/*!< stereo */
	channel_mono_l = CHANNEL_MONO_L,				/*!< mono - L only */
	channel_mono_r = CHANNEL_MONO_R,				/*!< mono - R only */
	channel_stereo_mono_l = CHANNEL_STEREO_MONO_L,	/*!< mono stereo - L first */
	channel_stereo_mono_r = CHANNEL_STEREO_MONO_R,	/*!< mono stereo - R first */
}audio_channel;

#define is_current_channel(val)	(val >= channel_mute && val <= channel_stereo_mono_r)

/**
 * @ingroup media_props
 * audio parameters
 */
typedef struct _media_audio{	
	audio_codec acodec;				/*!< audio encoder codec */
	audio_samplerate samplerate;	/*!< audio sample rate */
	audio_channel channel;			/*!< audio channel */
}media_audio;

typedef media_audio* Pmedia_audio;

/* audio codec config param */

typedef enum _audio_rc_mode{
	audio_rc_auto = AUDIO_RC_AUTO,
	audio_rc_cbr = AUDIO_RC_CBR,
	audio_rc_vbr = AUDIO_RC_VBR,
}audio_rc_mode;

typedef enum _audio_quality_mode{
	audio_quality_normal = AUDIO_QUALITY_NORMAL,	/* lowest bitrate */
	audio_quality_middle = AUDIO_QUALITY_MIDDLE,	/* balance bitrate */
	audio_quality_good = AUDIO_QUALITY_GOOD,		/* possible highest bitrate */
}audio_quality_mode;

typedef struct _audio_param{
	audio_rc_mode rcmode;
	audio_quality_mode quality;
	uint32_t functions;
}audio_param;

typedef audio_param* Paudio_param;

#endif
