#define _CRT_SECURE_NO_WARNINGS
#define MAX_SIZE 30

#include <conio.h>
#include <cstdlib>
#include <windows.h>
#include <string>
#include <vector>
#include <iostream>

using namespace std;

HANDLE lock_Unlock_Print;
HANDLE write;
HANDLE closeController;
bool mapFull = false;
vector<HANDLE> thrInfo;
vector<HANDLE> closeMutexs;
vector<HANDLE> printMutexs;
char procID[10];
int i = 0;

void showMenu();
void addOneThread();
void removeOneThread();
DWORD WINAPI threadFunction(void* arg);


int main()
{

	showMenu();


	lock_Unlock_Print = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, "lock_Unlock_Print");
	write = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, "write");
	closeController = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, "close");

	ReleaseSemaphore(write, 1, NULL);


	char key;
	while (true)
	{
		if (!thrInfo.empty() && WaitForSingleObject(write, 1) == WAIT_OBJECT_0)
		{
			if (i >= thrInfo.size() - 1)
				i = -1;
			i++;
			mapFull = false;
			ReleaseMutex(printMutexs[i]);
			while (!mapFull);
			WaitForSingleObject(printMutexs[i], INFINITE);
			ReleaseSemaphore(lock_Unlock_Print, 1, NULL);
		}
		if (_kbhit())
		{
			key = _getch();
			if (key == '+')
			{
				addOneThread();
			}
			else if (key == '-' && !thrInfo.empty())
			{
				removeOneThread();
			}
			else if (key == 'q')
			{
				while (!thrInfo.empty())
				{
					removeOneThread();
				}
				break;
			}
		}
	}
	ReleaseSemaphore(closeController, 1, NULL);
}


void showMenu()
{
	cout << "Press '+' to create new thread;" << endl;
	cout << "Press '-' to delete thread;" << endl;
	cout << "Press 'q' to quit;" << endl << endl;
}
void addOneThread()
{
	int tmp = thrInfo.size() + 1;
	HANDLE descr = CreateThread(NULL,			// дескриптор защиты
		0,										// начальный размер стека 
		threadFunction,							//  функция потока
		NULL,									// параметр потока 
		0,										// опции создания 
		NULL);									// идентификатор потока
	if (!descr) 
	{
		printf("Create thread failed (%d)\n", GetLastError());
	}

	thrInfo.push_back(descr);

	sprintf(procID, " %dclose", thrInfo.size());
	closeMutexs.push_back(CreateMutex(NULL, TRUE, procID));

	sprintf(procID, " %dprint", thrInfo.size());
	printMutexs.push_back(CreateMutex(NULL, TRUE, procID));
}
void removeOneThread()
{
	ReleaseMutex(closeMutexs.back());
	WaitForSingleObject(thrInfo.back(), INFINITE);

	CloseHandle(closeMutexs.back());
	CloseHandle(printMutexs.back());
	CloseHandle(thrInfo.back());

	closeMutexs.pop_back();
	printMutexs.pop_back();
	thrInfo.pop_back();
}

DWORD WINAPI threadFunction(void* arg)
{
	char procID[10];
	int id = thrInfo.size();

	sprintf(procID, " %dclose", id);
	HANDLE close_mutex = OpenMutex(SYNCHRONIZE, FALSE, procID);
	sprintf(procID, " %dprint", id);
	HANDLE print_mutex = OpenMutex(SYNCHRONIZE, FALSE, procID);
	HANDLE hFileMapping = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, "map");
	LPVOID buffer = MapViewOfFile(hFileMapping, FILE_MAP_ALL_ACCESS, 0, 0, MAX_SIZE);;

	char message[MAX_SIZE];
	sprintf(message, "Thread %d", id);

	while (true)
	{
		if (WaitForSingleObject(print_mutex, 1) == WAIT_OBJECT_0)
		{
			CopyMemory((PVOID)buffer, message, MAX_SIZE);
			ReleaseMutex(print_mutex);
			mapFull = true;
		}
		if (WaitForSingleObject(close_mutex, 1) == WAIT_OBJECT_0)
		{
			CloseHandle(close_mutex);
			CloseHandle(print_mutex);
			UnmapViewOfFile(buffer);
			CloseHandle(hFileMapping);
			return 0;
		}
	}

	return 0;
}