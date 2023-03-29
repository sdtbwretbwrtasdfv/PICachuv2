/* This file creates automaticly, do not change it manualy */
#pragma once
#include <stdint.h>
#include "windows.h"
#include "wininet.h"
#include "common.h"
HMODULE __cdecl _LoadLibraryA (LPCSTR);
LPVOID __cdecl _VirtualAlloc (LPVOID, SIZE_T, DWORD, DWORD);
BOOL __cdecl _VirtualProtect (LPVOID, SIZE_T, DWORD, PDWORD);
INT __cdecl _printf (LPCSTR, ...);
HINTERNET __cdecl _HttpOpenRequestA (HINTERNET, LPCSTR, LPCSTR, LPCSTR, LPCSTR, LPCSTR*, DWORD, DWORD_PTR);
BOOL __cdecl _HttpSendRequestA (HINTERNET, LPCSTR, DWORD, LPVOID, DWORD);
BOOL __cdecl _InternetCloseHandle (HINTERNET);
HINTERNET __cdecl _InternetConnectA (HINTERNET, LPCSTR, INTERNET_PORT, LPCSTR, LPCSTR, DWORD, DWORD, DWORD_PTR);
HINTERNET __cdecl _InternetOpenA (LPCSTR, DWORD, LPCSTR, LPCSTR, DWORD);
BOOL __cdecl _InternetReadFile (HINTERNET, LPVOID, DWORD, LPDWORD);
typedef HMODULE(WINAPI* tLoadLibraryA)(LPCSTR);
typedef LPVOID(WINAPI* tVirtualAlloc)(LPVOID, SIZE_T, DWORD, DWORD);
typedef BOOL(WINAPI* tVirtualProtect)(LPVOID, SIZE_T, DWORD, PDWORD);
typedef INT(WINAPI* tprintf)(LPCSTR, ...);
typedef HINTERNET(WINAPI* tHttpOpenRequestA)(HINTERNET, LPCSTR, LPCSTR, LPCSTR, LPCSTR, LPCSTR*, DWORD, DWORD_PTR);
typedef BOOL(WINAPI* tHttpSendRequestA)(HINTERNET, LPCSTR, DWORD, LPVOID, DWORD);
typedef BOOL(WINAPI* tInternetCloseHandle)(HINTERNET);
typedef HINTERNET(WINAPI* tInternetConnectA)(HINTERNET, LPCSTR, INTERNET_PORT, LPCSTR, LPCSTR, DWORD, DWORD, DWORD_PTR);
typedef HINTERNET(WINAPI* tInternetOpenA)(LPCSTR, DWORD, LPCSTR, LPCSTR, DWORD);
typedef BOOL(WINAPI* tInternetReadFile)(HINTERNET, LPVOID, DWORD, LPDWORD);
typedef struct {
	D_API( _LoadLibraryA );
	D_API( _VirtualAlloc );
	D_API( _VirtualProtect );
	D_API( _printf );
	D_API( _HttpOpenRequestA );
	D_API( _HttpSendRequestA );
	D_API( _InternetCloseHandle );
	D_API( _InternetConnectA );
	D_API( _InternetOpenA );
	D_API( _InternetReadFile );
} API ;
