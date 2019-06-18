#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>

int main (int argc, char* argv[])
{
  srand(time(NULL));

  int i;
  for (i = 0; argv[1][i] != '\0'; i++)
  {
    if (isalpha(argv[1][i]))
    {
      fprintf(stderr, "Please only enter numbers\n");
      return 1;
    }
  }

  int num = atoi(argv[1]);
  char key[num + 1];

  // Only use the alphabet and space characters, randomize them
  for (i = 0; i < num; i++)
  {
    key[i] = rand() % 27 + 65;

    if (key[i] == 91)
      key[i] = 32;
  }

  key[num] = '\n';

  printf("%s", key);
  return 0;
}
