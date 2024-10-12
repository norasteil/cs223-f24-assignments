/*----------------------------------------------
 * Author: Nora Steil
 * Date: 10/12/24
 * Description: Writes a binary file
 ---------------------------------------------*/
#include "write_ppm.h"
#include <stdio.h>
#include <string.h>

// implement *one* (but do not remove the other one)!

void write_ppm(const char *filename, struct ppm_pixel *pixels, int w, int h)
{
    FILE *fp = fopen(filename, "wb");

    // write header
    fprintf(fp, "P6\n%d %d\n255\n", w, h);

    // write binary
    fwrite(pixels, sizeof(struct ppm_pixel), w * h, fp);
    fclose(fp);
}

void write_ppm_2d(const char *filename, struct ppm_pixel **pixels, int w, int h)
{
}
