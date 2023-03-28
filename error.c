#include <stdio.h>
#include <stdlib.h>
#include "pgm.h"
#include "error.h"

void handleError(char readResult, const char *filename)
{
    if (readResult == EXIT_BAD_FILE_NAME)
        printf("ERROR: Bad File Name (%s)\n", filename);
    if (readResult == EXIT_BAD_MAGIC_NUMBER)
        printf("ERROR: Bad Magic Number (%s)\n", filename);
    if (readResult == EXIT_COMMENT_LINE)
        printf("ERROR: Bad Comment Line (%s)\n", filename);
    if (readResult == EXIT_BAD_DIMENSIONS)
        printf("ERROR: Bad Dimensions (%s)\n", filename);
    if (readResult == EXIT_BAD_MAXGRAY)
        printf("ERROR: Bad Max Gray Value (%s)\n", filename);
    if (readResult == EXIT_MALLOC_FAILED)
        printf("ERROR: Image Malloc Failed\n");
    if (readResult == EXIT_BAD_DATA)
        printf("ERROR: Bad Data (%s)\n", filename);
    if (readResult == EXIT_OUTPUT_FAILED)
        printf("ERROR: Output Failed (%s)\n", filename);
}