#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pgm.h"
#include "pgmFile.h"
#include "compare.h"
#include "error.h"

int main(int argc, char *argv[]) 
{
    //Check that the number of command lines is correct
    int ar = compargc(argc, argv);
    if (ar == 2)
        return EXIT_NO_ERRORS;
    else if (ar != EXIT_NO_ERRORS)
        return ar;

    //Save the filename read in and written out
    char *filename1 = argv[1];
    char *filename2 = argv[2];

    //Dynamically allocated the struct
    PGMImage *pgm1 = (PGMImage*)malloc(sizeof(PGMImage));
    PGMImage *pgm2 = (PGMImage*)malloc(sizeof(PGMImage));
    if (pgm1 == NULL || pgm2 == NULL) 
    {
        handleError(EXIT_MALLOC_FAILED, filename1);
        return EXIT_MALLOC_FAILED;
    }
    //Initialized the struct
    pgm1->width = 0;
    pgm1->height = 0;
    pgm1->maxGray = 255;
    pgm1->imageData = NULL;
    pgm1->commentLine = NULL;
    pgm1->nImageBytes = 0;
    pgm1->magicNum = 0;
    pgm1->width = 0;
    pgm1->height = 0;
    pgm1->maxGray = 255;
    pgm1->imageData = NULL;
    pgm1->commentLine = NULL;
    pgm1->nImageBytes = 0;
    pgm1->magicNum = 0;

    //Processing pgm file data read in
    int readResult = readPGM(filename1, pgm1);
    if (readResult != EXIT_NO_ERRORS)
    {
        handleError(readResult, filename1);
        return readResult;
    }
    readResult = readPGM(filename2, pgm2);
    if (readResult != EXIT_NO_ERRORS)
    {
        handleError(readResult, filename1);
        return readResult;
    }
    
    //Perform the comparison step
    comparePGM(pgm1, pgm2);

    //Release the memory allocations
    free(pgm1->commentLine);
    pgm1->commentLine = NULL;
	free(pgm1->imageData);
    pgm1->imageData = NULL;
    free(pgm2->commentLine);
    pgm2->commentLine = NULL;
	free(pgm2->imageData);
    pgm2->imageData = NULL;
    free(pgm2);
    pgm2 = NULL;
    return EXIT_NO_ERRORS;
}