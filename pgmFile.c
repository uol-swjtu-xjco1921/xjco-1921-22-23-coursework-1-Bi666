#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pgm.h"
#include "pgmFile.h"

int judgeargc(int argc, char **argv)
{
    if (argc == 1)
    {
        printf("Usage: %s inputImage.pgm outputImage.pgm\n", argv[0]);
        return 2;
    }
    if (argc != 3)
    {
        printf("ERROR: Bad Argument Count\n");
        return EXIT_WRONG_ARG_COUNT;
    }
    return EXIT_NO_ERRORS;
}

int readPGM(const char *filename, PGMImage *pgm)
{
    FILE *inputFile = fopen(filename, "r");
    if (inputFile == NULL)
        return EXIT_BAD_FILE_NAME;
    // Read magic number
    int dataResult = magic(inputFile, pgm);
    fclose(inputFile);
    if (dataResult != EXIT_NO_ERRORS)
        return dataResult;
    return EXIT_NO_ERRORS;
}

int magic(FILE *inputFile, PGMImage *pgm)
{
    unsigned char magic_number[2] = {'0','0'};
    unsigned short *magic_Number = (unsigned short *) magic_number;
    magic_number[0] = getc(inputFile);
    magic_number[1] = getc(inputFile);
    // Check magic number
    if (*magic_Number != MAGIC_NUMBER_RAW_PGM && *magic_Number != MAGIC_NUMBER_ASCII_PGM)
        return EXIT_BAD_MAGIC_NUMBER;
    if (pgm->magicNum != 0 && pgm->magicNum != *magic_Number)
        return EXIT_BAD_MAGIC_NUMBER;
    pgm->magicNum = *magic_Number;
    pgm->commentLine = (char *) malloc(MAX_COMMENT_LINE_LENGTH + 1);
    if (pgm->commentLine == NULL)
        return EXIT_MALLOC_FAILED;
    int dataResult = dimen(inputFile, pgm);
    if (dataResult != EXIT_NO_ERRORS)
    {
        free(pgm->commentLine);
        pgm->commentLine = NULL;
        free(pgm);
        pgm = NULL;
        return dataResult;
    }
    return EXIT_NO_ERRORS;
}

int dimen(FILE *inputFile, PGMImage *pgm)
{
    int scanCount = fscanf(inputFile, " ");
    // Read comment line if present
    char nextChar = fgetc(inputFile);
    if (nextChar == '#')
    {
        char *commentString = fgets(pgm->commentLine, MAX_COMMENT_LINE_LENGTH + 1, inputFile);
        if (commentString == NULL || strlen(commentString) <= 1 || strlen(commentString) > 127)
            return EXIT_COMMENT_LINE;
        for (int i = 0; i < strlen(commentString); i++)
            if (commentString[i] < 0 || commentString[i] > 127)
                return EXIT_COMMENT_LINE;
    }
    else ungetc(nextChar, inputFile);
    // Read image dimensions and max gray value
    scanCount = fscanf(inputFile, " %u %u %u", &(pgm->width), &(pgm->height), &(pgm->maxGray));
    if  (
            (scanCount != 3                )   ||
            (pgm->width  < MIN_IMAGE_DIMENSION  )   ||
            (pgm->width  > MAX_IMAGE_DIMENSION  )   ||
            (pgm->height < MIN_IMAGE_DIMENSION  )   ||
            (pgm->height > MAX_IMAGE_DIMENSION  )
        )
		return EXIT_BAD_DIMENSIONS;
    if (pgm->maxGray  != 255)
		return EXIT_BAD_MAXGRAY;
    pgm->nImageBytes = (pgm->width) * (pgm->height) * sizeof(unsigned char);
    // Allocate memory for image data
    pgm->imageData = (unsigned char *)malloc(pgm->nImageBytes);
    if (pgm->imageData == NULL)
		return EXIT_MALLOC_FAILED;
    if (pgm->magicNum == MAGIC_NUMBER_ASCII_PGM)
    {
        int dataResult = readASCII(inputFile, pgm);
        if (dataResult != EXIT_NO_ERRORS)
            return dataResult;
    }
    else if (pgm->magicNum == MAGIC_NUMBER_RAW_PGM)
    {
        int dataResult = readBINARY(inputFile, pgm);
        if (dataResult != EXIT_NO_ERRORS)
            return dataResult;
    }
    return EXIT_NO_ERRORS;
}

