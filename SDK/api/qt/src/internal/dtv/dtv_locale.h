#ifndef DTV_LOCALE_H
#define DTV_LOCALE_H

#include <mux/mux_define.h>
#include <core/base/output_modulator.h>
#include "dtv_channel.h"

typedef struct _qdtv_country
{
	const char* name;
	mux_spec_mode spec;
	mux_country_code country;
	modulator_type modulation;
	uint32_t bw_sw;
	video_codec video;
	audio_codec audio;
	const Pqdtv_ch_table channels;
}qdtv_country, * Pqdtv_country;

#define _ucuntry_start					static const qdtv_country qtv_country_tables[] = {
#define _ucountry(tag,s,c,m,b,v,a,ch)	{#tag,mux_spec_##s,c,modulator_##m,b,v,a,_uchannel_table_get(ch),},
#define _ucountry_end					{NULL,mux_spec_undefine,mux_country_undefined,modulator_unknown,0,encvideo_unknown,encaudio_mp2,NULL,},};

#define _ucountry_table					((const Pqdtv_country)&qtv_country_tables[0])

_ucuntry_start
	_ucountry(atsc, psip, atsc_usa, atsc, 6, encvideo_mpeg2, encaudio_ac3, uhf_6)
	_ucountry(brazil_abnt, abnt, abnt_brazil, isdb_t, 6, encvideo_h264, encaudio_aac_lc_latm, jp_uhf_6)
	_ucountry(china - dtmb, dvb, dtmb_china, dtmb, 8, encvideo_h264, encaudio_mp2, uhf_8)
	_ucountry(europe_dvb_t, dvb, mux_country_undefined, dvb_t, 8, encvideo_mpeg2, encaudio_mp2, uhf_8)
	_ucountry(europe_dvb_t2, dvb, mux_country_undefined, dvb_t2, 8, encvideo_hevc, encaudio_mp2, uhf_8)
	_ucountry(freetv_dvb_t, dvb, freetv_australian, dvb_t, 8, encvideo_mpeg2, encaudio_mp2, uhf_8)
	_ucountry(freetv_dvb_t2, dvb, freetv_australian, dvb_t2, 8, encvideo_mpeg2, encaudio_mp2, uhf_8)
	_ucountry(japan_arib, arib, arib_japan, isdb_t, 6, encvideo_mpeg2, encaudio_aac_lc_adts, jp_uhf_6)
	_ucountry(nordig_dvb_t, dvb, nordig_finland, dvb_t, 8, encvideo_mpeg2, encaudio_mp2, uhf_8)
	_ucountry(nordig_dvb_t2, dvb, nordig_finland, dvb_t2, 8, encvideo_hevc, encaudio_mp2, uhf_8)
	_ucountry(taiwan, dvb, dvb_taiwan, dvb_t, 6, encvideo_h264, encaudio_mp2, uhf_6)
_ucountry_end

#endif