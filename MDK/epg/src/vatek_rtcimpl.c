
#include "vatek_rtcimpl.h"


#define STM32_RTC_SYNC_PREDIV   255
#define STM32_RTC_ASYNC_PREDIV  127

typedef RTC_HandleTypeDef* RTC_HANDLE;

#if STM32_RTC_MODE == STM32_RTC_MODE_0
    #define RTC_BACKUP_STATUS   RTC_BKP_DR42 /* LAST RTC BKP REG */
#else
    #define RTC_BACKUP_STATUS   RTC_BKP_DR19 /* LAST RTC BKP REG */
#endif

#define RTC_STATUS_INITIAL  0xF001
#define RTC_STATUS_READY    0xF002
#define RTC_STATUS_ZERO     0x0000

/* Internal RTC defines */
#define RTC_LEAP_YEAR(year)             ((((year) % 4 == 0) && ((year) % 100 != 0)) || ((year) % 400 == 0))
#define RTC_BCD2BIN(x)                  ((((x) >> 4) & 0x0F) * 10 + ((x) & 0x0F))

static uint8_t RTC_Months[2][12] = {
	{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},	/* Not leap year */
	{31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}	/* Leap year */
};

static VAT_RTC_DATETIME stm32CurrentTime;
static RTC_HandleTypeDef* stm32rtcentry = NULL;
static RTC_DateTypeDef RTC_DateStruct;
static RTC_TimeTypeDef RTC_TimeStruct;

vatek_result VATRTCInit(RTC_HANDLE prtcparam)
{
    
    uint32_t status = 0;
    stm32rtcentry = prtcparam;
    stm32rtcentry->Instance = RTC;
    
#if STM32_RTC_MODE == STM32_RTC_MODE_1
    
	stm32rtcentry->Init.AsynchPrediv = STM32_RTC_ASYNC_PREDIV;
	stm32rtcentry->Init.SynchPrediv = STM32_RTC_SYNC_PREDIV;
	stm32rtcentry->Init.HourFormat = RTC_HOURFORMAT_24;
	stm32rtcentry->Init.OutPut = RTC_OUTPUT_DISABLE;
	stm32rtcentry->Init.OutPutType = RTC_OUTPUT_TYPE_PUSHPULL;
	stm32rtcentry->Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
    
#elif STM32_RTC_MODE == STM32_RTC_MODE_0
    stm32rtcentry->Init.AsynchPrediv = RTC_AUTO_1_SECOND;
    stm32rtcentry->Init.OutPut = RTC_OUTPUTSOURCE_NONE;
#else 
    #error "UNSUPPORT RTC MODE"
#endif
    
    __HAL_RCC_RTC_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();
#if STM32_RTC_MODE == STM32_RTC_MODE_0		
    __HAL_RCC_BKP_CLK_ENABLE();
#endif    
    HAL_PWR_EnableBkUpAccess();
    
    status = HAL_RTCEx_BKUPRead(stm32rtcentry, RTC_BACKUP_STATUS);
    if(status == RTC_STATUS_READY)
    {
        HAL_RTC_WaitForSynchro(stm32rtcentry);
        __HAL_RCC_CLEAR_RESET_FLAGS();
        
#if STM32_RTC_MODE == STM32_RTC_MODE_0
        
        HAL_RTC_GetDate(stm32rtcentry,&RTC_DateStruct,RTC_FORMAT_BIN);
        
        RTC_DateStruct.Year += HAL_RTCEx_BKUPRead(stm32rtcentry,RTC_BKP_DR1);
        RTC_DateStruct.Month += HAL_RTCEx_BKUPRead(stm32rtcentry,RTC_BKP_DR2);
        RTC_DateStruct.Date += HAL_RTCEx_BKUPRead(stm32rtcentry,RTC_BKP_DR3);
        RTC_DateStruct.WeekDay += HAL_RTCEx_BKUPRead(stm32rtcentry,RTC_BKP_DR4);
        RTC_DateStruct.WeekDay %= 8;
        HAL_RTC_SetDate(stm32rtcentry, &RTC_DateStruct, RTC_FORMAT_BIN);
#endif        
    }else{

		RTC_DateStruct.Year = 24;
		RTC_DateStruct.Month = 3;
		RTC_DateStruct.Date = 20;
		RTC_DateStruct.WeekDay = RTC_WEEKDAY_MONDAY;

		/* Set date */
		HAL_RTC_SetDate(stm32rtcentry, &RTC_DateStruct, RTC_FORMAT_BIN);

		/* Set time */
		RTC_TimeStruct.Hours = 0x10;
		RTC_TimeStruct.Minutes = 0x00;
		RTC_TimeStruct.Seconds = 0x00;
#if STM32_RTC_MODE == STM32_RTC_MODE_1
		RTC_TimeStruct.TimeFormat = RTC_HOURFORMAT_24;
		RTC_TimeStruct.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
		RTC_TimeStruct.StoreOperation = RTC_STOREOPERATION_RESET;
#endif

		/* Set time */
		HAL_RTC_SetTime(stm32rtcentry, &RTC_TimeStruct, RTC_FORMAT_BIN);
		
		/* Init RTC */
		HAL_RTC_Init(stm32rtcentry);
        
#if STM32_RTC_MODE == STM32_RTC_MODE_0        
        HAL_RTCEx_BKUPWrite(stm32rtcentry,RTC_BKP_DR1,RTC_DateStruct.Year);
        HAL_RTCEx_BKUPWrite(stm32rtcentry,RTC_BKP_DR1,RTC_DateStruct.Month);
        HAL_RTCEx_BKUPWrite(stm32rtcentry,RTC_BKP_DR1,RTC_DateStruct.Date);
        HAL_RTCEx_BKUPWrite(stm32rtcentry,RTC_BKP_DR1,RTC_DateStruct.WeekDay);
#endif  

		/* Save data to backup regiser */
		HAL_RTCEx_BKUPWrite(stm32rtcentry, RTC_BACKUP_STATUS, RTC_STATUS_READY);

    }
    
    return vatek_result_success;
} 


