#ifndef _STORAGE_SECTION_
#define _STORAGE_SECTION_

#include <core/base/binary_rom.h>
#include <core/storage/storage_broadcast.h>
#include <core/storage/storage_transform.h>
#include <core/storage/storage_r2tune.h>
#include <core/storage/storage_config.h>

#define STORAGE_APP_ADDRESS     LOADER_SIZE
#define STORAGE_PROTECT_1		((LOADER_SIZE - BINARY_SECTION_SIZE)/BINARY_SECTION_SIZE)
#define STORAGE_PROTECT_0		((LOADER_SIZE - (BINARY_SECTION_SIZE << 1))/BINARY_SECTION_SIZE)

#ifdef __cplusplus
extern "C" {
#endif

    HAL_API vatek_result storage_section_check_tag(uint8_t* psection,uint32_t* tag,uint32_t* sectionnums);

    HAL_API vatek_result storage_section_check_loader(uint8_t* psection);
    HAL_API vatek_result storage_section_check_app(uint8_t* psection);

    HAL_API vatek_result storage_section_get_loader(uint8_t* psection,Ploader_header plhdr);
    HAL_API vatek_result storage_section_get_app(uint8_t* psection,Papp_header papphdr);

    HAL_API vatek_result storage_section_set_loader(uint8_t* psection, Ploader_header plhdr);
    HAL_API vatek_result storage_section_set_app(uint8_t* psection, Papp_header papphdr);

    /* return size of raw data used */
    HAL_API vatek_result storage_section_put_resource(uint8_t* psection,int32_t size, Pbin_resource_section pbin,uint8_t* praw);
    /* return position of raw data */
    HAL_API vatek_result storage_section_get_resource(uint8_t* psection, Pbin_resource_section pbin);

#ifdef __cplusplus
}
#endif

#endif
