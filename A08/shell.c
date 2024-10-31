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

void display_prompt()
{
  char cwd[1024];
  char hostname[1024];
  struct passwd *pw = getpwuid(geteuid());

  getcwd(cwd, sizeof(cwd));
  gethostname(hostname, sizeof(hostname));

  printf(ANSI_COLOR_GREEN "%s@%s:" ANSI_COLOR_BLUE "%s" ANSI_COLOR_RESET "$ ", pw->pw_name, hostname, cwd);
}

int main()
{
  // print a prompt that shows working directory, look distinct from lab machine prompts
  // use readline() to get user input
  // use add_history() to save user history
  // quit if user types exit

  char *input;
  while (1)
  {
    display_prompt();

    input = readline("");

    if (input == NULL)
    {
      break;
    }

    if (*input)
    {
      add_history(input);
    }

    if (strcmp(input, "exit") == 0)
    {
      free(input);
      break;
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
      if (execvp(args[0], args) == -1)
      {
        perror("Error executing command");
      }
      exit(1);
    }
    else if (pid < 0)
    {
      perror("Fork failed");
    }
    else
    {
      wait(NULL);
    }

    free(input);
  }
  return 0;
}
