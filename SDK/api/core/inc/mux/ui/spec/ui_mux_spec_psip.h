
#ifndef _UI_MUX_SPEC_PSIP_
#define _UI_MUX_SPEC_PSIP_

#include <mux/ui/ui_props_mux.h>
#include <mux/spec/spec_psip.h>

_mux_string_start(mux_spec_psip)
	_mux_string(0, short_name, _MUX_STRING_UTF16, 14)
	_mux_string(1, long_name, _MUX_STRING_PSIP_MUTIL, 32)
_mux_string_end

_ui_enum_start(psip_cc_mode)
	_val(cc_mode_disable, disable)
	_val(cc_mode_608, cc_608)
_ui_enum_end

/* _psip channel */

_mux_ui_struct_start(mux_spec_psip_channel)
	_mux_h32(mux_spec_psip_channel,psip_flags,"psip flags")
	_mux_h16(mux_spec_psip_channel, transport_stream_id, "transport_stream_id")
	_mux_u8(mux_spec_psip_channel, gps_utc_offset, "gps utc offset")
	_mux_u16(mux_spec_psip_channel, daylight_saving, "daylight saving")
	_mux_enum(mux_spec_psip_channel, psip_cc_mode, cc_mode, "caption service mode")
	_mux_str(mux_spec_psip_channel, short_name, "short_name", 0)
_mux_ui_struct_end(mux_spec_psip_channel, mux_ui_level_channel)

_mux_ui_struct_start(mux_spec_psip_program)
	_mux_h16(mux_spec_psip_program, program_number, "program_number")
	_mux_h16(mux_spec_psip_program, source_id, "source_id")
	_mux_u16(mux_spec_psip_program, channel_major, "channel_major")
	_mux_u16(mux_spec_psip_program, channel_minor, "channel_minor")
	_mux_str(mux_spec_psip_program, long_name, "long_name", 1)
_mux_ui_struct_end(mux_spec_psip_program, mux_ui_level_program)

#endif
