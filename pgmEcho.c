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
    
    PGMImage *pgmE = (PGMImage*)malloc(sizeof(PGMImage));
    if (pgmE == NULL) 
    {
        handleError(EXIT_MALLOC_FAILED, inputFile);
        return EXIT_MALLOC_FAILED;
    }

    int readResult = readPGM(inputFile, pgmE);
    if (readResult != EXIT_NO_ERRORS)
    {
        handleError(readResult, inputFile);
        return readResult;
    }

    int writeResult = writeASCII(outputFile, pgmE);
    if (writeResult != EXIT_NO_ERRORS)
        handleError(writeResult, outputFile);
        
    free(pgmE->commentLine);
    pgmE->commentLine = NULL;
	free(pgmE->imageData);
    pgmE->imageData = NULL;
    free(pgmE);
    pgmE = NULL;
    return writeResult;
}