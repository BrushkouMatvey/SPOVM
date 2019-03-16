#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv)
{
	initscr();
	curs_set(0);

	time_t ltime;
	pid_t pid = fork();

	int st;

	switch(pid)
	{
		case -1: {
			printw("Error.");
			break;
		}

		case 0: {
			if(execlp("./child",NULL) == -1)
				printw("Error.");

			break;
		}

		default: {
			while(1)
			{
				refresh();
				time(&ltime);
				move(9,0);
				printw(ctime( &ltime ));

				if(waitpid(pid,&st,WNOHANG)>0)
					break;

				napms(50);
                                
			}
			move(20,0);
			printw("The child process was terminated.");
			if (WIFEXITED(st))
			{
				move(21,0);
				printw("%d", (int)WEXITSTATUS(st));
			}
		}
	}
	getch();
	endwin();
	return 0;
} 
