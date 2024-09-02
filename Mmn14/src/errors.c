#include "errors.h"
#include <stdio.h>
/*the all errors*/
void print_error(ErrorCode code, int num_line)
{
    fprintf(stdout, "Error on line %d: ", num_line);
    switch (code) {
    case SUCCESS:
        fprintf(stdout, "Success\n");
        break;
    case ILLEGAL_NUMBER:
        fprintf(stdout, "Error: Illegal number format\n");
        break;
    case THE_FIRST_PARAM_NOT_VALID:
        fprintf(stdout, "Error: The first parameter is not valid\n");
        break;
    case THE_SECOND_PARAM_NOT_VALID:
        fprintf(stdout, "Error: The second parameter is not valid\n");
        break;
    case LINE_TOO_LONG:
	fprintf(stdout,"The line is too long\n");
	break;
    case NO_COMMA:
        fprintf(stdout, "Error: Missing comma\n");
        break;
    case OPERAND_NOT_VALID:
        fprintf(stdout, "Error: Operand not valid\n");
        break;
    case EXTRA_PARAMETERS:
        fprintf(stdout, "Error: Extra parameters\n");
        break;
    case ILLEGAL_STRING:
        fprintf(stdout, "Error: Illegal string format\n");
        break;
    case MISSING_PARAM:
        fprintf(stdout, "Error: Missing parameter\n");
        break;
    case NO_SPACES:
        fprintf(stdout, "Error: No spaces allowed\n");
        break;
    case ILLEGAL_CHARACTER:
        fprintf(stdout, "Error: Illegal character\n");
        break;
    case EXCESS_TEXT:
        fprintf(stdout, "Error: Excess text after command\n");
        break;
    case ILLEGAL_LABEL:
        fprintf(stdout, "Error: Illegal label name\n");
        break;
    case UNDEFINED_LABEL:
        fprintf(stdout, "Error: Undefined label\n");
        break;
    case LABEL_DEFINED_TWICE:
        fprintf(stdout, "Error: Label defined twice\n");
        break;
    case MACRO_DEFINITION:
        fprintf(stdout, "Error: Macro definition error\n");
        break;
    case INVALID_MACRO_NAME:
        fprintf(stdout, "Error: Invalid macro name\n");
        break;
    case NO_MACRO_DEFINITION:
        fprintf(stdout, "Error: No macro definition\n");
        break;
    case EXTRA_TOKENS:
        fprintf(stdout, "Error: Extra tokens after macro definition\n");
        break;
    case EMPTY_LINE:
        fprintf(stdout, "Error: Empty line\n");
        break;
    case ALLOCATION_ERROR:
        fprintf(stdout, "Error: Memory allocation failed\n");
        break;
    case FILE_ERROR:
        fprintf(stdout, "Error: File operation failed\n");
        break;
    case NO_SPACE_BETWEEN_LABEL_AND_OPERATION:
        fprintf(stdout, "Error: No space between label and operation\n");
        break;
    case ILLEGAL_INSTRUCTION:
        fprintf(stdout, "Error: Illegal instruction\n");
        break;
    case TEXT_AFTER_END:
        fprintf(stdout, "Error: Text after end of command\n");
        break;
    case MACRO_SAME_AS_REGISTER:
        fprintf(stdout, "Error: Macro name same as register\n");
        break;
    case MACRO_SAME_AS_INSTRUCTION:
        fprintf(stdout, "Error: Macro name same as instruction\n");
        break;
    case MACRO_SAME_AS_DIRECTIVE:
        fprintf(stdout, "Error: Macro name same as directive\n");
        break;
    case MACRO_ALREADY_DEFINED:
        fprintf(stdout, "Error: Macro already defined\n");
        break;
    case MEMORY_OVERFLOW:
        fprintf(stdout, "Error: Memory overflow\n");
        break;
    case MULTIPLE_LABELS:
        fprintf(stdout, "Error: Multiple labels\n");
        break;
    case MISSING_OPERATION:
        fprintf(stdout, "Error: Missing operation\n");
        break;
    case UNRECOGNIZED_OPERATION:
        fprintf(stdout, "Error: Unrecognized operation\n");
        break;
    case THE_FILE_IS_EMPTY:
        fprintf(stdout, "Error: The file is empty\n");
        break;
    case FAILED_TO_EXPORT_DATA:
        fprintf(stdout, "Error: Failed to export data\n");
        break;
    case NUMBER_OUT_OF_RANGE:
        fprintf(stdout, "Error: Number out of range\n");
        break;
    case MISSING_DATA:
        fprintf(stdout, "Error: Missing data\n");
        break;
    case EXTRA_TOKENS_END:
        fprintf(stdout, "Error: Extra tokens after endmacr\n");
        break;
    case NO_LABEL:
        fprintf(stdout, "Error: No label after the declaration of entry/extern\n");
        break;
    case STRING_NOT_VALID:
        fprintf(stdout, "Error: String not valid\n");
        break;
    case ENTRY_ADD_FAILURE:
        fprintf(stdout, "Error: Failed to add entry\n");
        break;
    case ERRORS_FOUND:
        fprintf(stdout, "Error: Errors found on the first pass\n");
        break;
    case ENTRY_DEFINED_TWICE:
        fprintf(stdout, "Error: the label that mark as Entry defined twice as entry\n");
        break;
    default:
        fprintf(stdout, "Unknown error occurred\n");
    }
}

void print_error_second_pass(ErrorCode code)
{
    fprintf(stdout, "Error: ");
    switch (code) {
    case LABEL_NOT_DECLERED:
        fprintf(stdout, "Label not declared\n");
        break;
    case ENTRY_NOT_FOUND:
        fprintf(stdout, " lebel that marked as Entry not found in this file\n");
        break;
    default:
        fprintf(stdout, "Unknown error occurred\n");
    }
}

