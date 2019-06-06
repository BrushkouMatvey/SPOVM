#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string>
#include <cstring>
#include <vector>
#include <termios.h>
#include <semaphore.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <fcntl.h>
using namespace std;

const char* SEMAPHORE_NAME = "/semaphore_print";
const char* SEMAPHORE_NAME_2 = "/semaphore_write";
const char* SEMAPHORE_NAME_3 = "/close_controller";
const int MAX_SIZE = 30;

pid_t procPID;
int pipeRead;
int pipeWrite;

void runController();

int main()
{
	int pipeId[2];
	if(pipe(pipeId))
	{
      	printf("\nServer error: Dont open pipe!\n");
		return 0;
	}
	pipeRead = pipeId[0];
	pipeWrite = pipeId[1];

	sem_t* printSem;
	if ((printSem = sem_open(SEMAPHORE_NAME, O_CREAT, 0777, 0)) == SEM_FAILED) 
    {
    	cout << "error: creating print semaphore error" << endl;
    	return 1;
    }

    sem_t* writeSem;
	if ((writeSem = sem_open(SEMAPHORE_NAME_2, O_CREAT, 0777, 0)) == SEM_FAILED) 
    {
    	cout << "error: creating write semaphore error" << endl;
    	return 1;
    }

    sem_t* closeSem;
	if ((closeSem = sem_open(SEMAPHORE_NAME_3, O_CREAT, 0777, 0)) == SEM_FAILED) 
    {
    	cout << "error: creating close semaphore error" << endl;
    	return 1;
    }

    runController();

    char message[MAX_SIZE];
    int value;
    int closeSemCheck1;
    int closeSemCheck2;

    sem_getvalue(closeSem, &closeSemCheck1);
    if(closeSemCheck1 > 0)
    {
    	sem_wait(closeSem);
    }

    while (true) 
    {
		sem_wait(printSem);
		memset(message, '\0', MAX_SIZE);
		read(pipeRead, &message, MAX_SIZE);
		for (int i = 0; i < strlen(message); i++) 
		{
			cout << message[i] << endl;
			usleep(150000);
		}
		cout << endl;
		sem_post(writeSem);

		sem_getvalue(closeSem, &closeSemCheck2);
		if (closeSemCheck2 > 0) 
		{
			close(pipeRead);
			close(pipeWrite);
			break;
		}
	}
	return 0;
}

void runController()
{
	pid_t pid = fork();
	procPID = pid;

	if (pid < 0) 
	{
		std::cout << "Controller wasn't created !!!" << std::endl;
	}

	if (pid == 0) 
	{
		char pipeData[10];
		sprintf(pipeData, "%d", pipeWrite);
		if (execlp("./controller", pipeData, NULL) == -1) 
		{
			std::cout << "error: creating controller error" << std::endl;	
		}	
	}
}
