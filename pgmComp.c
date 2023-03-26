#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pgm.h"
#include "pgmFile.h"
#include "compare.h"
#include "error.h"

int main(int argc, char *argv[]) {
    int ar = compargc(argc, argv);
    if (ar == 2)
        return EXIT_NO_ERRORS;
    else if (ar != EXIT_NO_ERRORS)
        return ar;

    char *filename1 = argv[1];
    char *filename2 = argv[2];

    struct PGMImage *pgm1 = NULL;
    struct PGMImage *pgm2 = NULL;

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
    
    comparePGM(pgm1, pgm2);

    free(pgm1->commentLine);
    pgm1->commentLine = NULL;
	free(pgm-1>imageData);
    pgm1->imageData = NULL;
    free(pgm2->commentLine);
    pgm2->commentLine = NULL;
	free(pgm2->imageData);
    pgm2->imageData = NULL;
    free(pgm2);
    pgm2 = NULL;
    return EXIT_NO_ERRORS;
}