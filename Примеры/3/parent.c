#define _CRT_SECURE_NO_WARNINGS

#ifdef __linux__
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <ncurses.h>
#include <time.h>
#elif _WIN32 | _WIN64
#include <conio.h>
#include <windows.h>
#endif

#include <stdio.h>
#include <stdlib.h>

int main()
{
#ifdef __linux__
#elif _WIN32 | _WIN64
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );
#endif
    char str[255];
    scanf("%s", &str);
#ifdef __linux__
    int pid = fork();
    int st;

    switch(pid)
    {
        case -1: {
            printw("Error.");
            break;
        }

        case 0: {
            if(execlp("./child", str, NULL)==-1)
                printw("Error.");
            break;
        }

        default: {
            while(1)
            {
                if(waitpid(pid,&st,WNOHANG)>0)
                    break;

                napms(50);
            }
        }
    }
#elif _WIN32 | _WIN64
    char str1[255];
    sprintf(str1, "child.exe %s", str);
    if( !CreateProcess( NULL,            // No module name (use command line)
           str1,    // Command line
           NULL,                 // Process handle not inheritable
           NULL,                 // Thread handle not inheritable
           FALSE,                    // Set handle inheritance to FALSE
           CREATE_NEW_CONSOLE,       // lol
           NULL,                 // Use parent's environment block
           NULL,                 // Use parent's starting directory
           &si,                  // Pointer to STARTUPINFO structure
           &pi )                 // Pointer to PROCESS_INFORMATION structure
    )
    {
        printf( "CreateProcess failed (%d)\n", GetLastError());
        return -1;
    }

    while (WaitForSingleObject(pi.hProcess, 50)){}

    CloseHandle(pi.hProcess);
#endif
    return 0;
}
