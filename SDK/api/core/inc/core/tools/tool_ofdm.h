#ifndef _TOOL_OFDM_
#define _TOOL_OFDM_

#include <core/base/output_modulator.h>
#include <mux/mux_define.h>
#include <core/ui/ui_props_define.h>

typedef struct _ofdm_frame
{
	uint32_t frametick;
	uint32_t packetnums;
	uint32_t framerecv;
}ofdm_frame;

typedef ofdm_frame* Pofdm_frame;

typedef struct _ofdm_position
{
	int32_t packetnums;
	int32_t packetptr;
	int32_t pkttick;
	uint32_t bitrecv;
}ofdm_position;

typedef ofdm_position* Pofdm_position;

typedef vatek_result(*fpofdm_check_param)(Pmodulator_param pmod);
typedef uint32_t(*fpofdm_get_bitrate)(Pmodulator_param pmod);

typedef struct _ofdm_modulation
{
	int32_t defaultsize;
	const uint8_t* defaultparam;
	const Pui_prop_item uiprops;
	fpofdm_get_bitrate getbitrate;
	fpofdm_check_param checkparam;
}ofdm_modulation;

typedef ofdm_modulation* Pofdm_modulation;

const static uint8_t GI_RATE[][2] = { {32,33,},{16,17,},{8,9,},{4,5,}, };
const static uint8_t CR_RATE[][2] = { {1,2,},{2,3,},{3,4,},{5,6,},{7,8,}, };

#ifdef __cplusplus
extern "C" {
#endif

	HAL_API vatek_result tool_ofdm_get_modulation(modulator_type type,Pofdm_modulation* pofdm);

#ifdef __cplusplus
}
#endif

#endif
