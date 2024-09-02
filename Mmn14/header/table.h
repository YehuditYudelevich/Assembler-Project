
#ifndef TABLE_H
#define TABLE_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>



/* Maximum length of a symbol name*/
#define MAX_SYMBOL_NAME 31
/* Initial capacity of the symbol table */
#define MAX_SYMBOLS 100

/**
 * Enumeration of symbol types.
 * CODE - Represents code-related symbols.
 * DATA - Represents data-related symbols.
 * RELOCATABLE - Represents symbols that are relocatable.
 */
typedef enum {
    CODE, /* Code symbol type */
    DATA, /* Data symbol type */
    RELOCATABLE, /* Relocatable symbol type */
    UNKNOWN_SYMBOL
} SymbolType;

/**
 * Enumeration of symbol attributes.
 * ENTRY - Symbol marked as an entry point.
 * EXTERNAL - Symbol that is external.
 * INNER - Symbol that is internal to the module.
 */
typedef enum {
    ENTRY, /* Entry attribute */
    EXTERNAL, /* External attribute */
    INNER, /* Inner attribute */
    UNKNOWN_SYMBOL_ATTRIBUTE
} SymbolAttribute;

/*
 * Structure representing a symbol that are entry.
 * Contains a name, address, type, and attribute of the symbol.
 */
typedef struct{
    char name[MAX_SYMBOL_NAME + 1]; /* Symbol name */
    int address; /* Symbol address */
}entry;

/*
 * Structure representing a symbol that are entry.
 * Contains a name, address, type, and attribute of the symbol.
 */
typedef struct{
    entry* entries; /* Pointer to the array of symbols */
    int count; /* Number of symbols currently in the table */
    int capacity; /* Capacity of the symbol table */
}entryTab;

/**
 * Structure representing a symbol.
 * Contains a name, address, type, and attribute of the symbol.
 */
typedef struct {
    char name[MAX_SYMBOL_NAME + 1]; /* Symbol name */
    int address; /* Symbol address */
    SymbolType type; /* Type of the symbol (CODE, DATA, RELOCATABLE) */
    SymbolAttribute attribute; /* Attribute of the symbol (ENTRY, EXTERNAL, INNER) */
} Symbol;

/**
 * Structure representing a symbol.
 * Contains a name, address, type, and attribute of the symbol.
 */
typedef struct {
    Symbol* symbols; /* Pointer to the array of symbols */
    int count; /* Number of symbols currently in the table */
    int capacity; /* Capacity of the symbol table */
} SymbolTable;

/**
 * Initializes the symbol table by allocating memory
 * for the table and its symbols array. Sets initial capacity
 * and count.
 */
void init_symbol_table(void);

/**
 * Adds a new label to the symbol table. Expands the table if needed.
 *
 * @param name The name of the symbol.
 * @param address The address associated with the symbol.
 * @param type The type of the symbol (e.g., CODE, DATA, RELOCATABLE).
 * @param attribute The attribute of the symbol (e.g., ENTRY, EXTERNAL, INNER).
 * @return 1 on success, 0 on failure (e.g., memory allocation failure).
 */
int add_label(
    char* name, int address, SymbolType type, SymbolAttribute attribute);

/**
 * Checks if a symbol with the given name is already defined in the table.
 *
 * @param name The name of the symbol to find.
 * @return 0 if the symbol is found, 1 otherwise.
 */
int find_symbol(char* name);


/**
 * Retrieves the address of a symbol from the symbol table.
 *
 * @param name The name of the symbol.
 * @return The address of the symbol, or -1 if not found.
 */
int get_symbol_address(char* name);

/**
 * Removes a symbol from the symbol table by clearing its data.
 * 
 * This function searches for a symbol with the specified name in the symbol table and clears its
 * data if found. The symbol's name is set to an empty string, and its address, type, and attribute
 * are set to zero.
 * 
 * @param name A pointer to the name of the symbol to be removed.
 */
void remove_symbol(char* name);

/**
 * Frees the memory used by the symbol table and its symbols array.
 */
void free_symbol_table(void);

/**
 * Finds the name of the entry symbol.
 * 
 * This function searches for a symbol with the specified name in the entry table. If the symbol is
 * found, the function returns 0. Otherwise, it returns 1.
 * 
 * @param entry_table A pointer to the entry table.
 * @param name A pointer to the name of the symbol to find.
 * @return 0 if the symbol is found, 1 otherwise.
 */
int find_the_name_fo_entry_symbol(entryTab* entry_table,char* name);
/**
 * Initializes the entry table by allocating memory
 * for the table and its symbols array. Sets initial capacity
 * and count.
 */
void init_entry_tab(entryTab* entry_table);

/**
 * Adds a new entry to the entry table. Expands the table if needed.
 *
 * @param name The name of the symbol.
 * @param address The address associated with the symbol.
 * @return 1 on success, 0 on failure (e.g., memory allocation failure).
 */
int add_entry(entryTab* entry_table,char* name, int *IC);

/**
 * Frees the memory used by the entry table.
 * @param entry_table The entry table to free.
 */
void free_entry_tab(entryTab* entry_table);

/**
 * check if there is an external symbol in the symbol table.
 */
int include_extern();

/**
 * Checks if a symbol is external.
 * 
 * This function checks if a symbol is external by searching for it in the symbol table. If the
 * symbol is found and its attribute is EXTERNAL, the function returns 1. Otherwise, it returns 0.
 * 
 * @param file A pointer to the file to read from.
 * @return 1 if the symbol is external, 0 otherwise.
 */
int check_if_extern(FILE *file);


#endif
