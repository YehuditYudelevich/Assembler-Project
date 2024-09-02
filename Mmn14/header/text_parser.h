#include "macro.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef TEXT_PARSER_H
#define TEXT_PARSER_H
#define MAX_LENGTH_WORD 32
#define MASK 1
#define SHIFT_FOR_FIRST_WORD 3
#define SHIFT_FOR_SECOND_WORD 6


/**
 * Extracts a word from a string starting from the given index,
 * ignoring leading whitespace.
 *
 * @param buffer The input string.
 * @param index The index to start extraction from.
 * @return A pointer to the extracted word, or NULL if no word is found.
 */
char* word_without_space(const char* buffer, int index);
/**
 * Extracts a word from a string starting from the given index,
 * ignoring leading whitespace and stopping at commas.
 *the difference between this function and the previous one is that this function stops at commas
 * @param buffer The input string.
 * @param index The index to start extraction from.
 * @return A pointer to the extracted word, or NULL if no word is found.
 */
char* word_without_space_to_commas(const char* buffer, int index);
/**
 * Removes extra spaces from a string buffer.
 *
 * This function removes extra spaces from a string buffer by copying
 * characters from the input buffer to the output buffer, skipping
 * consecutive spaces.
 *
 * @param buffer The input buffer to remove spaces from.
 */
void removeExtraSpaces(char *buffer);
#endif
