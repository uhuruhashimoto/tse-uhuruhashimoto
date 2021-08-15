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

typedef struct arraywithindex {
    struct ctrdata **array;
    int *index;
} arraywithindex_t;

/**************** FUNCTION DECLARATIONS ***********************/
void query_user(index_t *index, char *dirname);
char **get_words(char *line, int *nwords);
void run_query(char **words, int nwords, index_t *index, char *dirname);
void display_result(counters_t *answer, char *dirname);

/*************** STATIC/LOCAL FUNCTIONS (HELPERS) ***********************/
static void printQuery(char **words, int nwords);
static void printSeparator();
static bool hasCorrectOperators(char **words, int nwords);
static char* getURL(int doc_id, char *dirname);
static void nullifyWords(char **words, int progress);
//iterators
static void intersection_iterator(void *arg, const int key, const int count1);
static void union_iterator(void *arg, const int key, const int count);
static void count_iterator(void *arg, const int key, const int count);
static void sort_iterator(void *arg, const int key, const int count);
static void copy_iterator(void *arg, const int key, const int count);
static void copy_struct_iterator(void *arg, const int key, const int count);
//unit tests
#ifdef UNITTEST
static void test_input();
static void test_union();
static void test_intersection();
static void test_sort();
#endif

/**************** DRIVER ***********************/
/*******************************************************************/

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
    test_input();
    test_intersection();
    test_union();
    test_sort();
    return status;
#endif
}

/**************** INPUT HANDLING ***********************/
/*******************************************************************/

//queries user in a loop using freadlinep
void query_user(index_t *index, char *dirname) {
    char *line;
    char **words;
    int *nwords = malloc(sizeof(int));
    //read each line
    while ((line = freadlinep(stdin)) != NULL) {
        //perform each query
        if ((words = get_words(line, nwords)) != NULL) {
            printQuery(words, *nwords);
            run_query(words, *nwords, index, dirname);
            printSeparator();
            free(line); //free the prev freadlinep command
            free(words); //free the array that stores it
        } //else query could not be run or is empty; error checking handled in get_words
    }
    free(nwords);
}

//prints query
static void printQuery(char **words, int nwords) {
    if (words == NULL) return;
    fprintf(stdout, "Query: ");
    for (int i = 0; i < nwords; i++) {
        fprintf(stdout, "%s ", words[i]);
    }
    fprintf(stdout, "\n");
}

//prints separator
static void printSeparator() {
    for (int i = 0; i<38; i++) {
        fprintf(stdout, "-");
    }
    fprintf(stdout, "\n");
}

// turns each line into an array of words
// uses provided allocation; allocates buffer to hold them (not words themselves)
// isalpha error checking and whitespace normalization included
char **get_words(char *line, int *nwords) {
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
                    nullifyWords(words, words_index);
                    free(words);
                    return NULL;
                }
                else {
                    //if non-alphabetic character encountered (ex !cat), print error and exit
                    fprintf(stderr, "Error: query contains non-alphabetic character %c\n", wordstart[0]);
                    nullifyWords(words, words_index);
                    free(words);
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
                    words_index++;
                    endofword = true;
                    endofline = true; //exit loop
                }
                else {
                    //if non-alphabetic character encountered (ex ca!t), print error and exit
                    fprintf(stderr, "Error: query contains non-alphabetic character %c\n", rest[0]);
                    nullifyWords(words, words_index);
                    free(words);
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

    if (hasCorrectOperators(words, words_index)) {
        *nwords = words_index;
        return words;
    } 
    else {
        return NULL;
    }
}

