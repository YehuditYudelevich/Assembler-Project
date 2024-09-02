
#include "first_pass.h"

/* Opcode table */
op_code OPCODE[] = { { "mov", 0 }, { "cmp", 1 }, { "add", 2 }, { "sub", 3 },
    { "lea", 4 }, { "clr", 5 }, { "not", 6 }, { "inc", 7 }, { "dec", 8 },
    { "jmp", 9 }, { "bne", 10 }, { "red", 11 }, { "prn", 12 }, { "jsr", 13 },
    { "rts", 14 }, { "stop", 15 } };

/*this parameters belong to the table that contain the all labels from the first pass, that encoded with the value 0, 
*and at the second pass we will encode the real value of the label
*/
UnknownReference* unknown_references = NULL;
int unknown_references_count = 0;
int unknown_references_capacity = CAPACITY_REFERENCES;

/* Process the first pass of the assembly file */
ErrorCode main_process_1( entryTab* entry_table,int* DC, int* IC, char* file, Word* instruction_array, Word* data_array)
  
{
    
    /* Counters */
    FILE* file_ptr;
    int num_line = 0;
    char buffer[MAX_LINE_LENGTH];
    ErrorCode success;
    int flag_error=0;
    init_unknown_references();
    if(init_unknown_references()!=SUCCESS){
        return ALLOCATION_ERROR;
        exit(1);
    }
    init_symbol_table();
    
    /* Open the file */
    file_ptr = fopen(file, "r");
    if (file_ptr == NULL) {
        return FILE_ERROR;
    }
    /* Process each line and send to the appropriate function */
  while (fgets(buffer, MAX_LINE_LENGTH + 1, file_ptr) != NULL) {
    num_line++;

    /* Check if the line exceeds the maximum length */
    if (strlen(buffer) > MAX_LINE_LENGTH) {
        print_error(LINE_TOO_LONG, num_line);
        flag_error = 1;
        continue;
    }

    if (buffer[0] == '\n' || buffer[0] == ';') {
        continue;
    }
    removeExtraSpaces(buffer);
    success = process_line(entry_table, buffer, IC, DC, instruction_array, data_array);
    if (success != SUCCESS) {
        print_error(success, num_line);
        flag_error = 1;
        continue;
    }
}
    /* At the end of the pass, update the memory value of the instruction
     * directives */
    if(flag_error==1){
        success=ERRORS_FOUND;
    }
    else{
        success=SUCCESS;
    }
    fclose(file_ptr);
    if (*IC == INITIAL_ADDRESS) {
        free_unknown_references();
        return THE_FILE_IS_EMPTY;
    }
    
    return success;
}

/* Process a line of the assembly file */
ErrorCode process_line(entryTab* entry_table,
    char* buffer, int* IC, int* DC, Word* instruction_array, Word* data_array)
{
    /* Check if we have not exceeded the array size */
    char *start, *next_word;
    ErrorCode success;
    int have_label = 0;
    if ((*IC) + (*DC) >= MEMORY_SIZE) {
        return MEMORY_OVERFLOW;
    }
    /* Initialize variables */
    start = word_without_space(buffer, 0);
    if(start==NULL){
        return MISSING_OPERATION;
    }
    next_word = word_without_space(buffer, strlen(start));
    
    if (next_word == NULL&&strcmp(start, "stop") != 0 && strcmp(start, "rts") != 0) {
        free(start);/*free the word*/
        return MISSING_OPERATION;
    }
    /*Check for .entry and .extern directives first*/
    if (strcmp(start, ".entry") == 0 || strcmp(start, ".extern") == 0) {
        success= check_for_directive(entry_table,
            start, next_word, DC, have_label, data_array);
        free(start); /*free the word*/
        free(next_word); /*free the word*/
        return success;
    }

    /* Check if there's a label*/
    if (legal_label_decl(start, next_word, IC, DC) == 1) {
        have_label = 1;
        free(next_word); /*free the word*/
        next_word = word_without_space(buffer, strlen(start)+1);
        
        if (next_word == NULL || *next_word == '\0') {
            free(start);/*free the word*/
            return MISSING_OPERATION;
        }
        /* Check for space between label and operation*/
        if (buffer[strlen(start) + 1] != ' ') {
            free(start);/*free the word*/
            free(next_word);/*free the word*/
            return NO_SPACE_BETWEEN_LABEL_AND_OPERATION;
        }

        /*Special case: label followed by .entry or .extern*/
        if (strcmp(next_word, ".entry") == 0
            || strcmp(next_word, ".extern") == 0) {
            /* Ignore the label and process the directive*/
            remove_symbol(start-1);
            success= check_for_directive( entry_table, start, buffer, DC, have_label,data_array);
            free(start);/*free the word*/
            free(next_word);/*free the word*/
            return success;
            
        }
    } else {/*no lable/ no valid label*/
        if(is_legal_label(start)==1){
            free(start);/*free the word*/
            free(next_word);/*free the word*/   
            return ILLEGAL_LABEL;
        }
        free(next_word);/*free the word*/
        next_word=word_without_space(buffer, 0);
        if(next_word==NULL){
            free(start);/*free the word*/
            return MISSING_OPERATION;
        }
    }

    /* Process instruction or directive*/
    if (is_directive(next_word) == 1) {
        success= check_for_directive(entry_table,next_word, buffer, DC, have_label,data_array);
    } else if (is_inst(next_word) == 1) {
        success = check_for_inst(next_word, buffer, IC, instruction_array);
    } else if (which_reg(next_word) != -1) {
        return ILLEGAL_INSTRUCTION;
    } else if (is_valid_name(next_word) == SUCCESS) {
        return ILLEGAL_INSTRUCTION;

    } else {
        return UNRECOGNIZED_OPERATION;
    }
    free(start);/*free the word*/
    free(next_word);/*free the word*/
    return success;
}
int get_opcode(char* inst)
{
    /* Get the opcode of each instruction */
    int i;
    for (i = 0; i < 16; i++) {
        if (strcmp(inst, OPCODE[i].name) == 0) {
            return OPCODE[i].code;
        }
    }
    return -1;
}

