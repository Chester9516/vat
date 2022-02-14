#include <core/tools/tool_output.h>
#include <core/tools/tool_isdb_t.h>
#include <core/tools/tool_dvb_t2.h>
#include <core/tools/tool_dvb.h>
#include <core/tools/tool_8vsb.h>
#include <core/tools/tool_j83abc.h>
#include <core/tools/tool_dtmb.h>

typedef struct _modoutput_clock
{
	int32_t ismalloc;
	mux_clock_tick tickpkt;
	mux_clock_tick tickframe;
	modulator_type modtype;
	uint32_t adjusttick;
	int32_t adjustvalue;
	ofdm_position pos;
	union
	{
		isdb_t_bts_frame bts;
		ofdm_frame ofdm;
		dvb_t2_frame t2frame;
	}_h;
}modoutput_clock, * Pmodoutput_clock;

vatek_result tool_output_clock_create(Pmodulator_param pmod, houtput_clock* houtclk)
{
	vatek_result nres = vatek_memfail;
	Pmodoutput_clock pout = (Pmodoutput_clock)malloc(sizeof(modoutput_clock));
	if (pout)
	{
		pout->ismalloc = 1;
		nres = tool_output_clock_config(pout, pmod);
		if (is_vatek_success(nres))*houtclk = pout;
	}
	return nres;
}

vatek_result tool_output_clock_create_mempool(Pmodulator_param pmod, Pth_mempool pmem, houtput_clock* houtclk)
{
	vatek_result nres = vatek_memfail;
	Pmodoutput_clock pout = (Pmodoutput_clock)th_mempool_malloc(pmem, sizeof(modoutput_clock));
	if (pout)
	{
		pout->ismalloc = 0;
		nres = tool_output_clock_config(pout, pmod);
		if (is_vatek_success(nres))*houtclk = pout;
	}
	return nres;
}

vatek_result tool_output_clock_config(houtput_clock houtclk, Pmodulator_param pmod)
{
	Pmodoutput_clock pout = (Pmodoutput_clock)houtclk;
	int32_t ismalloc = pout->ismalloc;
	vatek_result nres = vatek_success;
	memset(pout, 0, sizeof(modoutput_clock));
	pout->ismalloc = ismalloc;
	pout->modtype = modulator_unknown;

	if (pmod->type == modulator_isdb_t)nres = tool_isdb_t_bts_frame_reset(pmod, &pout->_h.bts);
	else if (pmod->type == modulator_atsc)nres = tool_8vsb_vsbframe_reset(pmod, &pout->_h.ofdm);
	else if (pmod->type == modulator_dvb_t)nres = tool_dvb_t_superframe_reset(pmod, &pout->_h.ofdm);
	else if (pmod->type == modulator_j83a || pmod->type == modulator_j83b || pmod->type == modulator_j83c)
		nres = tool_j83_j83frame_reset(pmod, &pout->_h.ofdm);
	else if (pmod->type == modulator_dtmb)nres = tool_dtmb_superframe_reset(pmod, &pout->_h.ofdm);
	else if (pmod->type == modulator_dvb_t2)nres = tool_dvb_t2_frame_reset(pmod, &pout->_h.t2frame);
	else nres = vatek_unsupport;
	if (is_vatek_success(nres))
	{
		pout->modtype = pmod->type;
		nres = tool_output_clock_reset(houtclk);
		if(!is_vatek_success(nres))pout->modtype = modulator_unknown;
	}
	return nres;
}

void tool_output_clock_set_tick(houtput_clock houtclk, Pmux_clock_tick pclk)
{
	Pmodoutput_clock pout = (Pmodoutput_clock)houtclk;
	memcpy(&pout->tickframe, pclk, sizeof(mux_clock_tick));
}

vatek_result tool_output_clock_get_pcrpacket(houtput_clock houtclk, int32_t* intervalms)
{
	Pmodoutput_clock pout = (Pmodoutput_clock)houtclk;
	if (pout->modtype == modulator_isdb_t)
	{
		Pisdb_t_bts_frame pbts = &pout->_h.bts;
		if(intervalms)*intervalms = pbts->frameus / 1000;
		return pbts->ofdm.packetnums;
	}
	else
	{
		Pofdm_frame pofdm = &pout->_h.ofdm;
		uint32_t tickms = OUTCLK_DEFAULT_PCRINTERVAL_MS;
		if (intervalms)*intervalms = tickms;
#if 0
		if(intervalms)tickms = *intervalms;
		if (!tickms)tickms = OUTCLK_DEFAULT_PCRINTERVAL_MS;
		else if (tickms > 255)tickms = OUTCLK_DEFAULT_PCRINTERVAL_MS;
#endif
		tickms = (uint32_t)(27000 * tickms);
		tickms /= (pofdm->frametick / pofdm->packetnums);
		return (vatek_result)tickms;
	}
}

