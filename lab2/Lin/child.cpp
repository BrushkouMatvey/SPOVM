#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <ncurses.h>

#include <stdlib.h>
#include <iostream>
#include <string.h>

using namespace std;

struct sigaction printSignal, closeSignal;
bool Print = false;
bool Close = false;
char parentPID[256];


void setPrint(int sign)
{
    Print = true;
}

void setClose(int sign)
{
    Close = true;
}

void initSignalHandlers()
{
    printSignal.sa_handler = setPrint;
    sigaction(SIGUSR1, &printSignal, NULL);

    closeSignal.sa_handler = setClose;
    sigaction(SIGUSR2, &closeSignal, NULL);
}


int main(int argc, char* argv[])
{

    sprintf(parentPID, "%d", atoi(argv[1]));
    initSignalHandlers();

    char buf[256];
    sprintf(buf, " Child%d ", atoi(argv[0]));


    while (true)
    {
        usleep(10000);
        if (Print)
        {
            Print = false;
            for (int i = 0; i < strlen(buf); i++)
            {
                if (Close)
                {
                    return 0;
                }
                cout << buf[i] << endl;
                napms(150);
            }
            kill(atoi(parentPID), SIGUSR1);
        }
        if (Close)
        {
            break;
        }
    }


    return 0;
}