#ifndef ERRORS_H
#define ERRORS_H
/*the list of the errors*/
typedef enum {
    SUCCESS = 0,
    ILLEGAL_NUMBER,
    THE_FIRST_PARAM_NOT_VALID,
    THE_SECOND_PARAM_NOT_VALID,
    NO_COMMA,
    OPERAND_NOT_VALID,
    LINE_TOO_LONG,
    EXTRA_PARAMETERS,
    ILLEGAL_STRING,
    MISSING_PARAM,
    NO_SPACES,
    ILLEGAL_CHARACTER,
    EXCESS_TEXT,
    ILLEGAL_LABEL,
    UNDEFINED_LABEL,
    LABEL_DEFINED_TWICE,
    MACRO_DEFINITION,
    INVALID_MACRO_NAME,
    NO_MACRO_DEFINITION,
    EXTRA_TOKENS,
    EXTRA_TOKENS_END,
    EMPTY_LINE,
    ALLOCATION_ERROR,
    FILE_ERROR,
    NO_SPACE_BETWEEN_LABEL_AND_OPERATION,
    ILLEGAL_INSTRUCTION,
    TEXT_AFTER_END,
    MACRO_SAME_AS_REGISTER,
    MACRO_SAME_AS_INSTRUCTION,
    MACRO_SAME_AS_DIRECTIVE,
    MACRO_ALREADY_DEFINED,
    MEMORY_OVERFLOW,
    MULTIPLE_LABELS,
    MISSING_OPERATION,
    UNRECOGNIZED_OPERATION,
    THE_FILE_IS_EMPTY,
    FAILED_TO_EXPORT_DATA,
    NUMBER_OUT_OF_RANGE,
    MISSING_DATA,
    NO_LABEL,
    STRING_NOT_VALID,
    ENTRY_ADD_FAILURE,
    ERRORS_FOUND,
    LABEL_NOT_DECLERED,
    ENTRY_NOT_FOUND,
    ENTRY_DEFINED_TWICE
} ErrorCode;

/**
 * @brief Prints an error message to stderr.
 *
 * This function prints a detailed error message to the standard error stream
 * (stderr) based on the provided error code and line number.
 *
 * @param code The error code indicating the type of error.
 * @param num_line The line number where the error occurred.
 */
void print_error(ErrorCode code, int num_line);

/**
 * @brief Prints an error message to stderr.
 *
 * This function prints a detailed error message to the standard error stream
 * (stderr) based on the provided error code.
 *
 * @param code The error code indicating the type of error.
 */
void print_error_second_pass(ErrorCode code);
#endif
