#ifndef _CALIBRATION_DEFINE_
#define _CALIBRATION_DEFINE_

#include <core/hal/halreg_calibration.h>
#include <core/base/output_rfmixer.h>

typedef struct _calibration_dac
{
	int8_t ioffect;
	int8_t qoffect;
	int8_t igain;
	int8_t qgain;
}calibration_dac;

typedef calibration_dac* Pcalibration_dac;

typedef struct _calibration_param
{
	int32_t clock;
	calibration_dac dac;
	r2_tune r2;
}calibration_param;

typedef calibration_param* Pcalibration_param;

#ifdef __cplusplus
extern "C" {
#endif

	HAL_API vatek_result calibration_set(hvatek_chip hchip, Pcalibration_param pcalibration,int32_t isapply);
	HAL_API vatek_result calibration_check(hvatek_chip hchip);
	HAL_API vatek_result calibration_get(hvatek_chip hchip, Pcalibration_param pcalibration);

#ifdef __cplusplus
}
#endif

#endif
