
#include <vatek_epg.h>
#include <string.h>


#define XAPP_PES_MEM_POOL_SIZE  0x8000


#define XAPPRAM __attribute__((section(".ccmram")))
extern uint8_t* xpes_malloc(int32_t size);
extern void xpes_memory_reset(void);


static XAPPRAM uint8_t xpes_mem_pool[XAPP_PES_MEM_POOL_SIZE];
static uint32_t xpes_mem_pos = 0;
static Plicensed_pes_stream xpes_streams = NULL;


 
vatek_result vatek_pesstream_reset(void)
{
    xpes_memory_reset();
    xpes_streams = NULL;
    return vatek_result_success;
}

uint8_t* vatek_pesstream_malloc(int32_t size)
{
    return xpes_malloc(size);
}

vatek_result vatek_pesstream_add(Plicensed_pes_stream pextpes)
{
    pextpes->next = NULL;
    if(xpes_streams == NULL)xpes_streams = pextpes;
    else
    {
        Plicensed_pes_stream pnext = xpes_streams;
        while(pnext->next)pnext = pnext->next;
        pnext->next = pextpes;
    }
    return vatek_result_success;
}

Plicensed_pes_stream vatek_pesstream_get(void)
{
    return xpes_streams;
}

void xpes_memory_reset(void)
{
    xpes_mem_pos = 0;
}

uint8_t* xpes_malloc(int32_t size)
{
    size = ((size + 3) >> 2) << 2;
    if(xpes_mem_pos + size > XAPP_PES_MEM_POOL_SIZE)
        printf("malloc xapp memory overflow\r\n");
    {
        uint8_t* ptr = &xpes_mem_pool[xpes_mem_pos];
        memset(ptr,0,size);
        xpes_mem_pos += size;
        return ptr;
    }
}



