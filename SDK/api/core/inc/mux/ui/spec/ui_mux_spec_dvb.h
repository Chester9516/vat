
#ifndef _UI_MUX_SPEC_DVB_
#define _UI_MUX_SPEC_DVB_

#include <mux/ui/ui_props_mux.h>
#include <mux/spec/spec_dvb.h>

_mux_string_start(mux_spec_iso)
_mux_string_end

_mux_string_start(mux_spec_dvb)
	_mux_string(0, network_name, _MUX_STRING_TYPE_ASCII, 16)
	_mux_string(1, service_name, _MUX_STRING_TYPE_ASCII, 16)
_mux_string_end

/* _dvb channel */
_mux_ui_struct_start(mux_spec_dvb_channel)
	_mux_h16(mux_spec_dvb_channel, transport_stream_id, "transport_stream_id")
	_mux_h16(mux_spec_dvb_channel, network_id, "network_id")
	_mux_str(mux_spec_dvb_channel, network_name, "network_name",0)
_mux_ui_struct_end(mux_spec_dvb_channel, mux_ui_level_channel)

/* _dvb program */
_mux_ui_struct_start(mux_spec_dvb_program)
	_mux_h16(mux_spec_dvb_program, original_network_id, "original_network_id")
	_mux_h16(mux_spec_dvb_program, program_number, "program_number")
	_mux_h16(mux_spec_dvb_program, channel_no, "channel_no")
	_mux_str(mux_spec_dvb_program, service_name,"service_name",1)
_mux_ui_struct_end(mux_spec_dvb_program, mux_ui_level_program)



#endif
