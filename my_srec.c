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

#define EXTRA_BYTE 1u

#define PAIR_OF_BYTES 2u

#define S_BYTE 0u
#define TYPE_BYTE 1u

#define ADDRESS_OFFSET 4u
#define DATA_OFFSET 3u

#define S1_ADDRESS_SIZE 4u
#define S2_ADDRESS_SIZE 6u
#define S3_ADDRESS_SIZE 8u

#define MAX_ADDRESS_SIZE 8u
#define MAX_DATA_SIZE 255u

/******************************************************************************
*                                   PROTOTYPE
******************************************************************************/

/* Show address and data */
static void showData(FILE *outFile, int8_t string[], uint8_t type);

/* Function to convert ASCII to Decimal on given character */
static uint8_t convert_ASCII_to_Dec(int8_t temp);

/* Function to calculate HEX value in decimal */
static uint16_t HexToDec(int8_t string[]);

/******************************************************************************
*                                   CODE
******************************************************************************/

/* Show address and data */
static void showData(FILE *outFile, int8_t string[], uint8_t type)
{
    uint8_t index = 0;
    uint8_t length = 0;
    int8_t address[MAX_ADDRESS_SIZE];
    int8_t data[MAX_DATA_SIZE];

    length = strlen(string);
    /* If the data is S1 */
    if (type == 1)
    {
        for (index = ADDRESS_OFFSET; index < (ADDRESS_OFFSET + S1_ADDRESS_SIZE); index++)
        {
            address[index - ADDRESS_OFFSET] = string[index];
        }

        address[index - ADDRESS_OFFSET] = '\0';
        fputs(address, outFile);

        for (index = ADDRESS_OFFSET + S1_ADDRESS_SIZE; index < length - DATA_OFFSET; index++)
        {
            data[index - ADDRESS_OFFSET - S1_ADDRESS_SIZE] = string[index];
        }

        data[index - ADDRESS_OFFSET - S1_ADDRESS_SIZE] = '\n';
        fputs("  ", outFile);
        fputs(data, outFile);
    }
    /* If the data is S2 */
    else if (type == 2)
    {
        for (index = ADDRESS_OFFSET; index < (ADDRESS_OFFSET + S2_ADDRESS_SIZE); index++)
        {
            address[index - ADDRESS_OFFSET] = string[index];
        }

        address[index - ADDRESS_OFFSET] = '\0';
        fputs(address, outFile);

        for (index = ADDRESS_OFFSET + S2_ADDRESS_SIZE; index < length - DATA_OFFSET; index++)
        {
            data[index - ADDRESS_OFFSET - S2_ADDRESS_SIZE] = string[index];
        }

        data[index - ADDRESS_OFFSET - S2_ADDRESS_SIZE] = '\n';
        fputs("  ", outFile);
        fputs(data, outFile);
    }
    /* If the line is S3 */
    else if (type == 3)
    {
        for (index = ADDRESS_OFFSET; index < (ADDRESS_OFFSET + S3_ADDRESS_SIZE); index++)
        {
            address[index - ADDRESS_OFFSET] = string[index];
        }

        address[index - ADDRESS_OFFSET - EXTRA_BYTE] = '\0';
        fputs(address, outFile);

        for (index = ADDRESS_OFFSET + S3_ADDRESS_SIZE; index < length - DATA_OFFSET; index++)
        {
            data[index - ADDRESS_OFFSET - S3_ADDRESS_SIZE] = string[index];
        }

        data[index - ADDRESS_OFFSET - S3_ADDRESS_SIZE] = '\n';
        data[index - ADDRESS_OFFSET - S3_ADDRESS_SIZE + 1] = '\0';
        fputs("  ", outFile);
        fputs(data, outFile);
    }
    else
    {
        /* do nothing */
    }
}

/* Function to convert ASCII to Decimal on given character */
static uint8_t convert_ASCII_to_Dec(int8_t temp)
{
    uint8_t result = 0;

    if (temp >= '0' && temp <= '9')
    {
        result = temp - '0';
    }
    else if (temp >= 'A' && temp <= 'F')
    {
        result = temp - 'A' + 10;
    }
    else
    {
        result  = -1;
    }
    return result;
}

/* Function to calculate HEX value in decimal */
static uint16_t HexToDec(int8_t string[])
{
    uint16_t result = 0;
    int16_t index = 0;
    uint8_t length = 0;
    uint8_t temp = 0;
    int8_t tempString[8];
    uint8_t base = 0;

    length = strlen(string);
    strcpy(tempString, string);

    for (index = length - EXTRA_BYTE; index >= 0; index--)
    {
        temp = convert_ASCII_to_Dec(tempString[index]);
        result += temp << (4 * base);
        base++;
    }

    return result;
}

