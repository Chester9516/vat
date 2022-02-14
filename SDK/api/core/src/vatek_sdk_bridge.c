#include <vatek_sdk_bridge.h>
#include <cross/cross_bridge.h>
#include "device/internal/cross_device_tool.h"

typedef struct _bridge_handle
{
	hvatek_chip hchip;
	Pcross_device hcross;
	hbridge_device hbridge;
	bdevice_info info;
}bridge_handle,*Pbridge_handle;

vatek_result vatek_bridge_open(hvatek_chip hchip, hvatek_bridge* hbridge)
{
	Pcross_device pcross = ((Pvatek_device)hchip)->cross;
	hbridge_device hdevice = cross_get_bridge_handle(pcross);
	vatek_result nres = vatek_unsupport;
	if (hdevice)
	{
		Pbridge_handle newbridge = (Pbridge_handle)malloc(sizeof(bridge_handle));
		nres = vatek_memfail;
		if (newbridge)
		{
			memset(newbridge, 0, sizeof(bridge_handle));
			newbridge->hchip = hchip;
			newbridge->hcross = pcross;
			newbridge->hbridge = hdevice;
			nres = bridge_device_lock(hdevice);
			if (is_vatek_success(nres))
				nres = bridge_device_get_info(hdevice, &newbridge->info);
			if (is_vatek_success(nres))*hbridge = newbridge;
			else free(newbridge);
		}
	}
	return nres;
}

void vatek_bridge_close(hvatek_bridge hbridge)
{
	Pbridge_handle pbridge = (Pbridge_handle)hbridge;
	bridge_device_unlock(pbridge->hbridge);
	free(pbridge);
}

Pbdevice_info vatek_bridge_get_info(hvatek_bridge hbridge)
{
	Pbridge_handle pbridge = (Pbridge_handle)hbridge;
	return &pbridge->info;
}

vatek_result vatek_bridge_get_av_source(hvatek_bridge hbridge, int32_t idx, Pbridge_source psource)
{
	Pbridge_handle pbridge = (Pbridge_handle)hbridge;
	vatek_result nres = vatek_badparam;
	if (idx < (int32_t)pbridge->info.source_nums)
		nres = bridge_device_get_source(pbridge->hbridge, idx, psource);
	return nres;
}

const char* vatek_bridge_get_av_source_name(hvatek_bridge hbridge, Pbridge_source psource)
{
	Pbridge_handle pbridge = (Pbridge_handle)hbridge;
	return bridge_device_get_source_name(pbridge->hbridge, psource);
}

vatek_result vatek_bridge_start_av_source(hvatek_bridge hbridge, Pbridge_source psource)
{
	Pbridge_handle pbridge = (Pbridge_handle)hbridge;
	return bridge_device_start_source(pbridge->hbridge, psource);
}

vatek_result vatek_bridge_get_av_source_status(hvatek_bridge hbridge, Pbridge_source psource)
{
	Pbridge_handle pbridge = (Pbridge_handle)hbridge;
	return bridge_device_get_source_status(pbridge->hbridge, psource);
}

vatek_result vatek_bridge_stop_av_source(hvatek_bridge hbridge)
{
	Pbridge_handle pbridge = (Pbridge_handle)hbridge;
	return bridge_device_stop_source(pbridge->hbridge);
}

vatek_result vatek_bridge_write_section(hvatek_bridge hbridge, int32_t section, uint8_t* pbuffer)
{
	Pbridge_handle pbridge = (Pbridge_handle)hbridge;
	vatek_result nres = bridge_device_storage_write(pbridge->hbridge, section, pbuffer);
	return nres;
}

vatek_result vatek_bridge_read_section(hvatek_bridge hbridge, int32_t section, uint8_t* pbuffer)
{
	Pbridge_handle pbridge = (Pbridge_handle)hbridge;
	vatek_result nres = bridge_device_storage_read(pbridge->hbridge, section, pbuffer);
	return nres;
}