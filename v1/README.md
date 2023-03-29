# PICachu     (╯°□°）╯︵ ┻━┻

## _Easy to use position independent code compile_
PICachu is a framework for writing x86/x64 position independent code for Windows.

Basic usage:
- Add needed WinAPI to "winapis.txt" using following format - *dllname.dll->func_name<-func_type<<(func_arg1, func_arg2, etc)*
- Add whatever ya whant to "src_mainpic/main.h"
- Use only headers and include them to main.h
- ./compile.sh

All functions that we specified in "winapis.txt" will be available in Api struct with underline prefix (call example: Api._printf(msg); ).
If you wanna make them also available outside "Mmain" just pass Api member to func.

Actually with this you can only resolv functions from:
- Apphelp.dll
- Magnification.dll
- advapi32.dll
- authz.dll
- comctl32.dll
- crypt32.dll
- cryptui.dll
- csrsrv.dll
- dnsapi.dll
- gdi32.dll
- guser32.dll
- iphlpapi.dll
- kernel32.dll
- lz32.dll
- mscoree.dll
- mswsock.dll
- netapi32.dll
- normaliz.dll
- ntdll.dll
- ole32.dll
- oleaut32.dll
- powrprof.dll
- propsys.dll
- psapi.dll
- rpcrt4.dll
- secur32.dll
- shcore.dll
- shell32.dll
- shlwapi.dll
- ucrtbase.dll
- urlmon.dll
- user32.dll
- userenv.dll
- winhttp.dll
- wininet.dll
- wintrust.dll
- ws2_32.dll

If you need some additional library, add just run "***scripts/gen_dll_if_statement.sh your_dll_name.dll***" and paste result to src_common/api_resolve.c to "DLL SECTION".

