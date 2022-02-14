#ifndef _TOOL_CRC32_
#define _TOOL_CRC32_

#include <core/vatek_base.h>

#ifdef __cplusplus
extern "C" {
#endif

	HAL_API uint32_t tool_crc32(uint8_t *data, int32_t len);
	HAL_API void tool_crc32_step_reset(uint32_t* crc32);
	HAL_API uint32_t tool_crc32_step_put(uint32_t crc32,uint8_t* data);
	HAL_API vatek_result tool_crc32_step_put_buffer(uint32_t* crc32, uint8_t* data, int32_t len);

#ifdef __cplusplus
}
#endif

#endif
