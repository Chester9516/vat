#include <core/tools/tool_helpful.h>

Pth_mempool th_mempool_create(int32_t size)
{
	uint8_t* prawbuf = NULL;
	size += sizeof(th_mempool);
	size = _V_ALIGN_WORD(size);

	prawbuf = (uint8_t*)malloc(size);
	if (prawbuf)
	{
		Pth_mempool pmpool = (Pth_mempool)&prawbuf[0];
		memset(prawbuf, 0, size);
		pmpool->prawbuf = &prawbuf[sizeof(th_mempool)];
		pmpool->pendbuf = prawbuf + size;
		pmpool->pcurptr = pmpool->prawbuf;
		return pmpool;
	}else VERR("malloc mempool null : [%s]",size);
	return NULL;
}

void th_mempool_config_raw(Pth_mempool pmempool,uint8_t* praw,int32_t len)
{
	pmempool->psurbuf = praw;
	pmempool->prawbuf = praw;
	pmempool->pendbuf = pmempool->prawbuf + len;
	pmempool->pcurptr = pmempool->prawbuf;
}

void th_mempool_free(Pth_mempool pmempool)
{
	if(!pmempool->psurbuf)
		free(pmempool);
}

void th_mempool_reset(Pth_mempool pmempool)
{
	pmempool->pcurptr = pmempool->prawbuf;
}

uint8_t* th_mempool_malloc(Pth_mempool pmempool, int32_t size)
{
	uint8_t* pnext = pmempool->pcurptr;
	size = _V_ALIGN_WORD(size);
	pnext += size;
	if (pnext > pmempool->pendbuf)VERR("qmempool buffer overflow");
	else
	{
		uint8_t* ptr = pmempool->pcurptr;
		memset(ptr, 0, size);
		pmempool->pcurptr = pnext;
		return ptr;
	}
	return NULL;
}

int32_t th_mempool_get_pointer(Pth_mempool pmempool, uint8_t** ptrbuf)
{
	int32_t size = (int32_t)(pmempool->pendbuf - pmempool->pcurptr);
	*ptrbuf = pmempool->pcurptr;

	return size;
}
