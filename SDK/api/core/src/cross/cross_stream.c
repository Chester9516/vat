#include <cross/cross_stream.h>
#include "./internal/stream_handle.h"

vatek_result cross_stream_open_test(uint32_t bitrate, hcross_stream* hcstream)
{
	Pcstream_handler phandler = NULL;
	vatek_result nres = cross_stream_test_get(bitrate, &phandler);
	if (is_vatek_success(nres))*hcstream = phandler;
	return nres;
}

vatek_result cross_stream_open_file(const char* szfilename, hcross_stream* hcstream)
{
	Pcstream_handler phandler = NULL;
	vatek_result nres = cross_stream_file_get(szfilename, &phandler);
	if (is_vatek_success(nres))*hcstream = phandler;
	return nres;
}

vatek_result cross_stream_open_udp(const char* szurl, hcross_stream* hcstream)
{
	Pcstream_handler phandler = NULL;
	vatek_result nres = cross_stream_udp_get(szurl, &phandler);
	if (is_vatek_success(nres))*hcstream = phandler;
	return nres;
}

vatek_result cross_stream_start(hcross_stream hcstream)
{
	Pcstream_handler phandler = (Pcstream_handler)hcstream;
	vatek_result nres = phandler->start(phandler->hstream);
	return nres;
}

vatek_result cross_stream_get_slice(hcross_stream hcstream, uint8_t** pslice)
{
	Pcstream_handler phandler = (Pcstream_handler)hcstream;
	return phandler->get_slice(phandler->hstream, pslice);
}

uint32_t cross_stream_get_bitrate(hcross_stream hcstream)
{
	Pcstream_handler phandler = (Pcstream_handler)hcstream;
	return phandler->get_bitrate(phandler->hstream);
}

void cross_stream_stop(hcross_stream hcstream)
{
	Pcstream_handler phandler = (Pcstream_handler)hcstream;
	phandler->stop(phandler->hstream);
}

void cross_stream_close(hcross_stream hcstream)
{
	Pcstream_handler phandler = (Pcstream_handler)hcstream;
	phandler->close(phandler->hstream);
}
