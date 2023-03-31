#include "compare.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pgm.h"

int compargc(int argc, char **argv) {
  // No commandline arguments are entered
  if (argc == 1) {
    printf("Usage: %s inputImage.pgm inputImage.pgm\n", argv[0]);
    return 2;
  }
  // Nnumber of commandline does not match
  if (argc != 3) {
    printf("ERROR: Bad Argument Count\n");
    return EXIT_WRONG_ARG_COUNT;
  }
  return EXIT_NO_ERRORS;
}

void comparePGM(PGMImage *pgm1, PGMImage *pgm2) {
  // Determines two files have same dimensions
  if (pgm1->width != pgm2->width || pgm1->height != pgm2->height) {
    printf("DIFFERENT\n");
    return;
  }
  unsigned char *nextGrayValue1 = pgm1->imageData;
  unsigned char *nextGrayValue2 = pgm2->imageData;
  // Determine if each data is same
  for (; nextGrayValue1 < pgm1->imageData + pgm1->nImageBytes;
       nextGrayValue1++, nextGrayValue2++) {
    if (*nextGrayValue1 != *nextGrayValue2) {
      printf("DIFFERENT\n");
      return;
    }
  }
  printf("IDENTICAL\n");
}