/*
* querier - asks user for input and displays ranked results from index
*
* Uhuru Hashimoto CS50 21X
*/

//standard macros
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
//library macros
#include "../libcs50/bag.h"
#include "../libcs50/set.h"
#include "../libcs50/counters.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/counters.h"
#include "../libcs50/jhash.h"
#include "../libcs50/memory.h"
#include "../libcs50/webpage.h"
#include "../libcs50/file.h"
#include "../common/pagedir.h"
#include "../common/index.h"
#include "../common/word.h"

/**************** FUNCTION DECLARATIONS ***********************/
int query_user(index_t *index, char *dirname);
bool hasSyntaxErrors(char *query);
bool beginsWith(char *line, char *prefix);
bool endsWith(char *line, char *suffix);
bool isAlphabet(char *line);
void search_query(char *line);

/*************** STATIC/LOCAL FUNCTIONS (HELPERS) ***********************/


// driver; checks arguments and initiates loop
int main(const int argc, char **argv) {
    int status = 0;

    //argument number check
    if (argc < 3 || argc > 3) {
        fprintf(stderr, "Error: wrong number of arguments.\n"
        "Usage: ./querier pageDirectory indexFilename\n");
        return ++status;
    }
    //directory check
    if (!isCrawlerDirectory(argv[1])) {
        fprintf(stderr, "Error: please provide a crawler-produced directory.\n");
        return ++status;
    }
    //file permissions check
    if (!isReadableFile(argv[2])) {
        fprintf(stderr, "Error: please provide a readable file\n");
        return ++status;
    }

    //load index
    index_t *index = index_load(argv[2]);
    if (index == NULL) {
        fprintf(stderr, "Error: unable to load index\n");
        return ++status;
    }

    //query user
    status += query_user(index, argv[1]);

    //clean and exit
    free(index);
    return status;
}

int query_user(index_t *index, char *dirname) {
    int ret = 0;
    char *line = NULL;

    // query user
    while ((line = freadlinep(stdin)) != NULL) { // read lines until EOF reached
        if (!hasSyntaxErrors(line) && strlen(line) > 0) {
            fprintf(stdout, "%s\n", line);
            search_query(line);
            free(line);
        }
        else { // normalization error (handled below) or empty query
            free(line);
            return ++ret;
        }
    }
    return ret;
}

// query helper: checks for syntax errors and prints a specific error for each case
bool hasSyntaxErrors(char *query) {
    // Error cases: 
    // normalization fails
    if (!NormalizeWord(query)) {
        fprintf(stderr, "Error: failed to normalize query\n");
        return true;
    }
    // non-alphabetic character encountered
    if (!isAlphabet(query)) {
        fprintf(stderr, "Error: query must consist solely of alphabetic characters\n");
        return true;
    }
    // query contains operator errors
    if (!hasGoodOperators(query)) { //error printing handled by function
        return true;
    }
    return false;
}

bool hasGoodOperators(char *line) {
    //use freadwordp
    //begins with and 
    //begins with or

    //has double and, double or, or combo

    //ends with and
    //ends with or
    return true;
}

// helper: checks character by character to see if query begins with prefix
bool beginsWith(char *line, char *prefix) {
    if (strlen(prefix) > strlen(line)) return false; //length check 
    for (int i = 0; i < strlen(prefix); i++) {
        if (line[i] != prefix[i]) {
            return false;
        }
    }
    return true;
}

// helper: checks character by character to see if query ends with prefix
bool endsWith(char *line, char *suffix) {
    if (strlen(suffix) > strlen(line)) return false; //length check
    for (int i = 0; i < strlen(suffix); i++) {
        int line_index = strlen(line) - i;
        int suff_index = strlen(suffix) - i;
        if (line[line_index] != suffix[suff_index]) {
            return false;
        }
    }
    return true;
}

// helper: checks that query is made of alphabetic characters and spaces
bool isAlphabet(char *line) {
    for (int i = 0; i < strlen(line); i++) {
        if (!isalpha(line[i])) {
            if (line[i] != ' ') {
                return false;
            }
        }
    }
    return true;
}

void search_query(char *line) {
    fprintf(stdout, "Searching query...\n");
}
