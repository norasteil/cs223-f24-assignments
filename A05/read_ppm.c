/*----------------------------------------------
 * Author: Nora Steil
 * Date: 10/3/24
 * Description: Read a binary file
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_ppm.h"

// Choose *one* to implement (do not remove the other one!)
// take filename as input, output array of struct pixel
// user responsible for freeing allocated memory
// implement your 2D array of pixels as either a flat array or an array of arrays
// flat array function: struct ppm_pixel* read_ppm(const char* filename, int* width, int* height)
// array of arrays function: struct ppm_pixel** read_ppm(const char* filename, int* width, int* height)
// use width & height to return width/height of image

// return null if filename invalid
// retrun null if memory can't be allocated
// return pointer to array
// assume safe to read header using fgets
// free data
// single comment starting w #

struct ppm_pixel *read_ppm(const char *filename, int *w, int *h)
{
  FILE *fp = fopen(filename, "rb");
  if (fp == NULL)
  {
    return NULL;
  }

  char header[3]; // skip header
  fgets(header, sizeof(header), fp);
  printf("%s\n", header);
  char comment[256]; // skip comment line
  fgets(comment, sizeof(comment), fp);
  printf("%s\n", comment);

  int maxColor;
  fscanf(fp, "%d %d %d", w, h, &maxColor); // get the width, height and max color of the image

  fgetc(fp); // skip the whitespace character

  struct ppm_pixel *pixels = malloc((*w) * (*h) * sizeof(struct ppm_pixel)); // allocate space for the 2d array
  if (pixels == NULL)                                                        // retun null if can't allocate
  {
    fclose(fp);
    return NULL;
  }

  fread(pixels, sizeof(struct ppm_pixel), (*w) * (*h), fp); // read defined amount of bytes

  fclose(fp);
  return pixels;
}

struct ppm_pixel **read_ppm_2d(const char *filename, int *w, int *h)
{
  return NULL;
}