/* Funcrtion to check S error on given line */
srec_error checkS(int8_t string[])
{
    srec_error flag = NO_ERROR;
    int8_t tmp[MAX_BUFFER];

    if (string[S_BYTE] != 'S')
    {
        flag = S_ERROR;
    }
    else
    {
        flag = NO_ERROR;
    }

    return flag;
}

/* Function to check Type error on given line */
srec_error checkType(int8_t string[])
{
    srec_error flag = NO_ERROR;
    int8_t check = 0;

    check = convert_ASCII_to_Dec(string[TYPE_BYTE]);

    if (check >= 0 && check <= 9)
    {
        flag = NO_ERROR;
    }
    else
    {
        flag = TYPE_ERROR;
    }

    return flag;
}

/* Function to check whether all the characters on given line are HEX or not */
srec_error checkHex(int8_t string[])
{
    srec_error flag = NO_ERROR;
    uint8_t index = 0;
    int8_t check = 0;
    uint8_t length = strlen(string);

    for (index = 1; index < length - EXTRA_BYTE; index++)
    {
        check = convert_ASCII_to_Dec(string[index]);
        if (check < 0 || check > 15)
        {
            flag = HEX_ERROR;
        }
        else
        {
            /* do nothing */
        }
    }

    return flag;
}

/* Function to check byte count on given line */
srec_error checkByteCount(int8_t *string)
{
    srec_error flag = NO_ERROR;
    uint8_t index = 0;
    uint8_t byteCount = 0;
    uint8_t check = 0;
    uint8_t length = 0;
    int8_t temp[PAIR_OF_BYTES + 1];

    /* Length of given line */
    length = strlen(string);

    /* For every 2 bytes increase check by 1 */
    for (index = ADDRESS_OFFSET; index < length - EXTRA_BYTE; index += PAIR_OF_BYTES)
    {
        check++;
    }
    temp[PAIR_OF_BYTES] = '\0';
    sscanf(&string[PAIR_OF_BYTES], "%2c", &temp);
    /* Byte Count value from the line */
    byteCount = HexToDec(temp);

    /* Check condition for flag value */
    if (byteCount == check)
    {
        flag = NO_ERROR;
    }
    else
    {
        flag = BYTE_COUNT_ERROR;
    }

    return flag;
}

/* Function to check the check sum of given line */
srec_error checkSum(int8_t string[])
{
    srec_error flag = NO_ERROR;
    uint8_t index = 0;
    uint8_t checkSum = 0;
    uint16_t sum = 0;
    uint8_t length = 0;
    int16_t temp = 0;
    int8_t tempString[MAX_BUFFER];
    int8_t checkSumBytes[MAX_BUFFER];

    length = strlen(string);

    sscanf(&string[length - DATA_OFFSET], "%2c", &checkSumBytes[0]);
    /* Get the checksum value from the line */
    checkSum = HexToDec(checkSumBytes);
 
    /* Calculate the checksum value */
    for (index = 2; index < (length - DATA_OFFSET); index += 2)
    {
        sscanf(&string[index], "%2c", &tempString[0]);
        temp = HexToDec(tempString);
        sum += temp;
    }

    sum = ~sum;
    sum = (sum & 0xFF);
    
    /* Check condition for flag value */
    if (sum == checkSum)
    {
        flag = NO_ERROR;
    }
    else
    {
        flag = CHECK_SUM_ERROR;
    }
    return flag;
}

/* Function to check line count of given file */
srec_error checkLineCount(int8_t string[], uint16_t count)
{
    srec_error flag = NO_ERROR;
    uint16_t lineCount = 0;
    int8_t temp[MAX_BUFFER];
    uint8_t type = 0;

    /* Get type of line */
    type = convert_ASCII_to_Dec(string[TYPE_BYTE]);
   
    /* If type is 5 */
    if (type == 5)
    {
        sscanf(&string[ADDRESS_OFFSET], "%4c", &temp[0]);
        /* Get line count value from the line */
        lineCount = HexToDec(temp);
        /* Check whether the line count value is correct */
        if (lineCount != count)
        {
            flag = LINE_COUNT_ERROR;
        }
        else
        {
            flag = NO_ERROR;
        }
    }

    /* If type is 6 */
    else if (type == 6)
    {
        sscanf(&string[ADDRESS_OFFSET], "%6c", &temp[0]);
        /* Get line count value from the line */
        lineCount = HexToDec(temp);

        /* Check whether the line count value is correct */
        if (lineCount != count)
        {
            flag = LINE_COUNT_ERROR;
        }
        else
        {
            flag = NO_ERROR;
        }
    }

    else
    {
        /* do nothing */
    }
    return flag;
}

/* Function to check Header of file */
srec_error checkHeader(int8_t string[])
{
    srec_error flag = NO_ERROR;
    uint8_t type;

    type = convert_ASCII_to_Dec(string[TYPE_BYTE]);
    if (type == 0)
    {
        flag = NO_ERROR;
    }
    else
    {
        flag = HEADER_ERROR;
    }
    return flag;
}

