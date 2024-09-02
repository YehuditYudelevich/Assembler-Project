/*
 * Assembler Simulation Project
 *
 * Created by Yehudit Yudlevich and Odalia Silber, this project simulates an assembler's functionality. It processes input files given as command-line arguments, validates them, and generates encoded binary output files.
 *
 * Features:
 * - **Input Handling**: Accepts multiple files for processing.
 * - **Validation**: Ensures file correctness before encoding.
 * - **Encoding**: Converts validated source code into binary format.
 * - **Output**: Produces corresponding encoded files.
 *
 * This project provides a practical simulation of assembler operations, making it a useful tool for understanding and implementing encoding processes.
 */

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "table.h"
#include "first_pass.h"
#include "globals.h"
#include "macro.h"
#include "secondPass.h"

int assembler(char* base_name);

int main(int argc, char* argv[])
{
    int i;
    int err = 0;
    printf("Welcome to our assembler!\nThis program takes assembly code  and "
           "assembles it,\nturning it into a object file type that the "
           "computer is capable of reading!\n");
    i = 1;
    /*print welcome message and explanation if someone just called the assembler
     * with no input files*/
    if (argc <= 1) {
        printf("To use, write in the console: ./assembler {file_name_1} ... "
               "{file_name_n}\n");
        return 1;
    }

    /* Process each input file by calling the assembler function */
    for (; i < argc; i++) {
        if (assembler(argv[i])) {
            err = 1;
        }
    }
    return err;
}
/*the assembler function*/
int assembler(char* base_name)
{
    FILE* input_file = NULL;
    FILE* output_file = NULL;
    int result_macro = 0;
    ErrorCode result1 = SUCCESS;
    ErrorCode result2 = SUCCESS;
    int IC = 100;
    int DC = 0;
    entryTab entry_table;
    Word instruction_array[MEMORY_SIZE];
    Word data_array[MEMORY_SIZE];

    char as_file_name[NAME_LENGTH];
    char am_file_name[NAME_LENGTH];
    char ob_file_name[NAME_LENGTH];
    char ent_file_name[NAME_LENGTH];
    char ext_file_name[NAME_LENGTH];

    memset(instruction_array, 0, MEMORY_SIZE * sizeof(Word));
    memset(data_array, 0, MEMORY_SIZE * sizeof(Word));

    memset(as_file_name, 0, NAME_LENGTH);
    memset(am_file_name, 0, NAME_LENGTH);
    memset(ob_file_name, 0, NAME_LENGTH);
    memset(ent_file_name, 0, NAME_LENGTH);
    memset(ext_file_name, 0, NAME_LENGTH);

    strcat(as_file_name, base_name);
    strcat(as_file_name, ".as");
    strcat(am_file_name, base_name);
    strcat(am_file_name, ".am");
    strcat(ob_file_name, base_name);
    strcat(ob_file_name, ".object");
    strcat(ent_file_name, base_name);
    strcat(ent_file_name, ".entries");
    strcat(ext_file_name, base_name);
    strcat(ext_file_name, ".externals");

    result_macro = main_process(as_file_name, am_file_name);

    if (result_macro == 0) {
        printf("Macro processing completed successfully. the file in '%s' .\n", am_file_name);
    } else {
        remove(am_file_name);
        printf("Error in macro processing of file '%s'.\n", as_file_name);
        free_macros();
        return 1;
    }
    
    free_macros();

    init_entry_tab(&entry_table);
    printf("the process macro end\n");

    result1 = main_process_1(&entry_table, &DC, &IC, am_file_name, instruction_array, data_array);
    if (result1 != SUCCESS) {
        printf("Error in first pass of file '%s'.\n", am_file_name);
        free_symbol_table();
        free_unknown_references();
        free_entry_tab(&entry_table);
        return 1;
    } else {
        printf("First pass completed successfully.\n");
        result2 = main_process_2(&entry_table, &IC, &DC, am_file_name, instruction_array, data_array, ob_file_name, ent_file_name, ext_file_name);
        if (result2 != SUCCESS) {
            printf("Error in second pass of file '%s'.\n", am_file_name);
            free_symbol_table();
            free_entry_tab(&entry_table);
            free_unknown_references();
            return 1;
        } else {
            printf("Second pass completed successfully check the files.\n");
        }
    }

    free_symbol_table();
    free_unknown_references();
    free_entry_tab(&entry_table);

    if (input_file != NULL) {
        fclose(input_file);
    }
    if (output_file != NULL) {
        fclose(output_file);
    }

    return 0;
}

