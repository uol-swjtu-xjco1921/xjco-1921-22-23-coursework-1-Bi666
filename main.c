#include <stdio.h>
#include <stdlib.h>
#include "pgm.h"
#include "error.h"
#include "pgmFile.h"

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        printf("ERROR: Bad Argument Count");
        return EXIT_WRONG_ARG_COUNT;
    }

    char *inputFile = argv[1];
    char *outputFile = argv[2];
    
    struct PGMImage *pgm = &(PGMImage) {
        .width = 0,
        .height = 0,
        .maxGray = 255,
        .imageData = NULL,
        .commentLine = NULL
    };

    int readResult = readPGM(inputFile, pgm);
    if (readResult != EXIT_NO_ERRORS)
    {
        handleError(readResult, inputFile);
        return readResult;
    }./

    int writeResult = writePGM(outputFile, pgm);
    if (writeResult != EXIT_NO_ERRORS)
    {
        handleError(writeResult, outputFile);
        return writeResult;
    }

    free(pgm->commentLine);
	free(pgm->imageData);
    return EXIT_NO_ERRORS;
}