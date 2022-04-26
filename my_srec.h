#ifndef _MY_SREC_
#define _MY_SREC_
/******************************************************************************
*                                   DEFINITION
******************************************************************************/

#define MAX_BUFFER 255u

/******************************************************************************
*                                   TYPE DEFINITION
******************************************************************************/

/* Error Type */
typedef enum
{
    NO_ERROR = 0,
    HEADER_ERROR,
    S_ERROR,
    TYPE_ERROR,
    HEX_ERROR,
    BYTE_COUNT_ERROR,
    CHECK_SUM_ERROR,
    LINE_COUNT_ERROR,
    TERMINATION_ERROR
} srec_error;

/******************************************************************************
*                                   API
******************************************************************************/

/**
 * @brief Funcrtion to check S error on given line
 * 
 * @param string 
 * @return srec_error 
 */
srec_error checkS(int8_t string[]);

/**
 * @brief Function to check Type error on given line
 * 
 * @param [in] string 
 * @return srec_error 
 */
srec_error checkType(int8_t string[]);

/**
 * @brief Function to check whether all the characters on given line are HEX or not
 * 
 * @param [in] string 
 * @return srec_error 
 */
srec_error checkHex(int8_t string[]);

/**
 * @brief Function to check byte count on given line
 * 
 * @param [in] string 
 * @return srec_error 
 */
srec_error checkByteCount(int8_t string[]);

/**
 * @brief Function to check the check sum of given line
 * 
 * @param [in] string 
 * @return srec_error 
 */
srec_error checkSum(int8_t string[]);

/**
 * @brief Function to check line count of given file
 * 
 * @param [in] string 
 * @param count 
 * @return srec_error 
 */
srec_error checkLineCount(int8_t string[], 
                          uint16_t count);

/**
 * @brief Function to check Header of file
 * 
 * @param [in] string 
 * @return srec_error 
 */
srec_error checkHeader(int8_t string[]);

/**
 * @brief Function to check temination of file
 * 
 * @param [in] type 
 * @param [in] typeData 
 * @return srec_error 
 */
srec_error checkTerminate(uint8_t type, 
                          uint8_t typeData);

/**
 * @brief Fuction to check all error in input file and output data + error
 * 
 * @param [in] input 
 * @param [out] output 
 */
void srecParser(FILE *input,
                FILE *output);

#endif /* _MY_SREC_ */

/******************************************************************************
*                                   END
******************************************************************************/
