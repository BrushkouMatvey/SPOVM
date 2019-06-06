#include <iostream>
#include <vector>
#include <unistd.h>
#include <semaphore.h>
#include <termios.h>
#include <string>
#include <fcntl.h>
#include <signal.h>
#include <pthread.h>
#include <sys/ioctl.h>
#include <mutex>
using namespace std;

const char* SEMAPHORE_NAME = "/semaphore_print";
const char* SEMAPHORE_NAME_2 = "/semaphore_write";
const char* SEMAPHORE_NAME_3 = "/close_controller";
const int MAX_SIZE = 30;

vector<pthread_t> thrInfo;
vector<pthread_mutex_t> closeMutexes;

int thrId;
int pipeWrite;
int closeControllerFlag = 0;

pthread_mutex_t mutex1;
pthread_cond_t synchr;

int canWrite = 1;

void addOneThread(int pipeWrite);
void removeNumThread(int num);
void removeOneThread();
int getch();
bool kbhit();
int showMenu();
void *thread_func(void *arg);

int main(int argc, char* argv[])
{
	pipeWrite = atoi(argv[0]);

    sem_t* print;
    sem_t* write;
    sem_t* closeSem;
    if ((print = sem_open(SEMAPHORE_NAME, 0)) == SEM_FAILED) 
    {
    	cout << "error: creating print semaphore error111d" << endl;
    	return 1;
    }

    if ((write = sem_open(SEMAPHORE_NAME_2, 0)) == SEM_FAILED) 
    {
    	cout << "error: creating write semaphore error" << endl;
    	return 1;
    }

 	if ((closeSem = sem_open(SEMAPHORE_NAME_3, 0)) == SEM_FAILED ) 
 	{
 		cout << "error: creating close semaphore error111" << endl;
        return 1;
    }

    if(pthread_mutex_init(&mutex1, NULL) != 0) 
    {
    	cout << "Mutex create error" << endl;
	}
	if (pthread_cond_init(&synchr, NULL) != 0) 
	{
		cout << "Synchronize object create error" << endl;
	}

	thrId = 1;

	showMenu();

	char key;
	while(true)
	{
		if (!thrInfo.empty() && !canWrite) 
		{
			pthread_mutex_lock(&mutex1);
			sem_post(print);
			sem_wait(write);

			canWrite = 1;
			pthread_cond_signal(&synchr);
			pthread_mutex_unlock(&mutex1);
		}

		if (closeControllerFlag && thrInfo.empty()) 
		{
				close(pipeWrite);
				sem_post(closeSem);
				sem_post(print);
				return 0;
		}

		if (kbhit()) 
		{
			key = getch();
			if (key == '+')
			{
				addOneThread(pipeWrite);
				thrId++;
			}

			if (key == '-')
			{
				if (!thrInfo.empty()) 
				{
				removeOneThread();
				}
			}
		}

		if (key == 'q')
		{
			while(thrInfo.size() != 0)
			{
				removeOneThread();
			}
			closeControllerFlag = 1;
		}

		
	}
}



void addOneThread(int pipeWrite)
{
	pthread_t tid;
    thrInfo.push_back(tid);

    pthread_mutex_t buff_mutex;
    closeMutexes.push_back(buff_mutex);
    if(pthread_mutex_init(&closeMutexes.back(), NULL) != 0) 
    {
    	std::cout << "Mutex create error" << std::endl;
	}
	pthread_mutex_lock(&closeMutexes.back());

	if (pthread_create(&thrInfo.back(), NULL, thread_func, NULL) != 0) 
	{
		std::cout << "There is an error with creating thread" << std::endl;
	}

}

void removeOneThread()
{
	pthread_mutex_unlock(&closeMutexes.back());
	pthread_cancel(thrInfo[thrId-1]);
}

void *thread_func(void *arg)
{
	char message[MAX_SIZE];
	sprintf(message, "Process%d", thrId-1);
	while (true) 
	{
		pthread_mutex_lock(&mutex1);
		while (!canWrite) 
		{
			pthread_cond_wait(&synchr, &mutex1);
		}
		write(pipeWrite, message, MAX_SIZE);
		canWrite = 0;
		pthread_mutex_unlock(&mutex1);

		if (thrInfo.back() == pthread_self()) 
		{
			if (!pthread_mutex_trylock(&closeMutexes.back())) 
			{
				printf("CLose %d", thrId);
				closeMutexes.pop_back();
				thrInfo.pop_back();
				thrId--;
				pthread_exit(NULL);
			}
		}
	}
	return (void*) true;
}


int getch()
{	
    struct termios oldattr, newattr;
    int ch;
    tcgetattr(STDIN_FILENO, &oldattr);
    newattr = oldattr;
    newattr.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newattr);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);
    return ch;
}

bool kbhit()
{
	termios term;
    tcgetattr(0, &term);

    termios term2 = term;
    term2.c_lflag &= ~ICANON;
    tcsetattr(0, TCSANOW, &term2);

    int byteswaiting;
    ioctl(0, FIONREAD, &byteswaiting);

    tcsetattr(0, TCSANOW, &term);

    return byteswaiting > 0;
}

int showMenu()
{
	cout << "Press '+' to create new thread;" << endl;
	cout << "Press '-' to delete thread;" << endl;
	cout << "Press 'q' to quit;" << endl << endl;
}