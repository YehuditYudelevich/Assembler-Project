
#include "parser.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>
char* REGS[] = { "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7" };
char* instructions[] = { "mov", "cmp", "add", "sub", "lea", "clr", "not", "inc",
    "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop" };
char* directive[] = { ".data", ".string", ".extern", ".entry" };
char* two_op[] = { "mov", "cmp", "add", "sub", "lea" };
char* one_op[]
    = { "clr", "not", "inc", "dec", "jmp", "bne", "red", "prn", "jsr" };
char* no_op[] = { "rts", "stop" };

int is_directive(char* str)
{
    /* Check if the given string is a directive */
    int i;
    /* Null pointer check */
    if (str == NULL) {
        return 0;
    }
    for (i = 0; i < NUM_DIRECTIVE; i++) {
        if (strcmp(str, directive[i]) == 0) {
            /* String matches a directive */
            return 1;
        }
    }
    /* String does not match any directive */
    return 0;
}
int which_reg( char* str)
{
    /* Determine the register index from the given string */
    int i;
    if (str == NULL) {
        return -1;
    }
    for (i = 0; i < NUM_REG; i++) {
        if (strcmp(str, REGS[i]) == 0) {
            /* String matches a register */
            return i;
        }
    }
    /* String does not match any register */
    return -1;
}

/* Check if the given string is an instruction */
int is_inst(char* str)
{
    /* Null pointer check */
    int i;
    if (str == NULL) {
        return 0;
    }

    for (i = 0; i < INSTRUCTION_SIZE; i++) {
        if (strcmp(str, instructions[i]) == 0) {
            return 1;
        }
    }
    /* String does not match any instruction */
    return 0;
}

/* Check if the given string is a legal label */
int is_legal_label(char* str)
{
    /* Null pointer check */
    if (str == NULL) {
        return 0;
    }
    /* String matches a register */
    if (which_reg(str) != -1) {
        return 1;
    }
    /* String matches a directive */
    if (is_directive(str) == 1) {
        return 0;
    }
    /* First character must be a letter */
    if (!isalpha(*str)) {
        return 0;
    }
    /* Check label length */
    if (strlen(str) > NAME_LABEL) {
        return 0;
    }
    /* String matches an instruction */
    if (is_inst(str)) {
        return 0;
    }

    /* Skip over valid characters in the label */
    while (*(++str) != '\0' && (isalpha(*str) || isdigit(*str))) {
        ; /*The loop is executed solely to skip over the valid characters in the
             string*/
    }
    if(*str==','){
        return 0;
    }
    if (*str == ' ' || *str == '\0') {
        /*valid*/
        return 1;
    } else {
        return 0;
    }
}

/* Validate label declaration and add it to the symbol table */
int legal_label_decl(char* str, char* next_word, int* IC, int* DC)
{
    size_t len = 0;
    SymbolType type = UNKNOWN_SYMBOL;
    SymbolAttribute attribute = UNKNOWN_SYMBOL_ATTRIBUTE;
    int address = 0;
    if (str == NULL || strlen(str) > NAME_LABEL || !isalpha(*str)) {
        /* Invalid label or length check */
        return 0;
    }
    len = strlen(str);

    if (len > 1 && str[len - 1] == ':') {
        str[len - 1] = '\0'; /* remove the : */
        if (find_symbol(str) == 0) {
            /* label already defined */
            return 0;
        }
        if(find_symbol(str)==2){
            return 2;/*label already define as external*/
        }
        if (is_directive(next_word)) {
            if (strcmp(next_word, ".data") == 0
                || strcmp(next_word, ".string") == 0) {
                type = DATA;
                address = *DC+*IC;
                attribute = INNER;
            } else if (strcmp(next_word, ".extern") == 0) {
                type = RELOCATABLE;
                address = 0;
                attribute = EXTERNAL;
            } else if (strcmp(next_word, ".entry") == 0) {
                /* .entry doesn't define a symbol, it just marks an existing one
                 */
                type = RELOCATABLE;
                address = 0;
                attribute = ENTRY;
            } 
        }
        else if (is_inst(next_word)==1) {
            type = RELOCATABLE;
            address = *IC;
            attribute = ENTRY;
        }
        return add_label(str, address, type, attribute); 
    
       /* Label declaration is invalid */
    
    }return 0; 
    
}
ErrorCode check_op1(char* start, char* op1)
{
    int i;
    char first_char = op1[0];
    char* rest_of_string = op1 + 1;
    if (strcmp(start, "lea") == 0) {
        if (is_legal_label(op1)==0) {
            /* Invalid label for 'lea' instruction */
            return THE_FIRST_PARAM_NOT_VALID;
        } else {
            return SUCCESS;
        }
    }
    if (is_legal_label(op1)==1) {
        /* Operand is a legal label */
        return SUCCESS;
    }
   if (first_char == '#') {
    i = 0;

    /* Check if the first character is a minus sign */
    if (rest_of_string[0] == '-') {
        i = 1; /* Start checking from the next character */
    }

    /* Validate that the rest of the string consists of digits */
    for (; rest_of_string[i] != '\0'; i++) {
        if (!isdigit((unsigned char)rest_of_string[i])) {
            /* Invalid immediate number format */
            return THE_FIRST_PARAM_NOT_VALID;
        }
    }

    } else if (first_char == '*') {
        if (which_reg(rest_of_string) == -1) {
            /* Invalid indirect addressing */
            return THE_FIRST_PARAM_NOT_VALID;
        }
    } else {
        if (which_reg(op1) == -1) {
            /* Operand is not a valid register */
            return THE_FIRST_PARAM_NOT_VALID;
        }
    }
    return SUCCESS;
}

