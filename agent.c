#include <windows.h>
#include <stdio.h>
#include <time.h>
extern __declspec(dllimport) char *normalMsg();

int main(int argc,char **argv) {
	DWORD WINAPI GetCurrentProcessId(void);
	DWORD myPID = GetCurrentProcessId();
	char date[64];
	time_t t;
	while (1){
		t = time(NULL);
		strftime(date, sizeof(date), "%Y/%m/%d %a %H:%M:%S", localtime(&t));
		printf("[%s](PID: %lu) %s\n", date, myPID, normalMsg());
		Sleep(2000);
	}
}
