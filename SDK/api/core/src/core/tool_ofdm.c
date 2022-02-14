#include <core/tools/tool_ofdm.h>
#include <core/tools/tool_8vsb.h>
#include <core/tools/tool_dvb.h>
#include <core/tools/tool_dtmb.h>
#include <core/tools/tool_isdb_t.h>
#include <core/tools/tool_j83abc.h>
#include <core/tools/tool_dvb_t2.h>

#include <core/ui/ui_props/ui_props_modulator.h>

#define _mparam_prop(type,tag)   {sizeof(tag),(const uint8_t*)&default_##tag,_ui_struct(tag),tool_##type##_get_bitrate,tool_##type##_check_param},
#define _mparam_j83(type,tag)    {sizeof(tag),(const uint8_t*)&default_##tag,_ui_struct(tag),tool_j83_get_bitrate,tool_j83_check_param},

static const ofdm_modulation modulations[modulator_mod_nums] =
{
	_mparam_prop(dvb_t,dvb_t_param)
	_mparam_j83(j83a,j83a_param)
	_mparam_prop(atsc,atsc_param)
	_mparam_j83(j83b,j83b_param)
	_mparam_prop(dtmb,dtmb_param)
	_mparam_prop(isdb_t,isdb_t_param)
	_mparam_j83(j83c,j83c_param)
	_mparam_prop(dvb_t2,dvb_t2_param)
};

vatek_result tool_ofdm_get_modulation(modulator_type type, Pofdm_modulation* pofdm)
{
	vatek_result nres = vatek_badparam;
	if (is_current_modulator(type))
	{
		*pofdm = (Pofdm_modulation)&modulations[type];
		nres = vatek_success;
	}
	return nres;
}