/* Get the register number */
int get_register(char* operand)
{
    /* Get the register number */
    if (operand[0] == 'r') {
        return atoi(operand + 1);
    }
    else if(operand[0]=='*'){
        return atoi(operand + 2);
    }
    return -1;
}

/* Get the addressing method of param */
int get_methode(char* operand)
{
    /* Check the addressing methods */
    if (operand[0] == '#') {
        return 0;
    } else if (operand[0] == 'r') {
        return 3;
    } else if (operand[0] == '*') {
        return 2;
    } else if (is_legal_label(operand) == 1) {
        return 1;
    }
    return -1;
}

/*a function that process instruction that have 2 parameters*/
int process_two_op(
    char* start, char* op1, char* op2, int* IC, Word* instruction_array)
{
    /* Declare variables */
    int opcode;
    int op_methode1;
    int op_methode2;
    int operand1;
    int operand2;
    int have_third = 1;
    Word first, second, third;
    /*get the methode*/
    opcode = get_opcode(start);
    op_methode1 = get_methode(op1);
    op_methode2 = get_methode(op2);
    first = encode_instruction_first_word(
        opcode, op_methode1, op_methode2, 4); /*the first word*/
    instruction_array[*IC - INITIAL_ADDRESS] = first;
    (*IC)++;
    if ((op_methode1 == 3 || op_methode1 == 2)
        && (op_methode2 == 3 || op_methode2 == 2)) { /*i need only one word*/
        operand1=get_register(op1);
        operand2=get_register(op2);
        /* Encoding the second word */
        second =special_case_2_registers_or_pointers(operand1, operand2, 4);
        /*export to the instruction array*/
        instruction_array[*IC - INITIAL_ADDRESS] = second;
        (*IC)++;
        return 1;
    }
    
    switch (op_methode1) {

    case 0: /*Immediate assistance*/
        second = special_case_for_2_or_1_word(op1, SHIFT_FOR_FIRST_WORD);
        break;
    case 1: /*Direct address*/
        /*the second word */
        second=special_case_for_2_or_1_word(op1, SHIFT_FOR_FIRST_WORD);
        /*add to a table for the second pass*/
        add_to_symbol_unknown_address(op1, *IC, second);
        break;
    /* Indirect addressing */
    case 2:
    /* Direct register addressing */
    case 3:
        /*the second word*/
        second=special_case_for_2_or_1_word(op1, SHIFT_FOR_SECOND_WORD);
        break;
    }
    
    switch (op_methode2) {
        
    /* Immediate addressing */
    case 0:
        third = special_case_for_2_or_1_word(op2, SHIFT_FOR_FIRST_WORD);
        break;
    /* Direct addressing */
    case 1:
       /*the third word*/
        third=special_case_for_2_or_1_word(op1, SHIFT_FOR_FIRST_WORD);
        /*add to a table for the second pass*/
        add_to_symbol_unknown_address(op2, *IC+1, third);
        break;
    /* Indirect addressing */
    case 2:
    /* Direct register addressing */
    case 3:
        /*the third word*/
        third =special_case_for_2_or_1_word(op2, SHIFT_FOR_FIRST_WORD);
        break;
    }
    /* Encoding the second word */
    instruction_array[*IC - INITIAL_ADDRESS] = second;
    (*IC)++;
    /* Encoding the third word, if present */
    if (have_third) {
        instruction_array[*IC - INITIAL_ADDRESS] = third;
        (*IC)++;
    }
    return 0;
}


