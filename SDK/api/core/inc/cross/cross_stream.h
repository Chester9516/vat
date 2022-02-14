#ifndef _CROSS_STREAM_
#define _CROSS_STREAM_

#include <cross/cross_os_api.h>
#include <core/base/media_video.h>
#include <core/base/media_audio.h>
#include <core/base/output_modulator.h>

typedef void* hcross_stream;

#ifdef __cplusplus
extern "C" {
#endif

	HAL_API vatek_result cross_stream_open_test(uint32_t bitrate, hcross_stream* hcstream);
	HAL_API vatek_result cross_stream_open_file(const char* szfilename, hcross_stream* hcstream);
	HAL_API vatek_result cross_stream_open_udp(const char* szurl, hcross_stream* hcstream);
	HAL_API vatek_result cross_stream_open_mux(Pmodulator_param pmod, hcross_stream* hcstream);

	HAL_API vatek_result cross_stream_start(hcross_stream hcstream);
	HAL_API vatek_result cross_stream_get_slice(hcross_stream hcstream,uint8_t** pslice);
	HAL_API uint32_t cross_stream_get_bitrate(hcross_stream hcstream);
	HAL_API void cross_stream_stop(hcross_stream hcstream);

	HAL_API void cross_stream_close(hcross_stream hcstream);

#ifdef __cplusplus
}
#endif

#endif
