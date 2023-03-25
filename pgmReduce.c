#include <stdio.h>
#include <stdlib.h>
#include "pgm.h"
#include "error.h"
#include "pgmFile.h"
#include "reduce.h"

int main(int argc, char **argv)
{
    int ar = reduargc(argc, argv);
    if (ar == 2)
        return EXIT_NO_ERRORS;
    else if (ar != EXIT_NO_ERRORS)
        return ar;

    char *inputFile = argv[1];
    char *outputFile = argv[3];
    char *endptr;
    long int n = strtol(argv[2], &endptr, 10);
    if (*endptr != '\0') {
        printf("Factor n is not a integer");
        return EXIT_MISCELLANEOUS;
    }
    else if (n == 0) {
        printf("Invalid Factor Integer n");
        return EXIT_MISCELLANEOUS;
    }
    
    struct PGMImage *pgmorg = &(PGMImage) {
        .width = 0,
        .height = 0,
        .maxGray = 255,
        .imageData = NULL,
        .commentLine = NULL,
        .nImageBytes = 0
    };
    struct PGMImage *pgmnew = &(PGMImage) {
        .width = 0,
        .height = 0,
        .maxGray = 255,
        .imageData = NULL,
        .commentLine = NULL,
        .nImageBytes = 0
    };

    int readResult = readPGM(inputFile, pgmorg);
    if (readResult != EXIT_NO_ERRORS)
    {
        handleError(readResult, inputFile);
        return readResult;
    }

    reducePGM(n, pgmorg, pgmnew);

    int writeResult = writeASCII(outputFile, pgmnew);
    if (writeResult != EXIT_NO_ERRORS)
    {
        handleError(writeResult, outputFile);
        return writeResult;
    }

    free(pgmorg->commentLine);
	free(pgmorg->imageData);
    free(pgmnew->commentLine);
	free(pgmnew->imageData);
    return EXIT_NO_ERRORS;
}