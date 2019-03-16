#ifdef __linux__
#include <sys/types.h>
#include <unistd.h>
#include <ncurses.h>
#include <time.h>

#define _CRT_SECURE_NO_WARNINGS
#elif _WIN32 | _WIN64
#include <conio.h>
#include <windows.h>
#endif

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    char c = 0;
    printf("%s - child\n", argv[0]);

#ifdef _WIN32 | _WIN64
    system("pause");
#elif __linux__
    napms(20000000);
    getch();
#endif

    return 0;
}