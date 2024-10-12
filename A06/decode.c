/*----------------------------------------------
 * Author: Nora Steil
 * Date: 10/11/24
 * Description: read in a PPM file and output a message stored in the least significant bits of each color
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include "read_ppm.h"

int main(int argc, char **argv)
{
  if (argc != 2)
  {
    printf("usage: decode <file.ppm>\n");
    return 0;
  }

  const char *filename = argv[1];
  int width;
  int height;

  struct ppm_pixel *pixels = read_ppm(filename, &width, &height);
  if (pixels == NULL)
  {
    printf("Error: unable to open file %s\n", filename);
    exit(1);
  }

  // read in filename as a command line arg
  // error if file can't be read
  // reuse read_ppm and write_ppm implementation
  // output size and max number of chars

  // int width = 0;
  // int height = 0;
  int max = (width * height * 3) / 8;

  printf("Reading %s with width %d and height %d\n", filename, width, height);
  printf("Max number of characters in the image: %d\n", max);

  int count = 0;
  char current = 0;

  for (int i = 0; i < width * height; i++)
  {
    unsigned char *pixelValues = (unsigned char *)(&pixels[i]);

    for (int j = 0; j < 3; j++)
    {
      current |= (pixelValues[j] & 1) << (7 - count);
      count++;

      if (count == 8)
      {
        if (current == '\0')
        {
          printf("\n");
          free(pixels);
          return 0;
        }

        printf("%c", current);
        count = 0;
        current = 0;
      }
    }
  }

  free(pixels);

  // read bits from red, green, and blue (top to bottom, left to right)
  // keep decoding until \0

  return 0;
}
