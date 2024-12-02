/*----------------------------------------------
 * Author: Nora Steil
 * Date: 11/25/24
 * Description: Builds a binary search tree of a given set of files using N threads
 ---------------------------------------------*/
#include <stdio.h>
#include "tree.h"
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>

struct tree_node *root = NULL;
pthread_mutex_t mutex;

struct thread_data
{
  int tid;
  char **files;
  int start_idx;
  int end_idx;
};

void *tree(void *userdata)
{
  struct thread_data *data = (struct thread_data *)userdata;

  for (int i = data->start_idx; i < data->end_idx + 1; i++)
  {
    FILE *file = fopen(data->files[i], "r");

    if (file == NULL)
    {
      printf("file %s is invalid\n", data->files[i]);
    }
    else
    {
      pthread_mutex_lock(&mutex);
      root = insert(data->files[i], root);
      pthread_mutex_unlock(&mutex);
      fclose(file);
    }
  }
  return NULL;
}

void depend(char *p_file)
{
  FILE *file = fopen(p_file, "r");
  if (file == NULL)
  {
    perror("failed to open file");
  }

  char buffer[256];
  while (fgets(buffer, sizeof(buffer), file) != NULL)
  {
    char const *pos = strstr(buffer, "#include");
    if (pos)
    {
      int idx = (int)(pos - buffer) + 0;
      while (buffer[idx] != '\0')
      {
        if (buffer[idx] != '<' && buffer[idx] != '>' && buffer[idx] != '"')
        {
          printf("%c", buffer[idx]);
        }
        idx++;
      }
    }
  }
  fclose(file);
}

int main(int argc, char *argv[])
{
  struct timeval tstart, tend;
  gettimeofday(&tstart, NULL);

  if (argc < 3)
  {
    fprintf(stderr, "usage: ./dependency <numThreads> <files>\n");
    return 1;
  }

  int fileNum = argc - 2;
  printf("Processing %d files\n", fileNum);
  char **files = (char **)malloc(fileNum * sizeof(char *));

  for (int i = 2; i < 2 + fileNum; i++)
  {
    files[i - 2] = argv[i];
  }

  int numProcesses = atoi(argv[1]);
  int *sub = (int *)malloc(numProcesses * sizeof(int));
  int num = 0;

  for (int i = 0; i < numProcesses; i++)
  {
    if (i == 0)
    {
      sub[i] = (fileNum + numProcesses - 1) / numProcesses;
      num += sub[i];
    }
    else if (i == numProcesses - 1)
    {
      sub[i] = fileNum - num;
    }
    else
    {
      sub[i] = fileNum / numProcesses;
      num += sub[i];
    }
  }

  pthread_t *threads = malloc(numProcesses * sizeof(pthread_t));
  struct thread_data *data = (struct thread_data *)malloc(numProcesses * sizeof(struct thread_data));

  for (int i = 0; i < numProcesses; i++)
  {
    data[i].tid = i;
    data[i].files = files;
    if (i == 0)
    {
      data[i].start_idx = 0;
      data[i].end_idx = sub[i] - 1;
    }
    else
    {
      data[i].start_idx = data[i - 1].end_idx + 1;
      data[i].end_idx = data[i].start_idx + sub[i] - 1;
    }
    pthread_create(&threads[i], NULL, tree, (void *)&data[i]);
    printf("Thread %d processing %d files (arguments %d to %d)\n", i, sub[i], data[i].start_idx + 2, data[i].end_idx + 2);
  }

  for (int i = 0; i < numProcesses; i++)
  {
    pthread_join(threads[i], NULL);
  }

  gettimeofday(&tend, NULL);
  float timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec) / 1.e6;
  printf("Elapsed time is %f\n", timer);
  char command[100];

  while (1)
  {
    printf("$ ");
    scanf("%s", command);
    if (strcmp(command, "list") == 0)
    {
      printSorted(root);
    }
    else if (strcmp(command, "quit") != 0)
    {
      int find = 0;
      for (int i = 0; i < fileNum; i++)
      {
        if (strcmp(files[i], command) == 0)
        {
          printf("%s has the following dependencies\n", command);
          depend(command);
          find = 1;
        }
      }
      if (find == 0)
      {
        printf("%s not found\n", command);
      }
    }
    else
    {
      exit(0);
    }
  }
  free(threads);
  free(data);
  free(files);
  free(data);

  return 0;
}
