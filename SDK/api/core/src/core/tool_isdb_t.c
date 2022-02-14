#include <core/tools/tool_isdb_t.h>
#include <core/tools/tool_dvb.h>

#define ISDB_T_TSP_LEN			204
#define ISDB_T_BTS_SYMBOLNUMS	204
#define ISDB_T_DATA_CARRIER		96
#define ISDB_T_SEGMENT_NUMS		13
#define ISDB_T_BTS_BASECLK		408

const uint8_t isdb_t_qam_bits[] = { 2,2,4,6, };
const static uint16_t isdb_t_carrier[] = { 2048, 8192,4096, };
const static uint16_t isdb_t_frametime[] = { 53014,54621,57834,64260, };

#define _fftrate(p)		((isdb_t_carrier[p->fft] / isdb_t_carrier[0]))
#define _crsymbol(p)	((isdb_t_carrier[p->fft] * GI_RATE[p->guardinterval][1])/ GI_RATE[p->guardinterval][0])
#define _crdata(p)		(ISDB_T_DATA_CARRIER * _fftrate(p) * ISDB_T_SEGMENT_NUMS)

vatek_result tool_isdb_t_bts_frame_reset(Pmodulator_param pmod, Pisdb_t_bts_frame pbts)
{
	vatek_result nres = tool_isdb_t_check_param(pmod);
	Pisdb_t_param pisdb = &pmod->mod.isdb_t;
	if (is_vatek_success(nres))
	{	
		int32_t cr_symbol = _crsymbol(pisdb);
		int32_t cr_data = _crdata(pisdb);
		int32_t i, j;
		int32_t bufbts = 0;
		int32_t bufout = 0;
		int32_t ptrpkts = 0;
		int32_t ptroutpkt = 0;
		int32_t ptrclk = 0;
		int32_t qambits = isdb_t_qam_bits[pisdb->constellation];
		int32_t symbolnums = ISDB_T_BTS_SYMBOLNUMS / _fftrate(pisdb);
		memset(pbts, 0, sizeof(isdb_t_bts_frame));

		for (i = 0; i < symbolnums; i++)
		{
			for (j = 0; j < cr_symbol; j++)
			{
				if (j < cr_data)bufbts += qambits;
				ptrclk++;
				if ((ptrclk % ISDB_T_BTS_BASECLK) == 0)
				{	
					bufbts *= CR_RATE[pisdb->coderate][0];
					bufout += (bufbts / CR_RATE[pisdb->coderate][1]) * 2;
					bufbts %= CR_RATE[pisdb->coderate][1];
					//bufout += ((bufbts * CR_RATE[pisdb->coderate][0]) / CR_RATE[pisdb->coderate][1]) * 2;
					//bufbts = 0;

					if (ptrpkts > 1)
					{
						if(bufout >= (ISDB_T_BTS_BASECLK * 8))
						{
							bufout -= ISDB_T_BTS_BASECLK * 8;
							pbts->tsp_maps[ptroutpkt] = ptrpkts + 1;
							ptroutpkt++;
						}
					}
					ptrpkts++;
				}
			}
		}

		i = (cr_symbol * symbolnums) / ISDB_T_BTS_BASECLK;
		if (i == ptrpkts && bufout == 0 && bufbts == 0)
		{
			pbts->frameus = isdb_t_frametime[pisdb->guardinterval];
			pbts->frame_sb = cr_symbol * symbolnums;
			pbts->ofdm.frametick = (uint32_t)(((uint64_t)pbts->frame_sb * 27000000) / ISDB_T_6MHZ_SYMBOL);
			pbts->ofdm.frametick += 1;
			pbts->ofdm.packetnums = ptroutpkt;
			pbts->ofdm.framerecv = 0;
			nres = vatek_success;
		}
	}
	return nres;
}

vatek_result tool_isdb_t_bts_frame_get_tick(Pisdb_t_bts_frame pbts, int32_t pktpos)
{
	uint32_t tick = pbts->tsp_maps[pktpos];
	tick *= ISDB_T_BTS_BASECLK;
	tick = (uint32_t)(((uint64_t)tick * pbts->ofdm.frametick) / pbts->frame_sb);
	return (vatek_result)tick;
}

vatek_result tool_isdb_t_check_param(Pmodulator_param pmod)
{
	vatek_result nres = vatek_badparam;
	if (pmod->type == modulator_isdb_t)
	{
		Pisdb_t_param pisdb = &pmod->mod.isdb_t;
		if (is_base_coderate(pisdb->coderate) &&
			is_base_guard_interval(pisdb->guardinterval) &&
			is_base_fft(pisdb->fft) &&
			is_isdb_t_ti_mode(pisdb->timeinterleaved))nres = vatek_success;
	}
	return nres;
}

uint32_t tool_isdb_t_get_bitrate(Pmodulator_param pmod)
{
	vatek_result nres = tool_isdb_t_check_param(pmod);
	if (is_vatek_success(nres))
	{
		Pisdb_t_param pisdb = &pmod->mod.isdb_t;
		uint32_t bitrate = _crdata(pisdb) * ISDB_T_BTS_SYMBOLNUMS;
		bitrate *= isdb_t_qam_bits[pisdb->constellation];
		bitrate = ((bitrate * CR_RATE[pisdb->coderate][0]) / CR_RATE[pisdb->coderate][1]);
		bitrate = (bitrate * 188) / 204;
		bitrate = (uint32_t)(((uint64_t)bitrate * ISDB_T_6MHZ_SYMBOL) / (_crsymbol(pisdb) * ISDB_T_BTS_SYMBOLNUMS));
		return bitrate;
	}
	return 0;
}
