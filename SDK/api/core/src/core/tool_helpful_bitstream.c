#include <core/tools/tool_helpful.h>

extern void bs_next(Pth_bitstream pbs);
extern void bs_write(Pth_bitstream pbs);

Pth_bitstream th_bs_create_pool(Pth_mempool pmempool, uint8_t* pbuf, int32_t len)
{
	Pth_bitstream pbs = (Pth_bitstream)th_mempool_malloc(pmempool, sizeof(th_bitstream));
	th_bs_config(pbs, pbuf, len);
	return pbs;
}

void th_bs_config(Pth_bitstream pbs, uint8_t* pbuf, int32_t len)
{
	memset(pbs, 0, sizeof(th_bitstream));
	pbs->pbuf = pbuf;
	pbs->size = len;
	pbs->pos = 0;
	pbs->bitrecv = 0;
	pbs->value = 0;
}

const uint8_t bit_mask[9] = { 0x00,0x01,0x03,0x07,0x0F,0x1F,0x3F,0x7F,0xFF, };

uint32_t th_bs_read(Pth_bitstream pbs, int32_t bits)
{
	uint32_t rval = 0;
	int32_t reach = 0;
	if (bits > 32)VWAR("read bad bits - %d", bits);

	while (bits)
	{
		bs_next(pbs);
		reach = pbs->bitrecv;
		if (bits < reach)reach = bits;
		rval <<= reach;
		rval |= (pbs->value >> (8 - reach));
		pbs->value <<= reach;
		pbs->bitrecv -= reach;
		bits -= reach;
	}
	return rval;
}

void th_bs_write(Pth_bitstream pbs, uint32_t val, int32_t bits)
{
	int32_t reach = 0;
	if (bits > 32)VWAR("write bad bits - %d", bits);

	while (bits)
	{
		bs_next(pbs);
		reach = bits;
		if (reach > pbs->bitrecv)reach = pbs->bitrecv;
		pbs->value <<= reach;
		pbs->value |= val >> (32 - reach);
		val <<= reach;
		pbs->bitrecv -= reach;
		bits -= reach;
		bs_write(pbs);
	}
}

inline void bs_next(Pth_bitstream pbs)
{
	if (!pbs->bitrecv)
	{
		pbs->value = pbs->pbuf[pbs->pos++];
		pbs->bitrecv = 8;
		if (pbs->pos > pbs->size)VWAR("bs read buffer overflow");
	}
}

inline void bs_write(Pth_bitstream pbs)
{
	if (!pbs->bitrecv)
	{
		if(pbs->pos)pbs->pbuf[pbs->pos - 1] = pbs->value;
	}
}