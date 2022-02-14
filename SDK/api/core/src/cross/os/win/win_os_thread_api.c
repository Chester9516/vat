
#include "internal/win_os_common.h"

typedef struct _win_thread
{
	cross_thread_param param;
	HANDLE hhandle;
	DWORD threadid;
}win_thread, *Pwin_thread;

hcross_thread cross_os_create_thread(fpcross_thread_function fpfun, void* userparam)
{
	Pwin_thread newthread = (Pwin_thread)malloc(sizeof(win_thread));
	memset(newthread, 0, sizeof(win_thread));
	newthread->param.userparam = userparam;

	newthread->hhandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)fpfun, &newthread->param, 0, &newthread->threadid);
	if (newthread->hhandle != INVALID_HANDLE_VALUE)
		return newthread;

	free(newthread);
	return NULL;
}

vatek_result cross_os_free_thread(hcross_thread hthread)
{
	Pwin_thread winthread = (Pwin_thread)hthread;
	vatek_result nres = cross_os_join_thread(hthread);
	CloseHandle(winthread->hhandle);
	free(winthread);
	return nres;
}

vatek_result cross_os_join_thread(hcross_thread hthread)
{
	Pwin_thread winthread = (Pwin_thread)hthread;
	uint32_t nres = WaitForSingleObject(winthread->hhandle, 1000);
	if (nres == WAIT_OBJECT_0)
		return winthread->param.result;
	else if (nres == WAIT_TIMEOUT)
	{
		cross_os_printf("WaitForSingleObject timeout");
		return vatek_timeout;
	}
	cross_os_printf("WaitForSingleObject unknown : %d", nres);
	return vatek_unknown;
}

vatek_result cross_os_get_thread_result(hcross_thread hthread)
{
	Pwin_thread winthread = (Pwin_thread)hthread;
	return winthread->param.result;
}
