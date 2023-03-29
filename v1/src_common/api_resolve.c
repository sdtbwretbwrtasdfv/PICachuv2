#include "api_structs.h"

#pragma GCC diagnostic ignored "-Wpointer-to-int-cast"
#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"
#pragma GCC diagnostic ignored "-Warray-bounds"

uint64_t getFunctionPtr(unsigned long, unsigned long);
static uint64_t getDllBase(unsigned long);
static uint64_t loadDll(unsigned long);
static uint64_t parseHdrForPtr(uint64_t, unsigned long);
static unsigned long djb2(unsigned char*);
static unsigned long unicode_djb2(const wchar_t*);
static WCHAR* toLower(WCHAR* str);


uint64_t getFunctionPtr(unsigned long dll_hash, unsigned long function_hash) {
	uint64_t dll_base = 0x00;
	uint64_t ptr_function = 0x00;
	dll_base = getDllBase(dll_hash);
	if (dll_base == 0) {
		dll_base = loadDll(dll_hash);
		if (dll_base == 0)
			return 0;
	}
	ptr_function = parseHdrForPtr(dll_base, function_hash);
	return ptr_function;
}
static uint64_t loadDll(unsigned long dll_hash) {
	uint64_t kernel32_base = 0x00;
	uint64_t fptr_loadLibary = 0x00;
	uint64_t ptr_loaded_dll = 0x00;
	kernel32_base = getDllBase(H_LIB_KERNEL32);

	if (kernel32_base == 0x00)
		return 0;

	fptr_loadLibary = parseHdrForPtr(kernel32_base, H_API_LOADLIBRARYA);

	if (fptr_loadLibary == 0x00)
		return 0;

	/* ################################### DLL SECTION ############################### */
	#ifdef H_LIB_APPHELP
		if (dll_hash == H_LIB_APPHELP) {
			char dll_name[] = { 'a', 'p', 'p', 'h', 'e', 'l', 'p', '.', 'd', 'l', 'l', 0x00 };
			ptr_loaded_dll = (uint64_t)((tLoadLibraryA)fptr_loadLibary)(dll_name);
		}
	#endif
	#ifdef H_LIB_MAGNIFICATION
		if (dll_hash == H_LIB_MAGNIFICATION) {
			char dll_name[] = { 'm', 'a', 'g', 'n', 'i', 'f', 'i', 'c', 'a', 't', 'i', 'o', 'n', '.', 'd', 'l', 'l', 0x00 };
			ptr_loaded_dll = (uint64_t)((tLoadLibraryA)fptr_loadLibary)(dll_name);
		}
	#endif
	#ifdef H_LIB_ADVAPI32
		if (dll_hash == H_LIB_ADVAPI32) {
			char dll_name[] = { 'a', 'd', 'v', 'a', 'p', 'i', '3', '2', '.', 'd', 'l', 'l', 0x00 };
			ptr_loaded_dll = (uint64_t)((tLoadLibraryA)fptr_loadLibary)(dll_name);
		}
	#endif
	#ifdef H_LIB_AUTHZ
		if (dll_hash == H_LIB_AUTHZ) {
			char dll_name[] = { 'a', 'u', 't', 'h', 'z', '.', 'd', 'l', 'l', 0x00 };
			ptr_loaded_dll = (uint64_t)((tLoadLibraryA)fptr_loadLibary)(dll_name);
		}
	#endif
	#ifdef H_LIB_COMCTL32
		if (dll_hash == H_LIB_COMCTL32) {
			char dll_name[] = { 'c', 'o', 'm', 'c', 't', 'l', '3', '2', '.', 'd', 'l', 'l', 0x00 };
			ptr_loaded_dll = (uint64_t)((tLoadLibraryA)fptr_loadLibary)(dll_name);
		}
	#endif
	#ifdef H_LIB_CRYPT32
		if (dll_hash == H_LIB_CRYPT32) {
			char dll_name[] = { 'c', 'r', 'y', 'p', 't', '3', '2', '.', 'd', 'l', 'l', 0x00 };
			ptr_loaded_dll = (uint64_t)((tLoadLibraryA)fptr_loadLibary)(dll_name);
		}
	#endif
	#ifdef H_LIB_CRYPTUI
		if (dll_hash == H_LIB_CRYPTUI) {
			char dll_name[] = { 'c', 'r', 'y', 'p', 't', 'u', 'i', '.', 'd', 'l', 'l', 0x00 };
			ptr_loaded_dll = (uint64_t)((tLoadLibraryA)fptr_loadLibary)(dll_name);
		}
	#endif
	#ifdef H_LIB_CSRSRV
		if (dll_hash == H_LIB_CSRSRV) {
			char dll_name[] = { 'c', 's', 'r', 's', 'r', 'v', '.', 'd', 'l', 'l', 0x00 };
			ptr_loaded_dll = (uint64_t)((tLoadLibraryA)fptr_loadLibary)(dll_name);
		}
	#endif
	#ifdef H_LIB_DNSAPI
		if (dll_hash == H_LIB_DNSAPI) {
			char dll_name[] = { 'd', 'n', 's', 'a', 'p', 'i', '.', 'd', 'l', 'l', 0x00 };
			ptr_loaded_dll = (uint64_t)((tLoadLibraryA)fptr_loadLibary)(dll_name);
		}
	#endif
	#ifdef H_LIB_GDI32
		if (dll_hash == H_LIB_GDI32) {
			char dll_name[] = { 'g', 'd', 'i', '3', '2', '.', 'd', 'l', 'l', 0x00 };
			ptr_loaded_dll = (uint64_t)((tLoadLibraryA)fptr_loadLibary)(dll_name);
		}
	#endif
	#ifdef H_LIB_GUSER32
		if (dll_hash == H_LIB_GUSER32) {
			char dll_name[] = { 'g', 'u', 's', 'e', 'r', '3', '2', '.', 'd', 'l', 'l', 0x00 };
			ptr_loaded_dll = (uint64_t)((tLoadLibraryA)fptr_loadLibary)(dll_name);
		}
	#endif
	#ifdef H_LIB_IPHLPAPI
		if (dll_hash == H_LIB_IPHLPAPI) {
			char dll_name[] = { 'i', 'p', 'h', 'l', 'p', 'a', 'p', 'i', '.', 'd', 'l', 'l', 0x00 };
			ptr_loaded_dll = (uint64_t)((tLoadLibraryA)fptr_loadLibary)(dll_name);
		}
	#endif
	#ifdef H_LIB_KERNEL32
		if (dll_hash == H_LIB_KERNEL32) {
			char dll_name[] = { 'k', 'e', 'r', 'n', 'e', 'l', '3', '2', '.', 'd', 'l', 'l', 0x00 };
			ptr_loaded_dll = (uint64_t)((tLoadLibraryA)fptr_loadLibary)(dll_name);
		}
	#endif
	#ifdef H_LIB_LZ32
		if (dll_hash == H_LIB_LZ32) {
			char dll_name[] = { 'l', 'z', '3', '2', '.', 'd', 'l', 'l', 0x00 };
			ptr_loaded_dll = (uint64_t)((tLoadLibraryA)fptr_loadLibary)(dll_name);
		}
	#endif
	#ifdef H_LIB_MSCOREE
		if (dll_hash == H_LIB_MSCOREE) {
			char dll_name[] = { 'm', 's', 'c', 'o', 'r', 'e', 'e', '.', 'd', 'l', 'l', 0x00 };
			ptr_loaded_dll = (uint64_t)((tLoadLibraryA)fptr_loadLibary)(dll_name);
		}
	#endif
	#ifdef H_LIB_MSWSOCK
		if (dll_hash == H_LIB_MSWSOCK) {
			char dll_name[] = { 'm', 's', 'w', 's', 'o', 'c', 'k', '.', 'd', 'l', 'l', 0x00 };
			ptr_loaded_dll = (uint64_t)((tLoadLibraryA)fptr_loadLibary)(dll_name);
		}
	#endif
	#ifdef H_LIB_NETAPI32
		if (dll_hash == H_LIB_NETAPI32) {
			char dll_name[] = { 'n', 'e', 't', 'a', 'p', 'i', '3', '2', '.', 'd', 'l', 'l', 0x00 };
			ptr_loaded_dll = (uint64_t)((tLoadLibraryA)fptr_loadLibary)(dll_name);
		}
	#endif
	#ifdef H_LIB_NORMALIZ
		if (dll_hash == H_LIB_NORMALIZ) {
			char dll_name[] = { 'n', 'o', 'r', 'm', 'a', 'l', 'i', 'z', '.', 'd', 'l', 'l', 0x00 };
			ptr_loaded_dll = (uint64_t)((tLoadLibraryA)fptr_loadLibary)(dll_name);
		}
	#endif
	#ifdef H_LIB_NTDLL
		if (dll_hash == H_LIB_NTDLL) {
			char dll_name[] = { 'n', 't', 'd', 'l', 'l', '.', 'd', 'l', 'l', 0x00 };
			ptr_loaded_dll = (uint64_t)((tLoadLibraryA)fptr_loadLibary)(dll_name);
		}
	#endif
	#ifdef H_LIB_OLE32
		if (dll_hash == H_LIB_OLE32) {
			char dll_name[] = { 'o', 'l', 'e', '3', '2', '.', 'd', 'l', 'l', 0x00 };
			ptr_loaded_dll = (uint64_t)((tLoadLibraryA)fptr_loadLibary)(dll_name);
		}
	#endif
	#ifdef H_LIB_OLEAUT32
		if (dll_hash == H_LIB_OLEAUT32) {
			char dll_name[] = { 'o', 'l', 'e', 'a', 'u', 't', '3', '2', '.', 'd', 'l', 'l', 0x00 };
			ptr_loaded_dll = (uint64_t)((tLoadLibraryA)fptr_loadLibary)(dll_name);
		}
	#endif
	#ifdef H_LIB_POWRPROF
		if (dll_hash == H_LIB_POWRPROF) {
			char dll_name[] = { 'p', 'o', 'w', 'r', 'p', 'r', 'o', 'f', '.', 'd', 'l', 'l', 0x00 };
			ptr_loaded_dll = (uint64_t)((tLoadLibraryA)fptr_loadLibary)(dll_name);
		}
	#endif
	#ifdef H_LIB_PROPSYS
		if (dll_hash == H_LIB_PROPSYS) {
			char dll_name[] = { 'p', 'r', 'o', 'p', 's', 'y', 's', '.', 'd', 'l', 'l', 0x00 };
			ptr_loaded_dll = (uint64_t)((tLoadLibraryA)fptr_loadLibary)(dll_name);
		}
	#endif
	#ifdef H_LIB_PSAPI
		if (dll_hash == H_LIB_PSAPI) {
			char dll_name[] = { 'p', 's', 'a', 'p', 'i', '.', 'd', 'l', 'l', 0x00 };
			ptr_loaded_dll = (uint64_t)((tLoadLibraryA)fptr_loadLibary)(dll_name);
		}
	#endif
	#ifdef H_LIB_RPCRT4
		if (dll_hash == H_LIB_RPCRT4) {
			char dll_name[] = { 'r', 'p', 'c', 'r', 't', '4', '.', 'd', 'l', 'l', 0x00 };
			ptr_loaded_dll = (uint64_t)((tLoadLibraryA)fptr_loadLibary)(dll_name);
		}
	#endif
	#ifdef H_LIB_SECUR32
		if (dll_hash == H_LIB_SECUR32) {
			char dll_name[] = { 's', 'e', 'c', 'u', 'r', '3', '2', '.', 'd', 'l', 'l', 0x00 };
			ptr_loaded_dll = (uint64_t)((tLoadLibraryA)fptr_loadLibary)(dll_name);
		}
	#endif
	#ifdef H_LIB_SHCORE
		if (dll_hash == H_LIB_SHCORE) {
			char dll_name[] = { 's', 'h', 'c', 'o', 'r', 'e', '.', 'd', 'l', 'l', 0x00 };
			ptr_loaded_dll = (uint64_t)((tLoadLibraryA)fptr_loadLibary)(dll_name);
		}
	#endif
	#ifdef H_LIB_SHELL32
		if (dll_hash == H_LIB_SHELL32) {
			char dll_name[] = { 's', 'h', 'e', 'l', 'l', '3', '2', '.', 'd', 'l', 'l', 0x00 };
			ptr_loaded_dll = (uint64_t)((tLoadLibraryA)fptr_loadLibary)(dll_name);
		}
	#endif
	#ifdef H_LIB_SHLWAPI
		if (dll_hash == H_LIB_SHLWAPI) {
			char dll_name[] = { 's', 'h', 'l', 'w', 'a', 'p', 'i', '.', 'd', 'l', 'l', 0x00 };
			ptr_loaded_dll = (uint64_t)((tLoadLibraryA)fptr_loadLibary)(dll_name);
		}
	#endif
	#ifdef H_LIB_UCRTBASE
		if (dll_hash == H_LIB_UCRTBASE) {
			char dll_name[] = { 'u', 'c', 'r', 't', 'b', 'a', 's', 'e', '.', 'd', 'l', 'l', 0x00 };
			ptr_loaded_dll = (uint64_t)((tLoadLibraryA)fptr_loadLibary)(dll_name);
		}
	#endif
	#ifdef H_LIB_URLMON
		if (dll_hash == H_LIB_URLMON) {
			char dll_name[] = { 'u', 'r', 'l', 'm', 'o', 'n', '.', 'd', 'l', 'l', 0x00 };
			ptr_loaded_dll = (uint64_t)((tLoadLibraryA)fptr_loadLibary)(dll_name);
		}
	#endif
	#ifdef H_LIB_USER32
		if (dll_hash == H_LIB_USER32) {
			char dll_name[] = { 'u', 's', 'e', 'r', '3', '2', '.', 'd', 'l', 'l', 0x00 };
			ptr_loaded_dll = (uint64_t)((tLoadLibraryA)fptr_loadLibary)(dll_name);
		}
	#endif
	#ifdef H_LIB_USERENV
		if (dll_hash == H_LIB_USERENV) {
			char dll_name[] = { 'u', 's', 'e', 'r', 'e', 'n', 'v', '.', 'd', 'l', 'l', 0x00 };
			ptr_loaded_dll = (uint64_t)((tLoadLibraryA)fptr_loadLibary)(dll_name);
		}
	#endif
	#ifdef H_LIB_WINHTTP
		if (dll_hash == H_LIB_WINHTTP) {
			char dll_name[] = { 'w', 'i', 'n', 'h', 't', 't', 'p', '.', 'd', 'l', 'l', 0x00 };
			ptr_loaded_dll = (uint64_t)((tLoadLibraryA)fptr_loadLibary)(dll_name);
		}
	#endif
	#ifdef H_LIB_WININET
		if (dll_hash == H_LIB_WININET) {
			char dll_name[] = { 'w', 'i', 'n', 'i', 'n', 'e', 't', '.', 'd', 'l', 'l', 0x00 };
			ptr_loaded_dll = (uint64_t)((tLoadLibraryA)fptr_loadLibary)(dll_name);
		}
	#endif
	#ifdef H_LIB_WINTRUST
		if (dll_hash == H_LIB_WINTRUST) {
			char dll_name[] = { 'w', 'i', 'n', 't', 'r', 'u', 's', 't', '.', 'd', 'l', 'l', 0x00 };
			ptr_loaded_dll = (uint64_t)((tLoadLibraryA)fptr_loadLibary)(dll_name);
		}
	#endif
	#ifdef H_LIB_WS2_32
		if (dll_hash == H_LIB_WS2_32) {
			char dll_name[] = { 'w', 's', '2', '_', '3', '2', '.', 'd', 'l', 'l', 0x00 };
			ptr_loaded_dll = (uint64_t)((tLoadLibraryA)fptr_loadLibary)(dll_name);
		}
	#endif
	/* ############################################################################### */

	return ptr_loaded_dll;
}
static uint64_t parseHdrForPtr(uint64_t dll_base, unsigned long function_hash) {
	PIMAGE_NT_HEADERS nt_hdrs = NULL;
	PIMAGE_DATA_DIRECTORY data_dir= NULL;
	PIMAGE_EXPORT_DIRECTORY export_dir= NULL;
	uint32_t* ptr_exportadrtable = 0x00;
	uint32_t* ptr_namepointertable = 0x00;
	uint16_t* ptr_ordinaltable = 0x00;
	uint32_t idx_functions = 0x00;
	unsigned char* ptr_function_name = NULL;
	nt_hdrs = (PIMAGE_NT_HEADERS)(dll_base + (uint64_t)((PIMAGE_DOS_HEADER)(size_t)dll_base)->e_lfanew);
	data_dir = (PIMAGE_DATA_DIRECTORY)&nt_hdrs->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];
	export_dir = (PIMAGE_EXPORT_DIRECTORY)(dll_base + (uint64_t)data_dir->VirtualAddress);
	ptr_exportadrtable = (uint32_t*)(dll_base + (uint64_t)export_dir->AddressOfFunctions);
	ptr_namepointertable = (uint32_t*)(dll_base + (uint64_t)export_dir->AddressOfNames);
	ptr_ordinaltable = (uint16_t*)(dll_base + (uint64_t)export_dir->AddressOfNameOrdinals);
	for(idx_functions = 0; idx_functions < export_dir->NumberOfNames; idx_functions++){
		ptr_function_name = (unsigned char*)dll_base + (ptr_namepointertable[idx_functions]);
		if (djb2(ptr_function_name) == function_hash) {
			WORD nameord = ptr_ordinaltable[idx_functions];
			DWORD rva = ptr_exportadrtable[nameord];
			return dll_base + rva;
		}
	}
	return 0;
}
static uint64_t getDllBase(unsigned long dll_hash) {
	_PPEB ptr_peb = NULL;
	PPEB_LDR_DATA ptr_ldr_data = NULL;
	PLDR_DATA_TABLE_ENTRY ptr_module_entry = NULL, ptr_start_module = NULL;
	PUNICODE_STR dll_name = NULL;
	#ifndef _WIN64
	    ptr_peb = (_PEB*)__readfsdword(0x30);
	#else
	    ptr_peb = (_PEB*)__readgsqword(0x60);
	#endif
	ptr_ldr_data = ptr_peb->pLdr;
	ptr_module_entry = ptr_start_module = (PLDR_DATA_TABLE_ENTRY)ptr_ldr_data->InMemoryOrderModuleList.Flink;
	do{
		dll_name = &ptr_module_entry->BaseDllName;
		if (dll_name->pBuffer == NULL)
			return 0;
		if (unicode_djb2(toLower(dll_name->pBuffer)) == dll_hash)
			return (uint64_t)ptr_module_entry->DllBase;
		ptr_module_entry = (PLDR_DATA_TABLE_ENTRY)ptr_module_entry->InMemoryOrderModuleList.Flink;
	} while (ptr_module_entry != ptr_start_module);
	return 0;
}
static unsigned long djb2(unsigned char* str) {
	unsigned long hash = SALT;
	int c;
	while ((c = *str++))
		hash = ((hash << 5) + hash) + c;
	return hash;
}
unsigned long  unicode_djb2(const wchar_t* str) {
	unsigned long hash = SALT;
	DWORD val;
	while (*str != 0) {
		val = (DWORD)*str++;
		hash = ((hash << 5) + hash) + val;
	}
	return hash;

}
static WCHAR* toLower(WCHAR *str) {
	WCHAR* start = str;
	while (*str) {
		if (*str <= L'Z' && *str >= 'A') {
			*str += 32;
		}
		str += 1;
	}
	return start;
}
