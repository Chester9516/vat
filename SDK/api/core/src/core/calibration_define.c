#include <core/base/calibration_define.h>
#include <core/ui/ui_props/ui_props_output.h>

#define _calibration_dac_val(p)			((((uint8_t)p->dac.ioffect) << 24) | (((uint8_t)p->dac.qoffect) << 16) | (((uint8_t)p->dac.igain) << 8) | ((uint8_t)p->dac.qgain))
#define _calibration_dac_ioffect(v)		((int8_t)((uint8_t)(v >> 24)))
#define _calibration_dac_qoffect(v)		((int8_t)((uint8_t)(v >> 16)))
#define _calibration_dac_igain(v)		((int8_t)((uint8_t)(v >> 8)))
#define _calibration_dac_qgain(v)		((int8_t)((uint8_t)(v)))

vatek_result calibration_set(hvatek_chip hchip, Pcalibration_param pcalibration, int32_t isapply)
{
	uint32_t val = _calibration_dac_val(pcalibration);
	vatek_result nres = vatek_chip_write_memory(hchip, HALREG_CALIBRATION_DAC, val);
	if (is_vatek_success(nres))
		nres = vatek_chip_write_memory(hchip, HALREG_CALIBRATION_CLOCK, pcalibration->clock);
	if (is_vatek_success(nres))
		nres = ui_props_write_hal(hchip, _ui_struct(r2_tune_calibration0), (uint8_t*)&pcalibration->r2);

	if (is_vatek_success(nres))
	{
		val = CALIBRATION_EN_TAG | (isapply != 0);
		nres = vatek_chip_write_memory(hchip, HALREG_CALIBRATION_CNTL, val);
	}
	return nres;
}

vatek_result calibration_check(hvatek_chip hchip)
{
	uint32_t val = 0;
	vatek_result nres = vatek_chip_read_memory(hchip, HALREG_CALIBRATION_CNTL, &val);
	if (is_vatek_success(nres))
	{
		if ((val & 0xFFFFFF00) != CALIBRATION_EN_TAG)
			nres = vatek_unsupport;
	}
	return nres;
}

vatek_result calibration_get(hvatek_chip hchip, Pcalibration_param pcalibration)
{
	vatek_result nres = calibration_check(hchip);
	if (is_vatek_success(nres))
	{
		uint32_t val = 0;
		nres = vatek_chip_read_memory(hchip, HALREG_CALIBRATION_CLOCK, &val);
		if (is_vatek_success(nres))
		{
			pcalibration->clock = (int32_t)val;
			nres = vatek_chip_read_memory(hchip, HALREG_CALIBRATION_DAC, &val);
			if (is_vatek_success(nres))
			{
				pcalibration->dac.ioffect = _calibration_dac_ioffect(val);
				pcalibration->dac.qoffect = _calibration_dac_qoffect(val);
				pcalibration->dac.igain = _calibration_dac_igain(val);
				pcalibration->dac.qgain = _calibration_dac_qgain(val);
				nres = ui_props_read_hal(hchip, _ui_struct(r2_tune_calibration0), (uint8_t*)&pcalibration->r2);
			}
		}
	}
	return nres;
}
