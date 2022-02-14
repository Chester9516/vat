
#ifndef _UI_MUX_SPEC_ISO_
#define _UI_MUX_SPEC_ISO_

#include <mux/ui/ui_props_mux.h>
#include <mux/spec/spec_iso_13818.h>

_mux_ui_struct_start(mux_spec_iso_channel)
	_mux_h16(mux_spec_iso_channel, transport_stream_id, "transport_stream_id")
_mux_ui_struct_end(mux_spec_iso_channel, mux_ui_level_channel)

/* _iso program */
_mux_ui_struct_start(mux_spec_iso_program)
	_mux_h16(mux_spec_iso_program, program_number, "program_number")
_mux_ui_struct_end(mux_spec_iso_program, mux_ui_level_program)

#endif