//checks for starting "and" and "or" or double operators; 
//returns null if found
//otherwise, returns string array unchanged
static bool hasCorrectOperators(char **words, int nwords) {
    char *last = NULL;
    //check for starting or ending operators
    if ((strcmp(words[0], "and")) == 0) {
        fprintf(stderr, "Error: query may not begin with \"and\"\n");
        free(words);
        return false;
    }
    else if ((strcmp(words[0], "or")) == 0) {
        fprintf(stderr, "Error: query may not begin with \"or\"\n");
        free(words);
        return false;
    }
    else if ((strcmp(words[nwords-1], "and")) == 0) {
        fprintf(stderr, "Error: query may not end with \"and\"\n");
        free(words);
        return false;
    }
    else if ((strcmp(words[nwords-1], "or")) == 0) {
        fprintf(stderr, "Error: query may not end with \"or\"\n");
        free(words);
        return false;
    }
    //check for double operators
    for (int i = 0; i < nwords; i++) {
        if (!NormalizeWord(words[i])) {
            fprintf(stderr, "Error: unable to normalize %s\n", words[i]);
            free(words);
            return false;
            }
        if ((strcmp(words[i], "and") == 0) || (strcmp(words[i], "or") == 0)) {
            if ((strcmp(last, "and") == 0) || (strcmp(last, "or") == 0)) {
                fprintf(stderr, "Error: query may not contain double operators\n");
                free(words);
                return false;
            }
        }
        last = words[i];
    }
    return true;
}

//clears words so no strings are freed twice 
//used when a non-alphabetic char is encountered halfway through the string parsing process
//(simply sets each word in words to null, so that the memory can be freed by a call to free 
//the original line)
static void nullifyWords(char **words, int progress) {
    for (int i = 0; i < progress; i++) {
        if (words[i] != NULL) words[i] = NULL;
    }
}

/**************** RUN QUERY ***********************/
/*******************************************************************/

//runs a query (assumes all error checking has occurred prior to search)
//memory allocation deals with result only; index remains unchanged
void run_query(char **words, int nwords, index_t *index, char *dirname) {
    //allocate holders for prod and result
    struct twoctr *prodholder = malloc(sizeof(struct twoctr *));
    struct twoctr *resultholder = malloc(sizeof(struct twoctr *));
    bool prod_is_empty = true;

    //fill holders with dummy counters
    counters_t *sumfirst = counters_new();
    counters_t *sum = counters_new();
    resultholder->first = sumfirst;
    resultholder->result = sum;

    //for each word
    for (int i = 0; i<nwords; i++) {
        //OR
        if (strcmp(words[i], "or") == 0) { 
            //put res into first
            counters_delete(resultholder->first);
            resultholder->first = resultholder->result;
            //allocate new result
            counters_t *restemp = counters_new();
            resultholder->result = restemp;
            //perform union
            counters_iterate(prodholder->result, resultholder, union_iterator);
            //delete prod
            counters_delete(prodholder->first);
            counters_delete(prodholder->result);
            prod_is_empty = true;

        }
        //AND
        else if (strcmp(words[i], "and") == 0) {} //just ignore it
        //ALL OTHER WORDS
        else { 
            if (prod_is_empty) {
                counters_t *pfirst = counters_new();
                counters_t *pres = counters_new();
                prodholder->first = pfirst;
                prodholder->result = pres;
                //perform intersection
                counters_iterate(index_find(index, words[i]), prodholder, copy_iterator);
            }
            else {
                //put res into first
                counters_delete(prodholder->first);
                prodholder->first = prodholder->result;
                //allocate new result
                counters_t *prodtemp = counters_new();
                prodholder->result = prodtemp;
                //perform intersection
                counters_iterate(index_find(index, words[i]), prodholder, intersection_iterator);

            }
        }
    }

    //After loop, one final union of prod and result
    counters_delete(resultholder->first);
    resultholder->first = resultholder->result;
    //allocate new result
    counters_t *restemp = counters_new();
    resultholder->result = restemp;
    //perform union
    counters_iterate(prodholder->result, resultholder, union_iterator);

    //TODO: for now: 
    fprintf(stdout, "RESULT OF SEARCH: ");
    counters_print(resultholder->result, stdout);
    fprintf(stdout, "\n");
    fprintf(stdout, "Attempting to display results...\n");

    //display results
    display_result(resultholder->result, dirname);

    counters_delete(prodholder->result);
    counters_delete(resultholder->result);
    free(prodholder); free(resultholder);
}


