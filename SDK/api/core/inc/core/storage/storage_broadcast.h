#ifndef _STORAGE_BROADCAST_
#define _STORAGE_BROADCAST_

#include <core/base/binary_rom.h>
#include <core/base/stream_base.h>
#include <core/base/output_base.h>

#define BIN_SECTION_BROADCAST		(BIN_SECTION_TAG | 0x10)
#define BROADCAST_RAWDATA_LEN	    1024*3

typedef struct _storage_broadcast
{
    modulator_param modulator;
	audio_param audio;							/*!< default audio param */
	mpeg2_param mpeg2[MEDIA_IDX_VIDEO_NUMS];	/*!< default mpeg2 encoder config [SD,HD,FULL HD]*/
	h264_param h264[MEDIA_IDX_VIDEO_NUMS];		/*!< default h264 encoder config [SD,HD,FULL HD]*/
}storage_broadcast;

typedef storage_broadcast* Pstorage_broadcast;

/* storage must less than section_size */
_VASSERT(sizeof(storage_broadcast) < BROADCAST_RAWDATA_LEN, storage_overflow);

#ifdef __cplusplus
extern "C" {
#endif

	HAL_API void audio_param_reset(Paudio_param paudio);
	HAL_API void mpeg2_param_reset(video_resolution resolution, Pmpeg2_param pmpeg2);
	HAL_API void h264_param_reset(video_resolution resolution, Ph264_param ph264);

	HAL_API void storage_broadcast_reset(Pstorage_broadcast pbc);
	HAL_API vatek_result storage_broadcast_set(Pstorage_broadcast pbc,uint8_t* psection);
	HAL_API vatek_result storage_broadcast_get(Pstorage_broadcast pbc,uint8_t* psection);

#ifdef __cplusplus
}
#endif

#endif
