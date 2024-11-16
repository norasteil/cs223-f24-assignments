/*----------------------------------------------
 * Author: Nora Steil
 * Date: 11/8/24
 * Description: Writes a binary file
 ---------------------------------------------*/
#include "write_ppm.h"
#include <stdio.h>
#include <string.h>

void write_ppm(const char *filename, struct ppm_pixel *pixels, int w, int h)
{
}

void write_ppm_2d(const char *filename, struct ppm_pixel **pxs, int w, int h)
{
    FILE *fp = fopen(filename, "wb");

    fprintf(fp, "P6\n");
    fprintf(fp, "#write a ppm file\n");
    fprintf(fp, "%d %d\n", w, h);
    fprintf(fp, "%d\n", 225);

    for (int i = 0; i < h; i++)
    {
        fwrite(pxs[i], sizeof(struct ppm_pixel), w, fp);
    }

    fclose(fp);
}
