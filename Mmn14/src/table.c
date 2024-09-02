

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "table.h"
#include "first_pass.h"
/*pointer to the SymbolTable*/
SymbolTable* symbolTable;

/* Initializes the symbol table.*/
void init_symbol_table(void)
{
    symbolTable = (SymbolTable*)malloc(sizeof(SymbolTable));
    if (symbolTable == NULL) {
        return; /* Allocation error */
    }
    symbolTable->capacity = MAX_SYMBOLS;
    symbolTable->count = 0;
    symbolTable->symbols
        = (Symbol*)malloc(symbolTable->capacity * sizeof(Symbol));
    if (symbolTable->symbols == NULL) {
        /* Allocation error, free the table */
        free(symbolTable);
        return;
    }
}

/* Adds a label to the symbol table.*/
int add_label(
    char* name, int address, SymbolType type, SymbolAttribute attribute)
{
    Symbol* new_table;
    if (symbolTable->count >= symbolTable->capacity) {
        /* If the symbol table is full, increase its capacity */
        symbolTable->capacity += MAX_SYMBOLS;
        new_table = (Symbol*)realloc(
            symbolTable->symbols, symbolTable->capacity * sizeof(Symbol));
        if (new_table == NULL) {
            return 0; /* Allocation error */
        }
        symbolTable->symbols = new_table;
    } /* Copy the name to the new symbol, ensuring it fits within
         MAX_SYMBOL_NAME */
    strncpy(
        symbolTable->symbols[symbolTable->count].name, name, MAX_SYMBOL_NAME);
    symbolTable->symbols[symbolTable->count].name[MAX_SYMBOL_NAME] = '\0';
    symbolTable->symbols[symbolTable->count].address = address;
    symbolTable->symbols[symbolTable->count].type = type;
    symbolTable->symbols[symbolTable->count].attribute = attribute;
    symbolTable->count++;
    return 1;
}
/* Finds a symbol by name.*/
int find_symbol(char* name)
{
    int i;
    if(symbolTable->count == 0){
    }
    for (i = 0; i < symbolTable->count; i++) {
        if (strcmp(symbolTable->symbols[i].name, name) == 0) {
            return 0; /*if the symbol is already defined*/
        }
    }
    return 1; /* Symbol is not defined */
}

/* Gets the address of a symbol by name.*/
int get_symbol_address(char* name)
{
    int i;
    /* Search for the symbol by name */
    for (i = 0; i < symbolTable->count; i++) {
        if (strcmp(symbolTable->symbols[i].name, name) == 0) {
            return symbolTable->symbols[i].address;
        }
    }
    return -1; /* Symbol not found */
}
/* Removes a symbol by name.*/
void remove_symbol(char* name)
{
    int i;
    /* Search for the symbol by name */
    for (i = 0; i < symbolTable->count; i++) {
        if (strcmp(symbolTable->symbols[i].name, name) == 0) {
            /* Remove the symbol by shifting the remaining symbols */
            symbolTable->symbols[i].name[0] = '\0';
            symbolTable->symbols[i].address = 0;
            symbolTable->symbols[i].type = 0;
            symbolTable->symbols[i].attribute = 0;
        }
    }
}

/* Frees all memory associated with the symbol table.*/
void free_symbol_table(void)
{
    if (symbolTable) {
        free(symbolTable->symbols); /* Free the array of symbols */
        free(symbolTable); /* Free the symbol table */
        symbolTable = NULL; /* Set the global pointer to NULL */
    }

}


/* Finds the name of the entry symbol.*/
int find_the_name_fo_entry_symbol(entryTab* entry_table,char* name){
    int i;
    for (i = 0; i < entry_table->count; i++) {
        if (strcmp(entry_table->entries[i].name, name) == 0) {
            return 0; /*if the symbol is already defined*/
        }
    }
    return 1; /* Symbol is not defined */
}

/* Initializes the entry table.*/
void init_entry_tab(entryTab* entry_table) {
    /* Allocate memory for the entry table and init the tabel */
    entry_table->count = 0;
    entry_table->capacity = MAX_SYMBOLS;
    entry_table->entries = (entry*)malloc(entry_table->capacity * sizeof(entry));
    if (entry_table->entries == NULL) {
        perror("Failed to initialize entry table");
        exit(EXIT_FAILURE);
    }
}

/* Adds a new entry to the entry table.*/
int add_entry(entryTab* entry_table,char* name, int *IC) {
    /* Check if the table needs to be resized */
    if (entry_table->count >= entry_table->capacity) {
        int new_capacity = entry_table->capacity * 2;
        entry* new_entries = (entry*)realloc(entry_table->entries, new_capacity * sizeof(entry));
        if (new_entries == NULL) {

            return -1; /* Indicate failure*/
        }
        /* Update the entry table */
        entry_table->entries = new_entries;
        entry_table->capacity = new_capacity;
    }
    /* Check if the entry already exists */
    if(find_the_name_fo_entry_symbol(entry_table,name)==0){
        return -1;/*entry fedenition twice*/
    }
    /* Add the new entry */
    strncpy(entry_table->entries[entry_table->count].name, name, MAX_SYMBOL_NAME);
    entry_table->entries[entry_table->count].name[MAX_SYMBOL_NAME] = '\0'; 
    entry_table->entries[entry_table->count].address = *IC;
    entry_table->count++;

    return 0; 
}

/* Frees the memory associated with the entry table.*/
void free_entry_tab(entryTab* entry_table) {
    free(entry_table->entries); /* Free the array of entries */
    entry_table->entries = NULL; /* Set the global pointer to NULL */
    entry_table->count = 0; /* Reset the count */
    entry_table->capacity = 0; /* Reset the capacity */
}

/* Check if there are external symbols */
int include_extern(){
    int i;
    /* Check if there are external symbols */
    for(i=0;i<symbolTable->count;i++){
        if(symbolTable->symbols[i].attribute==EXTERNAL){
            return 1;
        }
    }
    return 0;
}

/* Export the external references to a file */
int check_if_extern(FILE *file){
    int i,j;
    /* Check if there are external symbols */
    for(i=0;i<symbolTable->count;i++){
        if(symbolTable->symbols[i].attribute==EXTERNAL){
            /*go throw the labels*/
            for(j=0;j<unknown_references_count;j++){
                if(strcmp(symbolTable->symbols[i].name,unknown_references[j].label)==0){
                    /* Write the external symbol to the file */
                    fprintf(file, "%s %04d\n", symbolTable->symbols[i].name, unknown_references[j].IC);
                }
            }
        }
    }
    return 0;
}
