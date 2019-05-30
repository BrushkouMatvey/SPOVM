#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <iostream>
#include <conio.h>
#include <ctime>

using namespace std;

void workWithProcess();
void PrintLastError();

int main(int argc, char *argv[])
{
	workWithProcess();
	return 0;
}

void workWithProcess()
{
	LPSTR controllerCMD = (LPSTR)"controller.exe";
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));
	HANDLE closeController = CreateSemaphore(NULL, 0, 1, "closeController");
	if (!CreateProcess(NULL, controllerCMD, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
	{
		PrintLastError();
	}
	HANDLE print = CreateSemaphore(NULL, 0, 1, "print");
	HANDLE write = CreateSemaphore(NULL, 0, 1, "write");
	HANDLE hFileMapping = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 20, "map");
	LPVOID buffer = MapViewOfFile(hFileMapping, FILE_MAP_ALL_ACCESS, 0, 0, 20);

	char message[20];
	while (true)
	{
		ReleaseSemaphore(write, 1, NULL);
		WaitForSingleObject(print, INFINITE);
		memset(message, '\0', 20);
		strncpy(message, (char*)buffer, 20);
		cout << endl;
		for (int i = 0; i < strlen(message); i++)
		{
			cout << message[i];
			Sleep(100);
		}
		if(WaitForSingleObject(closeController,0) == WAIT_OBJECT_0)
		{
			break;
		}
		
	}
	
	CloseHandle(closeController);
	CloseHandle(print);
	CloseHandle(write);
	UnmapViewOfFile(buffer);
	CloseHandle(hFileMapping);

	return;

}

void PrintLastError()
{
	printf("Create Process failed (%d)\n", GetLastError());
	return;
}