/* Function to check temination of file */
srec_error checkTerminate(uint8_t type, uint8_t typeData)
{
    srec_error flag = NO_ERROR;

    if (type == 7 && typeData == 3)
    {
        flag = NO_ERROR;
    }
    else if (type == 8 && typeData == 2)
    {
        flag = NO_ERROR;
    }
    else if (type == 9 && typeData == 1)
    {
        flag = NO_ERROR;
    }
    else
    {
        flag = TERMINATION_ERROR;
    }

    return flag;
}

/* Fuction to check all error in input file and output data + error */
void srecParser(FILE *input, FILE *output)
{
    int8_t tempString[MAX_BUFFER];
    uint16_t counter = 1;
    uint16_t dataCount = 0;
    uint8_t dataType = 0;
    uint8_t type = 0;

    /* Check each line throughout the file */
    while (fgets(tempString, MAX_BUFFER, input) != NULL)
    {
        type = convert_ASCII_to_Dec(tempString[TYPE_BYTE]);
        /* Increase data count when type is 1, 2, 3 */
        if (type == 1 || type == 2 || type == 3)
        {
            dataCount++;
        }
        /* If line is empty */
        if (strcmp(tempString, "\n") == 0)
        {
            fprintf(output, "%-10d", counter);
            fputs("This is a blank line!\n", output);
        }
        /* Check if line start with S or not */
        else if (checkS(tempString) != NO_ERROR)
        {
            fprintf(output, "%-10d", counter);
            fputs("ERROR: This line does not start with S!\n", output);
        }
        /* Check if type is in range 0~9 or not */
        else if (checkType(tempString) != NO_ERROR)
        {
            fprintf(output, "%-10d", counter);
            fputs("ERROR: This line has wrong Type!\n", output);
        }
        /* Check if all the characters are Hex or not */
        else if (checkHex(tempString) != NO_ERROR)
        {
            fprintf(output, "%-10d", counter);
            fputs("ERROR: This line contains character with other type than HEX\n", output);
        }
        /* Check if byte count value is correct or not */
        else if (checkByteCount(tempString) != NO_ERROR)
        {
            fprintf(output, "%-10d", counter);
            fputs("ERROR: This line has incorrect Byte Count value\n", output);
        }
        /* Check if checksum value is correct or not */
        else if (checkSum(tempString) != NO_ERROR)
        {
            fprintf(output, "%-10d", counter);
            fputs("ERROR: This line has incorrect Checksum value!\n", output);
        }
        /* If all the syntax error are passed check follow: */
        else
        {
            /* check  Header*/
            if (type == 0)
            {
                /* Header at 1st line */
                if (checkHeader(tempString) == NO_ERROR && counter == 1)
                {
                    fprintf(output, "%-10d", counter);
                    fputs("File Header\n", output);
                }
                /* No header */
                else if (checkHeader(tempString) == HEADER_ERROR && counter == 1)
                {
                    fprintf(output, "%-10d", counter);
                    fputs("ERROR: File did not start with S0 line!\n", output);
                }
                /* S0 line but at wrong position */
                else if (checkHeader(tempString) == NO_ERROR && counter != 1)
                {
                    fprintf(output, "%-10d", counter);
                    fputs("File Header (wrong location!)\n", output);
                }
                else
                {
                    /* do nothing */
                }
            }
            /* Check if the line is S1, S2, S3 --> print data */
            else if (type == 1 || type == 2 || type == 3)
            {
                dataType = type;
                fprintf(output, "%-10d", counter);
                showData(output, tempString, type);
            }
            /* If S5, S6 --> check Line count */
            else if (type == 5 || type == 6)
            {
                if (checkLineCount(tempString, dataCount) == LINE_COUNT_ERROR)
                {
                    fprintf(output, "%-10d", counter);
                    fprintf(output, "Line Count Error! [Actual value: %d]\n", dataCount);
                }
                else
                {
                    fprintf(output, "%-10d", counter);
                    fprintf(output, "Line Count OK [%d data line(s)]\n", dataCount);
                }
            }
            /* If S7, S8, S9 --> Check termination */
            else if (type == 7 || type == 8 || type == 9)
            {
                if (checkTerminate(type, dataType) == TERMINATION_ERROR)
                {
                    fprintf(output, "%-10d", counter);
                    fputs("ERROR: Wrong type of termination!\n", output);
                }
                else
                {
                    fprintf(output, "%-10d", counter);
                    fputs("Termination OK\n", output);
                }
            }
            /* If S4 --> Not available */
            else if (type == 4)
            {
                fprintf(output, "%-10d", counter);
                fputs("ERROR: This line has type of S4 (which is not available at the moment)\n", output);
            }
            else
            {
                /* do nothing */
            }
        }
        counter++;
    }
}

/******************************************************************************
*                                   END
******************************************************************************/
