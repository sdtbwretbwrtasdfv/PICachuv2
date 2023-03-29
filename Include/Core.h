/* This file creates automaticly, do not change it manualy */
#include <Includes.h>
#include <Macros.h>
#include <Hashes.h>
UINT_PTR GetRIP( VOID );
	typedef struct {
		struct {
			WIN32_FUNC( LoadLibraryA );
			WIN32_FUNC( VirtualAlloc );
			WIN32_FUNC( ExitProcess );
			WIN32_FUNC( MessageBoxA );
			WIN32_FUNC( InternetOpenA );
			WIN32_FUNC( InternetConnectA );
			WIN32_FUNC( HttpOpenRequestA );
			WIN32_FUNC( HttpSendRequestA );
			WIN32_FUNC( InternetReadFile );
			WIN32_FUNC( InternetCloseHandle );
			WIN32_FUNC( printf );
		} Win32;
		struct {
			HMODULE Kernel32;
			HMODULE Msvcrt;
			HMODULE User32;
			HMODULE Wininet;
		} Modules;
} INSTANCE, *PINSTANCE;
