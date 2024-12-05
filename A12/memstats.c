/*----------------------------------------------
 * Author: Nora Steil
 * Date: 12/5/24
 * Description: A benchmark program that prints information about my malloc implementation
 ---------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>
#include "rand.h"

#define ROUNDS 3
#define BUFFER 5
#define LOOP 10

struct chunk
{
  int size;
  int used;
  struct chunk *next;
};

void memstats(struct chunk *freelist, void *buffer[], int len)
{
  int total_blocks = 0;
  int freed_blocks = 0;
  int used_blocks = 0;

  size_t tot_mem_alloc = 0;
  size_t freed_memory = 0;
  size_t used_memory = 0;

  struct chunk *current = freelist;
  while (current != NULL)
  {
    freed_blocks++;
    freed_memory += current->size;
    current = current->next;
  }

  for (int i = 0; i < len; i++)
  {
    if (buffer[i] != NULL)
    {
      struct chunk *block = (struct chunk *)((char *)buffer[i] - sizeof(struct chunk));
      used_blocks++;
      used_memory += block->size;
    }
  }

  total_blocks = freed_blocks + used_blocks;
  tot_mem_alloc = freed_memory + used_memory;

  void *curr_brk = sbrk(0);
  size_t alloc_mem = (char *)curr_brk - (char *)buffer;
  double underutil_mem = (alloc_mem - tot_mem_alloc) / (double)alloc_mem;

  printf("Total blocks: %d Free blocks: %d Used blocks: %d\n", total_blocks, freed_blocks, used_blocks);
  printf("Total memory allocated: %zu Free memory: %zu Used memory: %zu\n", tot_mem_alloc, freed_memory, used_memory);
  printf("Underutilized memory: %.2f\n", underutil_mem);
}

int main(int argc, char *argv[])
{

  printf("Starting test..\n");

  srand(100);

  double timer;
  struct timeval tstart, tend;

  gettimeofday(&tstart, NULL);

  void *buffer[BUFFER];
  for (int i = 0; i < BUFFER; i++)
  {
    buffer[i] = NULL;
  }

  void *init = sbrk(0);
  void *current;
  printf("The initial top of the heap is %p.\n", init);
  for (int j = 0; j < ROUNDS; j++)
  {
    printf("---------------\n%d\n", j);

    for (int i = 0; i < LOOP; i++)
    {
      int index = rand() % BUFFER;
      if (buffer[index] != NULL)
      {
        free(buffer[index]);
        buffer[index] = NULL;
        printf("Freeing index %d\n", index);
      }
      else
      {
        size_t size = (size_t)randExp(8, 4000);
        int *memory = NULL;
        memory = malloc(size);

        if (memory == NULL)
        {
          fprintf(stderr, "malloc failed\n");
          return (1);
        }
        *memory = 123;
        buffer[index] = memory;
        printf("Allocating %d bytes at index %d\n", (int)size, index);
      }
    }
    extern struct chunk *flist;
    current = sbrk(0);
    int allocated = current - init;
    init = current;

    printf("The new top of the heap is %p.\n", current);
    printf("Increased by %d (0x%x) bytes\n", allocated, allocated);
    memstats(flist, buffer, BUFFER);
  }

  for (int i = 0; i < BUFFER; i++)
  {
    free(buffer[i]);
  }
  gettimeofday(&tend, NULL);
  timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec) / 1.e6;
  printf("Time is %g\n", timer);

  return 0;
}