/* Process an instruction with one parameter */
int process_one_op(char* start, char* op1, int* IC, Word* instruction_array)
{
    /*will take 2 words*/
    /* Variable declaration */
    int opcode = 0;
    int op_methode = 0;
    Word first = 0;
    Word second = 0;
    /* Get the opcode*/
    opcode = get_opcode(start);
    /*get the methode*/
    op_methode = get_methode(op1);
    
    /*4 meens that there is no src reg*/
    first = encode_instruction_first_word(opcode, 4, op_methode, 4); /*the first word*/
    instruction_array[*IC - INITIAL_ADDRESS] = first;
    (*IC)++;
    /* Direct addressing */
    switch (op_methode)
    {
    case 0:/*immediate addressing*/
        second = special_case_for_2_or_1_word(op1, SHIFT_FOR_FIRST_WORD); /*the second word */
        break;
    case 1:
        second = special_case_for_2_or_1_word(op1, SHIFT_FOR_FIRST_WORD); /*the second word */
        add_to_symbol_unknown_address(op1, *IC, second); /*add to a table for the second pass*/
        break;
    case 2:
    case 3:
        second = special_case_for_2_or_1_word(op1, SHIFT_FOR_FIRST_WORD); /*the second word */
        break;
    
    default:
        break;
    }
    
    /* Encoding the second word */
    instruction_array[*IC - INITIAL_ADDRESS] = second;
    (*IC)++;
    return 0;
}

/* Process an instruction with no parameters */
void process_zero_op(char* start, int* IC, Word* instruction_array)
{
    /* Variable declaration */
    Word first;
    int opcode;
    /* Get the opcode */
    opcode = get_opcode(start);
    /*the first word 4 meens that there is no params*/
    first = encode_instruction_first_word(opcode, 4, 4, 4); 
    instruction_array[*IC - INITIAL_ADDRESS] = first;
    (*IC)++;
}

/* Process a directive and export it to the data array */
ErrorCode export_data(char* buffer, int* DC, Word* data_array) {
    char* end;
    long number;
    /* Find the .data directive */
    buffer = strstr(buffer, ".data");
    if (buffer == NULL) {
        return MISSING_DATA;
    }

    /* Move past the .data directive */
    buffer += strlen(".data");

    /* Skip spaces after .data */
    while (isspace((unsigned char)*buffer)) {
        buffer++;
    }

    /* Skip non-numeric prefixes before the first number */
    while (*buffer != '\0' && !isdigit((unsigned char)*buffer) && *buffer != '-' && *buffer != '+') {
        buffer++;
    }

    while (*buffer != '\0') {
        /* Skip spaces, tabs, and commas */
        while (isspace((unsigned char)*buffer) || *buffer == ',') {
            buffer++;
        }
        if (*buffer == '\0') {
            break;
        }

        /* Convert to long with base 10 */
        number = strtol(buffer, &end, 10);
        if (buffer == end) {
            return MISSING_DATA; /* No valid number found */
        }

        /* Check if the number is in range */
        if (number < -0x800 || number > 0x7FF) {
            return NUMBER_OUT_OF_RANGE;
        }

        /* Write each number in a separate word in memory */
        data_array[*DC] = (Word)number;
        (*DC)++;

        buffer = end; /* Move buffer to end to process the next number */
    }

    /* Return success */
    return SUCCESS;
}


/* Process a directive and export it to the data array */
int export_string(char* buffer, int* DC, Word* data_array) {
    int i;
    char* label;
    char* current;
    char* temp; /* Temporary variable to hold a duplicate of the current string */

    /* Extract the label from the buffer */
    label = word_without_space(buffer, 0);
    /* Check if the label is ".string" and adjust the position in the buffer accordingly */
    if (strcmp(label, ".string") == 0) {
        current = word_without_space_to_commas(buffer, strlen(".string")+1);
    } else {
        current = word_without_space_to_commas(buffer, strlen(label) + strlen(".string")+1);
    }
    free(label);/*free the word*/

    /* Create a duplicate of the current string to work with */
    temp = my_strdup(current); /* Allocate memory for a duplicate of the current string */

    /* Free the original current string memory */
    free(current);

    /* Tokenize the string on double quotes */
    current = temp;
    

    /* Check if the tokenization was successful */
    if (current == NULL) {
        free(temp); /* Free the memory allocated for temp if no token was found */
        return 0;
    }
    /* Check if there is enough space in the data array */
    if (*DC + strlen(current) >= MEMORY_SIZE) {
        free(temp); /* Free the memory allocated for temp if there is not enough space */
        return 0;
    }

    /* Copy each character of the string into the data array */
    for (i = 1; current[i] != '\0'; i++) {
        if(current[i]=='"'){
            continue;
        }
        data_array[*DC] = (unsigned short)current[i] & 0x7FFF;
        (*DC)++;
    }

    /* Null-terminate the data array and update DC */
    data_array[*DC] = 0;
    (*DC)++;
    /* Free the memory allocated for temp */
    free(temp);
    return 1;
}

