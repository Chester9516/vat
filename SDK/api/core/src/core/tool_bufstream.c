#include <core/tools/tool_bufstream.h>

void putbuf_buf(Pbufstream pbuf, uint8_t* pbufdata, int32_t len)
{
	memcpy(&pbuf->buf[pbuf->pos], pbufdata, len);
	pbuf->pos += len;
}

void getbuf_buf(Pbufstream pbuf, uint8_t* pbufdata, int32_t len)
{
	memcpy(pbufdata, &pbuf->buf[pbuf->pos], len);
	pbuf->pos += len;
}

uint8_t* setbuf_offset(Pbufstream pbuf, int32_t pos)
{
	pbuf->pos = pos;
	return getbuf_ptr(pbuf);
}

void putbuf_uint32_t(Pbufstream pbuf, uint32_t val)
{
	putbuf_byte(pbuf, (uint8_t)(val >> 24));
	putbuf_byte(pbuf, (uint8_t)(val >> 16));
	putbuf_byte(pbuf, (uint8_t)(val >> 8));
	putbuf_byte(pbuf, (uint8_t)val);
}

uint32_t getbuf_uint32_t(Pbufstream pbuf)
{
	uint32_t val = (getbuf_byte(pbuf) << 24) | (getbuf_byte(pbuf) << 16) | (getbuf_byte(pbuf) << 8) | (getbuf_byte(pbuf));
	return val;
}
