#include "ministdlib.h"

int Mmain(API Api, LPCWSTR PasswdFromExecute){

	char_on_stack(msg, "Hello world!");
	Api._MessageBoxA(NULL, msg, msg, MB_OK);
	return 0;
}
