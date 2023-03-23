#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pgm.h"
#include "compare.h"

int compargc(int argc, char **argv)
{
    if (argc == 1)
    {
        printf("Usage: %s inputImage.pgm inputImage.pgm", argv[0]);
        return 100;
    }
    if (argc != 3)
    {
        printf("ERROR: Bad Argument Count");
        return EXIT_WRONG_ARG_COUNT;
    }
    return EXIT_NO_ERRORS;
}

void comparePGM(PGMImage *pgm1, PGMImage *pgm2)
{
    if (pgm1->width != pgm2->width || pgm1->height != pgm2->height){
        printf("DIFFERENT");
        return;
    }
    unsigned char *nextGrayValue1 = pgm1->imageData;
    unsigned char *nextGrayValue2 = pgm2->imageData;
    for (; nextGrayValue1 < pgm1->imageData + pgm1->nImageBytes; nextGrayValue1++, nextGrayValue2++){
        if (*nextGrayValue1 != *nextGrayValue2){
             printf("DIFFERENT");
             return;
        }
    }
    printf("IDENTICAL");
}