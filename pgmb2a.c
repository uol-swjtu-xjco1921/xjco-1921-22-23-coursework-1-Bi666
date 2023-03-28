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
    pgm->width = 0;
    pgm->height = 0;
    pgm->maxGray = 255;
    pgm->imageData = NULL;
    pgm->commentLine = NULL;
    pgm->nImageBytes = 0;
    pgm->magicNum = MAGIC_NUMBER_RAW_PGM;

    int readResult = readPGM(inputFile, pgm);
    if (readResult != EXIT_NO_ERRORS)
    {
        handleError(readResult, inputFile);
        return readResult;
    }

    int writeResult = writeASCII(outputFile, pgm);
    if (writeResult != EXIT_NO_ERRORS)
        handleError(writeResult, outputFile);
    else printf("CONVERTED\n");
    free(pgm->commentLine);
    pgm->commentLine = NULL;
	free(pgm->imageData);
    pgm->imageData = NULL;
    free(pgm);
    pgm = NULL;
    return EXIT_NO_ERRORS;
}