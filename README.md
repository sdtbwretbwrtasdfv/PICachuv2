# PICachu v2     (╯°□°）╯︵ ┻━┻

## _Easy to use position independent code compile_
PICachu is a framework for writing x86/x64 position independent code for Windows.

Basic usage:
- Add needed WinAPI to "winapis.defs" using following format - *dllname.dll->func_name*
- Add header file for resolved APIs to "Include/Includes.h"
- Add whatever ya whant to "Source/Main.c"
- make

All functions that we specified in "winapis.txt" will be available in Instance.Win32. struct (call example: Instance.Win32.printf(msg); ).
If you wanna make them also available outside "Main" just pass Instance to func.

## How to use
Imagine that we want to port to PIC following code:

    #include <Windows.h>
    #include <WinInet.h>
    #include <stdio.h>

    #pragma comment(lib, "wininet.lib")

    int main() {
    	char user_agent[] = "user";
    	char sitename[] = "localhost";
    	int port = 8080;
    	const char* acceptTypes[] = { '*','/','*', NULL };
    	char method[] = "GET";
    	char site_param[] = "success";

    	HINTERNET hInternet = InternetOpenA(user_agent, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    	HINTERNET hConnect = InternetConnectA(hInternet, sitename, port, NULL, NULL, INTERNET_SERVICE_HTTP, 0, NULL);
    	HINTERNET hRequest = HttpOpenRequestA(hConnect, method, site_param, NULL, NULL, acceptTypes, 0, 0);
    	BOOL bRequestSent = HttpSendRequestA(hRequest, NULL, 0, NULL, 0);
    	BOOL bKeepReading = TRUE;
    	const int nBuffSize = 100000;
    	int size = 0;
    	char* buff = VirtualAlloc(0, 100000, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    	DWORD dwBytesRead = -1;
    	while (bKeepReading && dwBytesRead != 0) {
    		bKeepReading = InternetReadFile(hRequest, buff, nBuffSize, &dwBytesRead);
        }
    	printf(buff);
    	InternetCloseHandle(hRequest);
    	InternetCloseHandle(hConnect);
    	nternetCloseHandle(hInternet);
    	return 0;
    }
All we need to do:
- Add "#include <wininet.h>" to "Include/Includes.h"
- Add $dllname->ApiName for each function and store all strings like CHAR(test, "test").

**PICachu/Source/Main.c**:


    #include <Core.h>
    #include <Win32.h>

    SEC( text, B ) INT Main( INSTANCE Instance, LPCWSTR PassedToShellcode ) {
    	CHAR(sitename, 	"192.168.0.101");
    	CHAR(user_agent, 	"useragent");
    	CHAR(acceptTypes, 	"*/*");
    	CHAR(method, 		"GET");
    	CHAR(site_param, 	"test");
    	int port = 8080;
    	HINTERNET hInternet = Instance.Win32.InternetOpenA(user_agent, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    	HINTERNET hConnect 	= Instance.Win32.InternetConnectA(hInternet, sitename, port, NULL, NULL, INTERNET_SERVICE_HTTP, 0, NULL);
    	HINTERNET hRequest 	= Instance.Win32.HttpOpenRequestA(hConnect, method, site_param, NULL, NULL, acceptTypes, 0, 0);
    	BOOL bRequestSent 	= Instance.Win32.HttpSendRequestA(hRequest, NULL, 0, NULL, 0);
    	BOOL bKeepReading 	= TRUE;
    	const int nBuffSize = 100000;
    	int size 			= 0;
    	char* buff 		= Instance.Win32.VirtualAlloc(0, 100000, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    	DWORD dwBytesRead 	= -1;
    	while (bKeepReading && dwBytesRead != 0) {
    		bKeepReading = Instance.Win32.InternetReadFile(hRequest, buff, nBuffSize, &dwBytesRead);
        }
    	Instance.Win32.printf(buff);
    	Instance.Win32.InternetCloseHandle(hRequest);
    	Instance.Win32.InternetCloseHandle(hConnect);
    	Instance.Win32.InternetCloseHandle(hInternet);
    	return 0;
    }

**PICachu/winapis.defs:**


    kernel32.dll->LoadLibraryA
    kernel32.dll->VirtualAlloc
    kernel32.dll->ExitProcess
    user32.dll->MessageBoxA
    wininet.dll->InternetOpenA
    wininet.dll->InternetConnectA
    wininet.dll->HttpOpenRequestA
    wininet.dll->HttpSendRequestA
    wininet.dll->InternetReadFile
    wininet.dll->InternetCloseHandle
    msvcrt.dll->printf

**NOTE. After compile you will get randomly generated hashes for all used APIs!**

**Tesing:**

    (#node_1㉿#Usaaar)-> python3 -m http.server 8080
    Serving HTTP on :: port 8080 (http://[::]:8080/) ...
    ::ffff:192.168.0.101 - - [15/Mar/2023 16:05:35] code 404, message File not found
    ::ffff:192.168.0.101 - - [15/Mar/2023 16:05:35] "GET /test HTTP/1.1" 404 -
    ^C
    Keyboard interrupt received, exiting.
    (#node_1㉿#Usaaar)-> ls -la Bin
    total 96
    drwxr-xr-x@  8 user  staff    256 29 мар 16:33 .
    drwxr-xr-x  13 user  staff    416 29 мар 16:33 ..
    -rw-r--r--   1 user  staff   1535 29 мар 16:33 PICachu.x64.bin
    -rwxr-xr-x@  1 user  staff  16384 29 мар 16:33 PICachu.x64_inlined.exe
    -rw-r--r--   1 user  staff   1535 29 мар 16:33 PICachu.x86.bin
    -rwxr-xr-x@  1 user  staff  13312 29 мар 16:33 PICachu.x86_inlined.exe
    -rw-r--r--   1 user  staff    345 29 мар 16:33 asm.x64.o
    -rw-r--r--   1 user  staff    352 29 мар 16:33 asm.x86.o


In this example PICachu.x86_inlined.exe was launched under x64 Windows using.

## Size Matters
Sinse we don't use stdlib and all other usefull stuff, resulting PE will be small :)
## Why? For what?
Only for study. So the result may not be stable :)
PICachu automatically creates hashes for all used APIs, and also automatically creates the structure and rewrites Entry. So that the APIs can be accessed in the future.
## How to inline this after compile?
After compile just use scripts/bin2sc.sh to create PIC hex blob or use scripts/inline.sh
## Creds
PICachu V2 is a Frankenstein that baised on original PICachu and ShellcodeTemplate (https://github.com/Cracked5pider/ShellcodeTemplate)
***So huge credit goes to Cracked5pider and SecIdiot!!!***
## Warranty
DO NOT USE THIS FOR ILLEGAL PURPOSES.THE AUTHOR DOES NOT KEEP RESPONSIBILITY FOR ANY ILLEGAL ACTION YOU DO.
THERE IS NO WARRANTY FOR THE PROGRAM, TO THE EXTENT PERMITTED BY APPLICABLE LAW.
THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE PROGRAM IS WITH YOU
