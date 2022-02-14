#ifndef _VATEK_SDK_DEVICE_
#define _VATEK_SDK_DEVICE_

#include <core/vatek_base.h>
#include <core/base/chip_define.h>
#include <core/base/output_modulator.h>
#include <core/base/calibration_define.h>
#include <core/base/device_usb.h>

#ifdef __cplusplus
extern "C" {
#endif

	HAL_API vatek_result vatek_device_list_enum(uint32_t bus,hal_service_mode service,hvatek_devices* hdevices);
	HAL_API vatek_result vatek_device_list_enum_by_usbid(uint16_t vid, uint16_t pid, hvatek_devices* hdevices);
	HAL_API uint32_t vatek_device_list_get_bus(hvatek_devices hdevices, int32_t idx);
	HAL_API const char* vatek_device_list_get_name(hvatek_devices hdevices, int32_t idx);
	HAL_API hal_service_mode vatek_device_list_get_service(hvatek_devices hdevices, int32_t idx);
	HAL_API void vatek_device_list_free(hvatek_devices hdevices);
	HAL_API vatek_result vatek_device_open(hvatek_devices hdevices, int32_t idx, hvatek_chip* hchip);

	HAL_API vatek_result vatek_device_start_sine(hvatek_chip hchip,uint32_t freqkhz);
	HAL_API vatek_result vatek_device_start_test(hvatek_chip hchip, Pmodulator_param pmod,uint32_t freqkhz);
	HAL_API vatek_result vatek_device_polling(hvatek_chip hchip);
	HAL_API void vatek_device_stop(hvatek_chip hchip);

	HAL_API uint32_t vatek_device_get_bus(hvatek_chip hchip);
	HAL_API Pchip_info vatek_device_get_info(hvatek_chip hchip);
	HAL_API const char* vatek_device_get_name(hvatek_chip hchip);
	HAL_API vatek_result vatek_device_close(hvatek_chip hchip);

	HAL_API vatek_result vatek_device_close_reboot(hvatek_chip hchip);

	HAL_API vatek_result vatek_device_calibration_load(hvatek_chip hchip, Pcalibration_param pcalibration);
	HAL_API vatek_result vatek_device_calibration_apply(hvatek_chip hchip, Pcalibration_param pcalibration);
	HAL_API vatek_result vatek_device_calibration_save(hvatek_chip hchip, Pcalibration_param pcalibration);

	/* used with transform service for usb stream */
	HAL_API vatek_result vatek_device_stream_start(hvatek_chip hchip,Pmodulator_param pmod);
	HAL_API vatek_result vatek_device_stream_write(hvatek_chip hchip,uint8_t* pbuf,int32_t size);
	HAL_API vatek_result vatek_device_stream_stop(hvatek_chip hchip);

	/* usb device bulk operations */
	HAL_API vatek_result vatek_device_usbbulk_send(hvatek_chip hchip, Pusbbulk_command pcmd, Pusbbulk_result presult,uint8_t* pbuf,int32_t len);
	HAL_API vatek_result vatek_device_usbbulk_get_result(hvatek_chip hchip, Pusbbulk_result presult);
	HAL_API vatek_result vatek_device_usbbulk_write(hvatek_chip hchip, uint8_t* pbuf, int32_t len);
	HAL_API vatek_result vatek_device_usbbulk_read(hvatek_chip hchip, uint8_t* pbuf, int32_t len);
	HAL_API vatek_result vatek_device_usbbulk_get_size(hvatek_chip hchip);

#ifdef __cplusplus
}
#endif

#endif
