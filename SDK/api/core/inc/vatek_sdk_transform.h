#ifndef _VATEK_SDK_TRANSFORM_
#define _VATEK_SDK_TRANSFORM_

#include <service/service_transform.h>
#include <mux/mux_handle.h>

typedef void* hvatek_transform;

#ifdef __cplusplus
extern "C" {
#endif

	HAL_API vatek_result vatek_transform_open(hvatek_chip hchip,hvatek_transform* htr);

	HAL_API vatek_result vatek_transform_start_enum(hvatek_transform htr, Ptransform_enum penum);
	HAL_API vatek_result vatek_transform_start_capture(hvatek_transform htr, Ptransform_capture pcapture);
	HAL_API vatek_result vatek_transform_start_broadcast(hvatek_transform htr, Ptransform_broadcast pbc,uint32_t freqkhz);

	HAL_API vatek_result vatek_transform_polling(hvatek_transform htr, Ptransform_info* pinfo);
	HAL_API Ptransform_info vatek_transform_get_info(hvatek_transform htr);

	HAL_API vatek_result vatek_transform_get_packets(hvatek_transform htr, uint32_t* pktnums);
	HAL_API vatek_result vatek_transform_commit_packets(hvatek_transform htr);

	HAL_API vatek_result vatek_transform_stop(hvatek_transform htr);

	HAL_API vatek_result vatek_transform_enum_get_context(hvatek_transform htr, hmux_channel* hchannel);

	HAL_API void vatek_transform_close(hvatek_transform htr);

#ifdef __cplusplus
}
#endif

#endif
