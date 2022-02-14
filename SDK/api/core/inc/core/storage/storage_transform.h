#ifndef _STORAGE_TRANSFORM_
#define _STORAGE_TRANSFORM_

#include <core/base/output_base.h>

#define BIN_SECTION_TRANSFORM		(BIN_SECTION_TAG | 0x20)
#define TRANSFORM_RAWDATA_LEN	    1024*3

typedef struct _storage_transform
{
    modulator_param modulator;
}storage_transform;

typedef storage_transform* Pstorage_transform;

#ifdef __cplusplus
extern "C" {
#endif

	HAL_API void storage_transform_reset(Pstorage_transform ptr);
	HAL_API vatek_result storage_transform_set(Pstorage_transform ptr,uint8_t* psection);
	HAL_API vatek_result storage_transform_get(Pstorage_transform ptr,uint8_t* psection);

#ifdef __cplusplus
}
#endif

#endif