void display_result(counters_t *answer, char *dirname) {
    //count size of answer - count_iterator
    int *size = malloc(sizeof(int));
    *size = 0; //initialize to 0
    counters_iterate(answer, size, count_iterator);

    //create array of structs to store ctr data
    struct arraywithindex *sorted_and_index = malloc(sizeof(struct arraywithindex *));
    struct ctrdata **sorted = calloc(*size, sizeof(struct ctrdata *));
    int *ind = malloc(sizeof(int));
    *ind = 0;
    sorted_and_index -> array = sorted;
    sorted_and_index -> index = ind;


    //TODO: RANDO CHECK
    struct ctrdata **test = calloc(3, sizeof(struct ctrdata *));
    //seg faults here - not sure why; why doesn't an array work same as ptr?
    test[0] -> doc_id = 3;
    test[0] ->value = 1;


    //copy data into array
    counters_iterate(answer, sorted_and_index, copy_struct_iterator);
    //sort data
    *ind = 0;
    counters_iterate(answer, sorted_and_index, sort_iterator);

    //get URLs and print results 
    for (int i = 0; i < *size; i++) {
        char *url = getURL(sorted_and_index->array[i]->doc_id, dirname);
        fprintf(stdout, "Doc:   %d  Score   %d  Url: %s\n", sorted_and_index->array[i]->doc_id,
            sorted_and_index->array[i]->value, url);
        free(url);
    }
    //clean up
    free(sorted);
    free(ind);
    free(sorted_and_index);
    free(size);
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
/*******************************************************************/

//intersection
//takes intersection of two counters and puts in result counter
static void intersection_iterator(void *arg, const int key, const int count1) {
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
static void union_iterator(void *arg, const int key, const int count) {
    struct twoctr *two = arg;
    int count2 = 0;
    if ((count2 = counters_get(two->first, key)) != 0) { //if key is in both counters
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
    int j = 0;
    struct arraywithindex *array_with_index = arg;
    int i = *array_with_index -> index;
    struct ctrdata **sorted_array = array_with_index -> array;
    j = i - 1;

    while (j >= 0 && sorted_array[j] -> value > count) {
        sorted_array[j+1] -> doc_id = sorted_array[j] -> doc_id;
        sorted_array[j+1] -> value = sorted_array[j] -> value;
        j--;
    }

    *array_with_index -> index = i + 1;
}

//copies counter into result counter of two struct
//overwrites existing counter
static void copy_iterator(void *arg, const int key, const int count) {
    struct twoctr *two = arg;
    counters_set(two->result, key, count);
}

//copies counter array of ctrdata nodes
static void copy_struct_iterator(void *arg, const int key, const int count) {
    struct arraywithindex *array_with_index = arg;
    struct ctrdata **sorted_array = array_with_index -> array;
    int i = *array_with_index -> index;
    //copy data
    sorted_array[i] -> doc_id = key;
    sorted_array[i] -> value = count;
}

/**************** UNIT TESTS ***********************/
/*******************************************************************/

#ifdef UNITTEST
static void test_input() {
    //-------------TEST 1: GET_WORDS----------------------//
    //create also read test string from stdin by uncommenting line below
    //char *line = freadlinep(stdin);
    //char **words = get_words(line);
    //free(words);
    fprintf(stdout, "Testing get_words...\n");
    char *line1 = calloc(30, sizeof(char));
    char *line2 = calloc(30, sizeof(char));
    char *line3 = calloc(30, sizeof(char));
    char *line4 = calloc(30, sizeof(char));
    int *num1 = malloc(sizeof(int));
    int *num2 = malloc(sizeof(int));
    int *num3 = malloc(sizeof(int));
    int *num4 = malloc(sizeof(int));
    sprintf(line1, "This is a normal string");
    sprintf(line2, "THIS STRING IS CAPITALIZED");
    sprintf(line3, "This str!ing h@s err0rs");
    sprintf(line4, "This string has and or operator errors");
    //break line into words
    fprintf(stdout, "Testing with \"%s\"\n", line1);
    char **words1 = get_words(line1, num1);
    printQuery(words1, *num1);
    fprintf(stdout, "Testing with \"%s\"\n", line2);
    char **words2 = get_words(line2, num2);
    printQuery(words2, *num2);
    fprintf(stdout, "Testing with \"%s\"\n", line3);
    char **words3 = get_words(line3, num3);
    printQuery(words3, *num3);
    fprintf(stdout, "Testing with \"%s\"\n", line4);
    char **words4 = get_words(line4, num4);
    printQuery(words4, *num4);
    //free pointers
    free(num1); free(num2); free(num3); free(num4);
    free(line1); free(line2); free(line3); free(line4);
    free(words1); free(words2); free(words4);
}

static void test_intersection() {
    //-------------TEST 2: INTERSECTION----------------------//
    //test intersection
    fprintf(stdout, "Testing intersection...\n");
    counters_t *test1 = counters_new();
    counters_t *test2 = counters_new();
    counters_t *answer = counters_new();
    counters_set(test1, 1, 1);
    counters_set(test1, 2, 2);
    counters_set(test1, 3, 5);
    counters_set(test2, 3, 1);
    counters_set(test2, 4, 2);
    counters_set(test2, 5, 5);
    fprintf(stdout, "TEST COUNTERS: \n");
    counters_print(test1, stdout); 
    fprintf(stdout, "\n");
    counters_print(test2, stdout);
    fprintf(stdout, "\n");
    struct twoctr *two = malloc(sizeof(struct twoctr *));
    two->first = test1;
    two->result = answer;
    counters_iterate(test2, two, intersection_iterator); //intersection
    fprintf(stdout, "RESULT: \n");
    counters_print(answer, stdout);
    fprintf(stdout, "\n");
    counters_delete(test1); counters_delete(test2); counters_delete(answer);
    free(two);
}

static void test_union() {
    //-------------TEST 3: UNION----------------------//
    //test union
    fprintf(stdout, "Testing union...\n");
    counters_t *test1 =counters_new();
    counters_t *test2 = counters_new();
    counters_t *answer = counters_new();
    counters_set(test1, 1, 1);
    counters_set(test1, 2, 2);
    counters_set(test1, 3, 5);
    counters_set(test2, 3, 1);
    counters_set(test2, 4, 2);
    counters_set(test2, 5, 5);
    fprintf(stdout, "TEST COUNTERS: \n");
    counters_print(test1, stdout); 
    fprintf(stdout, "\n");
    counters_print(test2, stdout);
    fprintf(stdout, "\n");
    struct twoctr *two = malloc(sizeof(struct twoctr *));
    two->first = test1;
    two->result = answer;
    counters_iterate(test2, two, union_iterator); //union
    two->first = test2;
    counters_iterate(test1, two, union_iterator);
    fprintf(stdout, "RESULT: \n");
    counters_print(answer, stdout);
    fprintf(stdout, "\n");
    counters_delete(test1); counters_delete(test2); counters_delete(answer);
    free(two);
}

static void test_sort() {
    //-------------TEST 4: SORT----------------------//
    counters_t *test =counters_new();
    counters_set(test, 1, 1);
    counters_set(test, 2, 2);
    counters_set(test, 3, 5);
    fprintf(stdout, "TEST COUNTER: \n");
    counters_print(test, stdout); 
    fprintf(stdout, "\n");
    //struct ctrdata **sorted = malloc(sizeof())
    fprintf(stdout, "SORTED RESULT: \n");
    //display result
    counters_delete(test);
}
#endif
