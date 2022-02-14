#ifndef _VATEK_BASE_
#define _VATEK_BASE_

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#ifdef _VA2_STATIC_
    #define HAL_API
#else
    #ifdef _MSC_VER
        #ifdef _VA2_DLL_
            #define HAL_API __declspec(dllexport)
        #else
            #define HAL_API __declspec(dllimport)
        #endif
        #pragma warning( disable : 6011 6001 4819 6385 26451 26812 26495)
    #else 
        #ifdef __GNUC__
            #define HAL_API __attribute__ ((visibility ("default")))
        #else
            #define HAL_API
        #endif
    #endif
#endif

#define CHIP_RAISE_COMMAND_TIMEOUT      10000
#define CHIP_TS_PACKET_LEN              188
#define CHIP_STREAM_SLICE_LEN			24064
#define CHIP_STREAM_SLICE_PACKET_NUMS   (CHIP_STREAM_SLICE_LEN / CHIP_TS_PACKET_LEN)


typedef void* hvatek_devices;
typedef void* hvatek_chip;

/**
 * @ingroup core_props
 * vatek sdk result define
 */
typedef enum _vatek_result{
	vatek_keilc_struct = 0x7FFFFFFF,
    vatek_bufferempty = -14,
	vatek_sizeerr = -13,				/*!< size error (value not support)*/
	vatek_crcfail = -12,				/*!< check crc fail	*/
    vatek_memfail = -11,				/*!< memory alloc fail or overflow */
    vatek_format = -10,					/*!< format not current */
    vatek_nodevice = -9,				/*!< device not exists */
    vatek_busy = -8,					/*!< system is busy */
    vatek_timeout = -7,					/*!< wait operation timeout */
    vatek_hwfail = -6,					/*!< send command to device fail or call system api fail */ 
    vatek_badstatus = -5,				/*!< can not call at this device status */
    vatek_bufoverflow = -4,				/*!< memory size limited overflow */
    vatek_badparam = -3,				/*!< parameter set not supported */
    vatek_unsupport = -2,				/*!< function not supported */
    vatek_unknown = -1,					/*!< unknown fail */
    vatek_success = 0,					/*!< bigger than 0 success */
    vatek_quitcli = 100,
}vatek_result;

#define is_vatek_success(a)		(a >= vatek_success)

typedef enum _vatek_debug_level
{
    debug_level_all = 0,
    debug_level_warning = 1,
    debug_level_error = 2,
}vatek_debug_level;

/* compiler time assert check */
#define _VASSERT(COND,MSG) typedef char static_assertion_##MSG[(!!(COND))*2-1];

#ifdef __cplusplus
extern "C" {
#endif

	HAL_API uint32_t vatek_get_tick_ms(void);
	HAL_API void vatek_sleep_ms(uint32_t ms);

    HAL_API void vatek_print(const char* fmt,...);

    HAL_API void vatek_debug_print(vatek_debug_level level,const char* fmt,...);
    HAL_API void vatek_debug_set(vatek_debug_level level);

    HAL_API vatek_result vatek_chip_read_register(hvatek_chip hchip, int32_t addr, uint32_t* val);
    HAL_API vatek_result vatek_chip_write_register(hvatek_chip hchip, int32_t addr, uint32_t val);
    HAL_API vatek_result vatek_chip_read_memory(hvatek_chip hchip, int32_t addr, uint32_t* val);
    HAL_API vatek_result vatek_chip_write_memory(hvatek_chip hchip, int32_t addr, uint32_t val);
    HAL_API vatek_result vatek_chip_write_buffer(hvatek_chip hchip, int32_t addr,uint8_t* buf,int32_t wlen);
    HAL_API vatek_result vatek_chip_read_buffer(hvatek_chip hchip,int32_t addr,uint8_t* buf,int32_t wlen);

    #define readhal(a,v)        vatek_chip_read_memory(hchip,a,v)
    #define writehal(a,v)       vatek_chip_write_memory(hchip,a,v)
    #define writebuf(a,b,v)     vatek_chip_write_buffer(hchip,a,b,((v) >> 2))

    static inline uint32_t vatek_buffer_2_uint32(uint8_t* pbuf)
    {
        return (pbuf[0] << 24) | (pbuf[1] << 16) | (pbuf[2] << 8) | pbuf[3];
    }

    static inline void vatek_uint32_2_buffer(uint8_t* pbuf, uint32_t val)
    {
		*pbuf++ = (uint8_t)(val >> 24);
		*pbuf++ = (uint8_t)(val >> 16);
		*pbuf++ = (uint8_t)(val >> 8);
		*pbuf++ = (uint8_t)(val & 0xFF);
    }

    #define VERR(fmt,...)   vatek_debug_print(debug_level_error,"[%08x:%-8s] - %-24s - " fmt "\r\n",vatek_get_tick_ms(),"error",__func__,##__VA_ARGS__)
    #define VWAR(fmt,...)   vatek_debug_print(debug_level_warning,"[%08x:%-8s] - %-24s - " fmt "\r\n",vatek_get_tick_ms(),"warning",__func__,##__VA_ARGS__)
    #define VDBG(fmt,...)   vatek_debug_print(debug_level_all,"[%08x:%-8s] - %-24s - " fmt "\r\n",vatek_get_tick_ms(),"info",__func__,##__VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif
