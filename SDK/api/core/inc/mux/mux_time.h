
#ifndef _MUX_TIME_
#define _MUX_TIME_

#include <mux/mux_define.h>

typedef struct _mux_time{
	uint16_t year;
	uint8_t month;
	uint8_t day;
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
	uint8_t recv;
}mux_time;

typedef mux_time* Pmux_time;

#define MUXTIME_DATA_TO_UINT(time)	((time.year << 16) | (time.month << 8) | (time.day))
#define MUXTIME_TIME_TO_UINT(time)	((time.hour << 16) | (time.minute << 8) | (time.second))

#ifdef __cplusplus
extern "C" {
#endif

	HAL_API void muxtime_append_hours(Pmux_time ptime, int32_t hours);
	HAL_API void muxtime_append_days(Pmux_time ptime, int32_t days);
	HAL_API uint32_t muxtime_eclipse_hour(Pmux_time pstart, Pmux_time pcur);
	HAL_API uint8_t muxtime_get_month_days(Pmux_time ptime);
	HAL_API int32_t muxtime_is_leap_year(Pmux_time ptime);
	HAL_API uint32_t muxtime_date_to_reg(Pmux_time ptime);
	HAL_API uint32_t muxtime_clock_to_reg(Pmux_time ptime);
	HAL_API void muxtime_date_from_reg(Pmux_time ptime, uint32_t reg);
	HAL_API void muxtime_clock_from_reg(Pmux_time ptime, uint32_t reg);

#ifdef __cplusplus
}
#endif

#endif