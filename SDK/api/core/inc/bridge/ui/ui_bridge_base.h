#ifndef _UI_BRIDGE_BASE_
#define _UI_BRIDGE_BASE_

#include <core/ui/ui_props_define.h>
#include <bridge/bridge_base.h>

_ui_enum_start(bridge_status)
	_val(bstatus_invalid, invalid)
	_val(bstatus_hw_fail, hw_fail)
	_val(bstatus_idle, idle)
	_val(bstatus_active, active)
	_val(bstatus_active_protect, active_protect)
_ui_enum_end

#endif
