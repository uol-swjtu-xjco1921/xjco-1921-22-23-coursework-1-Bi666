#include <stdio.h>
#include <stdlib.h>
#include "pgm.h"
#include "error.h"
#include "pgmFile.h"
#include "reduce.h"

int main(int argc, char **argv)
{
    //Check that the number of command lines is correct
    int ar = reduargc(argc, argv);
    if (ar == 2)
        return EXIT_NO_ERRORS;
    else if (ar != EXIT_NO_ERRORS)
        return ar;

    //Save the filename read in and written out
    char *inputFile = argv[1];
    char *outputFile = argv[3];
    //Convert the factor to type of int
    char *endptr;
    int n = strtol(argv[2], &endptr, 10);
    if (*endptr != '\0' || n <= 0) 
    {
        printf("ERROR: Miscellaneous (Invalid Factor Integer n)\n");
        return EXIT_MISCELLANEOUS;
    }
    
    //Dynamically allocated the struct
    PGMImage *pgmorg = (PGMImage*)malloc(sizeof(PGMImage));
    if (pgmorg == NULL) 
    {
        handleError(EXIT_MALLOC_FAILED, inputFile);
        return EXIT_MALLOC_FAILED;
    }
    //Initialized the struct
    pgmorg->width = 0;
    pgmorg->height = 0;
    pgmorg->maxGray = 255;
    pgmorg->imageData = NULL;
    pgmorg->commentLine = NULL;
    pgmorg->nImageBytes = 0;
    pgmorg->magicNum = 0;

    //Processing pgm file data read in
    int readResult = readPGM(inputFile, pgmorg);
    if (readResult != EXIT_NO_ERRORS)
    {
        free(pgmorg);
        pgmorg = NULL;
        handleError(readResult, inputFile);
        return readResult;
    }

    PGMImage *pgmnew = (PGMImage*)malloc(sizeof(PGMImage));
    if (pgmnew == NULL) 
    {
        handleError(EXIT_MALLOC_FAILED, inputFile);
        return EXIT_MALLOC_FAILED;
    }
    pgmnew->width = 0;
    pgmnew->height = 0;
    pgmnew->maxGray = 255;
    pgmnew->imageData = NULL;
    pgmnew->commentLine = NULL;
    pgmnew->nImageBytes = 0;
    pgmnew->magicNum = 0;

    int reduceResult = reducePGM(n, pgmorg, pgmnew);
    if (reduceResult != EXIT_NO_ERRORS)
    {
        free(pgmorg->commentLine);
        pgmorg->commentLine = NULL;
        free(pgmorg->imageData);
        pgmorg->imageData = NULL;
        free(pgmorg);
        pgmorg = NULL;
        free(pgmnew);
        pgmnew = NULL;
        handleError(reduceResult, inputFile);
        return reduceResult;
    }
    
    //Writing out the output file
    int writeResult = 0;
    if (pgmnew->magicNum == MAGIC_NUMBER_ASCII_PGM)
        writeResult = writeASCII(outputFile, pgmnew);
    else 
        writeResult = writeBINARY(outputFile, pgmnew);
    if (writeResult != EXIT_NO_ERRORS)
        handleError(writeResult, outputFile);
    else
        printf("REDUCED\n");
    
    //Release the memory allocations
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