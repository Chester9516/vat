#ifndef _UI_SERVICE_BASE_
#define _UI_SERVICE_BASE_

#include <core/ui/ui_props_define.h>
#include <service/service_base.h>
#include <core/ui/ui_props/ui_props_stream.h>
#include <core/ui/ui_props/ui_props_output.h>

HALREG_S(BCINFO_STREAM	, 8	)
HALREG_S(BCINFO_OUTPUT	, 8	)
HALREG_S(BCINFO_STATUS	, 32)
HALREG_S(BCINFO_MODRATE	, 32)
HALREG_S(BCINFO_MUXRATE	, 32)
HALREG_S(BCINFO_CURRATE	, 32)
HALREG_S(BCINFO_DATARATE, 32)
HALREG_S(BCINFO_QUERYBUF, 32)

_ui_enum_start(broadcast_status)
	_val(bcstatus_fail_unknown, err_unknown)
	_val(bcstatus_fail_source, err_source)
	_val(bcstatus_fail_timeout, err_timeout)
	_val(bcstatus_fail_drop, err_drop)
	_val(bcstatus_fail_buffer, err_buffer)
	_val(bcstatus_fail_muxer, err_muxer)
	_val(bcstatus_fail_encode, err_encoder)
	_val(bcstatus_fail_media, err_media)
	_val(bcstatus_fail_demux, err_demux)
	_val(bcstatus_idle, idle)
	_val(bcstatus_wait_source, wait_source)
	_val(bcstatus_broadcast, broadcast)
	_val(bcstatus_finish, finish)
_ui_enum_end

_ui_struct_start(broadcast_info)
	_prop_enum(broadcast_info, stream_source, source, "info - stream source", BCINFO_STREAM)
	_prop_enum(broadcast_info, broadcast_status, status, "info - broadcast status", BCINFO_STATUS)
	_prop_u32(broadcast_info, mod_bitrate, "info - modulator bitrate", BCINFO_MODRATE)
	_prop_u32(broadcast_info, mux_bitrate, "info - mux bitrate", BCINFO_MUXRATE)
	_prop_u32(broadcast_info, cur_bitrate, "info - current bitrate", BCINFO_CURRATE)
	_prop_u32(broadcast_info, data_bitrate, "info - data bitrate", BCINFO_DATARATE)
	_prop_u32(broadcast_info, query_buffer,"info - query buffer", BCINFO_QUERYBUF)
_ui_struct_end

#endif
