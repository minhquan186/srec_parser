/******************************************************************************
*                                   INCLUDE
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "my_srec.h"

/******************************************************************************
*                                   DEFINITION
******************************************************************************/
#define INFILE "Input/Srec1.txt"
#define OUTFILE "output.txt"

/******************************************************************************
*                                   CODE
******************************************************************************/

uint8_t main()
{
    int8_t tmp[MAX_BUFFER];
    
    FILE *inputFile = NULL;
    FILE *outputFile = NULL;

    /* Open input file */
    inputFile = fopen(INFILE, "r+");
    /* Open file to write the output */
    outputFile = fopen(OUTFILE, "w+");
    
    if (inputFile == NULL || outputFile == NULL)
    {
        printf("Cannot open file!\n");
        fclose(inputFile);
        fclose(outputFile);
    }
    else
    {
        printf("\nParsing file...\n");
        /* Call out the parser */
        srecParser(inputFile, outputFile);
        /* Print out if parse process is successful */     
        printf("\nParsing completed! Check output file at output.txt\n");
    }

    /* Close files when done */
    fclose(inputFile);
    fclose(outputFile);
    printf("\nClosing file...\n");
    return 0;
}

/******************************************************************************
*                                   END
******************************************************************************/
