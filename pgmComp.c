#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pgm.h"
#include "pgmFile.h"
#include "compare.h"
#include "error.h"

int main(int argc, char *argv[]) {
    int ar = compargc(argc, argv);
    if (ar == 100)
        return EXIT_NO_ERRORS;
    else if (ar != EXIT_NO_ERRORS)
        return ar;

    char *filename1 = argv[1];
    char *filename2 = argv[2];

    struct PGMImage *pgm1 = &(PGMImage) {
        .width = 0,
        .height = 0,
        .maxGray = 255,
        .imageData = NULL,
        .commentLine = NULL,
        .nImageBytes = 0
    };
    struct PGMImage *pgm2 = &(PGMImage) {
        .width = 0,
        .height = 0,
        .maxGray = 255,
        .imageData = NULL,
        .commentLine = NULL,
        .nImageBytes = 0
    };

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
	free(pgm1->imageData);
    free(pgm2->commentLine);
	free(pgm2->imageData);
    return EXIT_NO_ERRORS;
}