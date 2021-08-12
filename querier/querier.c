/*
* querier - asks user for input and displays ranked results from index
* Usage: ./querier pageDirectory indexFilename
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

typedef struct twoctr {
    counters_t *first;
    counters_t *result;
} twoctr_t;

typedef struct ctrdata {
    int doc_id;
    int value;
} ctrdata_t;

/**************** FUNCTION DECLARATIONS ***********************/
void query_user(index_t *index, char *dirname);
char **get_words(char *line);
void run_query(char **words, int nwords, index_t *index, char *dirname);
void display_result(counters_t *answer, char *dirname);

/*************** STATIC/LOCAL FUNCTIONS (HELPERS) ***********************/
static void free_words(char **words);
static void printQuery(char **words, int nwords);
static void printSeparator();
static char ** checkOperators(char **words, int nwords);
static char* getURL(int doc_id, char *dirname);
//iterators
static void and_iterator(void *arg, const int key, const int count1);
static void or_iterator(void *arg, const int key, const int count);
static void count_iterator(void *arg, const int key, const int count);
static void sort_iterator(void *arg, const int key, const int count);
//unit test
#ifdef UNITTEST
static int unittest();
#endif

// driver; checks arguments and initiates loop
int main(const int argc, char **argv) {
    int status = 0;
#ifndef UNITTEST
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
#endif
#ifdef UNITTEST
    status += unittest();
    return status;
#endif
}

//queries user
void query_user(index_t *index, char *dirname) {
    char *line;
    char **words;
    //read each line
    while ((line = freadlinep(stdin)) != NULL) {
        //perform each query
        if ((words = get_words(line)) != NULL) {
            run_query(words, sizeof(words), index, dirname);
            printSeparator();
            free_words(words); //this frees words and the prev freadlinep command
        } //else query could not be run or is empty; error checking handled in get_words
    }
}

static void free_words(char **words) {
    for (int i = 0; i < sizeof(words); i++) {
        if (words[i] != NULL) free (words[i]);
    }
    free(words);
}

