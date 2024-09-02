#include "text_parser.h"


/*a function that return a word from a given index till the first space*/
char* word_without_space(const char* buffer, int index)
{
    char* word;
    int i = index, j = 0;
    /* Allocate dynamic buffer to hold the extracted word */
    word = (char*)malloc(MAX_LENGTH_WORD * sizeof(char));
    if (word == NULL) {
        return NULL;
    }
    memset(word, '\0', MAX_LENGTH_WORD);
    /* Skip leading whitespace and commas */
    while (buffer[i] != '\0' && (isspace(buffer[i]) || buffer[i] == ',')) {
        i++;
    }
    /* Extract characters of the word */
    while (buffer[i] != '\0' && !isspace(buffer[i]) && buffer[i] != ',') {
        if (j < MAX_LENGTH_WORD - 1) {
            word[j++] = buffer[i++];
        }
    }
    word[j] = '\0'; /* Null-terminate the extracted word */
    /* Return the word if found, otherwise free memory and return NULL */
    if (j > 0) {
        return word;
    } else {
        free(word);
        return NULL;
    }
}

/*a function that return a word from a given index, the diffrent from the privouse function that its include commas*/
char* word_without_space_to_commas(const char* buffer, int index)
{
    /* Allocate dynamic buffer to hold the extracted word */
    char* word;
    int i = index, j = 0;
    word = (char*)malloc(MAX_LENGTH_WORD * sizeof(char));
    if (word == NULL) {
        return NULL;
    }
    memset(word, '\0', MAX_LENGTH_WORD);
    /* Skip leading whitespace characters */
    while (buffer[i] != '\0' && isspace(buffer[i])) {
        i++;
    }
    /* Extract characters of the word */
    while (buffer[i] != '\0' && !isspace(buffer[i]) && j < 80 - 1) {
        word[j++] = buffer[i++];
    }
    word[j] = '\0'; /* Null-terminate the extracted word */

    /* Return the word if found, otherwise NULL */
    return (j > 0) ? word : NULL;
}

/*a function that remove exstra spaces from a given line*/
void removeExtraSpaces(char *buffer) {
    /* Remove extra spaces from the input buffer */
    int i, j;
    /* Iterate over the buffer and copy characters to the output buffer */
    for (i = 0, j = 0; buffer[i] != '\0'; i++) {
        /* Skip leading whitespace characters */
        if (buffer[i] != ' ' || (i > 0 && buffer[i - 1] != ' ')) {
            if (!(buffer[i] == ' ' && (buffer[i + 1] == ',' || buffer[j - 1] == ','))) {
                /* Copy the character to the output buffer */
                if (j < MAX_LINE_LENGTH) 
                    buffer[j++] = buffer[i];
            }
        }
    }
    /* Null-terminate the output buffer */
    buffer[j] = '\0';
}


