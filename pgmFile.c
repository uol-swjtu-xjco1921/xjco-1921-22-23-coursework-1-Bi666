#include <stdio.h>
#include <stdlib.h>
#include "pgm.h"
#include "pgmFile.h"

int readPGM(const char *filename, PGMImage *pgm)
{
    // Read magic number
    unsigned char magic_number[2] = {'0','0'};
    unsigned short *magic_Number = (unsigned short *) magic_number;
    FILE *inputFile = fopen(filename, "r");
    if (inputFile == NULL)
        return EXIT_BAD_FILE_NAME;
    magic_number[0] = getc(inputFile);
    magic_number[1] = getc(inputFile);
    // Check magic number
    if (*magic_Number != MAGIC_NUMBER_ASCII_PGM)
    {
        fclose(inputFile);
        return EXIT_BAD_MAGIC_NUMBER;
    }
    int scanCount = fscanf(inputFile, " ");
    // Read comment line if present
    char nextChar = fgetc(inputFile);
    if (nextChar == '#')
    {
        pgm->commentLine = (char *) malloc(MAX_COMMENT_LINE_LENGTH);
        char *commentString = fgets(pgm->commentLine, MAX_COMMENT_LINE_LENGTH, inputFile);
        if (commentString == NULL)
        {
            free(pgm->commentLine);
            fclose(inputFile);
            return EXIT_COMMENT_LINE;
        }
    }
    else
    {
        ungetc(nextChar, inputFile);
    }
    // Read image dimensions and max gray value
    scanCount = fscanf(inputFile, " %u %u %u", &(pgm->width), &(pgm->height), &(pgm->maxGray));
    if  (
            (scanCount != 3                )   ||
            (pgm->width  < MIN_IMAGE_DIMENSION  )   ||
            (pgm->width  > MAX_IMAGE_DIMENSION  )   ||
            (pgm->height < MIN_IMAGE_DIMENSION  )   ||
            (pgm->height > MAX_IMAGE_DIMENSION  )
        )
    {
		free(pgm->commentLine);
		fclose(inputFile);
		return EXIT_BAD_DIMENSIONS;
	}
    if (pgm->maxGray  != 255) {
        free(pgm->commentLine);
		fclose(inputFile);
		return EXIT_BAD_MAXGRAY;
    }
    long nImageBytes = (pgm->width) * (pgm->height) * sizeof(unsigned char);
    // Allocate memory for image data
    pgm->imageData = (unsigned char *)malloc(nImageBytes);
    if (pgm->imageData == NULL)
	{
		free(pgm->commentLine);
		fclose(inputFile);
		return EXIT_MALLOC_FAILED;
	}

    for (unsigned char *nextGrayValue = pgm->imageData; nextGrayValue < pgm->imageData + nImageBytes; nextGrayValue++)
    {
        int grayValue = -1;
		int scanCount = fscanf(inputFile, " %u", &grayValue);
		if ((scanCount != 1) || (grayValue < 0) || (grayValue > 255))
		{
			free(pgm->commentLine);
			free(pgm->imageData);
			fclose(inputFile);
			return EXIT_BAD_DATA;
		}
		*nextGrayValue = (unsigned char) grayValue;
	}
    fclose(inputFile);
}

int writePGM(const char *filename, PGMImage *pgm)
{
    FILE *outputFile = fopen(filename, "w");
    if (outputFile == NULL)
	{ 
		free(pgm->commentLine);
		free(pgm->imageData);
		return EXIT_OUTPUT_FAILED;
	}
	size_t nBytesWritten = fprintf(outputFile, "P2\n%d %d\n%d\n", pgm->width, pgm->height, pgm->maxGray);
	if (nBytesWritten < 0)
	{
		free(pgm->commentLine);
		free(pgm->imageData);
		return EXIT_OUTPUT_FAILED;
	}
    long nImageBytes = (pgm->width) * (pgm->height) * sizeof(unsigned char);
	for (unsigned char *nextGrayValue = pgm->imageData; nextGrayValue < pgm->imageData + nImageBytes; nextGrayValue++)
	{
		int nextCol = (nextGrayValue - pgm->imageData + 1) % (pgm->width);
		nBytesWritten = fprintf(outputFile, "%d%c", *nextGrayValue, (nextCol ? ' ' : '\n') );
		if (nBytesWritten < 0)
		{
			free(pgm->commentLine);
			free(pgm->imageData);
			return EXIT_OUTPUT_FAILED;
		}
	}
	return EXIT_NO_ERRORS;
}