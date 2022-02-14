#ifndef _TOOL_BUFSTREAM_
#define _TOOL_BUFSTREAM_

#include <core/vatek_base.h>

typedef struct _bufstream
{
	int32_t pos;
	uint8_t* buf;
}bufstream, *Pbufstream;

#ifdef __cplusplus
extern "C" {
#endif

	HAL_API void putbuf_buf(Pbufstream pbuf, uint8_t* pbufdata, int32_t len);
	HAL_API void getbuf_buf(Pbufstream pbuf, uint8_t* pbufdata, int32_t len);
	HAL_API uint32_t getbuf_uint32_t(Pbufstream pbuf);
	HAL_API void putbuf_uint32_t(Pbufstream pbuf, uint32_t val);
	HAL_API uint8_t* setbuf_offset(Pbufstream pbuf, int32_t pos);

	static inline void putbuf_byte(Pbufstream pbuf, uint8_t val) { pbuf->buf[pbuf->pos++] = val; }
	static inline uint8_t getbuf_byte(Pbufstream pbuf) { return pbuf->buf[pbuf->pos++]; }
	static inline uint8_t* getbuf_ptr(Pbufstream pbuf) { return &pbuf->buf[pbuf->pos]; }
	static inline void setbuf_ptr(Pbufstream pbuf, uint8_t* ptr) { pbuf->pos = (int32_t)(ptr - &pbuf->buf[0]); }

#ifdef __cplusplus
}
#endif

#endif
