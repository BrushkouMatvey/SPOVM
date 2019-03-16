#include <sys/types.h>
#include <unistd.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
	time_t ltime;
	char c;
	
	initscr();
	noecho();

	while(c != 27)
	{
		time(&ltime);
		move(8,0);
		printw(ctime(&ltime));
		
		halfdelay(2);
		c = getch();

		refresh();
	}
	return 0;
}  
