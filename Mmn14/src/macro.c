
#include "macro.h"
/*the name of the macro*/
char current_macro_name[MAX_MACRO_NAME];


int main_process(char* file_name, char* output_filename)
{
    /*the va*/
    int size_line = MAX_LINE_LENGTH;
    FILE* file;
    FILE* output_file;
    char* new_line=NULL;
    char* buffer=NULL;
    char* exstra=NULL;
    ErrorCode result;
    int num_line = 0;
    my_bool flag_error = FALSE;
    my_bool in_macro_definition = FALSE;

    /*open the file*/
    file = fopen(file_name, "r");
    if (file == NULL) {
        perror("Failed to open input file");
        return FILE_ERROR;
    }

   /*open the output file*/
    output_file = fopen(output_filename, "w");
    if (output_file == NULL) {
        perror("Failed to open output file");
        fclose(file);
        return FILE_ERROR;
    }
    /*allocate the buffer*/
    buffer = (char*)malloc(size_line);
    memset(buffer, 0, size_line);
    /*check if the allocation was successful*/
    if (buffer == NULL) {
        perror("Failed to allocate buffer");
        fclose(file);
        fclose(output_file);
        return ALLOCATION_ERROR;
    }
    /*init the macro table*/
    init_macro();
    /*read the file line by line*/
    while (1) {
        if (fgets(buffer, MAX_LINE_LENGTH, file) == NULL) {
        
            break;
        }
        removeExtraSpaces(buffer);
        /*check if the line is a comment*/
        if(buffer[0]==';'){
            continue;
        }
        if(buffer[0]=='\0'){
            continue;
        }
        num_line++;
        new_line = strchr(buffer, '\n');
        if (new_line) {
            *new_line = '\0';
        }
        /*check if the line is a macro definition*/
        if (in_macro_definition == TRUE) {
            /*check if the line is the end of the macro definition*/
            if (strncmp(buffer, "endmacr", strlen("endmacr")) == 0) {
                /*check if there are extra tokens after the end of the macro definition*/
                exstra=word_without_space(buffer, strlen("endmacr"));
                if (exstra != NULL) {
                    free(exstra);/*free the word*/
                    print_error(EXTRA_TOKENS_END, num_line);
                    flag_error = TRUE;
                }
                /*end the macro definition*/
                in_macro_definition = FALSE;
            } else {
                /*add the line to the macro*/
                add_line_to_macro(current_macro_name, buffer);
            }
        } else {
            /*read the line and process it*/
            result = read_line(buffer, output_file);
            /*check if we are in a macro defenition*/
            if (result == MACRO_DEFINITION) {
                in_macro_definition = TRUE;

            } else if (result != SUCCESS && result != EMPTY_LINE) {
                flag_error = TRUE;
                print_error(result, num_line);
                /*break;*/
            }
        }
    }
    /*free the buffer*/
    free(buffer);
    fclose(file);
    fclose(output_file);
    if (flag_error) {
        remove(output_filename);
    }
    return (flag_error ? 1 : 0);
}

/*a function that process every line*/
ErrorCode read_line(char* buffer, FILE* file_out)
{
    char* new_line = NULL;
    char* name = NULL;
    char* exstra = NULL;
    Macro* macro;
    ErrorCode name_check;
    /*check if the line is empty*/
    new_line = word_without_space(buffer, 0);
    if(new_line == NULL)
        return EMPTY_LINE;
   
        /*check if the line is a macro definition*/
    if (strstr(buffer, "macr") != NULL) {
        if (strcmp(new_line, "macr") == 0) {

            name = word_without_space(buffer, 5);
            if (name == NULL){
                free(new_line);/*free the word*/
                return NO_MACRO_DEFINITION;
            }
            name_check = is_valid_name(name);
            if (name_check != SUCCESS) {
                free(new_line);/*free the word*/
                free(name);/*free the word*/
                return name_check;
            }
            if (add_macro(name) != 1) {
                free(new_line);/*free the word*/
                free(name);/*free the word*/
                return ALLOCATION_ERROR;
            }
            strncpy(current_macro_name, name, MAX_MACRO_NAME - 1);
            current_macro_name[MAX_MACRO_NAME - 1] = '\0';
            exstra=word_without_space(buffer, strlen("macr") + strlen(name) + 1);
            if (exstra != NULL) {
                free(exstra);/*free the word*/
                free(new_line);/*free the word*/
                free(name);/*free the word*/
                return EXTRA_TOKENS;

            }
            free(name);/*free the word*/
            free(new_line);/*free the word*/
            
            return MACRO_DEFINITION;
        } else {
            /*check if the line is a macro call*/
            macro = find_macro(new_line);
            if (macro) {
                /*replace the macro call with the macro definition*/
                replace_macro(macro->name, file_out);

            } else {
                /*copy the text to the output file*/
                copy_text(file_out, new_line);
            }
            free(new_line);/*free the word*/
            return SUCCESS;
        }
    }
    copy_text(file_out, buffer);
    free(new_line);/*free the word*/
    free(name);/*free the word*/
    return SUCCESS;
}

/*check if the name is valid*/
ErrorCode is_valid_name(char* name)
{
    size_t j, k, i;
    /*check if the length of the name is valid*/
    if (strlen(name) > MAX_MACRO_NAME)
        return INVALID_MACRO_NAME;
    /*check if the name is already defined*/
    if (find_macro(name) != NULL)
        return MACRO_ALREADY_DEFINED;
    /*check if the name is the same as a register or instruction*/
    for (i = 0; i < NUM_REG; i++) {
        if (which_reg(name) !=-1) {
            return MACRO_SAME_AS_REGISTER;
        }
    }
    for (j = 0; j < instruction_count; j++) {
        if (strcmp(name, instructions[j]) == 0) {
            return MACRO_SAME_AS_INSTRUCTION;
        }
    }
    /*check if the name is valid*/
    for (k = 0; name[k]; k++) {
        if (!isalnum(name[k]) && name[k] != '_') {

            return INVALID_MACRO_NAME;
        }
    }
    return SUCCESS;
}

/*replace the macro call with the macro definition*/
void replace_macro(char* macro_name, FILE* file_out)
{
    size_t i;
    /*find the macro*/
    Macro* macro = find_macro(macro_name);
    if (macro) {
        /*copy the lines of the macro to the file*/
        for (i = 0; i < macro->line_count; i++) {
            fprintf(file_out, "%s\n", macro->lines[i]);
        }
    }
}

/*copy the text to the file*/
void copy_text(FILE* file_out, char* text)
{
    fprintf(file_out, "%s\n", text);
}

