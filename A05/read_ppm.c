/*----------------------------------------------
 * Author: Nora Steil
 * Date: 10/3/24
 * Description: Read a binary file
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_ppm.h"

struct ppm_pixel *read_ppm(const char *filename, int *w, int *h)
{
  FILE *fp = fopen(filename, "rb");
  if (fp == NULL)
  {
    return NULL;
  }

  // skip header
  char header[256];
  fgets(header, sizeof(header), fp);

  // skip comment line
  char comment[256];
  fgets(comment, sizeof(comment), fp);

  // get the width, height and max color of the image
  int maxColor;
  fscanf(fp, "%d %d %d", w, h, &maxColor);

  // skip the whitespace character
  fgetc(fp);

  // allocate space for the 2d array, retun null if can't allocate
  struct ppm_pixel *pixels = malloc((*w) * (*h) * sizeof(struct ppm_pixel));
  if (pixels == NULL)
  {
    fclose(fp);
    return NULL;
  }

  fread(pixels, sizeof(struct ppm_pixel), (*w) * (*h), fp);

  fclose(fp);
  return pixels;
}

struct ppm_pixel **read_ppm_2d(const char *filename, int *w, int *h)
{
  return NULL;
}