/* Encode the first word */
unsigned short encode_instruction_first_word(unsigned short opcode, unsigned short src_op,
    unsigned short dst_op, int ARE)
{
    /* Encode the instruction word */
    unsigned short word = 0;
    int shift_src = 0, shift_dest = 0;
    int mask=1;
    int mask_src=1;
    /* Encode each part of the word according to the operands */
    switch (src_op)
    {
        case 0:
            shift_src = 7;                          
            break;          

        case 1:
            shift_src = 8;
            break;

        case 2:
            shift_src = 9;
            break;

        case 3:
            shift_src = 10;
            break;
        /*for a inst withote parameters*/
        case 4:
            mask_src = 0;
            shift_src = 0;
            
            break;
    }

    switch (dst_op)
    {
        case 0:
            mask = 1;
            shift_dest = 3;                          
            break;          

        case 1:
            mask = 1;
            shift_dest = 4;
            break;

        case 2:
            mask = 1;
            shift_dest = 5;
            break;

        case 3:
            mask = 1;
            shift_dest = 6;
            break;
        case 4:
            mask=0;
            shift_dest = 0;
            break;
    }
    /* Combine the parts of the word */
    word |= (opcode << OPCODE_SHIFT);
    word |= (mask_src << shift_src);
    word |= (mask << shift_dest);                        
    word |= 4&0x7;                 
    return word;
}

/* Encode the word for the case we have 2 registers or pointers to registers*/
unsigned short special_case_2_registers_or_pointers(unsigned short value_reg_src,unsigned short value_reg_dst, unsigned short ARE){
    unsigned short word = 0;      
    /* Encode the word */             
    word |= (value_reg_src << SRC_MODE_SHIFT);/*6-8*/   
    word |= (value_reg_dst << DST_MODE_SHIFT);/*5-3*/   
    word |= (ARE << ARE_SHIFT&0x7);/*2-0*/

    return word;
}

/* Encode the next word*/
unsigned short special_case_for_2_or_1_word(char *op, unsigned short shift){
    int method1;
    int num_reg;
    unsigned short word = 0;
    method1 = get_methode(op);
    num_reg = get_register(op);
    /*  numbers = 0, labels = 1, pointers = 2, registers = 3*/
    switch (method1)
    {
        case 0:
        /* Immediate addressing */
            op=op+1;
            word |= ((atoi(op) << 3)&0x7FFF);
            word |= defualt_ARE;
            break;

        case 1:
        /* Direct addressing */
            word = 0;
            break;

        case 2:
        /*pointer to register*/
            word |= (num_reg << shift);
            word |= defualt_ARE; 
            break;

        case 3:
        /*register*/
            word |= (num_reg << shift);
            word |= defualt_ARE; 
            break;
    }
    /* Return the encoded word */
    return word;
}

ErrorCode add_to_symbol_unknown_address(char* label, int IC, Word word)
{
    /* Add an entry to the table of undefined labels */
    UnknownReference* new_unknown_references;
    /* Add an entry to the table of undefined labels */
    UnknownReference ref;

    if (unknown_references == NULL) {
        return ALLOCATION_ERROR; /* Error - memory not initialized */
    }
    /* If the table needs to be expanded */
    if (unknown_references_count >= unknown_references_capacity) {
        unknown_references_capacity *= 2;
        new_unknown_references = (UnknownReference*)realloc(unknown_references,
            unknown_references_capacity * sizeof(UnknownReference));
        if (new_unknown_references == NULL) {
            return ALLOCATION_ERROR; /*allocation failled*/
        }
        unknown_references = new_unknown_references;
    }
    /* Add the label and its address to the table */
    strncpy(ref.label, label, MAX_LENGTH_WORD);
    ref.label[MAX_LENGTH_WORD] = '\0';
    ref.word = word;
    ref.IC = IC;
    unknown_references[unknown_references_count++] = ref;

    return SUCCESS;
}
ErrorCode init_unknown_references(void)
{
    unknown_references_count = 0;
    unknown_references_capacity = 10; 
    /* Initialize the table of undefined labels */
    unknown_references = (UnknownReference*)malloc(
        unknown_references_capacity * sizeof(UnknownReference));
    if (unknown_references == NULL) {
        /*allocation failled*/
        return ALLOCATION_ERROR;
    }
    return SUCCESS;
}
/* Free the table of undefined labels */
void free_unknown_references(void)
{
    if (unknown_references != NULL) {
        free(unknown_references);
        unknown_references = NULL;
        unknown_references_count = 0;
        unknown_references_capacity = 0;
    }
}
