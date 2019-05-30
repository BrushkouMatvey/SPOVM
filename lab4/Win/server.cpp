#define _CRT_SECURE_NO_WARNINGS
#define MAX_SIZE 30
#include <conio.h>
#include <windows.h>
#include <iostream>
using namespace std;

PROCESS_INFORMATION runController();

int main()
{
	HANDLE lock_Unlock_Print = CreateSemaphore(NULL, 0, 1, "lock_Unlock_Print");
	HANDLE write = CreateSemaphore(NULL, 0, 1, "write");
	HANDLE closeController = CreateSemaphore(NULL, 0, 1, "close");	

	HANDLE hFileMapping = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, MAX_SIZE, "map");
	LPVOID buffer = MapViewOfFile(hFileMapping, FILE_MAP_ALL_ACCESS, 0, 0, MAX_SIZE);

	char message[MAX_SIZE];
	bool flag = false;
	PROCESS_INFORMATION procInfo = runController();
	while (true)
	{
		if (WaitForSingleObject(lock_Unlock_Print, 1) == WAIT_OBJECT_0)
		{
			memset(message, '\0', MAX_SIZE);
			strncpy(message, (char*)buffer, MAX_SIZE);
			cout << endl;
			for (int j = 0; j < strlen(message); j++)
			{
				printf("%c", message[j]);
				Sleep(100);
			}

			if (WaitForSingleObject(closeController, 1) == WAIT_OBJECT_0) 
			{
				flag = true;
				break;
			}

			ReleaseSemaphore(write, 1, NULL);
		}

		if (flag)
		{
			break;
		}

		if (WaitForSingleObject(closeController, 1) == WAIT_OBJECT_0) 
		{
			break;
		}
	}

	WaitForSingleObject(procInfo.hProcess, INFINITE);

	CloseHandle(lock_Unlock_Print);
	CloseHandle(write);
	CloseHandle(closeController);
	CloseHandle(procInfo.hProcess);
	CloseHandle(procInfo.hThread);
	UnmapViewOfFile(buffer);
	CloseHandle(hFileMapping);
	cout << endl;
	system("pause");
}


PROCESS_INFORMATION runController()
{
	LPSTR cmdArgs = (LPSTR)"controller.exe";
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	if (!CreateProcess(NULL, cmdArgs, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) 
	{
		printf("Create controller failed (%d)\n", GetLastError());
	}
	return pi;
}

