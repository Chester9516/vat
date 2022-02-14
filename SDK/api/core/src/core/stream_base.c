#include <core/base/stream_base.h>
#include <core/ui/ui_props/ui_props_stream.h>
#include <core/ui/ui_props_api.h>

const Pui_prop_item stream_source_get_ui_props(stream_source source)
{
	if (source == stream_source_tsin)
		return _ui_struct(tsin_param);
	else if (source == stream_source_usb)
		return _ui_struct(usb_param);
	return _ui_struct(encoder_param);
}

vatek_result stream_source_reset(vatek_ic_module icmodule, stream_source source, uint8_t* prawsource)
{
	vatek_result nres = vatek_success;
	if (source == stream_source_tsin)
		memcpy(prawsource, &default_tsin_param, sizeof(tsin_param));
	else if (source == stream_source_usb)
		memcpy(prawsource, &default_usb_param, sizeof(usb_param));
	else if (source == stream_source_encoder)
	{
		if (icmodule == ic_module_b3_lite)
			nres = encoder_param_reset(encvideo_h264, (Pencoder_param)prawsource);
		else nres = encoder_param_reset(encvideo_mpeg2, (Pencoder_param)prawsource);
	}
	else nres = vatek_badparam;
	return nres;
}

vatek_result stream_source_set(hvatek_chip hchip,stream_source source, uint8_t* prawsource)
{
	vatek_result nres = vatek_badparam;
	const Pui_prop_item puiprops = stream_source_get_ui_props(source);
	if (puiprops)
	{
		if (source == stream_source_encoder)
			nres = encoder_param_set(hchip, (Pencoder_param)prawsource);
		else nres = ui_props_write_hal(hchip, puiprops, prawsource);
	}
	return nres;
}

vatek_result stream_source_get(hvatek_chip hchip,stream_source source, uint8_t* prawsource)
{
	vatek_result nres = vatek_badparam;
	const Pui_prop_item puiprops = stream_source_get_ui_props(source);
	if (puiprops)
	{
		if (source == stream_source_encoder)
			nres = encoder_param_get(hchip, (Pencoder_param)prawsource);
		else nres = ui_props_read_hal(hchip, puiprops, prawsource);
	}
	return nres;
}
