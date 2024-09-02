#ifndef FIRST_PASS_H
#define FIRST_PASS_H

#include <stdio.h>
#include <ctype.h>
#include "table.h"
#include "errors.h"
#include "globals.h"
#include "parser.h"
#include "text_parser.h"
#define CAPACITY_REFERENCES 10

extern UnknownReference* unknown_references; /*Array of unknown references for label resolution */
extern int unknown_references_count; /* Number of unknown references */
extern int unknown_references_capacity; /* Capacity of the unknown references array */
/**
 * @brief Processes the main operations of the program.
 *
 * This function handles the main processing logic, including opening the file,
 * processing each line, and updating memory values. It initializes necessary
 * data structures, processes the file line by line, and handles various
 * errors during the process.
 *
 * @param file The name of the file to process.
 * @param instruction_array An array to store instruction words.
 * @param data_array An array to store data words.
 * @return ErrorCode Returns an error code indicating the result of the process.
 */
ErrorCode main_process_1(entryTab* entry_table,
    int* DC, int* IC, char* file, Word* instruction_array, Word* data_array);

/**
 * @brief Processes a single line from the file.
 *
 * This function analyzes a single line of input from the file, checking for
 * labels, directives, and instructions. It ensures that the line is correctly
 * formatted and updates the memory accordingly. It also handles specific cases
 * such as labels followed by directives or illegal labels.
 *
 * @param buffer The line of text to process.
 * @param IC A pointer to the instruction counter.
 * @param DC A pointer to the data counter.
 * @param instruction_array An array to store instruction words.
 * @param data_array An array to store data words.
 * @return ErrorCode Returns an error code indicating the result of the line
 * processing.
 */
ErrorCode process_line(entryTab* entry_table,
    char* buffer, int* IC, int* DC, Word* instruction_array, Word* data_array);

/**
 * @brief Gets the opcode for a given instruction.
 *
 * This function retrieves the opcode corresponding to the provided instruction
 * name by comparing it with a predefined list of opcodes. If the instruction
 * name is found, the function returns the associated opcode; otherwise, it
 * returns -1.
 *
 * @param inst The name of the instruction whose opcode is to be retrieved.
 * @return int The opcode of the instruction, or -1 if the instruction is not
 * recognized.
 */
int get_opcode(char* inst);

/**
 * @brief Gets the register number from a given operand.
 *
 * This function extracts the register number from a given operand string.
 * The operand string should be in the format "rN", where N is the register
 * number. If the operand does not start with 'r' or is not in the correct
 * format, the function returns -1.
 *
 * @param operand The operand string representing a register.
 * @return int The register number, or -1 if the operand is not a valid
 * register.
 */
int get_register(char* operand);

/**
 * @brief Determines the addressing method of a given operand.
 *
 * This function checks the operand string and identifies its addressing method
 * based on specific prefixes:
 * - '#' indicates an immediate addressing method.
 * - A legal label indicates a direct addressing method.
 * - '*' indicates an indirect addressing method.
 * - 'r' indicates a register addressing method.
 * If none of these conditions are met, the function returns -1.
 *
 * @param operand The operand string to check.
 * @return int An integer representing the addressing method:
 * - 0 for immediate
 * - 1 for direct (label)
 * - 2 for indirect
 * - 3 for register
 * - -1 if the operand does not match any known addressing method.
 */
int get_methode(char* operand);

/**
 * @brief Processes an instruction with two operands.
 *
 * This function processes a two-operand instruction, encoding it into the
 * instruction array. It handles various addressing methods and determines
 * whether additional words are needed for encoding. The instruction is encoded
 * into the appropriate format based on the addressing methods of the operands.
 *
 * @param start The instruction mnemonic.
 * @param op1 The first operand.
 * @param op2 The second operand.
 * @param IC A pointer to the instruction counter.
 * @param instruction_array An array to store the encoded instruction words.
 * @return int Returns 1 if only one additional word is needed, or 0 if two
 * additional words are needed.
 */
int process_two_op(char* start, char* op1, char* op2, int* IC, Word* instruction_array);

/**
 * @brief Processes an instruction with one operand.
 *
 * This function processes a one-operand instruction, encoding it into the
 * instruction array. It handles various addressing methods and encodes the
 * instruction into the appropriate format.
 *
 * @param start The instruction mnemonic.
 * @param op1 The operand.
 * @param IC A pointer to the instruction counter.
 * @param instruction_array An array to store the encoded instruction words.
 * @return int Returns 0 on success, or -1 if memory allocation fails.
 */
