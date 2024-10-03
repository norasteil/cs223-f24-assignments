/*----------------------------------------------
 * Author: Nora Steil
 * Date: 10/3/24
 * Description: Test read_ppm
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include "read_ppm.h"

int main()
{
  int w, h;
  struct ppm_pixel *pixels = read_ppm("feep-raw.ppm", &w, &h);

  // todo: print out the grid of pixels
  // write a short test that calls your function and prints the contents of feep-raw.ppm
  printf("Testing file feep-raw.ppm: %d %d\n", w, h);
  for (int i = 0; i < h; i++)
  {
    for (int j = 0; j < w; j++)
    {
      struct ppm_pixel *p = &pixels[i * w + j];
      printf("(%d, %d, %d) ", p->red, p->green, p->blue);
    }
    printf("\n");
  }
  free(pixels);
  return 0;
}