Script output should look like:

    (#node_1㉿#Usaaar)->  scripts/gen_dll_if_statement.sh ntdll.dll
    #ifdef H_LIB_NTDLL
    	if (dll_hash == H_LIB_NTDLL) {
    		char dll_name[] = { 'n', 't', 'd', 'l', 'l', '.', 'd', 'l', 'l', 0x00 };
    		ptr_loaded_dll = (uint64_t)((tLoadLibraryA)fptr_loadLibary)(dll_name);
    	}
    #endif


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
All we need to do is just add "Api._" to each function and store all strings as char arrays! And add WinApis to winapis.txt.

**PICachu/src_mainpic/main.h**:

    #include "ministdlib.h"

    int Mmain(API Api, LPCWSTR PasswdFromExecute){
    	char sitename[]		= { 'l','o','c','a','l','h','o','s','t',0 };
    	char user_agent[] 	= { 'u','s','e','r',0 };
    	char method[] 		= { 'G','E','T',0 };
    	char site_param[] 	= { 's','h','e','l','l','c','o','d','e','.','b','i','n',0 };
    	int port 			= 8080;

    	HINTERNET hInternet = Api._InternetOpenA(user_agent, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    	HINTERNET hConnect 	= Api._InternetConnectA(hInternet, sitename, port, NULL, NULL, INTERNET_SERVICE_HTTP, 0, NULL);
    	HINTERNET hRequest 	= Api._HttpOpenRequestA(hConnect, method, site_param, NULL, NULL, NULL, 0, 0);
    	Api._HttpSendRequestA(hRequest, NULL, 0, NULL, 0);
    	char* buff = Api._VirtualAlloc(0, 100000, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    	DWORD dwBytesRead = -1;
    	while (TRUE && dwBytesRead != 0) {
    		Api._InternetReadFile(hRequest, buff, 100000, &dwBytesRead);
    	}
    	Api._printf(buff);
    	Api._InternetCloseHandle(hRequest);
    	Api._InternetCloseHandle(hConnect);
    	Api._InternetCloseHandle(hInternet);
    	return 0;
    }
**PICachu/winapis.txt:**


    kernel32.dll->LoadLibraryA<-HMODULE<<(LPCSTR)
    kernel32.dll->VirtualProtect<-BOOL<<(LPVOID, SIZE_T, DWORD, PDWORD)
    msvcrt.dll->printf<-INT<<(LPCSTR, ...)

    wininet.dll->InternetOpenA<-HINTERNET<<(LPCSTR, DWORD, LPCSTR, LPCSTR, DWORD)
    wininet.dll->InternetConnectA<-HINTERNET<<(HINTERNET, LPCSTR, INTERNET_PORT, LPCSTR, LPCSTR, DWORD, DWORD, DWORD_PTR)
    wininet.dll->HttpOpenRequestA<-HINTERNET<<(HINTERNET, LPCSTR, LPCSTR, LPCSTR, LPCSTR, LPCSTR*, DWORD, DWORD_PTR)
    wininet.dll->HttpSendRequestA<-BOOL<<(HINTERNET, LPCSTR, DWORD, LPVOID, DWORD)
    kernel32.dll->VirtualAlloc<-LPVOID<<(LPVOID, SIZE_T, DWORD, DWORD)
    wininet.dll->InternetReadFile<-BOOL<<(HINTERNET, LPVOID, DWORD, LPDWORD)
    wininet.dll->InternetCloseHandle<-BOOL<<(HINTERNET)


**NOTE. After compile you will get randomly generated hashes for all used APIs:**

    (#node_1㉿#Usaaar)-> PICachu % ./compile.sh
    [*]Cleanning...
    rm: build/*.*: No such file or directory
    [*]Building API hashes file
    [*]Build adjust-stack
    [*]Build ApiResolve x64
    [*]Build ApiResolve x86
    [*]Build PIC for arch: x64
    [*]Build mainpic object
    [*]Build PIC for arch: x86
    [*]Build mainpic object
    .exe to shellcode
    .exe to .bin
    .exe to shellcode
    .exe to .bin

    (#node_1㉿#Usaaar)-> cat src_common/api_hashes.h
    #pragma once
    /* This file creates automaticly, do not change it manualy */
    /* seed1 : 18731, seed2: 5*/
    #define H_LIB_KERNEL32 0x864a8b9b
    #define H_LIB_MSVCRT 0xa0004b14
    #define H_LIB_WININET 0x40d52cd3
    #define H_API_LOADLIBRARYA 0x75c98e21
    #define H_API_VIRTUALPROTECT 0x43357033
    #define H_API_PRINTF 0x56e7645e
    #define H_API_INTERNETOPENA 0xcbeab287
    #define H_API_INTERNETCONNECTA 0x5293217f
    #define H_API_HTTPOPENREQUESTA 0xff3c7a07
    #define H_API_HTTPSENDREQUESTA 0x9f90d5df
    #define H_API_VIRTUALALLOC 0x4e35acbd
    #define H_API_INTERNETREADFILE 0x98f4ed0
    #define H_API_INTERNETCLOSEHANDLE 0x857e5556

    (#node_1㉿#Usaaar)-> ./compile.sh
    [*]Cleanning...
    [*]Building API hashes file
    [*]Build adjust-stack
    [*]Build ApiResolve x64
    [*]Build ApiResolve x86
    [*]Build PIC for arch: x64
    [*]Build mainpic object
    [*]Build PIC for arch: x86
    [*]Build mainpic object
    .exe to shellcode
    .exe to .bin
    .exe to shellcode
    .exe to .bin

    (#node_1㉿#Usaaar)-> cat src_common/api_hashes.h
    #pragma once
    /* This file creates automaticly, do not change it manualy */
    /* seed1 : 17224, seed2: 5*/
    #define H_LIB_KERNEL32 0x49d59938
    #define H_LIB_MSVCRT 0x8f414d71
    #define H_LIB_WININET 0x18367ad0
    #define H_API_LOADLIBRARYA 0x39549bbe
    #define H_API_VIRTUALPROTECT 0x15ba5910
    #define H_API_PRINTF 0x31d3c03b
    #define H_API_INTERNETOPENA 0xd773c4
    #define H_API_INTERNETCONNECTA 0xd9f5b59c
    #define H_API_HTTPOPENREQUESTA 0x869f0e24
    #define H_API_HTTPSENDREQUESTA 0x26f369fc
    #define H_API_VIRTUALALLOC 0x11c0ba5a
    #define H_API_INTERNETREADFILE 0x90f1e2ed
    #define H_API_INTERNETCLOSEHANDLE 0xbad85053

**Tesing:**

    (#node_1㉿#Usaaar)-> python3 -m http.server 8080
    Serving HTTP on :: port 8080 (http://[::]:8080/) ...
    ::ffff:192.168.0.101 - - [15/Mar/2023 16:05:35] code 404, message File not found
    ::ffff:192.168.0.101 - - [15/Mar/2023 16:05:35] "GET /shellcode.bin HTTP/1.1" 404 -
    ^C
    Keyboard interrupt received, exiting.
    (#node_1㉿#Usaaar)-> ls -la build
    total 560
    drwxr-xr-x@ 10 user  staff     320 15 мар 16:05 .
    drwxr-xr-x@ 14 user  staff     448 15 мар 16:05 ..
    -rwxr-xr-x@  1 user  staff  126793 15 мар 16:05 inlined_pic_x64.exe
    -rwxr-xr-x@  1 user  staff  126793 15 мар 16:05 inlined_pic_x86.exe
    -rwxr-xr-x@  1 user  staff    3584 15 мар 16:05 original_pic_x64.exe
    -rwxr-xr-x@  1 user  staff    3584 15 мар 16:05 original_pic_x86.exe
    -rwxr-xr-x@  1 user  staff    1680 15 мар 16:05 pic_x64.bin
    -rw-r--r--@  1 user  staff    6744 15 мар 16:05 pic_x64.bin.h
    -rwxr-xr-x@  1 user  staff    1808 15 мар 16:05 pic_x86.bin
    -rw-r--r--@  1 user  staff    7256 15 мар 16:05 pic_x86.bin.h

In this example inlined_pic_x64.exe was launched under x64 Windows using.

## Size Matters
Sinse we don't use stdlib and all other usefull stuff, resulting PE will be small :)
If needed... Check "src_loader/stdlib" or dyn resolve from msvcrt.dll.
```sh
#node_1㉿#Usaaar)-> cat helloworld.c
#include <stdio.h>
int main(){
	printf("Hello World!");
}    
(#node_1㉿#Usaaar)-> x86_64-w64-mingw32-gcc helloworld.c                                                                                                      
(#node_1㉿#Usaaar)-> ls -lh a.exe | awk '{print  $5}'   
285K
(#node_1㉿#Usaaar)-> echo "msvcrt.dll->printf<-INT<<(LPCSTR, ...)" >> winapis.txt
(#node_1㉿#Usaaar)-> python3 scripts/str2chars.py 'Hello world!'
char msg[] = {'H','e','l','l','o',' ','w','o','r','l','d','!', 0x00};
(#node_1㉿#Usaaar)-> cat src_mainpic/main.h
int Mmain(API Api, LPCWSTR PasswdFromExecute){
    char msg[] = {'H','e','l','l','o',' ','w','o','r','l','d','!', 0x00};
    Api._printf(msg);
    return 0;
}
(#node_1㉿#Usaaar)-> ./compile.sh
	[*]Cleanning...
	[*]Building API hashes file
	[*]Build adjust-stack
	[*]Build ApiResolve x64
	[*]Build ApiResolve x86
	[*]Build PIC for arch: x64
	[*]Build mainpic object
	[*]Build PIC for arch: x86
	[*]Build mainpic object
	[*]Compiling loader object arch: x64
	[*]Compiling loader object arch: x86
(#node_1㉿#Usaaar)-> ls -lh build/loader_x64.exe | awk '{print  $5}'    
	4.0K
```

```sh
PS C:\Users\User\Desktop\tests> .\a.exe
	Hello World!
PS C:\Users\User\Desktop\tests> .\loader_x64.exe
	Hello world!
PS C:\Users\User\Desktop\tests> .\loader_x86.exe
	Hello world!
PS C:\Users\User\Desktop\tests> .\pic_x64.exe
	Hello world!
PS C:\Users\User\Desktop\tests> .\pic_x86.exe
	Hello world!
PS C:\Users\User\Desktop\tests> dir
    Directory: C:\Users\User\Desktop\tests
	Mode                 LastWriteTime         Length Name
	----                 -------------         ------ ----
	-a----         6/13/2022   5:02 PM         291000 a.exe
	-a----         6/13/2022   5:06 PM           4096 loader_x64.exe
	-a----         6/13/2022   5:06 PM           4608 loader_x86.exe
	-a----         6/13/2022   5:06 PM           2560 pic_x64.exe
	-a----         6/13/2022   5:06 PM           2560 pic_x86.exe
```
## Why? For what?
Only for study. So the result may not be stable :)
PICachu automatically creates hashes for all used APIs, and also automatically creates the structure and rewrites Entry.c so that the APIs can be accessed in the future.

## Limitations
We cannot use strings since these will not be included in the .text section of our executable, we need to write them as byte arrays to ensure that they are dynamically constructed on the stack.
Helper - scripts/str2chars.py
```sh
(#node_1㉿#Usaaar)-> python3 scripts/str2chars.py test
char msg[] = {'t','e','s','t', 0x00};
```
Another way - use macro, for cast vars or funcs in a specific region of memory.
Something like:
```sh
#define D_SEC( x )	__attribute__(( section( ".text$" #x ) ))
D_SEC( SomeSectionThatShouldBeCreatingWhileLinking ) char * my_var = "hehehe";
```
But if you do so, then you will have to change the logic of the linker, by custom script.
## How to inline this after compile?
After compile just use scripts/bin2sc.sh to create PIC hex blob. Like this:
```sh
(#node_1㉿#Usaaar)-> cripts/bin2sc.sh -exe build/loader_x64.exe
(#node_1㉿#Usaaar)-> cat build/loader_x64.bin.h
char shellcode [] = "\x56\x48\x89\xe6\x48";
```
After that allocated blob inside the .text. Cast the array containing our blob into a function pointer and invoke it.
Example for Visual Studio:
```sh
#pragma section(".text")
__declspec(allocate(".text")) char our_blob[] =
"\xfc\xfc\xfc\xfc\xfc\xfc\xfc\xfc\xfc\xfc\xfc\xfc\xfc\xfc\xfc";

int main() {
    (*(void(*)())(&our_blob))();
}
```
## Warranty
DO NOT USE THIS FOR ILLEGAL PURPOSES.THE AUTHOR DOES NOT KEEP RESPONSIBILITY FOR ANY ILLEGAL ACTION YOU DO.
THERE IS NO WARRANTY FOR THE PROGRAM, TO THE EXTENT PERMITTED BY APPLICABLE LAW.
THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE PROGRAM IS WITH YOU
