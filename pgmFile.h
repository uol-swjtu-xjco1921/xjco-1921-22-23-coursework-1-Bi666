#include <stdio.h>
#include "pgm.h"

int judgeargc(int argc, char **argv);
int magic(FILE *inputFile, PGMImage *pgm);
int dimen(FILE *inputFile, PGMImage *pgm);
int readPGM(const char *filename, PGMImage *pgm);
int writePGM(const char *filename, PGMImage *pgm);