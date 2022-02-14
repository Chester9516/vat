#include <core/tools/tool_helpful.h>

#define MEMQUERY_EN_TAG		0xF9700122

Pth_memquery th_memquery_create_pool(Pth_mempool pmempool, uint8_t* pbuf, int32_t size)
{
	Pth_memquery newquery = (Pth_memquery)th_mempool_malloc(pmempool, sizeof(th_memquery));
	if (newquery)
	{
		newquery->prawbuf = pbuf;
		newquery->pendbuf = pbuf + size;
		newquery->refptr = 0;
		newquery->prptr = pbuf;
		newquery->pwptr = pbuf;
		newquery->size = size;

		newquery->pnextend = NULL;
		newquery->pnextptr = NULL;

		newquery->mempool = pmempool;
		return newquery;
	}
	return NULL;
}

Pth_memquery th_memquery_create(uint32_t size)
{
	int32_t len = size + sizeof(th_memquery);
	uint8_t* rawbuf = (uint8_t*)malloc(len);
	if (rawbuf)
	{
		Pth_memquery newquery = (Pth_memquery)&rawbuf[0];
		memset(rawbuf, 0, len);
		newquery->prawbuf = &rawbuf[sizeof(th_memquery)];
		newquery->pendbuf = newquery->prawbuf + size;
		newquery->refptr = 0;
		newquery->prptr = newquery->prawbuf;
		newquery->pwptr = newquery->prawbuf;
		newquery->size = size;
		newquery->pnextend = NULL;
		newquery->pnextptr = NULL;
		newquery->mempool = NULL;
		return newquery;
	}
	return NULL;
}

void th_memquery_destory(Pth_memquery pquery)
{
	if (!pquery->mempool)free(pquery);
}

uint8_t* th_memquery_get_raw_buffer(Pth_memquery pquery)
{
	return pquery->prawbuf;
}

uint8_t* th_memquery_malloc(Pth_memquery pquery, int32_t size)
{
	uint8_t* pnext = NULL;
	size += 11;
	size = (size >> 2) << 2;

	if (pquery->pnextptr)VWAR("th_memquery_malloc not empty");

	if (pquery->pwptr >= pquery->prptr)
	{
		int32_t recv = (int32_t)(pquery->pendbuf - pquery->pwptr);
		if (recv >= size)pnext = pquery->pwptr;
		else
		{
			recv = (int32_t)(pquery->prptr - pquery->prawbuf);
			if (recv >= size)
			{
				VWAR("th_memquery_malloc to beign");
				pnext = pquery->prawbuf;
			}
		}
	}
	else
	{
		int32_t recv = (int32_t)(pquery->prptr - pquery->pwptr);
		if (recv >= size)pnext = pquery->pwptr;
	}

	pquery->pnextptr = pnext;
	if (!pnext)VWAR("memquery malloc underflow : %d", size);
	else
	{
		uint32_t* pwptr = (uint32_t*)pquery->pnextptr;
		*pwptr = MEMQUERY_EN_TAG;
		pwptr++;
		*pwptr = size;

		pquery->pnextend = pnext + size;
		if (pquery->pnextend == pquery->pendbuf)
			pquery->pnextend = pquery->prawbuf;
		pnext += 8;
	}
	return pnext;
}

void th_memquery_push(Pth_memquery pquery, uint8_t* ptrbuf)
{
	if (!pquery->pnextptr)VWAR("th_memquery_push next null");
	if (ptrbuf != (pquery->pnextptr + 8))VWAR("th_memquery_push not match");
	else
	{
		pquery->refptr++;
		pquery->pwptr = pquery->pnextend;
		pquery->pnextptr = NULL;
	}
}

vatek_result th_memquery_free(Pth_memquery pquery, uint8_t* ptrbuf)
{
	vatek_result nres = vatek_badparam;
	if(pquery->refptr)
	{
		uint8_t* prptr = (uint8_t*)(ptrbuf - 8);
		if (prptr != pquery->prptr)
		{
			if (prptr == pquery->prawbuf)
			{
				VWAR("th_memquery_free to beign");
				pquery->prptr = pquery->prawbuf;
			}
			else VWAR("th_memquery_free rptr empty");
		}

		if (pquery->prptr)
		{
			uint32_t* pwptr = (uint32_t*)pquery->prptr;
			if (*pwptr == MEMQUERY_EN_TAG)
			{
				*pwptr = 0;
				pwptr++;
				pquery->prptr += *pwptr;
				pquery->refptr--;
				if (pquery->prptr == pquery->pendbuf)
					pquery->prptr = pquery->prawbuf;
				nres = vatek_success;
			}
			else VWAR("th_memquery_free not match : %08x", *pwptr);
		}
	}
	return nres;
}
