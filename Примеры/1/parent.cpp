#include <stdio.h>
#include <conio.h>
#include <windows.h>

int main()
{

	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	if (!CreateProcess(NULL,					// No module name (use command line)
		TEXT("child.exe"),	// Command line
		NULL,					// Process handle not inheritable
		NULL,					// Thread handle not inheritable
		FALSE,					// Set handle inheritance to FALSE
		CREATE_NEW_CONSOLE,		// lol
		NULL,					// Use parent's environment block
		NULL,					// Use parent's starting directory 
		&si,					// Pointer to STARTUPINFO structure
		&pi)					// Pointer to PROCESS_INFORMATION structure
		)
	{
		printf("CreateProcess failed (%d)\n", GetLastError());
		return -1;
	}

	SYSTEMTIME lt;

	while (WaitForSingleObject(pi.hProcess, 50))
	{
		GetLocalTime(&lt);
		printf("%02d:%02d:%02d p\r", lt.wHour, lt.wMinute, lt.wSecond);
	}

	CloseHandle(pi.hProcess);
	return 0;
}}