/* Check if the second operand of an instruction is valid */
ErrorCode check_op2(char* start, char* op2)
{
     /* Instructions with addressing methods 1, 2, and 3 */
    char* Addressing_methods123[]
        = { "mov", "add", "sub", "lea", "clr", "not", "inc", "dec", "red" };
    /* Instructions with addressing methods 1, 2 */
    char* Addressing_methods12[] = { "jmp", "bne", "jsr" };
    /* Instructions with addressing methods 1 and 2 and 3 and 0*/
    char* Addressing_methods0123[] = { "cmp", "prn" };
    int num_of_inst_method123 = 9;
    int num_of_inst_method12 = 3;
    int num_of_inst_method0123 = 2;

    char first_char = op2[0];
    char* rest_of_string = op2 + 1;

    int i, j, k, l;
    /* Check if the operands are valid */
    for (i = 0; i < num_of_inst_method123; i++) {
        /* Check for instructions with addressing methods 1, 2, and 3 */
        if (strcmp(start, Addressing_methods123[i]) == 0) {
           
            if (is_legal_label(op2)==1) {
                /* Operand is a legal label */
                return SUCCESS;
            } else if (first_char == '*') {
                if (which_reg(rest_of_string) != -1) {
                    return SUCCESS;
                }
            } else if (which_reg(op2) != -1) {
                /* Operand is a valid register */
                return SUCCESS;
            }
        }
    }
    for (j = 0; j < num_of_inst_method12; j++) {
        if (strcmp(start, Addressing_methods12[j]) == 0) {
            if (is_legal_label(op2)==1) {
                /* Operand is a legal label */
                return SUCCESS;
            } else if (first_char == '*') {
                if (which_reg(rest_of_string) == 1) {
                    /* Indirect addressing with specific register */
                    return SUCCESS;
                }
            }
        }
    }
    for (k = 0; k < num_of_inst_method0123; k++) {
        if (strcmp(start, Addressing_methods0123[k]) == 0) {
            if (is_legal_label(op2)==1) {
                /* Operand is a legal label */
                return SUCCESS;
            } else if (first_char == '#') {
                for (l = 0; rest_of_string[l] != '\0'; l++) {
                    if (isdigit((unsigned char)rest_of_string[l])) {
                        /* Immediate number is valid */
                        return SUCCESS;
                    }
                }
            } else if (first_char == '*') {
                if (which_reg(rest_of_string) == 1) {
                    /* Indirect addressing with specific register */
                    return SUCCESS;
                }
                /* Operand is a specific register */
                return SUCCESS;
            }
        }
    }
    /* Second operand is not valid */
    return THE_SECOND_PARAM_NOT_VALID;
}

