#ifndef QTV_UI_DEFINE_H
#define QTV_UI_DEFINE_H

#include <core/qtv_define.h>
#include <core/qtv_service_usb.h>
#include <core/ui/ui_props_define.h>
#include <core/ui/ui_props/ui_props_stream.h>

_ui_enum_start(qtv_filter_mode)
	_val(qfilter_disable, disable)
	_val(qfilter_enum, enum)
	//_val(qfilter_customer, "customer")
_ui_enum_end

_ui_struct_start(qtv_source_param)
	_prop_enum(qtv_source_param, stream_source, source, "source - select current stream source", NULLHAL)
	_prop_enum(qtv_source_param, qtv_filter_mode, filter, "source - transform pid filter", NULLHAL)
_ui_struct_end

_ui_struct_start(qtv_source_param_filter_only)
	_prop_enum(qtv_source_param, qtv_filter_mode, filter, "source - transform pid filter", NULLHAL)
_ui_struct_end

_ui_enum_start(qtv_property_pathmode)
	_val(qpathmode_null,null)
	_val(qpathmode_passthrough, passthroudh)
	_val(qpathmode_file_ts,file_ts)
	_val(qpathmode_url_udp,udp)
_ui_enum_end

_ui_struct_start(qtv_usbstream_param)
	_prop_enum(qtv_usbstream_param, qtv_property_pathmode,mode,"usbstream - source mode(path)",NULLHAL)
	_prop_enum(qtv_usbstream_param, qtv_filter_mode, filter,"usbstream - filter mode",NULLHAL)
	_prop_path(qtv_usbstream_param, url,"usbstream - source [url | path]", _UIPROP_STRBUF_LEN_MAX)
_ui_struct_end

_ui_struct_start(qtv_usbstream_pcronly)
	_prop_enum(usb_param,pcr_adjust_mode,pcrmode,"usbstream - pcr adjust mode",NULLHAL)
_ui_struct_end

_ui_struct_start(qtv_usbstream_param_test)
	_prop_enum(qtv_usbstream_param, qtv_property_pathmode, mode, "usbstream - source mode(path)", NULLHAL)
_ui_struct_end

_ui_struct_start(qtv_usbstream_passthroudh)
	_prop_enum(qtv_usbstream_param, qtv_property_pathmode, mode, "usbstream - source mode(path)", NULLHAL)
	_prop_path(qtv_usbstream_param, url, "usbstream - source [url | path]", _UIPROP_STRBUF_LEN_MAX)
_ui_struct_end

_ui_struct_start(qtv_output_param)
	_prop_u32(qtv_output_param, frequency,"output - frequency(KHz)",NULLHAL)
_ui_struct_end

_ui_struct_start(qtv_usbmuxer_param)
	_prop_h16(qtv_usbmuxer_param, pcr_pid, "usbmux - pcr pid", NULLHAL)
	_prop_u16(qtv_usbmuxer_param, pcr_interval, "usbmux - pcr interval ms", NULLHAL)
	_prop_u16(qtv_usbmuxer_param, latency_ms,"usbmux - latency ms",NULLHAL)
_ui_struct_end

#endif
