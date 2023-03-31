#include <stdio.h>
#include <stdlib.h>
#include "pgm.h"
#include "error.h"
#include "pgmFile.h"

int main(int argc, char **argv)
{
    //Check that the number of command lines is correct
    int ar = judgeargc(argc, argv);
    if (ar == 2)
        return EXIT_NO_ERRORS;
    else if (ar != EXIT_NO_ERRORS)
        return ar;

    //Save the filename read in and written out
    char *inputFile = argv[1];
    char *outputFile = argv[2];
    
    //Dynamically allocated the struct
    PGMImage *pgmE = (PGMImage*)malloc(sizeof(PGMImage));
    if (pgmE == NULL) 
    {
        handleError(EXIT_MALLOC_FAILED, inputFile);
        return EXIT_MALLOC_FAILED;
    }
    //Initialized the struct
    pgmE->width = 0;
    pgmE->height = 0;
    pgmE->maxGray = 255;
    pgmE->imageData = NULL;
    pgmE->commentLine = NULL;
    pgmE->nImageBytes = 0;
    pgmE->magicNum = 0;

    //Processing pgm file data read in
    int readResult = readPGM(inputFile, pgmE);
    if (readResult != EXIT_NO_ERRORS)
    {
        handleError(readResult, inputFile);
        return readResult;
    }

    //Writing out the output file
    int writeResult = 0;
    if (pgmE->magicNum == MAGIC_NUMBER_ASCII_PGM)
        writeResult = writeASCII(outputFile, pgmE);
    else 
        writeResult = writeBINARY(outputFile, pgmE);

    if (writeResult != EXIT_NO_ERRORS)
        handleError(writeResult, outputFile);
    else
        printf("ECHOED\n");

    //Release the memory allocations
    free(pgmE->commentLine);
    pgmE->commentLine = NULL;
	free(pgmE->imageData);
    pgmE->imageData = NULL;
    free(pgmE);
    pgmE = NULL;
    return writeResult;
}