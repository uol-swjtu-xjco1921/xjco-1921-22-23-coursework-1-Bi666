#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pgm.h"
#include "compare.h"

int reduargc(int argc, char **argv)
{
    if (argc == 1)
    {
        printf("Usage: %s inputImage.pgm reduction_factor outputImage.pgm", argv[0]);
        return 2;
    }
    if (argc != 4)
    {
        printf("ERROR: Bad Argument Count");
        return EXIT_WRONG_ARG_COUNT;
    }
    return EXIT_NO_ERRORS;
}

void reducePGM(int n, PGMImage *pgmorg, PGMImage *pgmnew)
{
    pgmnew->width = pgmorg->width / n;
    pgmnew->height = pgmorg->height / n;
    unsigned char *next = pgmnew->imageData;
    for (int row = 0; row < pgmorg->height; row += n)
        for (int col = 0; col < pgmorg->width; col += n) {
            unsigned char *nextGrayValue = pgmorg->imageData + row * pgmorg->width + col;
            *next = *nextGrayValue;
            next++;
        }
}