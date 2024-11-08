#include "write_ppm.h"
#include <stdio.h>
#include <string.h>

void write_ppm(const char *filename, struct ppm_pixel *pixels, int w, int h)
{
    // FILE *fp = fopen(filename, "wb");

    // // check if file is null
    // if (fp == NULL)
    // {
    //     return;
    // }

    // // write header
    // fprintf(fp, "P6\n");
    // fprintf(fp, "Nora Steil\n");
    // fprintf(fp, "%d %d\n", w, h);
    // fprintf(fp, "255\n");

    // // write pixels
    // for (int i = 0; i < w * h; i++)
    // {
    //     fwrite(pixels, sizeof(struct ppm_pixel), 1, fp);
    // }
    // // fwrite(pixels, sizeof(struct ppm_pixel), w * h, fp);
    // fclose(fp);
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
