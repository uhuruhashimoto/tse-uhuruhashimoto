/*
* word.c - module containing normalization functions
*
* Uhuru Hashimoto CS50 21X */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

bool NormalizeWord(char *word) {
    if (word == NULL) {
        return false;
    }

    for (int i = 0; i < strlen(word); i++) {
        word[i] = tolower(word[i]);
    }

    return true;
}
