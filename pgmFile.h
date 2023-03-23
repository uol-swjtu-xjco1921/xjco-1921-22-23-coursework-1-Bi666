#include <stdio.h>
#include "pgm.h"

int judgeargc(int argc, char **argv);
int magic(FILE *inputFile, PGMImage *pgm);
int dimen(FILE *inputFile, PGMImage *pgm, unsigned short *magic_Number);
int readBINARY(FILE *inputFile, PGMImage *pgm);
int readASCII(FILE *inputFile, PGMImage *pgm);
int readPGM(const char *filename, PGMImage *pgm);
int writeASCII(const char *filename, PGMImage *pgm);
int writeBINARY(const char *filename, PGMImage *pgm);