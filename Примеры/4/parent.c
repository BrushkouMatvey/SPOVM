#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>

int main(int argc, char const *argv[])
{
  int st;
  pid_t pid = fork();
  switch (pid) {
    case -1:
      printf("Error\n");
      break;
    case 0:
      execlp("xterm", "-e", "./script.sh", NULL);
      // execlp("gnome-terminal", "-e", "./main", NULL);
      break;
    default:
      printf("%d\n", pid);
      while (!(waitpid(pid, &st, WNOHANG) > 0));
      break;
  }
  getchar();
  return 0;
}
