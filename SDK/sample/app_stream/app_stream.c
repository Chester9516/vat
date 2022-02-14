#include <vatek_sdk_usbstream.h>
#include <vatek_sdk_device.h>
#include <cross/cross_os_api.h>

#include "../common/inc/tool_printf.h"
#include "../common/inc/tool_tspacket.h"
#include "../common/inc/tool_stream.h"

static usbstream_param usbcmd = 
{
	.mode = ustream_mode_sync,
	.remux = ustream_remux_pcr,
	.pcradjust = pcr_disable,
	.freq_khz = 473000,							/* output _rf frequency */
	.modulator =
	{
		6,
		modulator_dvb_t,
		ifmode_disable,0,0,
		.mod = {.dvb_t = {dvb_t_qam64,fft_8k,guard_interval_1_16,coderate_5_6,},},
	},
	.sync = {NULL,NULL},
};

extern vatek_result source_sync_get_buffer(void* param, uint8_t** pslicebuf);
extern vatek_result parser_cmd_source(int32_t argc, char** argv,Ptsstream_source psource,Pusbstream_param pustream);

int main(int argc, char *argv[])
{
	hvatek_devices hdevlist = NULL;
	hvatek_chip hchip = NULL;
	hvatek_usbstream hustream = NULL;
	tsstream_source streamsource = { 0, };
	vatek_result nres = vatek_success;
	
#if 0
	/* 
		the easiest way to changed modulation mode used modulator_param_reset.
		example : 
			modulator_param_reset(modulator_atsc,&usbcmd.modulator);
			modulator_param_reset(modulator_j83b,&usbcmd.modulator);
			modulator_param_reset(modulator_dvb_t,&usbcmd.modulator);
			modulator_param_reset(modulator_isdb_t,&usbcmd.modulator);

	*/
	modulator_param_reset(modulator_isdb_t, &usbcmd.modulator);
	usbcmd.modulator.ifmode = ifmode_iqoffset;
	usbcmd.modulator.iffreq_offset = 143;
	nres = modulator_param_get_bitrate(&usbcmd.modulator);
#endif
	nres = parser_cmd_source(argc, argv, &streamsource,&usbcmd);
	/* 
		step 1 :
		- initialized supported device and open
	*/

	if (is_vatek_success(nres))
	{
		nres = vatek_device_list_enum(DEVICE_BUS_USB,service_transform,&hdevlist);
		if (is_vatek_success(nres))
		{
			if (nres == 0)
			{
				nres = vatek_nodevice;
				_disp_err("can not found device.");
			}
			else
			{
				nres = vatek_device_open(hdevlist, 0, &hchip);
				if (!is_vatek_success(nres))
					_disp_err("open device fail : %d", nres);
				else
				{
					Pchip_info pinfo = vatek_device_get_info(hchip);
					printf_chip_info(pinfo);
					nres = streamsource.start(streamsource.hsource);
				}
			}
		}else _disp_err("enum device fail : %d",nres);
	}

	/*
		step 2 :
			- open usb_stream open
			- config used usb_stream sync mode and start stream
	*/

	if (is_vatek_success(nres))
	{
		nres = vatek_usbstream_open(hchip, &hustream);
		if (!is_vatek_success(nres))
			_disp_err("open usb_stream fail - %d", nres);
		else
		{
			usbcmd.mode = ustream_mode_sync;
			usbcmd.sync.param = &streamsource;
			usbcmd.sync.getbuffer = source_sync_get_buffer;
			nres = vatek_usbstream_start(hustream,&usbcmd);
			if (!is_vatek_success(nres))
				_disp_err("start usb_stream fail : %d", nres);
		}
	}

	/* 
		step 3 : 
			- source_sync_get_buffer would call from internal when usb_stream had valid buffer
			- main loop could polling usb_stream status and check information
			- when finished close usb_stream 
	*/

	if (is_vatek_success(nres))
	{
		_disp_l("usb_stream start. press any key to stop");
		int32_t is_stop = 0;
		Ptransform_info pinfo = NULL;
		uint32_t ntickms = cross_os_get_tick_ms();
		while (!is_stop)
		{
			usbstream_status status = vatek_usbstream_get_status(hustream,&pinfo);
			if (status == usbstream_status_running)
			{
				if (cross_os_get_tick_ms() - ntickms > 1000)
				{
					ntickms = cross_os_get_tick_ms();
					_disp_l("usbstream - [%d:%d:%d]",
						pinfo->info.status,
						pinfo->info.data_bitrate,
						pinfo->info.cur_bitrate);
				}
			}
	
			if (!is_vatek_success(nres))break;
			if (cross_os_get_ch_no_wait() != -1)is_stop = 1;
			else cross_os_sleep(1);
		}

		nres = vatek_usbstream_stop(hustream);
		if (!is_vatek_success(nres))_disp_err("stop usb_stream fail : %d", nres);
		vatek_usbstream_close(hustream);
	}

	/* 
		setp 4 : 
		before quit demo stop and free both device and source
	*/

	if (hchip)vatek_device_close(hchip);
	if (hdevlist)vatek_device_list_free(hdevlist);
	if (streamsource.hsource)
		streamsource.free(streamsource.hsource);
	printf_app_end();
	cross_os_sleep(10);
	return (int32_t)1;
}

vatek_result source_sync_get_buffer(void* param, uint8_t** pslicebuf)
{
	Ptsstream_source ptssource = (Ptsstream_source)param;
	vatek_result nres = ptssource->check(ptssource->hsource);
	if (nres > vatek_success)
	{
		*pslicebuf = ptssource->get(ptssource->hsource);
		nres = (vatek_result)1;
	}
	return nres;
}

vatek_result parser_cmd_source(int32_t argc, char** argv, Ptsstream_source psource, Pusbstream_param pustream)
{
	vatek_result nres = vatek_unsupport;
	if(argc == 1)nres = stream_source_test_get(&usbcmd.modulator, psource);
	else if(argc > 3)
	{
		nres = vatek_success;
		if (strcmp(argv[1], "file") == 0)nres = stream_source_file_get(argv[2], psource);
		else if (strcmp(argv[1], "udp") == 0 || strcmp(argv[1], "rtp") == 0)
			nres = stream_source_udp_get(argv[2], psource);
		else nres = vatek_unsupport;

		if (is_vatek_success(nres))
		{
			pustream->remux = ustream_remux_pcr;
			if (argc == 4)
			{
				if (strcmp(argv[3], "passthrough") == 0)
					pustream->remux = ustream_remux_passthrough;
			}
		}
	}
	
	if(nres == vatek_unsupport)
	{
		_disp_l("support command below : ");
		_disp_l("	- app_stream [empty] : test stream mode");
		_disp_l("	- app_stream file [*.ts|*.trp] [pcr|passthrough]");
		_disp_l("	- app_stream udp [ip address] [pcr|passthrough]");
		_disp_l("	- app_stream rtp [ip address] [pcr|passthrough]");
	}

	return nres;
}