int readASCII(FILE *inputFile, PGMImage *pgm){
    for (unsigned char *nextGrayValue = pgm->imageData; nextGrayValue < pgm->imageData + pgm->nImageBytes; nextGrayValue++)
    {
        int grayValue = -1;
		int scanCount = fscanf(inputFile, " %u", &grayValue);
		if ((scanCount != 1) || (grayValue < 0) || (grayValue > 255))
		{
			free(pgm->imageData);
            pgm->imageData = NULL;
			return EXIT_BAD_DATA;
		}
		*nextGrayValue = (unsigned char) grayValue;
	}
    int grayValue = -1;
    int scanCount = fscanf(inputFile, " %u", &grayValue);
    if (scanCount == 1)
		{
			free(pgm->imageData);
            pgm->imageData = NULL;
			return EXIT_BAD_DATA;
		}
    return EXIT_NO_ERRORS;
}

int readBINARY(FILE *inputFile, PGMImage *pgm)
{
    unsigned char line;
    fread(&line, sizeof(unsigned char), 1, inputFile);
    size_t nBytes = fread(pgm->imageData, sizeof(unsigned char), pgm->width * pgm->height, inputFile);
    size_t empty = fread(pgm->imageData, sizeof(unsigned char), 1, inputFile);
    if (nBytes != pgm->width * pgm->height || empty != 0)
    {
        free(pgm->imageData);
        pgm->imageData = NULL;
        return EXIT_BAD_DATA;
    }
    return EXIT_NO_ERRORS;
}

int writeASCII(const char *filename, PGMImage *pgm)
{
    FILE *outputFile = fopen(filename, "w");
    if (outputFile == NULL)
		return EXIT_OUTPUT_FAILED;
	size_t nBytesWritten = fprintf(outputFile, "P2\n%d %d\n%d\n", pgm->width, pgm->height, pgm->maxGray);
	if (nBytesWritten < 0)
	{
		fclose(outputFile);
		return EXIT_OUTPUT_FAILED;
	}
	for (unsigned char *nextGrayValue = pgm->imageData; nextGrayValue < pgm->imageData + pgm->nImageBytes; nextGrayValue++)
	{
		int nextCol = (nextGrayValue - pgm->imageData + 1) % (pgm->width);
		nBytesWritten = fprintf(outputFile, "%d%c", *nextGrayValue, (nextCol ? ' ' : '\n') );
		if (nBytesWritten < 0)
		{
			fclose(outputFile);
			return EXIT_OUTPUT_FAILED;
		}
	}
    fclose(outputFile);
    return EXIT_NO_ERRORS;
}

int writeBINARY(const char *filename, PGMImage *pgm)
{
    FILE *outputFile = fopen(filename, "w");
    if (outputFile == NULL)
		return EXIT_OUTPUT_FAILED;
	size_t nBytesWritten = fprintf(outputFile, "P5\n%d %d\n%d\n", pgm->width, pgm->height, pgm->maxGray);
	if (nBytesWritten < 0)
	{
		fclose(outputFile);
		return EXIT_OUTPUT_FAILED;
	}
    for (unsigned char *nextGrayValue = pgm->imageData; nextGrayValue < pgm->imageData + pgm->nImageBytes; nextGrayValue++)
	{
        nBytesWritten = fputc(*nextGrayValue, outputFile);
		if (nBytesWritten < 0)
		{
			fclose(outputFile);
			return EXIT_OUTPUT_FAILED;
		}
	}
    fclose(outputFile);
    return EXIT_NO_ERRORS;
}