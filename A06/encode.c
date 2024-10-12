/*----------------------------------------------
 * Author: Nora Steil
 * Date: 10/12/24
 * Description: Take in a message from the user and encode it into an image
 ---------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "read_ppm.h"
#include "write_ppm.h"

int main(int argc, char **argv)
{
  if (argc != 2)
  {
    printf("usage: encode <file.ppm>\n");
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

  int max = (width * height * 3) / 8;

  printf("Reading %s with width %d and height %d\n", filename, width, height);
  printf("Max number of characters in the image: %d\n", max);

  char phrase[32];
  printf("Enter a phrase: ");
  scanf("%s", phrase);

  int count = 0;

  for (int i = 0; i < max; i++)
  {
    unsigned char current = phrase[i];
    for (int j = 0; j < 8; j++)
    {
      int bit = (current >> (7 - j)) & 1;

      unsigned char *pixelValues = (unsigned char *)&pixels[count / 3];
      pixelValues[count % 3] = (pixelValues[count % 3] & 0xFE) | bit;

      count++;
    }
  }

  char outputFile[32] = {0};

  for (int i = 0; i < 32; i++)
  {
    char current = filename[i];
    if (current == '\0' || current == '.')
    {
      break;
    }
    outputFile[i] = current;
  }

  strcat(outputFile, "-encoded.ppm");

  write_ppm(outputFile, pixels, width, height);

  free(pixels);
  return 0;
}
