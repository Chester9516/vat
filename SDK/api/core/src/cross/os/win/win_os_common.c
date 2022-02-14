
#include "internal/win_os_common.h"
#include <stdio.h>

#define WIN_GLOBAL_OBJBASE	"Global\\%s"
const char* win_get_global_name(const char* tag)
{

	static char sztmp[256];
	sprintf(sztmp, WIN_GLOBAL_OBJBASE, tag);
	return &sztmp[0];
}

void win_get_last_error(void)
{
	uint32_t id = GetLastError();
	#define err_code(name)	else if(id == name)cross_os_printf("win_err : %s",#name);

	if (id == ERROR_SUCCESS)return;
	err_code(ERROR_ALREADY_EXISTS)
	err_code(ERROR_ACCESS_DENIED)
}