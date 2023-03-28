#include <stdio.h>
#include <stdlib.h>
#include "pgm.h"
#include "error.h"
#include "pgmFile.h"

int main(int argc, char **argv)
{
    int ar = judgeargc(argc, argv);
    if (ar == 2)
        return EXIT_NO_ERRORS;
    else if (ar != EXIT_NO_ERRORS)
        return ar;

    char *inputFile = argv[1];
    char *outputFile = argv[2];
    
    PGMImage *pgm = (PGMImage*)malloc(sizeof(PGMImage));
    if (pgm == NULL) 
    {
        handleError(EXIT_MALLOC_FAILED, inputFile);
        return EXIT_MALLOC_FAILED;
    }

    int readResult = readPGM(inputFile, pgm);
    if (readResult != EXIT_NO_ERRORS)
    {
        handleError(readResult, inputFile);
        return readResult;
    }

    int writeResult = writeASCII(outputFile, pgm);
    if (writeResult != EXIT_NO_ERRORS)
        handleError(writeResult, outputFile);

    printf("CONVERTED\n");
    free(pgm->commentLine);
    pgm->commentLine = NULL;
	free(pgm->imageData);
    pgm->imageData = NULL;
    free(pgm);
    pgm = NULL;
    return EXIT_NO_ERRORS;
}