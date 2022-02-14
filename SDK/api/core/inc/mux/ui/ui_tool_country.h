

#ifndef _UI_TOOL_COUNTRY_
#define _UI_TOOL_COUNTRY_

#include <mux/mux_define.h>

#if 0

typedef struct _mux_ui_country
{
	const char* name;
	mux_country_code country;
}mux_ui_country, *Pmux_ui_country;

#define _country_table_start			static const mux_ui_country countr_ui_all[] = {
#define _country_table_end				{NULL,mux_country_undefined,},};
#define _country_name(name,code)		{#name,code,},
#define _country(name)					{#name,name,},
#define _country_dvb(name)				{#name,dvb_##name,},
#define _country_nordig(name)			{#name,nordig_##name,},
#define _country_dtmb(name)				{#name,dtmb_##name,},
#define _country_arib(name)				{#name,arib_##name,},
#define _country_abnt(name)				{#name,abnt_##name,},
#define _country_atsc(name)				{#name,atsc_##name,},

_country_table_start
	_country_name(undefined, mux_country_undefined)
	_country_dvb(taiwan)
	_country_dvb(asean)
	_country_dvb(uk)
	_country_dvb(spanish)
	_country_dvb(france)
	_country_dvb(italian)
	_country_dvb(colombia)
	_country_dvb(india)
	_country_dvb(ghana)
	_country_dvb(singapore)
	_country_dvb(georgin)
	_country_nordig(finland)
	_country_dtmb(china)
	_country_dtmb(hk)
	_country_dtmb(cuba)
	_country_arib(japan)
	_country_abnt(brazil)
	_country_abnt(argentina)
	_country_abnt(bolivia)
	_country_abnt(botswana)
	_country_abnt(costa_rica)
	_country_abnt(chile)
	_country_abnt(ecuador)
	_country_abnt(paraguay)
	_country_abnt(philippines)
	_country_abnt(peru)
	_country_abnt(uruguay)
	_country_abnt(venezuela)
	_country_atsc(usa)
	_country_atsc(korean)
	_country_atsc(canada)
	_country_atsc(mexico)
	_country_atsc(dominican)
_country_table_end

#define _country_table			countr_ui_all

#endif

#endif

