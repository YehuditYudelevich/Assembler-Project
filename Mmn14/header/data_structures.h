#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*size of macro name*/
#define MAX_MACRO_NAME 30

/* Macro structure to hold information about a macro.*/
typedef struct {
    char name[MAX_MACRO_NAME]; /*the name of the macro*/
    char** lines; /* Pointer to an array of strings fot the lines*/
    size_t line_count; /*the number of lines in the current macro*/
    size_t capacity; /*the capacity of lines*/
} Macro;

/* Macro list structure to hold a list of macros.*/
typedef struct {
    Macro* macros; /*Pointer to an array of Macro structures*/
    size_t count; /*the number of macros currently in list*/
    size_t capacity; /*the total capacity*/
} MacroList;

/**
 * @brief Initializes the macro list.
 *
 * This function initializes the global macro list by setting its macros pointer to NULL,
 * count to 0, and capacity to 0.
 */
void init_macro(void);

/**
 * @brief Adds a new macro to the macro list.
 *
 * This function adds a new macro to the global macro list. If the list's capacity is full,
 * it reallocates memory to accommodate the new macro. The macro's name is truncated if it
 * exceeds the maximum allowed length.
 *
 * @param name The name of the macro to be added.
 * @return 1 on success, 0 on allocation error.
 */
int add_macro(char* name);

/**
 * @brief Adds a line to an existing macro.
 *
 * This function adds a line to an existing macro identified by its name. If the macro's
 * line capacity is full, it reallocates memory to accommodate the new line.
 *
 * @param name The name of the macro to which the line should be added.
 * @param line The line to be added to the macro.
 */
void add_line_to_macro(char* name, char* line);

/**
 * @brief Finds a macro by its name.
 *
 * This function searches for a macro in the macro list by its name.
 * If the macro is found, it returns a pointer to the macro; otherwise, it returns NULL.
 *
 * @param name The name of the macro to find.
 * @return A pointer to the macro if found, otherwise NULL.
 */
Macro* find_macro(char* name);

/**
 * @brief Frees memory allocated for macros.
 *
 * This function frees all memory allocated for the macros in the macro list,
 * including the lines of each macro. It resets the macro list to its initial state.
 */
void free_macros(void);
/**
 * @brief Duplicates a string.
 *
 * This function duplicates a string by allocating memory for a new string and copying the
 * contents of the original string to the new one.
 *
 * @param str The string to be duplicated.
 * @return A pointer to the duplicated string.
 */
char *my_strdup(char *str);

#endif
