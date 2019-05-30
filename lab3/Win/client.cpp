#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <time.h>
#include <string.h>

using namespace std;

int main(int argc, char*argv[])
{
	char semID[20];
	sprintf(semID, argv[1]);
	HANDLE closeProcess = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, semID);
	HANDLE write = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, "write");
	HANDLE print = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, "print");
		
	char message[20];
	sprintf(message, "Client %d", atoi(argv[1]));
	char emptyRow[40];
	memset(emptyRow, '\0', 20);
	//const char *waitMsg = "Waiting...";
	HANDLE hFileMapping = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, "map");
	LPVOID buffer = MapViewOfFile(hFileMapping, FILE_MAP_ALL_ACCESS, 0, 0, 20);
	
	
	while (true)
	{
		WaitForSingleObject(write, INFINITE);

		if(WaitForSingleObject(closeProcess, 0) == WAIT_OBJECT_0)
		{			
			ReleaseSemaphore(write, 1, NULL);
			break;
		}
		CopyMemory((PVOID)buffer, message, 20);
		ReleaseSemaphore(print, 1, NULL);
	}

	CloseHandle(write);
	CloseHandle(print);
	UnmapViewOfFile(buffer);
	CloseHandle(hFileMapping);
	CloseHandle(closeProcess);

	return 0;
}