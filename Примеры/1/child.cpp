#include <stdio.h>
#include <conio.h>
#include <windows.h>

int main()
{
	STARTUPINFO df;
	SYSTEMTIME lt;

	while (kbhit() == 0)  
	{
		GetLocalTime(&lt);
		printf("%02d:%02d:%02d c\r", lt.wHour, lt.wMinute, lt.wSecond);
		Sleep(50);
	}
	return 0;
}