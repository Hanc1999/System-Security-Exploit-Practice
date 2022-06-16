#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int vul1(char *arg)
{
  char buffer[400];
  snprintf(buffer, sizeof buffer, arg);
  return 0;
}

int main(int argc, char *argv[])
{
  if (argc != 2)
    {
      fprintf(stderr, "program5: argc != 2\n");
      exit(EXIT_FAILURE);
    }
  vul1(argv[1]);

  return 0;
}
