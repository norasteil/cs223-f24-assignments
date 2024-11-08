/*----------------------------------------------
 * Author: Nora Steil
 * Date: 11/8/24
 * Description: Outputs a ppm image of a mandelbrot set using threads
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>
#include "read_ppm.h"
#include "write_ppm.h"

struct threadInfo
{
  struct ppm_pixel **image;
  struct ppm_pixel *palette;
  int size;
  int threadId;
  int max;
  int xmin;
  int xmax;
  int ymin;
  int ymax;
};

void *thread_generate(void *args)
{
  struct threadInfo *info = (struct threadInfo *)args;
  int id = info->threadId;
  struct ppm_pixel **image = info->image;
  struct ppm_pixel *palette = info->palette;
  int size = info->size;
  int maxIter = info->max;
  int xmin = info->xmin;
  int xmax = info->xmax;
  int ymin = info->ymin;
  int ymax = info->ymax;
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

  printf("Thread %lu) sub-image block: cols (%d, %d) to rows (%d, %d)\n", (unsigned long)pthread_self(), cStart, cEnd, rStart, rEnd);

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

      while (iter < maxIter && ((x * x * y * y) < 4))
      {
        float xtemp = x * x - y * y + xA;
        y = 2 * x * y + yA;
        x = xtemp;
        iter++;
      }

      if (iter < maxIter)
      {
        image[j][i].red = palette[iter].red;
        image[j][i].green = palette[iter].green;
        image[j][i].blue = palette[iter].blue;
      }
      else // set color to black
      {
        image[j][i].red = 0;
        image[j][i].green = 0;
        image[j][i].blue = 0;
      }
    }
  }
  printf("thread %lu) finished \n", (unsigned long)pthread_self());
  return (void *)0;
}

int main(int argc, char *argv[])
{
  int size = 2000;
  float xmin = -2.0;
  float xmax = 0.47;
  float ymin = -1.12;
  float ymax = 1.12;
  int maxIterations = 1000;
  int numProcesses = 4;

  double timer;
  struct timeval tstart, tend;

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

  // todo: your code here
  struct ppm_pixel **image = malloc((size) * sizeof(struct ppm_pixel *));

  for (int i = 0; i < size; i++)
  {
    image[i] = malloc((size) * sizeof(struct ppm_pixel));
  }

  srand(time(0));
  struct ppm_pixel *palette = malloc((maxIterations) * sizeof(struct ppm_pixel));

  for (int i = 0; i < maxIterations; i++)
  {
    palette[i].red = rand() % 255;
    palette[i].green = rand() % 255;
    palette[i].blue = rand() % 255;
  }

  gettimeofday(&tstart, NULL);

  pthread_t *threads = malloc(4 * sizeof(pthread_t));
  struct threadInfo threadStruct[4];

  for (int i = 0; i < 4; i++)
  {
    threadStruct[i].threadId = i;
    threadStruct[i].image = image;
    threadStruct[i].size = size;
    threadStruct[i].max = maxIterations;
    threadStruct[i].palette = palette;
    threadStruct[i].xmin = xmin;
    threadStruct[i].xmax = xmax;
    threadStruct[i].ymin = ymin;
    threadStruct[i].ymax = ymax;
  }

  for (int i = 0; i < 4; i++)
  {
    pthread_create(&threads[i], NULL, thread_generate, &threadStruct[i]);
  }

  for (int i = 0; i < 4; i++)
  {
    pthread_join(threads[i], NULL);
  }

  gettimeofday(&tend, NULL);
  timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec) / 1.e6;
  printf("computed mandelbrot set (%dx%d) in %.6f seconds\n", size, size, timer);

  char fileName[128];
  snprintf(fileName, 128, "mandelbrot-%d-%ld.ppm", size, time(0));
  printf("writing file: %s\n", fileName);

  write_ppm_2d(fileName, image, size, size);

  for (int i = 0; i < size; i++)
  {
    free(image[i]);
  }
  free(image);

  free(palette);
  free(threads);
}
