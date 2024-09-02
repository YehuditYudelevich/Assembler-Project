#ifndef GLOBALS_H
#define GLOBALS_H

#include <stddef.h>
#define instruction_count 16
/*the size fo the mamory array*/
#define MEMORY_SIZE 4096
/*the size of the word*/
#define WORD_SIZE 15
/*the first address*/
#define INITIAL_ADDRESS 100
#define NAME_LENGTH 82
#define defualt_ARE 4
/*for the second/third word*/
#define OPCODE_SHIFT 11
#define SRC_MODE_SHIFT 6
#define DST_MODE_SHIFT 3
#define ARE_SHIFT 0
/* Masks for extracting fields from the instruction word */

/* Type definitions */
typedef signed short Word;

/* Structure to represent an operation code (opcode) */
typedef struct {
    char* name; /*the name*/
    int code; /*The numerical code associated with the operation*/
} op_code;

/* Structure to represent an unknown reference (for resolving labels) */
typedef struct {
    char label[32]; /*the name of the label*/
    Word word; /*the address where the label is referenced*/
    int IC; /*The current instruction counter at the reference*/
} UnknownReference;
#endif /* GLOBALS_H */
