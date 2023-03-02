#ifndef PGM_H
#define PGM_H

#include <stdio.h>
#include <stdlib.h>

#define EXIT_NO_ERRORS 0
#define EXIT_WRONG_ARG_COUNT 1
#define EXIT_BAD_INPUT_FILE 2
#define EXIT_BAD_OUTPUT_FILE 3

#define MAGIC_NUMBER_RAW_PGM 0x3550
#define MAGIC_NUMBER_ASCII_PGM 0x3250
#define MIN_IMAGE_DIMENSION 1
#define MAX_IMAGE_DIMENSION 65536
#define MAX_COMMENT_LINE_LENGTH 128

typedef struct PGMImage {
    unsigned int width;
    unsigned int height;
    unsigned int maxGray;
    unsigned char *imageData;
    char *commentLine;
} PGMImage;

#endif