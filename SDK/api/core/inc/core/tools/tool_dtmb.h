#ifndef _TOOL_DTMB_
#define _TOOL_DTMB_

#include <core/tools/tool_ofdm.h>

#if 0

typedef struct _dtmb_superframe
{
	uint32_t symbolrate;
	uint32_t sframe_pktnums;
	uint32_t sframe_tick;
	uint32_t sframe_pkttick;
}dtmb_superframe;

typedef dtmb_superframe* Pdtmb_superframe;

#endif

#ifdef __cplusplus
extern "C" {
#endif

	HAL_API vatek_result tool_dtmb_superframe_reset(Pmodulator_param pmod, Pofdm_frame pframe);

#if 0
	HAL_API vatek_result tool_dtmb_superframe_reset(Pmodulator_param pmod, Pdtmb_superframe psframe);
	HAL_API vatek_result tool_dtmb_superframe_get_tick(Pdtmb_superframe psframe, int32_t pktpos);
	HAL_API void tool_dtmb_superframe_append_frame(Pdtmb_superframe psframe, Pmux_clock_tick pclk);
#endif

	HAL_API uint32_t tool_dtmb_get_bitrate(Pmodulator_param pmod);
	HAL_API vatek_result tool_dtmb_check_param(Pmodulator_param pmod);

#ifdef __cplusplus
}
#endif

#endif
