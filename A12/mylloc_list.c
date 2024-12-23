/*----------------------------------------------
 * Author: Nora Steil
 * Date: 12/5/24
 * Description: Implements malloc/free with a free list and using the first fit strategy
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct chunk
{
  int size;
  int used;
  struct chunk *next;
};
struct chunk *flist = NULL;

void *malloc(size_t size)
{
  if (size == 0)
  {
    return NULL;
  }
  struct chunk *next = flist;
  struct chunk *prev = NULL;

  while (next != NULL)
  {
    if (next->size >= size)
    {
      if (prev != NULL)
      {
        prev->next = next->next;
      }
      else
      {
        flist = next->next;
      }
      return (void *)(next + 1);
    }
    else
    {
      prev = next;
      next = next->next;
    }
  }

  void *memory = sbrk(size + sizeof(struct chunk));
  if (memory == (void *)-1)
  {
    return NULL;
  }
  else
  {
    struct chunk *cnk = (struct chunk *)memory;
    cnk->size = size;
    return (void *)(cnk + 1);
  }
}

void free(void *memory)
{
  if (memory != NULL)
  {
    struct chunk *cnk = (struct chunk *)((struct chunk *)memory - 1);
    cnk->next = flist;
    flist = cnk;
  }
  return;
}