static void printQuery(char **words, int nwords) {
    fprintf(stdout, "Query: ");
    for (int i = 0; i < nwords+1; i++) {
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
// uses provided allocation; allocates buffer to hold them (not words themselves)
// isalpha error checking and whitespace normalization included
char **get_words(char *line) {
    const int max_words_in_line = 81;
    //initialize buffer
    char **words = calloc(max_words_in_line, sizeof(char));
    //initialize pointers
    char *wordstart;
    char *rest;
    int i = 0;
    int words_index = 0;
    bool startofword = false;
    bool endofword = false;
    bool endofline = false;

    while (!endofline) { //stop only after null terminator
        //slide word pointer
        while (!startofword) {
            wordstart = &line[i];
            if (isalpha(wordstart[0])) { 
                startofword = true;
            }
            else {
                if (isspace(wordstart[0])) {
                    //ignore whitespace
                    i++;
                }
                else if (wordstart[0] == '\0') {
                    //empty string: return empty array
                    return NULL;
                }
                else {
                    //if non-alphabetic character encountered (ex !cat), print error and exit
                    fprintf(stderr, "Error: query contains non-alphabetic character %c\n", wordstart[0]);
                    return NULL;
                }
            }
        }
        //slide rest pointer
        while (!endofword) {
            rest = &line[i];
            if (isalpha(rest[0])) {
                i++;
            }
            else {
                if (isspace(rest[0])) { //if space encountered, we've reached the end of a word (or line)
                    endofword = true;
                    if (rest[0] == '\n') { //reached end of line (in case lines contain \n)
                        //replace with /0
                        rest[0] = '\0';
                        //put in array
                        words[words_index] = wordstart;
                        endofline = true; //exit loop
                    }
                    else {
                        //replace with /0
                        rest[0] = '\0';
                        //put in array
                        words[words_index] = wordstart;
                        words_index++;
                    }
                }
                else if (rest[0] == '\0') { //end of line
                    //put in array
                    words[words_index] = wordstart;
                    endofword = true;
                    endofline = true; //exit loop
                }
                else {
                    //if non-alphabetic character encountered (ex ca!t), print error and exit
                    fprintf(stderr, "Error: query contains non-alphabetic character %c\n", rest[0]);
                    return NULL;
                }
            }
        }
        //move wordstart and rest to rest+1 position in preparation for loop
        if (!endofline) {
            i++;
            rest = &line[i];
            wordstart = &line[i];
            //reset flags
            startofword = false;
            endofword = false;
        }
    }

    //TODO: normalize query 
    words = checkOperators(words, words_index); //returns null if operator error found; otherwise unchanged array
    printQuery(words, words_index);
    return words;
}

//runs a query (assumes all error checking has occurred prior to search)
//memory allocation deals with result only; index remains unchanged
void run_query(char **words, int nwords, index_t *index, char *dirname) {
    counters_t *result = malloc(sizeof(counters_t*));
    counters_t *ctr1 = index_find(index, words[0]);
    struct twoctr *two = malloc(sizeof(struct twoctr *));
    two->first = ctr1;
    two->result = result;
    //loop through words and respond as per operators
    for (int i = 0; i < nwords; i++) {
        if ((strcmp(words[i], "or")) == 0) {
            //reset two datatype; result becomes ctr1
            two->first = result;
            counters_delete(two->result);
            two->result = malloc(sizeof(counters_t*));
            //perform union
            counters_iterate(index_find(index, words[i]), two, or_iterator);
        }
        else if ((strcmp(words[i], "and")) == 0) {
            //ignore if word is "and"
        }
        else {
            //reset two datatype; result becomes ctr1
            two->first = result;
            counters_delete(two->result);
            two->result = malloc(sizeof(counters_t*));
            //perform intersection
            counters_iterate(index_find(index, words[i]), two, and_iterator);
        }
    }
    display_result(two->result, dirname);
    counters_delete(two->result);
}


void display_result(counters_t *answer, char *dirname) {
    //count size of answer - count_iterator
    int *size = malloc(sizeof(int));
    counters_iterate(answer, size, count_iterator);

    //create array of structs to store ctr data
    struct ctrdata **sorted = calloc(*size, sizeof(struct ctrdata *));
    //add to diff struct - sort_iterator
    counters_iterate(answer, sorted, sort_iterator);

    //get URLs and print results 
    for (int i = 0; i < *size; i++) {
        char *url = getURL(sorted[i]->doc_id, dirname);
        fprintf(stdout, "Doc:   %d  Score   %d  Url: %s\n", sorted[i]->doc_id, sorted[i]->value, url);
        free(url);
    }
    //clean up
    free(size);
}

//checks for starting "and" and "or" or double operators; 
//returns null if found
//otherwise, returns string array unchanged
static char ** checkOperators(char **words, int nwords) {
    char *last = NULL;
    //check for starting or ending operators
    if ((strcmp(words[0], "and")) == 0) {
        fprintf(stderr, "Error: query may not begin with \"and\"\n");
        return NULL;
    }
    else if ((strcmp(words[0], "or")) == 0) {
        fprintf(stderr, "Error: query may not begin with \"or\"\n");
        return NULL;
    }
    else if ((strcmp(words[nwords], "and")) == 0) {
        fprintf(stderr, "Error: query may not end with \"and\"\n");
        return NULL;
    }
    else if ((strcmp(words[nwords], "or")) == 0) {
        fprintf(stderr, "Error: query may not end with \"or\"\n");
        return NULL;
    }
    //check for double operators
    for (int i = 0; i < nwords; i++) {
        if ((strcmp(words[i], "and") == 0) || (strcmp(words[i], "or") == 0)) {
            if ((strcmp(last, "and") == 0) || (strcmp(last, "or") == 0)) {
                fprintf(stderr, "Error: query may not contain double operators\n");
                return NULL;
            }
        }
        last = words[i];
    }
    return words;
}

// allocates string to give url (first line of dirname/doc_id)
// returns allocated string (left to user to free)
static char* getURL(int doc_id, char *dirname) {
    FILE *fp = NULL;
    char *doc = intToString(doc_id);
    char *filename = filenameCreator(dirname, doc);
    char *ret = NULL;
    const char *message = "No Url Available"; //failure message for url

    if ((fp = fopen(filename, "r")) != NULL) {
        ret = freadlinep(fp);
    }
    else {
        ret = malloc(sizeof(char)*sizeof(message)); //allocate string to be freed later
        strcpy(ret, message);
    }
    free(filename);
    free(doc);
    return ret;
}

/**************** COUNTER ITERATORS ***********************/

//intersection
//takes intersection of two counters and puts in result counter
static void and_iterator(void *arg, const int key, const int count1) {
    struct twoctr *two  = arg;
    int count2 = 0; 
    if ((count2 = counters_get(two->first, key)) != 0) { //if key is in both counters
        if (count2 > count1) {
            counters_set(two->result, key, count1); //set answer with minimum value
        }
        else {
            counters_set(two->result, key, count2);
        }
    }

}

//union
//takes union of two counters and puts in result counter
static void or_iterator(void *arg, const int key, const int count) {
    struct twoctr *two = arg;
    int count2 = 0;
    if ((count2 = counters_get(two->first, key)) != 0) { //if it's in both
        counters_set(two->result, key, count2 + count); //set answer with the sum of both values
    }
    else {
        counters_set(two->result, key, count); //else add it (it's in the union)
    }
}

//counts items in counters_t and adds to *arg (*nitem, an int ptr)
static void count_iterator(void *arg, const int key, const int count) {
    int *nitems = arg;
    if (arg != NULL) (*nitems)++;
}

//sorts counters and adds to an array of structs provided in *arg
static void sort_iterator(void *arg, const int key, const int count) {
    //initialize struct (with doc_id and value)
    //TODO: struct ctrdata **sorted = arg;

    //add (key, count) pair to sorted array 
    //sort by count

    //compare val to each (k,v) pair in sorted array
    //insert when val >= (v)
}

#ifdef UNITTEST
static int unittest() {
    int ret = 0;
    //-------------TEST 1: GET_WORDS----------------------//
    fprintf(stdout, "Testing get_words...\n");
    //create test string (can also use sprintf)
    char *line = freadlinep(stdin); 
    fprintf(stdout, "Testing with %s\n", line);
    //break line into words
    char **words = get_words(line);
    if (words == NULL) { 
        ret++; //syntax errors
    }
    
    //free pointers
    free(line);
    free(words);

    //-------------TEST 1: SORT----------------------//

    return ret;
}
#endif




