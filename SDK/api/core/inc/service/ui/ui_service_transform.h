#ifndef _UI_SERVICE_TRANSFORM_
#define _UI_SERVICE_TRANSFORM_

#include <core/ui/ui_props/ui_props_chip.h>
#include <service/ui/ui_service_base.h>
#include <service/service_transform.h>

_ui_enum_start(transform_mode)
	_val(trmode_enum,enum)
	_val(trmode_capture, capture)
	_val(trmode_broadcast, broadcast)
_ui_enum_end

HALREG_S(TRENUM_TIMEOUT, 32)
HALREG_S(TRANSFORM_INPUT, 16)

HALREG_S(TRCAPTURE_PID, 16)
HALREG_S(TRCAPTURE_TIMEOUT, 16)
HALREG_S(TRCAPTURE_SECTION_NUM, 16)
HALREG_S(TRCAPTURE_PACKET_NUMS, 16)

_ui_struct_start(transform_enum)
	_prop_enum(transform_enum, stream_source, source, "stream source", TRANSFORM_INPUT)
	_prop_u16(transform_enum, timeout, "process timeout [ms]", TRENUM_TIMEOUT)
_ui_struct_end

_ui_struct_start(transform_capture)
	_prop_enum(transform_capture, stream_source, source, "stream source", TRANSFORM_INPUT)
	_prop_h16(transform_capture, pid, "capture pid", TRCAPTURE_PID)
	_prop_u16(transform_capture, timeout, "capture timeout [ms]", TRCAPTURE_TIMEOUT)
	_prop_u16(transform_capture, section_num, "capture psi table section number", TRCAPTURE_SECTION_NUM)
	_prop_u16(transform_capture, packet_nums, "capture max packet number", TRCAPTURE_PACKET_NUMS)
_ui_struct_end

_ui_struct_start(transform_broadcast)
	_prop_enum(transform_capture, stream_source, source, "stream source", TRANSFORM_INPUT)
_ui_struct_end

#endif
