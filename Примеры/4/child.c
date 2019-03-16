#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>

int main(int argc, char const *argv[])
{
  printf("%d\n", argc);
  for (int i = 0; i < argc; i++)
  {
    printf("%s\n", argv[i]);
  }
  getchar();
  return 0;
}
