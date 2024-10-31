/*----------------------------------------------
 * Author: Nora Steil
 * Date: 10/31/24
 * Description: Creates a custom shell that uses fork to spawn processes
 * NOTE: I had trouble with readline on my mac, so I ran shell.c like this:
 * gcc -g -Wall -Wvla -Werror -Wno-unused-variable -Wno-unused-but-set-variable shell.c read_ppm.c write_ppm.c -o shell -I/opt/homebrew/opt/readline/include -L/opt/homebrew/opt/readline/lib -lreadline
 * ./shell
 ---------------------------------------------*/

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <pwd.h>
#include <string.h>
#include <stdlib.h>

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_RESET "\x1b[0m"

int main()
{
  printf(ANSI_COLOR_MAGENTA "Welcome to Nora's custom shell\n" ANSI_COLOR_RESET);
  char *input;
  while (1)
  {
    char cwd[1024];
    char hostname[1024];
    struct passwd *pw = getpwuid(geteuid());

    getcwd(cwd, sizeof(cwd));
    gethostname(hostname, sizeof(hostname));

    printf(ANSI_COLOR_GREEN "%s@%s:" ANSI_COLOR_BLUE "%s" ANSI_COLOR_RESET "$ ", pw->pw_name, hostname, cwd);

    input = readline("");

    if (input == NULL || strcmp(input, "exit") == 0)
    {
      free(input);
      break;
    }

    if (*input)
    {
      add_history(input);
    }

    char *args[128];
    char *token = strtok(input, " ");
    int i = 0;
    while (token != NULL && i < 127)
    {
      args[i++] = token;
      token = strtok(NULL, " ");
    }
    args[i] = NULL;

    pid_t pid = fork();
    if (pid == 0)
    {
      int ret = execvp(args[0], args);
      if (ret < 0)
      {
        printf(ANSI_COLOR_YELLOW "%s:" ANSI_COLOR_RED " command not found\n" ANSI_COLOR_RESET, args[0]);
      }
      free(input);
      exit(0);
    }
    else if (pid < 0)
    {
      perror(ANSI_COLOR_RED "Fork failed" ANSI_COLOR_RESET);
    }
    else
    {
      wait(NULL);
    }

    free(input);
  }
  return 0;
}
