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
bool hasNoDoubleOperators(char *line);
bool isAlphabet(char *line);
void run_query(char **words, int nwords, index_t *index, char *pageDirectory);

/*************** STATIC/LOCAL FUNCTIONS (HELPERS) ***********************/
static void printSeparator();
static char **get_words(char *line);
static void free_words(char **words);

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
    index_delete(index, counters_delete);
    return status;
}

int query_user(index_t *index, char *dirname) {
    int ret = 0;
    char *line = NULL;

    // query user
    while ((line = freadlinep(stdin)) != NULL) { // read lines until EOF reached
        if (!hasSyntaxErrors(line) && strlen(line) > 0) {
            fprintf(stdout, "Query: %s\n", line);
            char **words = get_words(line);
            if (size(words) > 0) {
                run_query(words, size(words), index, dirname);
            }
            printSeparator();
            free_words(words);
            free(line);
        }
        else { // normalization error (handled below) or empty query
            free(line);
            return ++ret;
        }
    }
    return ret;
}

static char **get_words(char *line) {

}

static void free_words(char **words) {
    for (int i = 0; i < size(words); i++) {
        if (words[i] != NULL) free (words[i]);
    }
    free(words);
}

static void printSeparator() {
    for (int i = 0; i<38; i++) {
        fprintf(stdout, "-");
    }
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
    // query begins with "and" or "or" (already normalized)
     if (beginsWith(query, "and") || beginsWith(query, " and")) {
         fprintf(stderr, "Error: query may not begin with an \"and\" operator\n");
         return true;
     }
     if (beginsWith(query, "or") || beginsWith(query, " or")) {
         fprintf(stderr, "Error: query may not begin with an \"or\" operator\n");
         return true;
     }
     // query ends with "and" or "or"
     if (endsWith(query, "and") || endsWith(query, "and ")) {
         fprintf(stderr, "Error: query may not end with an \"and\" operator\n");
         return true;
     }
     if (endsWith(query, "or") || endsWith(query, "or ")) {
         fprintf(stderr, "Error: query may not end with an \"or\" operator\n");
         return true;
     }
    // query contains operator errors
    if (!hasNoDoubleOperators(query)) { //error printing handled by function
        return true;
    }
    return false;
}

bool hasNoDoubleOperators(char *line) {
    //TODO: error check without freadwordp
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

//runs a query (assumes all error checking has occurred prior to search)
void run_query(char **words, int nwords, index_t *index, char *pageDirectory) {
    fprintf(stdout, "Searching query...\n");
}
