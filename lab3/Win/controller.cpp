#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <iostream>
#include <conio.h>
#include <ctime>
#include <vector>
using namespace std;


vector<HANDLE> closeSem;
vector<PROCESS_INFORMATION> procInfo;
HANDLE closeController = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, "closeController");

void showMenu();
PROCESS_INFORMATION createOneProcess();
void removeOneProcess();

int main(int argc, char *argv[])
{		

	char key;
	showMenu();

	while (true)
	{
		if (_kbhit())
		{
			key = _getch();
			if (key == '+')
			{
				procInfo.push_back(createOneProcess());
			}
			else if (key == '-')
			{
				if (!procInfo.empty())
				{
					removeOneProcess();
				}
			}
			else if (key == 'q')
			{
				while (!procInfo.empty())
				{
					removeOneProcess();
				}
				ReleaseSemaphore(closeController, 1, NULL);
				CloseHandle(closeController);
			}
		}
	}
	return 0;
}

void showMenu()
{
	cout << "Press '+' to create new process" << endl;
	cout << "Press '-' to remove process" << endl;
	cout << "Press 'q' to quit" << endl;
}

PROCESS_INFORMATION createOneProcess()
{
	char semID[20];
	sprintf(semID, "%d", procInfo.size() + 1);
	closeSem.push_back(CreateSemaphore(NULL, 0, 1, semID));

	char clientCMD[20];
	sprintf(clientCMD, "Client.exe %s", semID);
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));
	
	if (!CreateProcess(NULL, clientCMD, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) 
	{
		cout << "ERROR." << endl;
	}
	return pi;
}

void removeOneProcess()
{
	ReleaseSemaphore(closeSem.back(), 1, NULL);
	CloseHandle(closeSem.back());
	closeSem.pop_back();
	CloseHandle(procInfo.back().hProcess);
	CloseHandle(procInfo.back().hThread);
	procInfo.pop_back();
}