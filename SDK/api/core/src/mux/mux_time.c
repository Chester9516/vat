
#include <mux/mux_time.h>

void muxtime_append_hours(Pmux_time ptime, int32_t hours)
{
	uint32_t val = hours + ptime->hour;
	if (val < 24)ptime->hour = val;
	else
	{
		ptime->hour = (uint8_t)(val % 24);
		muxtime_append_days(ptime, val / 24);
	}
}

void muxtime_append_days(Pmux_time ptime, int32_t days)
{
	uint32_t mdays = muxtime_get_month_days(ptime);
	uint32_t val = ptime->day + days;

	do
	{
		if (val > mdays)
		{
			val -= mdays;
			ptime->month++;
			if (ptime->month > 12)
			{
				ptime->month = 1;
				ptime->year++;
			}
			mdays = muxtime_get_month_days(ptime);
		}

	} while (mdays < val);

	ptime->day = (uint8_t)val;
}

uint32_t muxtime_eclipse_hour(Pmux_time pstart, Pmux_time pcur)
{
	if (pstart->year == pcur->year &&
		pstart->month == pcur->month &&
		pstart->day == pcur->day &&
		pcur->hour >= pstart->hour)return pcur->hour - pstart->hour;
	else return 24;  /* different over one day */
}

uint8_t muxtime_get_month_days(Pmux_time ptime)
{
	static const int date_days_month[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	if (ptime->month == 2)
	{
		if (muxtime_is_leap_year(ptime))return 29;
		else return 28;
	}
	return date_days_month[ptime->month];
}

int32_t muxtime_is_leap_year(Pmux_time ptime)
{
	#define is_leap_year(year) ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
	return is_leap_year(ptime->year);
}

uint32_t muxtime_date_to_reg(Pmux_time ptime)
{
	return (ptime->year << 16) | (ptime->month << 8) | (ptime->day);
}

uint32_t muxtime_clock_to_reg(Pmux_time ptime)
{
	return (ptime->hour << 16) | (ptime->minute << 8) | ptime->second;
}

void muxtime_date_from_reg(Pmux_time ptime, uint32_t reg)
{
	ptime->year = (uint16_t)(reg >> 16);
	ptime->month = (uint8_t)(reg >> 8);
	ptime->day = (uint8_t)reg;
}

void muxtime_clock_from_reg(Pmux_time ptime, uint32_t reg)
{
	ptime->hour = (uint8_t)(reg >> 16);
	ptime->minute = (uint8_t)(reg >> 8);
	ptime->second = (uint8_t)reg;
}
