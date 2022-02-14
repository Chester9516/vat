#ifndef _UI_PROPS_OUTPUT_
#define _UI_PROPS_OUTPUT_

#include <core/base/output_base.h>
#include <core/base/output_rfmixer.h>
#include <core/base/calibration_define.h>
#include <core/ui/ui_props/ui_props_modulator.h>

HALREG_S(R2_PARAM_I, 16)
HALREG_S(R2_PARAM_Q, 16)
HALREG_S(R2_PARAM_IMAGE, 16)
HALREG_S(R2_PARAM_PHASE, 16)
HALREG_S(R2_PARAM_GPIO, 16)
HALREG_S(R2_PARAM_GAIN, 16)

HALREG_S(CALIBRATION_R2_0_I, 16)
HALREG_S(CALIBRATION_R2_0_Q, 16)
HALREG_S(CALIBRATION_R2_0_IMAGE, 16)
HALREG_S(CALIBRATION_R2_0_PHASE, 16)

_ui_struct_start(r2_tune)
_prop_u16(r2_tune, ioffset, "i offect", R2_PARAM_I)
	_prop_u16(r2_tune, qoffset, "q offect", R2_PARAM_Q)
	_prop_u16(r2_tune, imgoffset, "image offect", R2_PARAM_IMAGE)
	_prop_u16(r2_tune, phaseoffset, "phase offect", R2_PARAM_PHASE)
_ui_struct_end

_ui_struct_start(r2_tune_rule)
_prop_u16(r2_tune_rule, pagain, "gain", R2_PARAM_GAIN)
	_prop_u16(r2_tune_rule, gpiocntl, "gpio", R2_PARAM_GPIO)
_ui_struct_end

_ui_struct_start(r2_tune_calibration0)
	_prop_u16(r2_tune, ioffset, "i offect", CALIBRATION_R2_0_I)
	_prop_u16(r2_tune, qoffset, "q offect", CALIBRATION_R2_0_Q)
	_prop_u16(r2_tune, imgoffset, "image offect", CALIBRATION_R2_0_IMAGE)
	_prop_u16(r2_tune, phaseoffset, "phase offect", CALIBRATION_R2_0_PHASE)
_ui_struct_end

#endif
