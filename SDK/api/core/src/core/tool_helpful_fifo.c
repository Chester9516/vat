#include <core/tools/tool_helpful.h>

Pth_fifo th_fifo_create_pool(int32_t len,Pth_mempool pmpool)
{
	int32_t size = sizeof(th_fifo) + sizeof(void*) * len;
	uint8_t* pbuf = th_mempool_malloc(pmpool,size);
	if (pbuf)
	{
		Pth_fifo pfifo = (Pth_fifo)pbuf;
		pfifo->pmempool = pmpool;
		pfifo->len = len;
		pfifo->fifo_list = (void**)&pbuf[sizeof(th_fifo)];
		return pfifo;
	}
	return NULL;
}

Pth_fifo th_fifo_create(int32_t len)
{
	int32_t size = sizeof(th_fifo) + sizeof(void*) * len;
	uint8_t* pbuf = malloc(size);
	if (pbuf)
	{
		Pth_fifo pfifo = (Pth_fifo)pbuf;
		memset(pbuf, 0, size);
		pfifo->len = len;
		pfifo->fifo_list = (void**)&pbuf[sizeof(th_fifo)];
		return pfifo;
	}
	return NULL;
}

void th_fifo_free(Pth_fifo pfifo)
{
	if(!pfifo->pmempool)
		free(pfifo);
}

void th_fifo_reset(Pth_fifo pfifo)
{
	pfifo->pos = 0;
}

void th_fifo_push(Pth_fifo pfifo, void* ptrobj)
{
	if (pfifo->pos < (pfifo->len - 1))
	{
		pfifo->fifo_list[pfifo->pos++] = ptrobj;
	}
	else VERR("th_fifo_push overflow");
}

void* th_fifo_pop(Pth_fifo pfifo)
{
	void* ptrobj = th_fifo_peek(pfifo);
	if (ptrobj)pfifo->pos--;
	return ptrobj;
}

void* th_fifo_peek(Pth_fifo pfifo)
{
	if (pfifo->pos)
		return pfifo->fifo_list[pfifo->pos - 1];
	return NULL;
}
