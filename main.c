#include <stdio.h>
#include <stdlib.h>
#include "pgm.h"
#include "error.h"

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        printf("Usage: %s inputImage.pgm outputImage.pgm\n", argv[0]);
        return EXIT_WRONG_ARG_COUNT;
    }

    char *inputFile = argv[1];
    char *outputFile = argv[2];
    PGMImage *pgm = {
        0,
        0,
        255,
        NULL
    };

    int readResult = readPGM(inputFile, pgm);
    if (readResult != EXIT_NO_ERRORS)
    {
        handleError(readResult, inputFile);
        return readResult;
    }

    int writeResult = writePGM(outputFile, pgm);
    if (writeResult != EXIT_NO_ERRORS)
    {
        handleError(writeResult, outputFile);
        free(pgm->imageData);
        return writeResult;
    }

    free(pgm->imageData);
    return EXIT_NO_ERRORS;
}