PVAT_RTC_DATETIME VATRTCGetTime(VAT_RTC_TIMEFMT nfmt)
{
	if (nfmt == RTC_TIMEFMT_BIN) 
        HAL_RTC_GetTime(stm32rtcentry,&RTC_TimeStruct,RTC_FORMAT_BIN);
    else HAL_RTC_GetTime(stm32rtcentry,&RTC_TimeStruct,RTC_FORMAT_BCD);
	
	/* Format hours */
	stm32CurrentTime.nHours = RTC_TimeStruct.Hours;
	stm32CurrentTime.nMinutes = RTC_TimeStruct.Minutes;
	stm32CurrentTime.nSeconds = RTC_TimeStruct.Seconds;
	
	/* Get subseconds */
	//stm32CurrentTime.nSubseconds = RTC->SSR;
	
	/* Get date */
	if (nfmt == RTC_TIMEFMT_BIN)
        HAL_RTC_GetDate(stm32rtcentry, &RTC_DateStruct, RTC_FORMAT_BIN);
	else HAL_RTC_GetDate(stm32rtcentry, &RTC_DateStruct, RTC_FORMAT_BCD);
	
	/* Format date */
	stm32CurrentTime.nYear = RTC_DateStruct.Year;
	stm32CurrentTime.nMonth = RTC_DateStruct.Month;
	stm32CurrentTime.nDay = RTC_DateStruct.Date;
	stm32CurrentTime.nWeekDay = RTC_DateStruct.WeekDay;


	/* Return OK */
	return &stm32CurrentTime;    
}

vatek_result VATRTCSetTime(PVAT_RTC_DATETIME prtcdate,VAT_RTC_TIMEFMT nfmt)
{
    VAT_RTC_DATETIME tmptime;
    
	if (nfmt == RTC_TIMEFMT_BCD) {
		tmptime.nDay = RTC_BCD2BIN(prtcdate->nDay);
		tmptime.nMonth = RTC_BCD2BIN(prtcdate->nMonth);
		tmptime.nYear = RTC_BCD2BIN(prtcdate->nYear);
		tmptime.nHours = RTC_BCD2BIN(prtcdate->nHours);
		tmptime.nMinutes = RTC_BCD2BIN(prtcdate->nMinutes);
		tmptime.nSeconds = RTC_BCD2BIN(prtcdate->nSeconds);
		tmptime.nWeekDay = RTC_BCD2BIN(prtcdate->nWeekDay);
	} else {
		tmptime.nDay = prtcdate->nDay;
		tmptime.nMonth = prtcdate->nMonth;
		tmptime.nYear = prtcdate->nYear;
		tmptime.nHours = prtcdate->nHours;
		tmptime.nMinutes = prtcdate->nMinutes;
		tmptime.nSeconds = prtcdate->nSeconds;
		tmptime.nWeekDay = prtcdate->nWeekDay;
	}
	
	/* Check year and month */
	//if(tmptime.nYear)tmptime.nYear = 16;
  if(tmptime.nMonth == 0 || tmptime.nMonth > 12)tmptime.nMonth = 12;
	if(tmptime.nDay == 0 || tmptime.nDay > RTC_Months[RTC_LEAP_YEAR(2000 + tmptime.nYear) ? 1 : 0][tmptime.nMonth - 1])
        tmptime.nDay = RTC_Months[RTC_LEAP_YEAR(2000 + tmptime.nYear) ? 1 : 0][tmptime.nMonth - 1];
    
    if(tmptime.nHours > 23)tmptime.nHours = 23;
    if(tmptime.nMinutes > 59)tmptime.nHours = 59;
    if(tmptime.nSeconds > 59)tmptime.nHours = 59;
    if(tmptime.nWeekDay == 0 || tmptime.nWeekDay > 7)tmptime.nWeekDay = 7;
	
	/* Fill time */
	RTC_TimeStruct.Hours = tmptime.nHours;
	RTC_TimeStruct.Minutes = tmptime.nMinutes;
	RTC_TimeStruct.Seconds = tmptime.nSeconds;
	
	/* Fill date */
	RTC_DateStruct.Date = tmptime.nDay;
	RTC_DateStruct.Month = tmptime.nMonth;
	RTC_DateStruct.Year = tmptime.nYear;
	RTC_DateStruct.WeekDay = tmptime.nWeekDay;
   
	/* Set time */
    
    HAL_RTC_SetTime(stm32rtcentry,&RTC_TimeStruct,RTC_FORMAT_BIN);
    HAL_RTC_SetDate(stm32rtcentry,&RTC_DateStruct,RTC_FORMAT_BIN);
    
#if STM32_RTC_MODE == STM32_RTC_MODE_0        
        HAL_RTCEx_BKUPWrite(stm32rtcentry,RTC_BKP_DR1,RTC_DateStruct.Year);
        HAL_RTCEx_BKUPWrite(stm32rtcentry,RTC_BKP_DR2,RTC_DateStruct.Month);
        HAL_RTCEx_BKUPWrite(stm32rtcentry,RTC_BKP_DR3,RTC_DateStruct.Date);
        HAL_RTCEx_BKUPWrite(stm32rtcentry,RTC_BKP_DR4,RTC_DateStruct.WeekDay);
#endif  
   
	
	/* Return OK */
	return vatek_result_success;
    
}


