#ifndef _STORAGE_R2TURN_
#define _STORAGE_R2TURN_

#include <core/base/output_rfmixer.h>

#ifdef __cplusplus
extern "C" {
#endif

	HAL_API void storage_r2tune_reset(Pr2_tune_handle pr2);
	HAL_API vatek_result storage_r2tune_set(Pr2_tune_handle pr2,uint8_t* psection);
	HAL_API vatek_result storage_r2tune_get(Pr2_tune_handle pr2,uint8_t* psection);

#ifdef __cplusplus
}
#endif

#endif
