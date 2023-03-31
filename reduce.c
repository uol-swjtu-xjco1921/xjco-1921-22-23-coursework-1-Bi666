#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "compare.h"
#include "pgm.h"

int reduargc(int argc, char **argv) {
  // No commandline arguments are entered
  if (argc == 1) {
    printf("Usage: %s inputImage.pgm reduction_factor outputImage.pgm\n",
           argv[0]);
    return 2;
  }
  // Nnumber of commandline does not match
  if (argc != 4) {
    printf("ERROR: Bad Argument Count\n");
    return EXIT_WRONG_ARG_COUNT;
  }
  return EXIT_NO_ERRORS;
}

int reducePGM(int n, PGMImage *pgmorg, PGMImage *pgmnew) {
  // Write dimension to the new file
  pgmnew->magicNum = pgmorg->magicNum;
  pgmnew->maxGray = 255;
  pgmnew->width = (pgmorg->width - 1) / n + 1;
  pgmnew->height = (pgmorg->height - 1) / n + 1;
  ;
  pgmnew->nImageBytes =
      (pgmnew->width) * (pgmnew->height) * sizeof(unsigned char);
  pgmnew->imageData = (unsigned char *)malloc(pgmnew->nImageBytes);
  if (pgmnew->imageData == NULL) {
    free(pgmorg->commentLine);
    pgmorg->commentLine = NULL;
    free(pgmorg->imageData);
    pgmorg->imageData = NULL;
    free(pgmorg);
    pgmorg = NULL;
    free(pgmnew);
    pgmnew = NULL;
    return EXIT_MALLOC_FAILED;
  }
  // Write data into the new file
  unsigned char *next = pgmnew->imageData;
  for (int row = 0; row < pgmorg->height; row += n)
    for (int col = 0; col < pgmorg->width; col += n) {
      unsigned char *nextGrayValue =
          pgmorg->imageData + row * pgmorg->width + col;
      *next = *nextGrayValue;
      next++;
    }
  return EXIT_NO_ERRORS;
}