/* Check and process the instruction based on its type */
ErrorCode check_for_inst(
    char* start, char* buffer, int* IC, Word* instruction_array)
{
    
    char *op1 = NULL, *comma = NULL, *op2 = NULL, *end = NULL;
    int i, j, k;
    ErrorCode args_valid = ILLEGAL_INSTRUCTION;
    size_t start_len = strlen(start);

    size_t buffer_len = strlen(buffer);
    int l;
    int start_op;
    l=0;
    start_op = 0;
    /* Find the start of the instruction */
    for (l = 0; l <= buffer_len - start_len; l++) {
        if (strncmp(&buffer[l], start, start_len) == 0) {
            start_op = l; 
            break;
        }
    }
    /* Check for two operand instructions */
    for (i = 0; i < TWO_OP_SIZE; i++) {
        if (strcmp(start, two_op[i]) == 0) {
            op1 = word_without_space(buffer, strlen(start)+1+start_op);
            /* Missing first operand? */
            if (op1 == NULL) {
                return MISSING_PARAM;
            }
            /* Check if the first operand is valid */
            args_valid = check_op1(start, op1);
            if (args_valid != SUCCESS) {
                free(op1);/*free the word*/
                return args_valid;
            }
            /* Check for a comma */
            comma = word_without_space_to_commas(buffer, strlen(start) + strlen(op1)+start_op+1);
            /* Missing comma? */
            if (comma == NULL || *comma != ',') {
                free(op1);/*free the word*/
                return NO_COMMA;
            }
            free(comma);
            comma=NULL;
            /* Find the second operand */
            op2 = word_without_space(buffer, strlen(start) + strlen(op1)+start_op + 1);
            
        
            /* Missing second operand? */
            if (op2 == NULL) {
                free(op1);/*free the word*/
                return MISSING_PARAM;
            }
            /* Check if the second operand is valid */
            args_valid = check_op2(start, op2);
            if (args_valid != SUCCESS) {
                free(op1);/*free the word*/
                free(op2);/*free the word*/
                return args_valid;
            }
            /*check if there is another param*/
            end = word_without_space_to_commas(
                buffer, strlen(start) + strlen(op1) +strlen(op2)+2+start_op);
            
            /* Extra parameters after the instruction? */
            if (end != NULL) {
                free(op1);/*free the word*/
                free(op2);/*free the word*/
                free(end);/*free the word*/
                return EXTRA_PARAMETERS;
            }
            
            /* Process the instruction */
            process_two_op(start,op1, op2, IC, instruction_array);
            free(op1);/*free the word*/
            free(op2);/*free the word*/
            return SUCCESS;
        }
    }
    /* Check for one operand instructions */
    for (j = 0; j < ONE_OP_SIZE; j++) {
        /* Check if the instruction is a one operand instruction */
        if (strcmp(start, one_op[j]) == 0) {
            op1 = word_without_space(buffer, strlen(start)+1+start_op);
            /* Missing operand ?*/
            if (op1 == NULL) {
                return MISSING_PARAM;
            }
            /* Check if the operand is valid */
            args_valid = check_op1(start, op1);
            if (args_valid != SUCCESS) {
                free(op1);/*free the word*/
                return args_valid;
            }
            /* Check for extra parameters */
            comma = word_without_space_to_commas(buffer, strlen(start) + strlen(op1)+start_op+1);
            /* Extra parameters ?*/
            if (comma != NULL) {
                free(op1);/*free the word*/
                free(comma);
                return EXTRA_PARAMETERS;
            }
            free(comma);
            /* Process the instruction */
            process_one_op(start, op1, IC, instruction_array);
            free(op1);/*free the word*/
    
            return SUCCESS;
        }
    }
    /* Check for zero operand instructions */
    for (k = 0; k < NO_OP_SIZE; k++) {
        if (strcmp(start, no_op[k]) == 0) {
            
            op1 = word_without_space_to_commas(buffer, strlen(start)+start_op);
           
            /* Extra parameters? */
            if (op1 != NULL) {
                free(op1);
                return EXTRA_PARAMETERS;
            }
            /* Process the instruction */
            process_zero_op(start, IC, instruction_array);
            return SUCCESS;
        }
    }
    /* Free allocated memory */
    if (op1) free(op1);
    if (op2) free(op2);
    if (comma) free(comma);
    if (end) free(end);
    /* Instruction not recognized */
    return ILLEGAL_INSTRUCTION;
}

