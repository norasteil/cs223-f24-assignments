/*----------------------------------------------
 * Author: Nora Steil
 * Date: 11/25/24
 * Description: Uses N threads to search for a keyword in a set of files
 ---------------------------------------------*/
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>

struct thread_data
{
  int start_idx;
  int end_idx;
  char **list;
  char *keyword;
  int thread_id;
  int line_count;
};

char **find_keyword(const char *command, int *num_files)
{
  FILE *file;
  char buffer[1024];
  char **files = malloc(100 * sizeof(char *));
  *num_files = 0;

  file = popen(command, "r");
  if (file == NULL)
  {
    perror("Error executing command");
    return NULL;
  }

  while (fgets(buffer, sizeof(buffer), file))
  {
    buffer[strcspn(buffer, "\n")] = 0;
    files[*num_files] = malloc(strlen(buffer) + 1);
    strcpy(files[*num_files], buffer);
    (*num_files)++;

    if (*num_files >= 100)
    {
      break;
    }
  }

  fclose(file);
  return files;
}

void *search(void *userdata)
{
  struct thread_data *data = (struct thread_data *)userdata;
  for (int i = data->start_idx; i < data->end_idx; i++)
  {
    FILE *file = fopen(data->list[i], "r");
    char buffer[100];
    while (fgets(buffer, sizeof(buffer), file))
    {
      if (strstr(buffer, data->keyword))
      {
        pthread_mutex_t mutex;
        pthread_mutex_init(&mutex, NULL);
        pthread_mutex_lock(&mutex);
        data->line_count++;
        printf("%d) %s: %s", data->thread_id, data->list[i], buffer);
        pthread_mutex_unlock(&mutex);
      }
    }
    fclose(file);
  }
  return (void *)NULL;
}

int main(int argc, char **argv)
{
  const int num_threads = atoi(argv[1]);

  if (argc != 4)
  {
    fprintf(stderr, "Usage: ./grep <numThreads> <keyword> <files>");
    return 1;
  }

  char *files_input = argv[3];
  int num_files = 0;
  char **files = NULL;

  files = find_keyword(files_input, &num_files);
  if (files == NULL)
  {
    return 1;
  }

  printf("searching %d files for keyword %s\n", num_files, argv[2]);

  double timer;
  struct timeval tstart, tend;

  gettimeofday(&tstart, NULL);

  pthread_t *threads = (pthread_t *)malloc(sizeof(pthread_t) * num_threads);
  struct thread_data *data = (struct thread_data *)malloc(sizeof(struct thread_data) * num_threads);
  int subsize = num_files / num_threads;
  int remainder = num_files % num_threads;

  for (int i = 0; i < num_threads; i++)
  {
    data[i].list = files;
    data[i].keyword = argv[2];
    data[i].thread_id = i;
    data[i].line_count = 0;
    data[i].start_idx = subsize * i;
    if ((remainder != 0) && (i == num_threads - 1))
    {
      data[i].end_idx = subsize * (i + 1) + remainder;
    }
    else
    {
      data[i].end_idx = subsize * (i + 1);
    }

    pthread_create(&threads[i], NULL, search, (void *)&data[i]);
    printf("thread %d searching %d files (%d to %d)\n", data[i].thread_id, data[i].end_idx - data[i].start_idx, data[i].start_idx + 1, data[i].end_idx);
  }

  for (int i = 0; i < num_threads; i++)
  {
    pthread_join(threads[i], NULL);
  }

  gettimeofday(&tend, NULL);
  timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec) / 1.e6;
  printf("elapsed time: %g seconds\n", timer);

  for (int i = 0; i < num_threads; i++)
  {
    printf("thread %d found %d lines containing keyword %s\n", data[i].thread_id, data[i].line_count, argv[2]);
  }

  for (int i = 0; i < num_files; i++)
  {
    free(files[i]);
  }

  free(files);
  free(threads);
  free(data);

  return 0;
}
