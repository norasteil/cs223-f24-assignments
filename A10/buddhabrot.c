/*----------------------------------------------
 * Author: Nora Steil
 * Date: 11/15/24
 * Description: Makes a buddhabrot using threads
 ---------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <pthread.h>
#include "read_ppm.h"
#include "write_ppm.h"
#include <math.h>
#include <sys/time.h>

pthread_mutex_t count_mutex;
pthread_attr_t barrier;

struct threadData
{
  struct ppm_pixel **image;
  int **count;
  int *maxCount;
  int **member;
  int size;
  int threadId;
  int max;
  int xmin, xmax, ymin, ymax;
};

void *start(void *args)
{
  struct threadData *data = (struct threadData *)args;
  int **member = data->member;
  int **count = data->count;
  int *maxCount = data->maxCount;
  int id = data->threadId;
  struct ppm_pixel **image = data->image;
  int size = data->size;
  int maxIterations = data->max;
  int xmin = data->xmin;
  int xmax = data->xmax;
  int ymin = data->ymin;
  int ymax = data->ymax;
  int cStart = 0;
  int cEnd = 0;
  int rStart = 0;
  int rEnd = 0;
  if (id == 0)
  {
    cEnd = size / 2;
    rEnd = size / 2;
  }
  else if (id == 1)
  {
    cStart = size / 2;
    cEnd = size;
    rEnd = size / 2;
  }
  else if (id == 2)
  {
    cEnd = size / 2;
    rStart = size / 2;
    rEnd = size;
  }
  else
  {
    cStart = size / 2;
    cEnd = size;
    rStart = size / 2;
    rEnd = size;
  }

  pthread_mutex_lock(&count_mutex);

  // printf("Thread %lu) ", pthread_self());
  printf("Sub-image block: cols (%d, %d) to rows (%d, %d)\n", cStart, cEnd, rStart, rEnd);
  pthread_mutex_unlock(&count_mutex);

  // step 1
  for (int i = rStart; i < rEnd; i++)
  {
    for (int j = cStart; j < cEnd; j++)
    {
      float xfrac = i / (float)size;
      float yfrac = j / (float)size;
      float xA = xmin + xfrac * (xmax - xmin);
      float yA = ymin + yfrac * (ymax - ymin);
      float x = 0;
      float y = 0;
      int iter = 0;
      while (iter < maxIterations && ((x * x + y * y) < (2 * 2)))
      {
        float xtmp = x * x - y * y + xA;
        y = 2 * x * y + yA;
        x = xtmp;
        iter++;
      }

      if (iter < maxIterations)
      { // escaped
        member[j][i] = 0;
      }

      else
      {
        member[j][i] = 1;
      }
    }
  }

  for (int i = rStart; i < rEnd; i++)
  {
    for (int j = cStart; j < cEnd; j++)
    {
      if (member[j][i] == 1)
        continue;
      float xfrac = i / (float)size;
      float yfrac = j / (float)size;
      float xB = xmin + xfrac * (xmax - xmin);
      float yB = ymin + yfrac * (ymax - ymin);
      float x = 0;
      float y = 0;
      while ((x * x + y * y) < (2 * 2))
      {
        float xtmp = x * x - y * y + xB;
        y = 2 * x * y + yB;
        x = xtmp;

        int yrow = round(size * (y - ymin) / (float)(ymax - ymin));
        int xcol = round(size * (x - xmin) / (float)(xmax - xmin));

        if (yrow < 0 || yrow >= size)
          continue;
        if (xcol < 0 || xcol >= size)
          continue;

        count[yrow][xcol]++;
        pthread_mutex_lock(&count_mutex);
        if (count[yrow][xcol] > *maxCount)
        {
          maxCount = &count[yrow][xcol];
        }
        pthread_mutex_unlock(&count_mutex);
      }
    }
  }

  printf("maxCount of thread %d: %d\n", id, *maxCount);
  pthread_attr_init(&barrier);

  float gamma = 0.681;
  float factor = 1.0 / gamma;
  for (int i = rStart; i < rEnd; i++)
  {
    for (int j = cStart; j < cEnd; j++)
    {
      float value = 0;
      if (count[j][i] > 0)
      {
        value = log(count[j][i]) / log(*maxCount);
        value = pow(value, factor);
      }
      image[j][i].red = value * 255;
      image[j][i].green = value * 255;
      image[j][i].blue = value * 255;
    }
  }

  // printf("Thread %lu) finished\n", pthread_self());
  return (void *)0;
}

int main(int argc, char *argv[])
{
  struct timeval tstart, tend;
  pthread_mutex_init(&count_mutex, NULL);

  int size = 480;
  float xmin = -2.0;
  float xmax = 0.47;
  float ymin = -1.12;
  float ymax = 1.12;
  int maxIterations = 1000;
  int numProcesses = 4;
  int maxCount = 0;

  int opt;
  while ((opt = getopt(argc, argv, ":s:l:r:t:b:p:")) != -1)
  {
    switch (opt)
    {
    case 's':
      size = atoi(optarg);
      break;
    case 'l':
      xmin = atof(optarg);
      break;
    case 'r':
      xmax = atof(optarg);
      break;
    case 't':
      ymax = atof(optarg);
      break;
    case 'b':
      ymin = atof(optarg);
      break;
    case '?':
      printf("usage: %s -s <size> -l <xmin> -r <xmax> "
             "-b <ymin> -t <ymax> -p <numProcesses>\n",
             argv[0]);
      break;
    }
  }
  printf("Generating mandelbrot with size %dx%d\n", size, size);
  printf("  Num processes = %d\n", numProcesses);
  printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
  printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);

  struct ppm_pixel **image = malloc((size) * sizeof(struct ppm_pixel *));
  for (int i = 0; i < size; i++)
  {
    image[i] = malloc((size) * sizeof(struct ppm_pixel));
  }

  int **count = malloc((size) * sizeof(int *));
  for (int i = 0; i < size; i++)
  {
    count[i] = malloc((size) * sizeof(int));
  }

  for (int i = 0; i < size; i++)
  {
    for (int j = 0; j < size; j++)
    {
      count[i][j] = 0;
    }
  }

  pthread_attr_init(&barrier);

  srand(time(0));
  gettimeofday(&tstart, NULL);

  pthread_t *threads = malloc(4 * sizeof(pthread_t));

  int **member = malloc((size) * sizeof(int *));
  for (int i = 0; i < size; i++)
  {
    member[i] = malloc((size) * sizeof(int));
  }

  struct threadData thread[4];

  for (int i = 0; i < 4; i++)
  {
    thread[i].threadId = i;
    thread[i].image = image;
    thread[i].count = count;
    thread[i].member = member;
    thread[i].size = size;
    thread[i].maxCount = &maxCount;
    thread[i].max = maxIterations;
    thread[i].xmin = xmin;
    thread[i].xmax = xmax;
    thread[i].ymin = ymin;
    thread[i].ymax = ymax;
  }

  for (int i = 0; i < 4; i++)
  {
    pthread_create(&threads[i], NULL, start, &thread[i]);
  }

  for (int i = 0; i < 4; i++)
  {
    pthread_join(threads[i], NULL);
  }

  gettimeofday(&tend, NULL);
  float timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec) / 1.e6;
  printf("Computed mandelbrot set (%dx%d) in %.6f seconds\n", size, size, timer);

  pthread_mutex_destroy(&count_mutex);
  pthread_attr_destroy(&barrier);

  char fileName[128];
  snprintf(fileName, 128, "mandelbrot-%d-%ld.ppm", size, time(0));
  printf("Writing file: %s\n", fileName);

  write_ppm_2d(fileName, image, size, size);

  for (int i = 0; i < size; i++)
  {
    free(image[i]);
  }
  free(image);

  for (int i = 0; i < size; i++)
  {
    free(member[i]);
  }
  free(member);

  for (int i = 0; i < size; i++)
  {
    free(count[i]);
  }
  free(count);
  free(threads);
}
