
#ifndef PARSER_H
#define PARSER_H
#include "table.h"
#include "data_structures.h"
#include "errors.h"
#include "first_pass.h"
#include "globals.h"
#include "macro.h"
#include "text_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_DIRECTIVE 4
#define NUM_REG 7
#define NAME_LABEL 32
#define TWO_OP_SIZE 5
#define ONE_OP_SIZE 9
#define NO_OP_SIZE 2



/**
 * Checks if the given string is a directive.
 *
 * @param str The string to check.
 * @return 1 if the string is a directive, 0 otherwise.
 */
int is_directive(char* str);

/**
 * Determines the register index from the given string.
 *
 * @param str The string representing a register.
 * @return The register index if valid, -1 otherwise.
 */
int which_reg( char* str);

/**
 * Checks if the given string is an instruction.
 *
 * @param str The string to check.
 * @return 1 if the string is a valid instruction, 0 otherwise.
 */
int is_inst(char* str);

/**
 * Checks if the given string is a legal label.
 *
 * @param str The string to check.
 * @return 1 if the string is a legal label, 0 otherwise.
 */
int is_legal_label(char* str);

/**
 * Validates a label declaration and adds it to the symbol table.
 *
 * @param str The label to check.
 * @param next_word The next word following the label.
 * @param IC The current instruction counter.
 * @param DC The current data counter.
 * @return 1 on success, 0 on failure (e.g., invalid label or duplicate label).
 */
int legal_label_decl(char* str, char* next_word, int* IC, int* DC);

/**
 * Checks and processes directives based on their type.
 *
 * @param start The directive keyword.
 * @param buffer The buffer containing the directive parameters.
 * @param DC The current data counter.
 * @param have_label Indicates if a label was present.
 * @param data_array The array to store the processed data.
 * @return ErrorCode indicating the result of the directive processing.
 */
ErrorCode check_for_directive(entryTab* entry_table,char* start, char* buffer, int* DC, int have_label, Word* data_array);

/**
 * Checks if the second operand of an instruction is valid.
 *
 * @param start The instruction mnemonic.
 * @param op2 The second operand to validate.
 * @return ErrorCode indicating the result of the validation.
 */
ErrorCode check_for_inst(char* start, char* buffer, int* IC, Word* instruction_array);

/**
 * Checks and processes the '.data' directive.
 *
 * @param start The directive keyword.
 * @param buffer The buffer containing the directive parameters.
 * @return ErrorCode indicating the result of the '.data' directive processing.
 */
ErrorCode check_for_data(char* buffer);

/**
 * Checks and processes the '.string' directive.
 *
 * @param start The directive keyword.
 * @param buffer The buffer containing the directive parameters.
 * @param index The index in the buffer where the string starts.
 * @return ErrorCode indicating the result of the '.string' directive
 * processing.
 */
ErrorCode check_for_string(char* start, char* buffer, int index);

/**
 * Checks and processes the '.entry' directive.
 *
 * @param start The directive keyword.
 * @param buffer The buffer containing the directive parameters.
 * @param index The index in the buffer where the label starts.
 * @return ErrorCode indicating the result of the '.entry' directive processing.
 */
ErrorCode check_for_entry( entryTab* entry_table,char* start, char* buffer, int index,int have_label, int* IC);

/**
 * Checks and processes the '.extern' directive.
 *
 * @param start The directive keyword.
 * @param buffer The buffer containing the directive parameters.
 * @param index The index in the buffer where the label starts.
 * @return ErrorCode indicating the result of the '.extern' directive
 * processing.
 */
ErrorCode check_for_extern(char* start, char* buffer, int index,int have_label);

/**
 * Checks if the first operand of an instruction is valid.
 *
 * @param start The instruction mnemonic.
 * @param op1 The first operand to validate.
 * @return ErrorCode indicating the result of the validation.
 */
ErrorCode check_op1(char* start, char* op1);

/**
 * Checks if the second operand of an instruction is valid.
 *
 * @param start The instruction mnemonic.
 * @param op2 The second operand to validate.
 * @return ErrorCode indicating the result of the validation.
 */
ErrorCode check_op2(char* start, char* op2);

/**
 *  Finds the index of the first numeric character in a string.
 * 
 * This function searches for the index of the first digit, minus sign, or plus sign in the input string.
 * 
 * @param buffer A pointer to the input string.
 * 
 * @return The index of the first numeric character. Returns 0 if no numeric character is found.
 */
int find_first_number(char* buffer);

#endif
