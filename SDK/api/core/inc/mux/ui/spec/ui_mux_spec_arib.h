
#ifndef _UI_MUX_SPEC_ARIB_
#define _UI_MUX_SPEC_ARIB_

#include <mux/ui/ui_props_mux.h>
#include <mux/spec/spec_arib.h>

_ui_enum_start(arib_abnt_copy)
	_val(arib_abnt_free, free)
	_val(arib_abnt_copyonce, copy_once)
	_val(arib_abnt_forbidden, forbidden)
_ui_enum_end

/* _arib string table */

_mux_string_start(mux_spec_arib)
	_mux_string(0, network_name, _MUX_STRING_TYPE_JIS_ENG, 20)
	_mux_string(1, ts_name, _MUX_STRING_TYPE_JIS_ENG, 20)
	_mux_string(2, service_name, _MUX_STRING_TYPE_JIS_ENG, 20)
_mux_string_end

_mux_string_start(mux_spec_abnt)
	_mux_string(0, network_name, _MUX_STRING_TYPE_ASCII, 20)
	_mux_string(1, ts_name, _MUX_STRING_TYPE_ASCII, 20)
	_mux_string(2, service_name, _MUX_STRING_TYPE_ASCII, 20)
_mux_string_end

/* _arib channel and program */
_mux_ui_struct_start(mux_spec_arib_channel)
	_mux_u8(mux_spec_arib_channel, region_id, "region_id")
	_mux_u8(mux_spec_arib_channel, broadcaster_id, "broadcaster_id")
	_mux_u8(mux_spec_arib_channel, remote_control_key_id, "remote_control_key_id")
	_mux_str(mux_spec_arib_channel, network_name, "network_name", 0)
_mux_ui_struct_end(mux_spec_arib_channel, mux_ui_level_channel)

_mux_ui_struct_start(mux_spec_arib_program)
	_mux_u8(mux_spec_arib_program, service_no, "arib - service number")
	_mux_isolang(mux_spec_arib_program, main_lang, "arib - main_lang")
	_mux_isolang(mux_spec_arib_program, sub_lang, "arib - sub_lang")
	_mux_enum(mux_spec_arib_program, arib_abnt_copy, copy_flag, "copy_flag")
	_mux_str(mux_spec_arib_program, service_name, "service_name", 2)
	_mux_str(mux_spec_arib_program, ts_name, "ts_name", 1)
_mux_ui_struct_end(mux_spec_arib_program, mux_ui_level_program)

/* _abnt channel and program */

_mux_ui_struct_start(mux_spec_abnt_channel)
	_mux_u8(mux_spec_abnt_channel, remote_control_key_id, "remote_control_key_id")
	_mux_h16(mux_spec_abnt_channel, original_network_id, "original_network_id")
	_mux_h16(mux_spec_abnt_channel, area_code, "area_code")
	_mux_str(mux_spec_abnt_channel, network_name, "network_name", 0)
_mux_ui_struct_end(mux_spec_abnt_channel, mux_ui_level_channel)

_mux_ui_struct_start(mux_spec_abnt_program)
	_mux_u8(mux_spec_abnt_program, service_no, "arib - service number")
	_mux_isolang(mux_spec_abnt_program, main_lang, "arib - main_lang")
	_mux_isolang(mux_spec_abnt_program, sub_lang, "arib - sub_lang")
	_mux_enum(mux_spec_abnt_program, arib_abnt_copy, copy_flag, "copy_flag")
	_mux_str(mux_spec_abnt_program, service_name, "service_name", 2)
	_mux_str(mux_spec_abnt_program, ts_name, "ts_name", 1)
_mux_ui_struct_end(mux_spec_abnt_program, mux_ui_level_program)

#endif
