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
    int n = strtol(argv[2], &endptr, 10);
    if (*endptr != '\0' || n == 0) {
        printf("Invalid Factor Integer n");
        return EXIT_MISCELLANEOUS;
    }
    
    struct PGMImage *pgmorg = NULL;
    struct PGMImage *pgmnew = NULL;

    int readResult = readPGM(inputFile, pgmorg);
    if (readResult != EXIT_NO_ERRORS)
    {
        handleError(readResult, inputFile);
        return readResult;
    }

    pgmnew = (PGMImage*)malloc(sizeof(PGMImage));
    reducePGM(n, pgmorg, pgmnew);

    int writeResult = writeASCII(outputFile, pgmnew);
    if (writeResult != EXIT_NO_ERRORS)
        handleError(writeResult, outputFile);

    free(pgmorg->commentLine);
    pgmorg->commentLine = NULL;
	free(pgmorg->imageData);
    pgmorg->imageData = NULL;
    free(pgmorg);
    pgmorg = NULL;
	free(pgmnew->imageData);
    pgmnew->imageData = NULL;
    free(pgmnew);
    pgmnew = NULL;
    return EXIT_NO_ERRORS;
}