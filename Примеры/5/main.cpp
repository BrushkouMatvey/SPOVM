#include <windows.h>
#include <iostream>
#include <conio.h>
#include <vector>
#include <ctime>

using namespace std;

void PrintLastError()
{
	printf( "Create Process failed (%d)\n", GetLastError());
	getch();
	return ;
}

void printString(int number)
{
	char EventID[10];
	sprintf(EventID," %d", number);
	HANDLE print = OpenEvent(EVENT_ALL_ACCESS, FALSE, EventID);
	
	sprintf(EventID," %dc", number);
	HANDLE close = OpenEvent(EVENT_ALL_ACCESS, FALSE, EventID);

	char buf[255];
	long int PK = number;
	sprintf(buf,"PK: %d. Process %d\n\0", PK, number);

	while(true)
	{
		if(WaitForSingleObject(print, 1) == WAIT_OBJECT_0) 
		{ 	
			for(int i = 0; i < strlen(buf); i++)        
			{
				if(WaitForSingleObject(close, 1) == WAIT_OBJECT_0)
				{
					CloseHandle(close);
					CloseHandle(print); 
					return;
				}			
				printf("%c",buf[i]);
				Sleep(50);			}
			ResetEvent(print);
		}

		if(WaitForSingleObject(close, 1) == WAIT_OBJECT_0) 
		{
			CloseHandle(close); 
			CloseHandle(print); 
			return;
		}
	}
	return;
}

PROCESS_INFORMATION CreateNewProcess(char* path, char *cmdline)
{
	STARTUPINFO si;
	ZeroMemory (&si, sizeof(STARTUPINFO));
	si.cb = sizeof (STARTUPINFO);

	PROCESS_INFORMATION pi;
	if(!CreateProcess(path, cmdline, NULL, NULL, FALSE, NULL, NULL, NULL, &si, &pi))
		PrintLastError();
	return pi;
}

void Work_with_Process(char *path)
{																	
	HANDLE close; // дескриптор события
	HANDLE print;
	char EventID[10];
	bool flag = false;

	PROCESS_INFORMATION procInf;
	int id = rand() % 100;
	sprintf(EventID," %d", id);
	print = CreateEvent(NULL ,TRUE, FALSE, EventID); 
	sprintf(EventID," %dc", id);
	close = CreateEvent(NULL ,FALSE, FALSE, EventID); 

	procInf = CreateNewProcess(path, EventID); 
	char key = 0;
	while(true)
	{
		if (kbhit())
		{
			key = getch();
			if (key == 'p')
			{
				if (flag == false)
				{
					SetEvent(print);
					flag = true;
				}
			}
			if (key == 'q')
			{
				SetEvent(close);
				WaitForSingleObject(procInf.hProcess, INFINITE);
				CloseHandle(close);
				CloseHandle(print); 
				CloseHandle(procInf.hProcess); 
				CloseHandle(procInf.hThread);
				system("pause");
				return;
			}
		}
		if (WaitForSingleObject(print, 1) == WAIT_TIMEOUT)
		{
			flag = false;
		}
	}	
}

int main(int argc, char* argv[])
{
    (argc == 2)? printString(atoi(argv[1])) : Work_with_Process(argv[0]); 
	return 0;
}