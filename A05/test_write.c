/*----------------------------------------------
 * Author: Nora Steil
 * Date: 10/4/24
 * Description: Test write_ppm
 ---------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "read_ppm.h"
#include "write_ppm.h"

int main(int argc, char **argv)
{
  int w, h;
  struct ppm_pixel *pixels = read_ppm("feep-raw.ppm", &w, &h);

  // test writing the file to test.ppm, reload it, and print the contents
  write_ppm("test.ppm", pixels, w, h);

  printf("Testing file test.ppm: %d %d\n", w, h);
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
