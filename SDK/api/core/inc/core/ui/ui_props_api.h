#ifndef _UI_PROPS_API_
#define _UI_PROPS_API_

#include <core/ui/ui_props_define.h>

#ifdef __cplusplus 
extern "C" {
#endif

	HAL_API vatek_result ui_props_set_uint32(const Pui_prop_item pprop, uint8_t* pblock, uint32_t val);
	HAL_API vatek_result ui_props_get_uint32(const Pui_prop_item pprop, uint8_t* pblock, uint32_t* val);

	HAL_API vatek_result ui_props_set_strbuf(const Pui_prop_item props, uint8_t* pblock, uint8_t* val);
	HAL_API vatek_result ui_props_get_strbuf(const Pui_prop_item props, uint8_t* pblock, uint8_t** val);
	HAL_API vatek_result ui_props_get_strbuf_len(const Pui_prop_item props);
    
	HAL_API vatek_result ui_props_read_hal(hvatek_chip hchip,const Pui_prop_item pprop, uint8_t* pblock);
	HAL_API vatek_result ui_props_write_hal(hvatek_chip hchip,const Pui_prop_item pprop, uint8_t* pblock);

	HAL_API const Pui_prop_item ui_props_check_values(const Pui_prop_item pprops, uint8_t* pblock);
	HAL_API void ui_props_get_value_str(const Pui_prop_item pprop, uint8_t* pblock, char* szval, int32_t len);
	HAL_API const char* ui_props_get_type_str(ui_prop_type type,const Pui_prop_val pvals);

	HAL_API const Pui_prop_item ui_props_get_param(const Pui_prop_item pprops, const char* name);
	HAL_API vatek_result ui_props_is_same_param(const Pui_prop_item props, const char* name,const Pui_prop_item prop);

	HAL_API const char* ui_enum_val2str(const Pui_prop_val vals, uint32_t val);
	HAL_API vatek_result ui_enum_str2val(const Pui_prop_val vals, const char* str,uint32_t* val);

	HAL_API void ui_props_printf(const char* fmt_n_t_v,const char* fmt_sv, const Pui_prop_item puiprops, uint8_t* prawbuf);

	#define ui_props_is_struct_param(type,name,prop)	is_vatek_success(ui_props_is_same_param(_ui_struct(type),#name,prop))

	#define ui_enum_get_str(t,val)		ui_enum_val2str(_ui_enum(t),(uint32_t)val)
	#define ui_enum_get_val(t,str,val)	ui_enum_str2val(_ui_enum(t),str,&val)

#ifdef __cplusplus
}
#endif

#endif
