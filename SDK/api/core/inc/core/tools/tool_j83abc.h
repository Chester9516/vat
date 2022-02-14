#ifndef _TOOL_J83ABC_
#define _TOOL_J83ABC_

#include <core/tools/tool_ofdm.h>

#define J83A_SYMBOL_MIN		5000
#define J83A_SYMBOL_MAX		8000

#define is_j83a_valid_sb(sb)	(sb >= J83A_SYMBOL_MIN && sb <= J83A_SYMBOL_MAX)

#define J83B_Q64_SYMBOL		5056941
#define J83B_Q256_SYMBOL	5360537

#define J83C_SYMBOL_RATE	5274

#ifdef __cplusplus
extern "C" {
#endif

	HAL_API vatek_result tool_j83_j83frame_reset(Pmodulator_param pmod, Pofdm_frame pframe);
	HAL_API uint32_t tool_j83_get_bitrate(Pmodulator_param pmod);
	HAL_API vatek_result tool_j83_check_param(Pmodulator_param pmod);

#ifdef __cplusplus
}
#endif

#endif
