/*----------------------------------------------
 * Author: Nora Steil
 * Date: 10/31/24
 * Description: Works with fork to spawn processes
 ---------------------------------------------*/

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
  pid_t pid;

  printf("%d A0\n", getpid());
  fflush(stdout);
  pid = fork();

  if (pid) // B0
  {
    printf("%d B0\n", getpid());
    fflush(stdout);
    pid = fork();

    if (pid) // C0
    {
      printf("%d C0\n", getpid());
      fflush(stdout);
    }
    else // child of C0
    {
      printf("%d C1\n", getpid());
      fflush(stdout);
    }
  }
  else // child of B0
  {
    printf("%d B1\n", getpid());
    fflush(stdout);
  }

  printf("%d Bye\n", getpid());
  return 0;
}
