
#ifndef MACRO_H
#define MACRO_H
#include "data_structures.h"
#include "errors.h"
#include "parser.h"
#include "globals.h"
#include "text_parser.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*the name of the macro*/
#define MAX_MACRO_NAME 30
/*the maximum number of lines in a macro*/
#define MAX_MACRO_LINES 100
/*the maximum length of a line in a macro*/
#define MAX_LINE_LENGTH 82
/*the num og the registers*/
#define NUM_REG 7
/*the size of the instruction*/
#define INSTRUCTION_SIZE 16
typedef enum { FALSE,
    TRUE } my_bool;
extern char* instructions[];

/**
 * @brief Reads a line from the input file and processes it.
 *
 * This function checks if the line defines a macro or references a macro. It handles macro
 * definitions, macro replacements, and copying text to the output file. It updates the
 * macro definition status based on the content of the line.
 *
 * @param buffer The buffer containing the line read from the input file.
 * @param file_out The file pointer for the output file.
 * @param in_macro_definition Pointer to a boolean flag indicating if a macro definition is in progress.
 * @return ErrorCode Returns SUCCESS if the line was processed successfully, or an error code if an issue occurred.
 */
ErrorCode read_line(char* buffer, FILE* file_out);

/**
 * @brief Validates a macro name.
 *
 * This function checks if the macro name is valid based on various criteria, such as length,
 * whether it is already defined, and if it conflicts with register or instruction names.
 *
 * @param name The name of the macro to validate.
 * @return ErrorCode Returns SUCCESS if the name is valid, or an appropriate error code if it is not.
 */
ErrorCode is_valid_name(char* name);

/**
 * @brief Replaces a macro call with its definition in the output file.
 *
 * This function writes the lines of a macro definition to the output file in place of a macro call.
 *
 * @param macro_name The name of the macro to replace.
 * @param file_out The file pointer for the output file.
 */
void replace_macro(char* macro_name, FILE* file_out);

/**
 * @brief Copies a line of text to the output file.
 *
 * This function writes a line of text to the output file.
 *
 * @param file_out The file pointer for the output file.
 * @param text The text to write to the output file.
 */
void copy_text(FILE* file_out, char* text);

/**
 * @brief Processes the input file and generates the output file with macro replacements.
 *
 * This function opens the input file and output file, processes each line to handle macro
 * definitions and replacements, and writes the result to the output file. It handles errors
 * and memory allocation issues.
 *
 * @param file_name The name of the input file.
 * @param output_filename The name of the output file.
 * @return int Returns 0 on success, or 1 if an error occurred.
 */
int main_process(char* file_name, char* output_filename);

#endif
