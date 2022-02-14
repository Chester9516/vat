#include <core/tools/tool_dvb.h>

#define _DVBT_SB(m)			((uint32_t)(m*8000000)/7)
#define DVB_T_OFDMFRAME_SYMBOL          68
#define DVB_T_SUPERFRAME_OFDMFRAME      4

const static uint8_t dvb_t_qam_bits[] = { 2,0,4,0,6, };
const static uint16_t dvb_t_carrier[][2] = { {2048,1512,},{8192,6048,},{4096,3024}, };

static const uint32_t dvb_t_sb[] = { _DVBT_SB(5),_DVBT_SB(6),_DVBT_SB(7),_DVBT_SB(8), };

#define _crsymbol_base(p)		((dvb_t_carrier[p->fft][0] * GI_RATE[p->guardinterval][1])/ GI_RATE[p->guardinterval][0])
#define _crdata_base(p)			(dvb_t_carrier[p->fft][1])

#define _crsymbol(p)			(_crsymbol_base(p) * DVB_T_OFDMFRAME_SYMBOL * DVB_T_SUPERFRAME_OFDMFRAME)
#define _crdata(p)				(_crdata_base(p) * DVB_T_OFDMFRAME_SYMBOL * DVB_T_SUPERFRAME_OFDMFRAME)

vatek_result tool_dvb_t_superframe_reset(Pmodulator_param pmod, Pofdm_frame pframe)
{
	vatek_result nres = tool_dvb_t_check_param(pmod);
	if (is_vatek_success(nres))
	{
		Pdvb_t_param pdvb = &pmod->mod.dvb_t;
		uint32_t bitrate = _crdata(pdvb) * dvb_t_qam_bits[pdvb->constellation];
		bitrate = ((bitrate * CR_RATE[pdvb->coderate][0]) / CR_RATE[pdvb->coderate][1]);
		bitrate = (bitrate * 188) / 204;
		pframe->frametick = (uint32_t)(((uint64_t)_crsymbol(pdvb) * 27000000) / dvb_t_sb[pmod->bandwidth_symbolrate - 5]);
		pframe->packetnums = bitrate / (TS_PACKET_LEN * 8);
		pframe->framerecv = bitrate % (TS_PACKET_LEN * 8);
	}
	return nres;
}

vatek_result tool_dvb_t_check_param(Pmodulator_param pmod)
{
	vatek_result nres = vatek_badparam;
	if (pmod->type == modulator_dvb_t)
	{
		Pdvb_t_param pdvb = &pmod->mod.dvb_t;
		if (is_dvb_t_valid_bw(pmod->bandwidth_symbolrate) &&
			is_constellation_dvb_t(pdvb->constellation) &&
			is_base_coderate(pdvb->coderate) &&
			is_base_guard_interval(pdvb->guardinterval) &&
			is_base_fft(pdvb->fft))nres = vatek_success;
	}
	return nres;
}

uint32_t tool_dvb_t_get_bitrate(Pmodulator_param pmod)
{
	vatek_result nres = tool_dvb_t_check_param(pmod);
	if (is_vatek_success(nres))
	{
		Pdvb_t_param pdvb = &pmod->mod.dvb_t;
		uint32_t bitrate = _crdata(pdvb);
		bitrate *= dvb_t_qam_bits[pdvb->constellation];
		bitrate = ((bitrate * CR_RATE[pdvb->coderate][0]) / CR_RATE[pdvb->coderate][1]);
		bitrate = (bitrate * 188) / 204;
		bitrate = (uint32_t)(((uint64_t)bitrate * dvb_t_sb[pmod->bandwidth_symbolrate - 5]) / _crsymbol(pdvb));
		return bitrate;
	}
	return 0;
}
