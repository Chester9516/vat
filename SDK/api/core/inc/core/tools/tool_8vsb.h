#ifndef _TOOL_8VSB_
#define _TOOL_8VSB_

#include <core/tools/tool_ofdm.h>

#define ATSC_8VSB_SYMBOLRATE	10762238	
#define VSB_SEGMENT_SB_NUMS		832
#define VSB_FIELD_SEGMENT_NUMS	313
#define VSB_FRAME_FIELD_NUMS	2

#define VSB_FIELD_PACKET_NUMS	(VSB_FIELD_SEGMENT_NUMS - 1)
#define VSB_FRAME_PACKET_NUMS	(VSB_FIELD_PACKET_NUMS * VSB_FRAME_FIELD_NUMS)

#ifdef __cplusplus
extern "C" {
#endif

	HAL_API vatek_result tool_8vsb_vsbframe_reset(Pmodulator_param pmod, Pofdm_frame pframe);
	HAL_API uint32_t tool_atsc_get_bitrate(Pmodulator_param pmod);
	HAL_API vatek_result tool_atsc_check_param(Pmodulator_param pmod);

#ifdef __cplusplus
}
#endif

#endif
