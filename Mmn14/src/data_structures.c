#include "data_structures.h"

MacroList macro_list;
/*
 * Initializes the global macro list.
 * Sets macros to NULL, count to 0, and capacity to 0.
 */
void init_macro(void)
{
    macro_list.macros = NULL;
    macro_list.count = 0;
    macro_list.capacity = 0;
}
/*add a new macro*/
int add_macro(char* name)
{
    Macro* new_macro;
    Macro* new_macros;
    size_t new_capacity;
    size_t name_length;


    printf("MACRO ADD (%s)\n", name);

    /* If the macro list is full, increase its capacity */
    if (macro_list.count == macro_list.capacity) {
        new_capacity = macro_list.capacity == 0 ? 1 : macro_list.capacity * 2;
        new_macros = realloc(macro_list.macros, new_capacity * sizeof(Macro));
        if (!new_macros) {
            return 0; /* Allocation error */
        }
        macro_list.macros = new_macros;
        macro_list.capacity = new_capacity;
    }
    /* Add the new macro to the list */
    new_macro = &macro_list.macros[macro_list.count++];
    /* Copy the name to the new macro, ensuring it fits within MAX_MACRO_NAME */
    name_length = strlen(name);
    if (name_length >= MAX_MACRO_NAME) {
        strncpy(new_macro->name, name, MAX_MACRO_NAME - 1);
        new_macro->name[MAX_MACRO_NAME - 1] = '\0';
    } else {
        strcpy(new_macro->name, name);
    }
    /* Initialize the lines, line_count, and capacity of the new macro */
    new_macro->lines = NULL;
    new_macro->line_count = 0;
    new_macro->capacity = 0;

    return 1; /* Success */
}
/* Adds a new line to the macro with the given name*/
void add_line_to_macro(char* name, char* line)
{
    Macro* macro = NULL;
    size_t i;
    size_t new_capacity;
    char** new_lines;

    /* Find the macro by name */
    for (i = 0; i < macro_list.count; i++) {
        if (strcmp(macro_list.macros[i].name, name) == 0) {
            macro = &macro_list.macros[i];
            break;
        }
    }

    /* If the macro is not found, exit the function */
    if (!macro) {
        return;
    }
    /* If the macro is full, increase its capacity */
    if (macro->line_count == macro->capacity) {
        new_capacity = macro->capacity == 0 ? 1 : macro->capacity * 2;
        new_lines = realloc(macro->lines, new_capacity * sizeof(char*));
        if (!new_lines) {
            return;
        }
        macro->lines = new_lines;
        macro->capacity = new_capacity;
    }
    /* Add the new line to the macro */
    macro->lines[macro->line_count++] = my_strdup(line); /* Duplicate the line and add it to the array */
}
/* Finds a macro by name.*/
Macro* find_macro(char* name)
{
    size_t i;
    /*go over the table and find the macro, if exist*/
    for (i = 0; i < macro_list.count; i++) {
        if (strcmp(macro_list.macros[i].name, name) == 0) {
            /* Return pointer to the found macro */
            return &macro_list.macros[i];
        }
    }
    return NULL;
}

void free_macros(void)
{
    size_t i;
    size_t j;
    /* Loop through each macro in the macro list */
    for (i = 0; i < macro_list.count; i++) {
        /* Free each line in the macro */
        for (j = 0; j < macro_list.macros[i].line_count; j++) {
            free(macro_list.macros[i].lines[j]);
        }
        /* Free the array of lines */
        free(macro_list.macros[i].lines);
    }
    /* Free the array of macros */
    free(macro_list.macros);
    macro_list.macros = NULL;
    macro_list.capacity = 0;
    macro_list.count = 0;
}
char *my_strdup(char *str) {
    size_t len = strlen(str) + 1;
    char *copy = malloc(len);
    if (copy) {
        memcpy(copy, str, len);
    }
    return copy;
}

