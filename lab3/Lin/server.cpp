#include <iostream>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <termios.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <fcntl.h>

const std::string PRINT_NAME = "PRINT";
const int SIZE = 20;
const int MAX_SIZE_PROCESS = 5;
int sem_id;
int pipeRead;
int pipeWrite;

union semun
{
    int              val;
    struct semid_ds *buf;
    unsigned short  *array;
    struct seminfo  *__buf;
} semunion;

void ShowError(){
     printf("Error\n");
}

int getchars(void){
    struct termios oldattr, newattr;
    int ch;
    tcgetattr( STDIN_FILENO, &oldattr );
    newattr = oldattr;
    newattr.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
    return ch;
}

int WaitSemaphore(int sem_id, int num)
{
    struct sembuf buf;
    buf.sem_op = -1;
    buf.sem_flg = IPC_NOWAIT;
    buf.sem_num = num;
    return semop(sem_id, &buf, 1);
}

int ReleaseSemaphore(int sem_id, int num)
{
    struct sembuf buf;
    buf.sem_op = 1;
    //- Если какая-либо операция, для которой задан флаг IPC_NOWAIT, не может быть
    //успешно выполнена, системный вызов завершается неудачей, причем 
    //ни у одного из семафоров не будет изменено значение
    buf.sem_flg = IPC_NOWAIT;
    buf.sem_num = num;
    return semop(sem_id, &buf, 1);
}

void runController(int sem_id,int pipe){
	char pipeW[20],semID[20];
	
	pid_t pid = fork();
	sprintf(semID,"%d",sem_id);
	sprintf(pipeW,"%d",pipe);
    	if(pid < 0) {
        	std::cout << "Unlucky attempt!" << std::endl;
    	}
    	else if(pid > 0){
    	}
    	else {   
		execlp("./controller",semID,pipeW,NULL);
		printf("cannot run controller");
		exit(-1);
   	}		
}

int main(){
	int pipeId[2];
	char str[20];
	sem_id = semget(IPC_PRIVATE,MAX_SIZE_PROCESS*2 + 3, IPC_CREAT|0777);
	semctl(sem_id,0,SETALL,0);//init sem
	if(sem_id == -1) ShowError();//error

	if(pipe(pipeId)){
		semctl(sem_id, 0, IPC_RMID, semunion);
      		printf("\nServer error: Dont open pipe!\n");
		return 0;
	}
	pipeRead = pipeId[0];
	pipeWrite = pipeId[1];

	runController(sem_id,pipeWrite);

	while(true){
		if(WaitSemaphore(sem_id,10)!= -1){
			memset(str, '\0', SIZE);
			read(pipeRead,&str,SIZE);

			for(unsigned int i=0; i < strlen(str);i++){
        		std::cout << str[i] << "\n";
				usleep(200000);
    			}
			printf("\n");

			ReleaseSemaphore(sem_id,11);
		}
		if(WaitSemaphore(sem_id,12)!= -1){
			break;
		}
	}
	
	close(pipeRead);
	close(pipeWrite);
	semctl(sem_id,0,IPC_RMID,semunion);
	return 0;
}
