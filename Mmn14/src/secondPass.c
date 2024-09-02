#include "secondPass.h"

/* Processes the second pass of the assembly */
ErrorCode main_process_2(entryTab* entry_table, int* IC, int* DC, char* file, Word* instruction_array, Word* data_array, char* ob_file_name, char* ent_file_name, char* ext_file_name) {
    ErrorCode error1;
    ErrorCode error2;
    

     /* Check if all labels used in instructions are declared */
    error1 = check_if_declered(instruction_array);
    /* If any label is not declared, return error */
    if (error1 == LABEL_NOT_DECLERED)
    {
        print_error_second_pass(LABEL_NOT_DECLERED);
        return LABEL_NOT_DECLERED;
    }
    /* Check if all entry labels are declared */
    error2 = check_if_entry_labels_declered(entry_table);
    if (error2 == ENTRY_NOT_FOUND)
    {
        print_error_second_pass(ENTRY_NOT_FOUND);
        return ENTRY_NOT_FOUND;
    }
    /* Export the entry file if there are entries in the table */
    if(entry_table->count!=0){
        export_ent_file(entry_table,ent_file_name);
    }
    /* Export the extern file if there are external symbols */
    if(include_extern()==1){
        export_ext_file(ext_file_name);
    }
    /* Export the object file */
    export_ob_file(ob_file_name, IC, DC, instruction_array, data_array);

    return SUCCESS;

}

/* Export the entry table to a file */
void export_ent_file(entryTab* entry_table, char* ent_file_name)
{
    FILE* ent_file;
    int i;
    int address;
    /* Open the entry file for writing */
    ent_file = openFile(ent_file_name, "w");
    if(ent_file==NULL){
        printf("Error opening file\n");
        exit(EXIT_FAILURE);
    }
    /* Write each entry's name and address to the file */
    for (i = 0; i < entry_table->count; i++)
    {
        address=get_symbol_address(entry_table->entries[i].name);
        fprintf(ent_file, "%s %04d\n", entry_table->entries[i].name, address);
    }
    fclose(ent_file);
}

/* Export the external references to a file */
void export_ext_file(char* ext_file_name)
{
    FILE* ext_file;
    /* Open the extern file for writing */
    ext_file = openFile(ext_file_name, "w");
    if(ext_file==NULL){
        printf("Error opening file\n");
        exit(EXIT_FAILURE);
    }
    /* Check if there are external symbols and write them to the file */
    if(check_if_extern(ext_file)!=0){
        printf("Error writing to file\n");
        exit(EXIT_FAILURE);
    }
    fclose(ext_file);
}

/* Export the object file */
void export_ob_file(char* ob_file_name, int *IC, int *DC, Word* instruction_array, Word* data_array)
{
    FILE* ob_file;
    int i;
    int j;
    /* Open the object file for writing */
    ob_file = openFile(ob_file_name, "w");

    if(ob_file==NULL){
        printf("Error opening file\n");
        exit(EXIT_FAILURE);
    }
    /* Write the IC and DC to the file */
    fprintf(ob_file, "    %d %d\n", *IC-100, *DC);
     /* Write the instruction array in octal format */
    for (i = 0; i < *IC-100; i++)
    {
        print_the_address(ob_file, i+100);
        printToOBFileOctal(ob_file, instruction_array[i]);
        
    }
   
     /* Write the data array in octal format */
    for (j = 0; j < *DC; j++)
    {
        print_the_address(ob_file, j+*IC);
        printToOBFileOctal(ob_file, data_array[j]);
        
    }
    fclose(ob_file);
    

}

/* Print the address to the file */
void print_the_address(FILE* file, int address){
    /* Print the address*/
    fprintf(file, "%04d  ", address);
}

/* Check if all labels used in instructions are declared */
ErrorCode check_if_declered(Word *instruction_array){
    int i;
    int curr_ic;
    int address;
    unsigned short word_to_export;
    /* Check if each label in unknown_references is declared */
    for(i=0;i<unknown_references_count;i++){
        if(find_symbol(unknown_references[i].label)==0){
            curr_ic=unknown_references[i].IC;
            address=get_symbol_address(unknown_references[i].label);
            /* Encode the address and ARE (absolute/relocatable/extern) into the word */
            if(address==0){/*extern*/
                word_to_export=encode_unknown_references_word(address,1);
            }
            else{
                word_to_export=encode_unknown_references_word(address,2);
            }
            /* Update the instruction array with the encoded word */
            instruction_array[curr_ic - 100] = word_to_export;
            
        } 
    }
    return SUCCESS;
}

/* Check if all entry labels are declared in this file */
ErrorCode check_if_entry_labels_declered(entryTab* entry_table){
    int i;
    ErrorCode result=SUCCESS;
    /* Check if each entry in the entry table is declared */
    for(i=0;i<entry_table->count;i++){
        /* If the entry is not declared, return error */
        if(find_symbol(entry_table->entries[i].name)!=0){
            result= ENTRY_NOT_FOUND;
        }    
    }
    return result; 
} 

/* Encodes the address and ARE into a word */
unsigned short encode_unknown_references_word(int address,int ARE){
    unsigned short word = 0; 
    word|=address<<3;
    word|=ARE&0x7;
    return word;
}

/* Open a file */
FILE* openFile(const char* file_name, const char* mode) {
    return fopen(file_name, mode);
}

/* Print the number to the file in octal format */
void printToOBFileOctal(FILE* file, unsigned int number) {
    /* Mask the number to get the rightmost 15 bits */
    unsigned short mask = 0x7FFF;
    /* Get the rightmost 15 bits */
    unsigned short rightmost5 =number&mask;
    /* Print the number in octal format on 5 pedding*/
    fprintf(file, "%05o\n", rightmost5 );
}




