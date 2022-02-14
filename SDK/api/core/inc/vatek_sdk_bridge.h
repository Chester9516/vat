#ifndef _VATEK_SDK_BRIDGE_
#define _VATEK_SDK_BRIDGE_

#include <vatek_sdk_device.h>
#include <bridge/bridge_device.h>

typedef void* hvatek_bridge;

#ifdef __cplusplus
extern "C" {
#endif

	HAL_API vatek_result vatek_bridge_open(hvatek_chip hchip, hvatek_bridge* hbridge);

	HAL_API Pbdevice_info vatek_bridge_get_info(hvatek_bridge hbridge);
	HAL_API vatek_result vatek_bridge_get_av_source(hvatek_bridge hbridge, int32_t idx,Pbridge_source psource);
	HAL_API const char* vatek_bridge_get_av_source_name(hvatek_bridge hbridge,Pbridge_source psource);

	HAL_API vatek_result vatek_bridge_start_av_source(hvatek_bridge hbridge, Pbridge_source psource);
	HAL_API vatek_result vatek_bridge_get_av_source_status(hvatek_bridge hbridge, Pbridge_source psource);
	HAL_API vatek_result vatek_bridge_stop_av_source(hvatek_bridge hbridge);

	HAL_API vatek_result vatek_bridge_write_section(hvatek_bridge hbridge, int32_t section, uint8_t* pbuffer);
	HAL_API vatek_result vatek_bridge_read_section(hvatek_bridge hbridge, int32_t section, uint8_t* pbuffer);

	HAL_API void vatek_bridge_close(hvatek_bridge hbridge);

#ifdef __cplusplus
}
#endif

#endif
