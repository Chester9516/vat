
#include "internal/win_os_common.h"

vatek_result cross_os_create_event(const char* tag, hcross_event* hevent)
{
	HANDLE newevent = INVALID_HANDLE_VALUE;
	//newevent = CreateEventA(NULL, FALSE, FALSE, win_get_global_name(tag));
	newevent = CreateEventA(NULL, FALSE, FALSE,tag);
	if (newevent != INVALID_HANDLE_VALUE)
	{
		*hevent = newevent;
		return vatek_success;
	}
	else win_get_last_error();
	return vatek_memfail;
}

vatek_result cross_os_open_event(const char* tag, hcross_event* hevent)
{
	HANDLE newevent = INVALID_HANDLE_VALUE;
	//newevent = OpenEventA(0, FALSE, win_get_global_name(tag));
	newevent = OpenEventA(0, FALSE, tag);
	if (newevent != INVALID_HANDLE_VALUE)
	{
		*hevent = newevent;
		return vatek_success;
	}
	else win_get_last_error();
	return vatek_memfail;
}

vatek_result cross_os_wait_event_timeout(hcross_event hevent, int32_t ms)
{
	uint32_t ncode = WaitForSingleObject((HANDLE)hevent, ms);
	if (ncode == WAIT_OBJECT_0)return vatek_success;
	else return vatek_badstatus;
}

vatek_result cross_os_wait_event(hcross_event hevent)
{
	uint32_t ncode = WaitForSingleObject((HANDLE)hevent, INFINITE);
	if(ncode == WAIT_OBJECT_0)return vatek_success;
	else
	{
		cross_os_printf("wait event fail : %08x", ncode);
		return vatek_badstatus;
	}
}

vatek_result cross_os_raise_event(hcross_event hevent)
{
	if (SetEvent((HANDLE)hevent))return vatek_success;
	else return vatek_unsupport;
}

void cross_os_free_event(hcross_event hevent)
{
	CloseHandle(hevent);
}