/* Check and process directives based on their type */
ErrorCode check_for_directive(entryTab* entry_table,char* start, char* buffer, int* DC, int have_label,Word* data_array)
{
    ErrorCode ok1;
    char* directive;
    directive=start;

    if (directive == NULL && buffer == directive) {
        /* Null pointer or excess text */
        return NO_LABEL;/*להחזיר שאין מספיק פרמטרים*/
    }
    
    if (strcmp(directive, ".data") == 0) {
        /* Check for '.data' directive */
        ok1 = check_for_data(buffer);
        /* Import data */
        if (ok1 == SUCCESS) {
            if (export_data(buffer, DC, data_array) != SUCCESS) {
                /* Failed to export data */
                return FAILED_TO_EXPORT_DATA;
            }
        }
    } else if (strcmp(directive, ".string") == 0) {
        /* Check for '.string' directive */
        ok1 = check_for_string(directive, buffer, strlen(start));
        /* Import string data */
        if (ok1 == SUCCESS) {
            /* Import string data */
            export_string(buffer, DC, data_array);
        }
    
    } else if (strcmp(directive, ".entry") == 0) {
        ok1 = check_for_entry(entry_table, directive, buffer, strlen(start),have_label,DC);
    } else if (strcmp(directive, ".extern") == 0) {
        ok1 = check_for_extern(directive, buffer, strlen(start),have_label);
    } else {
        return EXCESS_TEXT; /* Directive not recognized */
    }

    return ok1;
}

/* Check and process '.extern' directive */
ErrorCode check_for_extern(char* start, char* buffer, int index, int have_label)
{
    char* label;
   
    if(have_label==1){
        /*found the label*/
        label = word_without_space(buffer, index+strlen(start)+2);/*2 for the 2 spaces*/
    }
    else{
        label=buffer;
    }
    /* Check if the label is valid */
    if (label == NULL || *label == '\0') {
        if(have_label==1){
            free(label);/*free the word*/
        }
        return NO_SPACES; /* Missing or empty label */
    }
    /* Check if the label is valid */
    if (is_legal_label(label)==0) {
        if(have_label==1){
            free(label);/*free the word*/
        }
        return ILLEGAL_LABEL; /* Invalid label format */
    }
    if (find_symbol(label) != 1) {
        if(have_label==1){
            free(label);/*free the word*/
        }
        return MULTIPLE_LABELS; /* Label already defined */
    }
    if(add_label(label, 0, RELOCATABLE, EXTERNAL)!=1); /* Add label as external */
    if(have_label==1){
        free(label);/*free the word*/
        return ALLOCATION_ERROR;
    }
    return SUCCESS;
}

/* Check and process '.entry' directive */
ErrorCode check_for_entry(entryTab* entry_table,char* start, char* buffer, int index,int have_label, int* IC)
{
    char* label;
    ErrorCode result;
   /*found the label*/
    if(have_label==1){
        label = word_without_space(buffer, index+strlen(start)+2);/*2 for the 2 spaces*/
    }
    else{
        label=buffer;
    }
    /* Check if the label is valid */
    if (label == NULL || *label == '\0') {
        if(have_label==1){
            free(label);/*free the word*/
        }
        return NO_LABEL; /* Missing or empty label */
    }
    if (is_legal_label(label)==0) {
        if(have_label==1){
            free(label);/*free the word*/
        }
        return ILLEGAL_LABEL; /* Invalid label format */
    }
    result=add_entry(entry_table,label,IC); /* Add label to entry list */
    if(result!=0){
        if(add_entry(entry_table,label,IC)==1){
            return ENTRY_DEFINED_TWICE;
            free(label);/*free the word*/
        }
        if(have_label==1){
            free(label);/*free the word*/
        }
        return ENTRY_ADD_FAILURE;
    } /* Add label to entry list */
    if(have_label==1){
        free(label);/*free the word*/
    }
    return SUCCESS;
}

