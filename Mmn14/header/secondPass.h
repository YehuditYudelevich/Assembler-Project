
#ifndef SECOND_PASS_H
#define SECOND_PASS_H
#include "errors.h"
#include "table.h"
#include "globals.h"
#include "first_pass.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Processes the second pass of the assembly.
 * 
 * @param entry_table Pointer to the entry table.
 * @param IC Pointer to the instruction counter.
 * @param DC Pointer to the data counter.
 * @param file Name of the input file.
 * @param instruction_array Array of instruction words.
 * @param data_array Array of data words.
 * @param ob_file_name Name of the output object file.
 * @param ent_file_name Name of the output entry file.
 * @param ext_file_name Name of the output extern file.
 * 
 * @return ErrorCode indicating the result of the operation.
 */
ErrorCode main_process_2(entryTab* entry_table, int* IC, int* DC, char* file, Word* instruction_array, Word* data_array, char* ob_file_name, char* ent_file_name, char* ext_file_name);


/**
 * Exports the entry table to a file.
 * 
 * @param entry_table Pointer to the entry table.
 * @param ent_file_name Name of the output entry file.
 */
void export_ent_file(entryTab* entry_table, char* ent_file_name);

/**
 * Exports the external references to a file.
 * 
 * @param ext_file_name Name of the output extern file.
 */
void export_ext_file(char* ext_file_name);

/**
 * Exports the object file.
 * 
 * @param ob_file_name Name of the output object file.
 * @param IC Pointer to the instruction counter.
 * @param DC Pointer to the data counter.
 * @param instruction_array Array of instruction words.
 * @param data_array Array of data words.
 */
void export_ob_file(char* ob_file_name, int *IC, int *DC, Word* instruction_array, Word* data_array);

/**
 * Checks if all labels are declared.
 * 
 * @return ErrorCode indicating the result of the check.
 */
ErrorCode check_if_declered(Word* instruction_array);

/**
 * Checks if all entry labels are declared in this file.
 * 
 * @param entry_table Pointer to the entry table.
 * 
 * @return ErrorCode indicating the result of the check.
 */
ErrorCode check_if_entry_labels_declered(entryTab* entry_table);

/**
 * Prints the address to the file.
 * 
 * @param file Pointer to the file.
 * @param address Address to be printed.
 */

void print_the_address(FILE* file, int address);

/**
 * Encodes the address and ARE into a word.
 * 
 * @param address Address to be encoded.
 * @param ARE Addressing mode.
 * 
 * @return Encoded word.
 */
unsigned short encode_unknown_references_word(int address, int ARE);


/**
 * Prints a number in octal format to the file.
 * 
 * @param file Pointer to the file.
 * @param number Number to be printed.
 */
void printToOBFileOctal(FILE* file,unsigned int binary_code);


/**
 * Opens a file with the given name and mode.
 * 
 * @param file_name Name of the file.
 * @param mode Mode to open the file.
 * 
 * @return File pointer to the opened file.
 */
FILE* openFile(const char* file_name, const char* mode);



#endif