void tool_output_clock_adjust_tick(houtput_clock houtclk,uint32_t tick27mhz)
{
	Pmodoutput_clock pout = (Pmodoutput_clock)houtclk;
	pout->adjusttick = tick27mhz;
	//pout->adjustvalue = 0;
}

Pmux_clock_tick tool_output_clock_append(houtput_clock houtclk)
{
	Pmodoutput_clock pout = (Pmodoutput_clock)houtclk;
	pout->pos.packetptr++;
	pout->adjustvalue += pout->adjusttick;

	if (pout->pos.packetptr == pout->pos.packetnums)
	{
		Pofdm_frame pofdm = &pout->_h.ofdm;
		uint32_t adjvalue = pout->adjustvalue / 5000;

		pout->adjustvalue %= 5000;

		if (pout->modtype == modulator_isdb_t)pofdm = &pout->_h.bts.ofdm;
		else if (pout->modtype == modulator_dvb_t2)pofdm = &pout->_h.t2frame.ofdm;

		pout->pos.packetnums = pofdm->packetnums;
		pout->pos.bitrecv += pofdm->framerecv;
		if (pout->pos.bitrecv > (TS_PACKET_LEN * 8))
		{
			pout->pos.packetnums++;
			pout->pos.bitrecv -= (TS_PACKET_LEN * 8);
		}




		pout->pos.pkttick = (pofdm->frametick + adjvalue) / pout->pos.packetnums;
		pout->pos.packetptr = 0;
		mux_clock_append(&pout->tickframe, (pofdm->frametick + adjvalue));
		memcpy(&pout->tickpkt, &pout->tickframe, sizeof(mux_clock_tick));
	}
	else mux_clock_append(&pout->tickpkt, pout->pos.pkttick);

	return &pout->tickpkt;
}

Pmux_clock_tick tool_output_clock_get_tick(houtput_clock houtclk)
{
	Pmodoutput_clock pout = (Pmodoutput_clock)houtclk;
	if (pout->modtype == modulator_isdb_t)
	{
		Pisdb_t_bts_frame pbts = &pout->_h.bts;
		vatek_result nres = tool_isdb_t_bts_frame_get_tick(pbts, pout->pos.packetptr);
		if (is_vatek_success(nres))
		{
			memcpy(&pout->tickpkt, &pout->tickframe, sizeof(mux_clock_tick));
			mux_clock_append(&pout->tickpkt, nres);
		}
		else VWAR("bad isdb_t frame position : [%d:%d]", pout->pos.packetptr, pout->pos.packetnums);
	}
	else
	{
		Pofdm_frame pofdm = &pout->_h.ofdm;
		uint32_t frametick = (uint32_t)(((uint64_t)pout->pos.packetptr * pofdm->frametick) / pofdm->packetnums);
		memcpy(&pout->tickpkt, &pout->tickframe, sizeof(mux_clock_tick));
		mux_clock_append(&pout->tickpkt, frametick);
	}
	return &pout->tickpkt;
}

vatek_result tool_output_clock_reset(houtput_clock houtclk)
{
	vatek_result nres = vatek_badstatus;
	Pmodoutput_clock pout = (Pmodoutput_clock)houtclk;
	if (is_current_modulator(pout->modtype))
	{
		Pofdm_frame pofdm = &pout->_h.ofdm;
		if (pout->modtype == modulator_isdb_t)
			pofdm = &pout->_h.bts.ofdm;
		memset(&pout->pos, 0, sizeof(ofdm_position));
		memset(&pout->tickframe, 0, sizeof(mux_clock_tick));
		memset(&pout->tickpkt, 0, sizeof(mux_clock_tick));
		pout->pos.packetnums = pofdm->packetnums;
		pout->pos.pkttick = pofdm->frametick / pofdm->packetnums;
		nres = vatek_success;
	}
	return nres;
}

void tool_output_clock_free(houtput_clock houtclk)
{
	Pmodoutput_clock pout = (Pmodoutput_clock)houtclk;
	if (pout->ismalloc)free(pout);
}
