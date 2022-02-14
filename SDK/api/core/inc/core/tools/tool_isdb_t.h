#ifndef _TOOL_ISDB_T_
#define _TOOL_ISDB_T_

#include <core/tools/tool_ofdm.h>

#define ISDB_T_6MHZ_SYMBOL		8126984 /* 512/63 */
#define ISDB_T_TSP_MAXNUMS		(3276 >> 2)

typedef struct _isdb_t_bts_frame
{
	ofdm_frame ofdm;
	int32_t frameus;
	uint32_t frame_sb;
	uint16_t tsp_maps[ISDB_T_TSP_MAXNUMS];
}isdb_t_bts_frame;

typedef isdb_t_bts_frame* Pisdb_t_bts_frame;

#ifdef __cplusplus
extern "C" {
#endif

	HAL_API vatek_result tool_isdb_t_bts_frame_reset(Pmodulator_param pmod, Pisdb_t_bts_frame pbts);
	HAL_API vatek_result tool_isdb_t_bts_frame_get_tick(Pisdb_t_bts_frame pbts, int32_t pktpos);

	HAL_API uint32_t tool_isdb_t_get_bitrate(Pmodulator_param pmod);
	HAL_API vatek_result tool_isdb_t_check_param(Pmodulator_param pmod);

#ifdef __cplusplus
}
#endif

#endif
