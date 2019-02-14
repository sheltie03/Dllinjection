
// ANTI DLL INJECTION by _FIL73R_
#include <windows.h>
#include <stdio.h>
#include <time.h>
extern __declspec(dllimport) char *normalMsg();

BOOLEAN BlockAPI(HANDLE,CHAR *,CHAR *);
void AntiInject();


int main(int argc,char **argv) {
	CreateThread(0,0, (LPTHREAD_START_ROUTINE)AntiInject, 0, 0, 0);
	char date[64];
	time_t t;
	while (1){
		t = time(NULL);
		strftime(date, sizeof(date), "%Y/%m/%d %a %H:%M:%S", localtime(&t));
		printf("[%s] %s\n", date, normalMsg());
		Sleep(2000);
	}
}

void AntiInject()
{
   HANDLE hProc = GetCurrentProcess();
   while (TRUE) {
      BlockAPI(hProc, "NTDLL.DLL", "LdrLoadDll");
      Sleep(100);
   }
}

BOOLEAN BlockAPI(HANDLE hProcess, CHAR *libName, CHAR *apiName)
{
   CHAR pRet[] = { 0xC3 };
   HINSTANCE hLib = NULL;
   VOID *pAddr = NULL;
   BOOL bRet = FALSE;
   DWORD dwRet = 0;

   hLib = LoadLibrary(libName);
   if (hLib) {
       pAddr = (VOID*)GetProcAddress(hLib, apiName);
       if (pAddr) {
           if (WriteProcessMemory(hProcess, (LPVOID)pAddr, (LPCVOID)pRet, sizeof(pRet), &dwRet)) {
              if (dwRet) {
                 bRet = TRUE;
              }
           }
       }
       FreeLibrary(hLib);
   }
   return bRet;
}
