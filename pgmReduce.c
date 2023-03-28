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
        printf("Invalid Factor Integer n\n");
        return EXIT_MISCELLANEOUS;
    }
    
    PGMImage *pgmorg = (PGMImage*)malloc(sizeof(PGMImage));
    PGMImage *pgmnew = (PGMImage*)malloc(sizeof(PGMImage));
    if (pgmorg == NULL || pgmnew == NULL) 
    {
        handleError(EXIT_MALLOC_FAILED, inputFile);
        return EXIT_MALLOC_FAILED;
    }

    int readResult = readPGM(inputFile, pgmorg);
    if (readResult != EXIT_NO_ERRORS)
    {
        handleError(readResult, inputFile);
        return readResult;
    }

    int reduceResult = reducePGM(n, pgmorg, pgmnew);
    if (reduceResult != EXIT_NO_ERRORS)
    {
        handleError(reduceResult, inputFile);
        return reduceResult;
    }
    
    int writeResult = 0;
    if (pgmnew->magicNum == MAGIC_NUMBER_ASCII_PGM)
        writeResult = writeASCII(outputFile, pgmnew);
    else 
        writeResult = writeBINARY(outputFile, pgmnew);
    if (writeResult != EXIT_NO_ERRORS)
        handleError(writeResult, outputFile);
    else
        printf("REDUCED\n");
    
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
    return writeResult;
}