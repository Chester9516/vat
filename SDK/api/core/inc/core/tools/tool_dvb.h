#ifndef _TOOL_DVB_
#define _TOOL_DVB_

#include <core/tools/tool_ofdm.h>

#define DVB_T_BW_MIN                    5
#define DVB_T_BW_MAX                    8

#define is_dvb_t_valid_bw(m)            (m >= DVB_T_BW_MIN && m <= DVB_T_BW_MAX)

#ifdef __cplusplus
extern "C" {
#endif

    HAL_API vatek_result tool_dvb_t_superframe_reset(Pmodulator_param pmod, Pofdm_frame pframe);
    HAL_API vatek_result tool_dvb_t_check_param(Pmodulator_param pmod);
    HAL_API uint32_t tool_dvb_t_get_bitrate(Pmodulator_param pmod);

#ifdef __cplusplus
}
#endif

#endif
