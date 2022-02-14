#include <mux/mux_handle.h>
#include "internal/mux_core.h"

vatek_result mux_open_raw(hmux_core hmux,hmux_rawtable* hraw)
{
    vatek_result nres = vatek_badstatus;
    Phandle_mux pmux = (Phandle_mux)hmux;
    if(mux_check_idle(pmux))
    {
        Phandle_raw newraw = (Phandle_raw)th_mempool_malloc(pmux->mempool,sizeof(handle_raw));
        nres = vatek_memfail;
        if(newraw)
        {
            pmux->hmode = hmux_mode_raw;
            pmux->mode = mux_psi_rawtable;
            pmux->_h.raw = newraw;
            if(hraw)*hraw = pmux;
            nres = vatek_success;
        }
    }
    return nres;
}

vatek_result muxraw_get_handle(hmux_core hmux, hmux_rawtable* hraw)
{
    vatek_result nres = vatek_badstatus;
    Phandle_mux pmux = (Phandle_mux)hmux;
    if (pmux->hmode == hmux_mode_raw)
    {
        *hraw = pmux;
        nres = vatek_success;
    }
    return nres;
}

vatek_result muxraw_create_table(hmux_rawtable hraw, int32_t packetnums, int32_t intervalms,Ppsi_table_raw* ptable)
{
    Phandle_mux pmux = (Phandle_mux)hraw;
    vatek_result nres = vatek_badstatus;
    if(pmux->hmode == hmux_mode_raw)
    {
		int32_t len = sizeof(psi_table_raw) + packetnums * TS_PACKET_LEN;
		Ppsi_table_raw newpsi = (Ppsi_table_raw)th_mempool_malloc(pmux->mempool, len);
		nres = vatek_memfail;
		if (newpsi)
		{
			newpsi->interval_ms = intervalms;
			newpsi->packet_nums = packetnums;
			newpsi->packet_buf = &((uint8_t*)newpsi)[sizeof(psi_table_raw)];
			*ptable = newpsi;
			nres = vatek_success;
		}
    }
    return nres;
}

vatek_result muxraw_push_table(hmux_rawtable hraw, Ppsi_table_raw ptable)
{
    Phandle_mux pmux = (Phandle_mux)hraw;
    vatek_result nres = vatek_badstatus;

    if(pmux->hmode == hmux_mode_raw)
    {
		Ppsiraw_handle praw = &pmux->_h.raw->rawtables;
		ptable->next = NULL;
		if (!praw->rawtables)
			praw->rawtables = ptable;
		else praw->lasttable->next = ptable;
		praw->lasttable = ptable;
		nres = vatek_success;
    }
    return nres;
}

vatek_result muxraw_get_table(hmux_rawtable hraw, Ppsi_table_raw* ptables)
{
	Phandle_mux pmux = (Phandle_mux)hraw;
	vatek_result nres = vatek_badstatus;

	if (pmux->hmode == hmux_mode_raw)
	{
        Ppsiraw_handle praw = &pmux->_h.raw->rawtables;
        *ptables = praw->rawtables;
		nres = vatek_success;
	}
	return nres;
}
