#include <windows.h>
__declspec(dllexport) char *normalMsg(){
	return "operating normally(200) : OK";
}

BOOL WINAPI DllMain(HINSTANCE h,DWORD reasin,PVOID imp){
	return TRUE;
}
