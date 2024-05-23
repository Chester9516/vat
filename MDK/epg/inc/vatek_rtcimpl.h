



#ifndef _STM32_IMPL_
#define _STM32_IMPL_

#define STM32_F1XX 1
#define STM32_F2XX 2
#define STM32_F4XX 4

#include <vatek_define.h>


#if defined(STM32F405xx) || defined(STM32F407xx) || defined(STM32F401xC) || defined(STM32F417xx)
        #include "stm32f4xx_hal.h"
        #define STM32_TARGET STM32_F4XX
#else
    #if defined(STM32F103xE) || defined(STM32F103xB) || defined(STM32F101xB) || defined(STM32F107xC)
        #include "stm32f1xx_hal.h"
        #define STM32_TARGET STM32_F1XX
    #else
        #include "stm32f2xx_hal.h"
        #define STM32_TARGET STM32_F2XX
    #endif
/**************************	[USER DEFINE] **************************************/

/************************ [END OF USER DEFINE] *********************************/			
#endif


#define STM32_RTC_MODE_0            0       /* STM32 F1XX RTC        */
#define STM32_RTC_MODE_1            1       /* STM32 F2XX - F4XX RTC */

#if defined(STM32F1)
    #define STM32_EXTEND_BPK_RAM    0
    #define STM32_RTC_MODE          STM32_RTC_MODE_0
#elif defined(STM32F2)
    #define STM32_EXTEND_BPK_RAM    0
    #define STM32_RTC_MODE          STM32_RTC_MODE_1
#elif defined(STM32F4)
	#if defined(STM32F401xC)	//xC and xB
		#define STM32_EXTEND_BPK_RAM    1
		#define STM32_RTC_MODE          STM32_RTC_MODE_1
	#else
    	#define STM32_EXTEND_BPK_RAM    1
		#define STM32_RTC_MODE          STM32_RTC_MODE_1
	#endif
/**************************	[USER DEFINE] **************************************/

/************************ [END OF USER DEFINE] *********************************/			
#endif

typedef I2C_HandleTypeDef* I2C_HANDLE;
typedef UART_HandleTypeDef* UART_HANDLE;
typedef RTC_HandleTypeDef* RTC_HANDLE;

#endif


#ifndef _VATRTC_INTF_
#define _VATRTC_INTF_

typedef struct _VAT_RTC_DATETIME
{
    uint8_t nSeconds;
    uint8_t nMinutes;
    uint8_t nHours;
    uint8_t nWeekDay;
    uint8_t nDay;
    uint8_t nMonth;
    uint8_t nYear;
    uint8_t nRecv;
}VAT_RTC_DATETIME,*PVAT_RTC_DATETIME;

typedef enum _VAT_RTC_TIMEFMT
{
    RTC_TIMEFMT_BIN = 0,
    RTC_TIMEFMT_BCD = 1,
}VAT_RTC_TIMEFMT;

typedef struct _vatek_time_rtc
{
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t min;
    uint8_t secound;
}vatek_time_rtc,*Pvatek_time_rtc;


#define BCD_TO_BIN(a)   (((a >> 4) * 10) + (a & 0xF)) 
#define BIN_TO_BCD(a)   (((a/10) << 4) | (a % 10))


#ifdef __cplusplus
extern "C" {
#endif

#if ENABLE_RTC
VATAPI_RESULT VATRTCInit(RTC_HANDLE prtcparam);
#endif
	
PVAT_RTC_DATETIME VATRTCGetTime(VAT_RTC_TIMEFMT nfmt);
vatek_result VATRTCSetTime(PVAT_RTC_DATETIME prtcdate,VAT_RTC_TIMEFMT nfmt);
vatek_result VATRTCInit(RTC_HANDLE prtcparam);

#ifdef __cplusplus
}
#endif

#endif