/* Check and process '.string' directive */
ErrorCode check_for_string(char* start, char* buffer, int index) {
    char* ptr;
    char* end;
    char* label;
    int len;
    int i, j;
    char* current;
    label = word_without_space(buffer, 0);
    /* Determine the length up to the string */
    if (strcmp(label, ".string") == 0) {
        len = strlen(label) + 1;
    } else {
        len = strlen(label) + index + 2;
    }
    free(label); /* Free the word */
    
    /* Loop that removes spaces from the string */
    for (i = len, j = len; i < strlen(buffer); i++) {
        if (buffer[i] != ' ') {
            buffer[j++] = buffer[i];
        }
    }
    buffer[j] = '\0';
    
    current = word_without_space_to_commas(buffer, len);
    /* Check if the first character is a double quote */
    if (current == NULL || *current != '"') {
        return STRING_NOT_VALID; /* Missing or invalid string */
    }

    while (*current == '"') {
        current++; /* Skip the opening quote */
        
        end = strchr(current, '"');
        if (end == NULL) {
            return STRING_NOT_VALID; /* Missing closing quote */
        }

        /* Check if all characters between the quotes are printable */
        for (ptr = current; ptr < end; ptr++) {
            if (!isprint((unsigned char)*ptr)) {
                return ILLEGAL_CHARACTER; /* Non-printable character in string */
            }
        }
        
        current = end + 1; /* Move past the closing quote */
        
        /* Check if there is more text or another string */
        if (*current == '\0') {
            return SUCCESS; /* All strings are valid */
        } else if (*current == '"') {
            continue; /* Another string found, keep checking */
        } else {
            return EXCESS_TEXT; /* Invalid text after the last quote */
        }
    }
    
    return SUCCESS; 
}

ErrorCode check_for_data(char* buffer)
{

    char*current;
    int first_number;
    char* end;
    int expect_number = 1; /*Expecting a number at the start*/
    int found_comma = 0; /*Flag to indicate if we found a comma*/
    /*find the index of the first number*/
    first_number=find_first_number(buffer);
    /*current contain the first number*/
    current=word_without_space(buffer,first_number);
    if (current ==0) {

        return MISSING_DATA; /* Missing data */
    }
    while (*current != '\0') {
        /* Skip whitespace */
        while (*current == ' ' || *current == '\t') {
            current++;
        }
        if (*current == '\0') {
            break;
        }
        if (expect_number) {
            if (*current == ',') {
                free(current);
                return ILLEGAL_NUMBER; /* Double comma */
            }
            /* Check for optional '+' or '-' sign */
            if (*current == '+' || *current == '-') {
                current++;
            }

            end = current;
            strtol(current, &end, 10);
            if (current == end) {
                free(current);
                return ILLEGAL_NUMBER;
            }
            /* Skip the number */
            current = end;
            expect_number = 0;
            found_comma = 0;
        } else {
            /* Check for a comma */
            if (*current != ',') {
                free(current);
                return ILLEGAL_NUMBER; /* Illegal character */
            }
            if (found_comma) {
                free(current);
                return ILLEGAL_NUMBER; /* Illegal character */
            }
            current++;
            expect_number = 1; /* Expecting a number */
            found_comma = 1; /* Found a comma */
        }
        /* Skip whitespace after number or comma */
        while (*current == ' ' || *current == '\t') {
            current++;
        }
    }
    /* Check for trailing comma */
    if (found_comma) {
        free(current);
        return ILLEGAL_NUMBER; /* Ends with an illegal character */
    }
    return SUCCESS;
}

/* Find the index of the first numeric character in a string */
int find_first_number(char* buffer) {
    int index = 0;
    int in_word = 0; /*0 for false, 1 for true*/ 

    while (*buffer != '\0') {
        if (isspace((unsigned char)*buffer) || *buffer == ':' || *buffer == ',' || *buffer == '.') {
            in_word = 0; /* not in a word*/
        } else if (isdigit((unsigned char)*buffer) || *buffer == '-' || *buffer == '+') {
            if (!in_word) {
                return index;
            }
        } else {
            in_word = 1; /*in a word*/ 
        }
        
        buffer++;
        index++;
    }
    
    return -1; 
}