int process_one_op(char* start, char* op1, int* IC, Word* instruction_array);

/**
 * @brief Processes an instruction with no operands.
 *
 * This function processes an instruction with no operands, encoding it into the
 * instruction array.
 *
 * @param start The instruction mnemonic.
 * @param IC A pointer to the instruction counter.
 * @param instruction_array An array to store the encoded instruction word.
 */
void process_zero_op(char* start, int* IC, Word* instruction_array);

/**
 * @brief Exports data from a buffer to the data array.
 *
 * This function parses and exports data from a buffer to the data array. Each
 * number is written to the data array as a separate word. It also validates the
 * number range.
 *
 * @param start The data directive (not used in the function).
 * @param buffer The buffer containing data values.
 * @param DC A pointer to the data counter.
 * @param data_array An array to store the data words.
 * @return ErrorCode Returns SUCCESS on success, or an error code if data is
 * missing or out of range.
 */
ErrorCode export_data(char* buffer, int* DC, Word* data_array);

/**
 * @brief Exports a string from a buffer to the data array.
 *
 * This function exports a string from a buffer to the data array, encoding each
 * character as its ASCII value. The string is followed by a null terminator.
 *
 * @param start The string directive (not used in the function).
 * @param buffer The buffer containing the string.
 * @param DC A pointer to the data counter.
 * @param data_array An array to store the data words.
 * @return int Returns 1 on success, or 0 if the data array is not large enough.
 */

int export_string(char* buffer, int* DC, Word* data_array);

/**
 * @brief Encodes the first word.
 *
 * This function encodes an instruction word by combining the opcode, source
 * operand method, destination operand method, and ARE field into a single
 * 16-bit word,
 * this function is used for the first word of the instruction.
 *
 * @param opcode The opcode of the instruction.
 * @param src_op The source operand method.
 * @param dst_op The destination operand method.
 * @param ARE The ARE field value.
 * @return unsigned short The encoded instruction word.
 */
unsigned short encode_instruction_first_word(unsigned short opcode, unsigned short src_op,unsigned short dst_op, int ARE);

/**
 * @brief Encodes the next word.
 *
 * This function is special for the second word, that contain 2 registers or 2 pointers to registers.
 *
 * @param reg_src The value of the first operand.
 * @param reg_dest The value of second operand.
 * @param ARE The ARE field value(4).
 * @return unsigned short The encoded operand word.
 */
unsigned short special_case_2_registers_or_pointers(unsigned short value_reg_src,unsigned short value_reg_dst, unsigned short ARE);

/**
 * @brief Encodes the next word.
 *
 * This function encodes an instruction word by combining the opcode, source,
 * its used for the second word and the third word of the instruction.
 * @param op The methode of the first param.
 * @param ARE The ARE field value(3 or 6).
 * @return unsigned short The encoded operand word.
 */
unsigned short special_case_for_2_or_1_word(char *op, unsigned short shift);

/**
 * @brief Adds a label to the table of unknown symbol addresses.
 *
 * This function adds a label to the table of unknown symbol addresses. It
 * resizes the table if necessary and handles memory allocation errors. The
 * table includes the label, its address, and the encoded instruction word(0).
 *
 * @param label The label of the symbol with an unknown address.
 * @param IC The instruction counter where the entry should be added.
 * @param word The encoded instruction word(0).
 * @param instruction_array An array to store the instruction words.
 * @return ErrorCode Returns SUCCESS on success, or ALLOCATION_ERROR if memory
 * allocation fails.
 */
ErrorCode add_to_symbol_unknown_address(char* label, int IC, Word word);

/**
 * @brief Initializes the table of unknown symbol addresses.
 *
 * This function initializes the table used to store unknown symbol addresses.
 * It allocates memory for the table and handles allocation errors.
 *
 * @return ErrorCode Returns SUCCESS on success, or ALLOCATION_ERROR if memory
 * allocation fails.
 */
ErrorCode init_unknown_references(void);

/**
 * @brief Frees the memory used by the table of unknown symbol addresses.
 * This function frees the memory used by the table of unknown symbol addresses.
 */
void free_unknown_references(void);

#endif /* FIRST_PASS_H */
