#ifndef _TOOL_OUTPUT_
#define _TOOL_OUTPUT_

#include <core/tools/tool_ofdm.h>
#include <core/tools/tool_helpful.h>

#define OUTCLK_DEFAULT_PCRINTERVAL_MS		40

typedef void* houtput_clock;

#ifdef __cplusplus
extern "C" {
#endif

	HAL_API vatek_result tool_output_clock_create(Pmodulator_param pmod, houtput_clock* houtclk);
	HAL_API vatek_result tool_output_clock_create_mempool(Pmodulator_param pmod,Pth_mempool pmem, houtput_clock* houtclk);
	HAL_API vatek_result tool_output_clock_config(houtput_clock houtclk, Pmodulator_param pmod);
	HAL_API void tool_output_clock_set_tick(houtput_clock houtclk,Pmux_clock_tick pclk);

	HAL_API void tool_output_clock_adjust_tick(houtput_clock houtclk,uint32_t tick27mhz);

	HAL_API Pmux_clock_tick tool_output_clock_append(houtput_clock houtclk);
	HAL_API vatek_result tool_output_clock_get_pcrpacket(houtput_clock houtclk, int32_t* intervalms);
	HAL_API Pmux_clock_tick tool_output_clock_get_tick(houtput_clock houtclk);

	HAL_API vatek_result tool_output_clock_reset(houtput_clock houtclk);
	HAL_API void tool_output_clock_free(houtput_clock houtclk);

#ifdef __cplusplus
}
#endif

#endif
