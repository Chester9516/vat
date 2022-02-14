#include <core/vatek_base.h>
#include "device/internal/cross_device_tool.h"
#include <stdarg.h>
#if _MSC_VER
	#include <windows.h>
#endif

vatek_debug_level m_debug_level = debug_level_warning;

uint32_t vatek_get_tick_ms(void)
{
	return cross_os_get_tick_ms();
}

void vatek_sleep_ms(uint32_t ms)
{
	cross_os_sleep(ms);
}

void vatek_debug_set(vatek_debug_level level)
{
	m_debug_level = level;
}

void vatek_print(const char* fmt, ...)
{
#if _MSC_VER
	char sztmp[256];
	va_list arg;
	va_start(arg, fmt);
	vsprintf(sztmp, fmt, arg);
	va_end(arg);
	OutputDebugStringA(sztmp);
#else
	va_list arg;
	va_start(arg, fmt);
	vprintf(fmt, arg);
	va_end(arg);
#endif
}

void vatek_debug_print(vatek_debug_level level, const char* fmt, ...)
{
	if (level >= m_debug_level)
	{
#if _MSC_VER
		char sztmp[256];
		va_list arg;
		va_start(arg, fmt);
		vsprintf(sztmp, fmt, arg);
		va_end(arg);
		OutputDebugStringA(sztmp);
#else
		va_list arg;
		va_start(arg, fmt);
		vprintf(fmt, arg);
		va_end(arg);
#endif
	}
}

vatek_result vatek_chip_read_register(hvatek_chip hchip, int32_t addr, uint32_t* val)
{
	Pcross_device pcross = ((Pvatek_device)hchip)->cross;
	return pcross->core->read_register(pcross->hcross, addr, val);
}

vatek_result vatek_chip_write_register(hvatek_chip hchip, int32_t addr, uint32_t val)
{
	Pcross_device pcross = ((Pvatek_device)hchip)->cross;
	return pcross->core->write_register(pcross->hcross, addr, val);
}

vatek_result vatek_chip_read_memory(hvatek_chip hchip, int32_t addr, uint32_t* val)
{
	Pcross_device pcross = ((Pvatek_device)hchip)->cross;
	return pcross->core->read_memory(pcross->hcross, addr, val);
}

vatek_result vatek_chip_write_memory(hvatek_chip hchip, int32_t addr, uint32_t val)
{
	Pcross_device pcross = ((Pvatek_device)hchip)->cross;
	return pcross->core->write_memory(pcross->hcross, addr, val);
}

vatek_result vatek_chip_write_buffer(hvatek_chip hchip, int32_t addr, uint8_t* buf, int32_t wlen)
{
	Pcross_device pcross = ((Pvatek_device)hchip)->cross;
	return pcross->core->write_buffer(pcross->hcross, addr, buf, wlen);
}

vatek_result vatek_chip_read_buffer(hvatek_chip hchip, int32_t addr, uint8_t* buf, int32_t wlen)
{
	Pcross_device pcross = ((Pvatek_device)hchip)->cross;
	return pcross->core->read_buffer(pcross->hcross, addr, buf, wlen);
}