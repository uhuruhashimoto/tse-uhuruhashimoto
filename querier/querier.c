/*/* querier - asks user for input and displays ranked results from index
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
#define MAX_LINES 100;
#define MAX_WORDS 50;

typedef struct twoctr {
    counters_t *ctr1;
    counters_t *result;
} twoctr_t;

/**************** FUNCTION DECLARATIONS ***********************/
void query_user(index_t *index, char *dirname);

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
    query_user(index, argv[1]);

    //clean and exit
    index_delete(index, counters_delete);
    return status;
}

//queries user
void query_user(index_t *index, char *dirname) {
    char **lines = malloc(sizeof(char)*MAX_QUERIES);
    char *line;
    char **words;
    int i = 0;
    int actual_query_num;

    //read each line
    while ((line = freadlinep(stdin)) != NULL) {
        lines[i] = line;
        i++;
    }
    //at end of loop, actual line number has been found
    actual_query_num = i;

    //perform each query
    for (int j = 0; i < actual_query_num; j++) {  // read until end of inputs
        if ((words = get_words(lines[i])) != NULL) {
            printQuery(words);
            run_query(words, size(words), index, dirname);
            printSeparator();
            free_words(words); //this frees words and the prev freadlinep command
        } //else query could not be run; error checking handled in get_words
    }
}

static void free_words(char **words) {
    for (int i = 0; i < size(words); i++) {
        if (words[i] != NULL) free (words[i]);
    }
    free(words);
}

static void printQuery(char **words) {
    fprintf(stdout, "Query: ");
    for (int i = 0; i < size(words); i++) {
        fprintf(stdout, "%s ", words[i]);
    }
    fprintf(stdout, "\n");
}

static void printSeparator() {
    for (int i = 0; i<38; i++) {
        fprintf(stdout, "-");
    }
}

// turns each line into an array of words
// uses provided allocation; no extra necessary
// isalpha error checking and whitespace normalization included
char **get_words(char *line) {
    //initialize buffer
    char **words;
    char *wordstart;
    char *rest;
    int i = 0;

    while (i <= size(line)) {
        //slide word pointer

        //slide rest pointer
        //check if isalpha, isspace, and is \0 (end of line)

        //move word to rest+1 position
    }


}

//runs a query (assumes all error checking has occurred prior to search)
//memory allocation deals with result only; index remains unchanged
counters_t *run_query(char **words, int nwords, index_t *index, char *dirname) {
    counters_t *result = malloc(sizeof(counters_t*));
    counters_t *first = index_find(words[0]);
    struct twoctr two = {first, result};

    for (int i = 0; i < nwords; i++) {
        if (words[i] == "or") {
            //reset two datatype; result becomes ctr1
            two->ctr1 = result;
            counters_delete(two->result);
            two->result = malloc(sizeof(counters_t*));
            //perform union
            counters_iterate(index_get(words[i]), two, or_iterator);
        }
        else if (words[i] == "and") {
            //ignore if word is "and"
        }
        else {
            //reset two datatype; result becomes ctr1
            two->ctr1 = result;
            counters_delete(two->result);
            two->result = malloc(sizeof(counters_t*));
            //perform intersection
            counters_iterate(index_get(words[i]), two, and_iterator);
        }
    }

    display_result(two->result, dirname);
    counters_delete(two->result);
}


void display_result(counters_t *answer, char *dirname) {
    //count size of answer - count_iterator
    int *size = malloc(sizeof(int));
    counters_iterate(answer, count, count_iterator);

    //create array of struct counters_t
    //add to diff struct - sort_iterator


    //get URLs
    //print results 
}

/**************** COUNTER ITERATORS ***********************/

//intersection
//takes intersection of two counters and puts in result counter
static void and_iterator(void *arg, const int key, const int count) {

}

//union
//takes union of two counters and puts in result counter
static void or_iterator(void *arg, const int key, const int count) {

}

//counts items in counters_t and adds to *arg (*nitem, an int ptr)
static void count_iterator(void *arg, const int key, const int count) {
    int *nitems = arg;
    if (arg != NULL && key != NULL && count != NULL) (*nitems)++;
}

//sorts counters and adds to an array of structs provided in *arg
static void sort_iterator(void *arg, const int key, const int count) {

}




