#ifndef _UI_SERVICE_BROADCAST_
#define _UI_SERVICE_BROADCAST_

#include <service/ui/ui_service_base.h>
#include <service/service_broadcast.h>

/* stream : common*/
HALREG_S(MUX_PCR_PID, 13)
HALREG_S(MUX_PADDING_PID, 13)
HALREG_S(MUX_BITRATE, 32)
HALREG_S(MUX_STUFFING_PID, 13)
HALREG_S(MUX_FLAGS, 32)

_ui_flags_start(mux_param, en_function)
	//_val(MUX_FLAG_EN_PSIP, en_psip)
	//_val(MUX_FLAG_DISABLE_PCR, en_disable_pcr)
_ui_flags_end

_ui_struct_start(mux_param)
	_prop_h16(mux_param, pcr_pid, "mux - pcr pid", MUX_PCR_PID)
	_prop_h16(mux_param, padding_pid, "mux - padding pid", MUX_PADDING_PID)
	_prop_u32(mux_param, bitrate, "mux - bitrate (bps)", MUX_BITRATE)
	_prop_u16(mux_param, stuffing_pid, "mux - suffing pid", MUX_STUFFING_PID)
	_prop_flag(mux_param, en_function, "mux - functions", MUX_FLAGS)
_ui_struct_end

#endif
