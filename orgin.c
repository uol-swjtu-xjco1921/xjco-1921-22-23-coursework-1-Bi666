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

int main(int argc, char **argv) {
  if (argc != 3) {
    printf("Usage: %s inputImage.pgm outputImage.pgm\n", argv[0]);
    return EXIT_WRONG_ARG_COUNT;
  }
  unsigned char magic_number[2] = {'0', '0'};
  unsigned short *magic_Number = (unsigned short *)magic_number;
  char *commentLine = NULL;
  unsigned int width = 0, height = 0;
  unsigned int maxGray = 255;
  unsigned char *imageData = NULL;
  FILE *inputFile = fopen(argv[1], "r");
  if (inputFile == NULL) return EXIT_BAD_INPUT_FILE;
  magic_number[0] = getc(inputFile);
  magic_number[1] = getc(inputFile);
  if (*magic_Number != MAGIC_NUMBER_ASCII_PGM) {
    fclose(inputFile);
    printf("Error: Failed to read pgm image from file %s\n", argv[1]);
    return EXIT_BAD_INPUT_FILE;
  }
  int scanCount = fscanf(inputFile, " ");
  char nextChar = fgetc(inputFile);
  if (nextChar == '#') {
    commentLine = (char *)malloc(MAX_COMMENT_LINE_LENGTH);
    char *commentString =
        fgets(commentLine, MAX_COMMENT_LINE_LENGTH, inputFile);
    if (commentString == NULL) {
      free(commentLine);
      fclose(inputFile);
      printf("Error: Failed to read pgm image from file %s\n", argv[1]);
      return EXIT_BAD_INPUT_FILE;
    }
  } else {
    ungetc(nextChar, inputFile);
  }
  scanCount = fscanf(inputFile, " %u %u %u", &(width), &(height), &(maxGray));
  if ((scanCount != 3) || (width < MIN_IMAGE_DIMENSION) ||
      (width > MAX_IMAGE_DIMENSION) || (height < MIN_IMAGE_DIMENSION) ||
      (height > MAX_IMAGE_DIMENSION) || (maxGray != 255)) {
    free(commentLine);
    fclose(inputFile);
    printf("Error: Failed to read pgm image from file %s\n", argv[1]);
    return EXIT_BAD_INPUT_FILE;
  }
  long nImageBytes = width * height * sizeof(unsigned char);
  imageData = (unsigned char *)malloc(nImageBytes);
  if (imageData == NULL) {
    free(commentLine);
    fclose(inputFile);
    printf("Error: Failed to read pgm image from file %s\n", argv[1]);
    return EXIT_BAD_INPUT_FILE;
  }
  for (unsigned char *nextGrayValue = imageData;
       nextGrayValue < imageData + nImageBytes; nextGrayValue++) {
    int grayValue = -1;
    int scanCount = fscanf(inputFile, " %u", &grayValue);
    if ((scanCount != 1) || (grayValue < 0) || (grayValue > 255)) {
      free(commentLine);
      free(imageData);
      fclose(inputFile);
      printf("Error: Failed to read pgm image from file %s\n", argv[1]);
      return EXIT_BAD_INPUT_FILE;
    }
    *nextGrayValue = (unsigned char)grayValue;
  }
  fclose(inputFile);
  FILE *outputFile = fopen(argv[2], "w");
  if (outputFile == NULL) {
    free(commentLine);
    free(imageData);
    printf("Error: Failed to write pgm image to file %s\n", argv[2]);
    return EXIT_BAD_OUTPUT_FILE;
  }
  size_t nBytesWritten =
      fprintf(outputFile, "P2\n%d %d\n%d\n", width, height, maxGray);
  if (nBytesWritten < 0) {
    free(commentLine);
    free(imageData);
    printf("Error: Failed to write pgm image to file %s\n", argv[2]);
    return EXIT_BAD_OUTPUT_FILE;
  }
  for (unsigned char *nextGrayValue = imageData;
       nextGrayValue < imageData + nImageBytes; nextGrayValue++) {
    int nextCol = (nextGrayValue - imageData + 1) % width;
    nBytesWritten =
        fprintf(outputFile, "%d%c", *nextGrayValue, (nextCol ? ' ' : '\n'));
    if (nBytesWritten < 0) {
      free(commentLine);
      free(imageData);
      printf("Error: Failed to write pgm image to file %s\n", argv[2]);
      return EXIT_BAD_OUTPUT_FILE;
    }
  }
  return EXIT_NO_ERRORS;
}