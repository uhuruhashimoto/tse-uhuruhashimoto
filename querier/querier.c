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
    char **lines = malloc(sizeof(char)*MAX_LINES);
    char *line;
    char **words;
    int i = 0;
    int actual_query_num;

    //read each line
    while ((line = freadlinep(stdin)) != NULL) {
        //perform each query
        if ((words = get_words(lines[i])) != NULL) {
            printQuery(words);
            run_query(words, size(words), index, dirname);
            printSeparator();
            free_words(words); //this frees words and the prev freadlinep command
        } //else query could not be run or is empty; error checking handled in get_words
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
    char **words = NULL;
    //initialize pointers
    char *wordstart;
    char *rest;
    int i = 0;
    int words_index = 0;
    bool startofword = false;
    bool endofword = false;

    while (i <= size(line)) {
        //slide word pointer
        while (!startofword) {
            wordstart = line[i];
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
            rest = line[i];
            if (isalpha(rest[0])) {
                i++;
            }
            else {
                if (isspace(rest[0])) { //if space encountered, we've reached the end of a word
                    endofword = true;
                    //replace with /0
                    rest[0] = '\0';
                    //put in array
                    words[words_index] = wordstart;
                }
                else if (rest[0] == '\0') {
                    //reached end of line (already null-terminated)
                    endofword = true;
                    //put in array
                    words[words_index] = wordstart;
                }
                else {
                    //if non-alphabetic character encountered (ex ca!t), print error and exit
                    fprintf(stderr, "Error: query contains non-alphabetic character %c\n", rest[0]);
                    return NULL;
                }
            }
        }
        //move wordstart and rest to rest+1 position
        i++;
        rest = words[i];
        wordstart = words[i];
        //reset flags
        wordstart = false;
        endofword = false;
    }

    words = checkSeparators(words); //returns null if operator error found; otherwise unchanged array
    return words;
}

//runs a query (assumes all error checking has occurred prior to search)
//memory allocation deals with result only; index remains unchanged
counters_t *run_query(char **words, int nwords, index_t *index, char *dirname) {
    counters_t *result = malloc(sizeof(counters_t*));
    counters_t *ctr1 = index_find(words[0]);
    struct twoctr two = {ctr1, result};
    //loop through words and respond as per operators
    for (int i = 0; i < nwords; i++) {
        if (words[i] == "or") {
            //reset two datatype; result becomes ctr1
            two->first = result;
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
            two->first = result;
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
    counters_iterate(answer, size, count_iterator);

    //create array of structs to store ctr data
    struct ctrdata **sorted = calloc(sizeof(struct ctrdata *)*size);
    //add to diff struct - sort_iterator
    counters_iterate(answer, sorted, sort_iterator);

    //get URLs and print results 
    for (int i = 0; i < size; i++) {
        char *url = getURL(sorted->doc_id, dirname);
        fprintf("Doc:   %d  Score   %d  Url: %s\n", sorted->doc_id, sorted->value, url);
        free(url);
    }
    //clean up
    free(size);
}

//checks for starting "and" and "or" or double operators; 
//returns null if found
//otherwise, returns string array unchanged
static char ** checkOperators(char **words) {
    char *last = NULL;
    //check for starting or ending operators
    if (words[0] == "and") {
        fprintf(stderr, "Error: query may not begin with \"and\"\n");
        return NULL;
    }
    else if (words[0] == "or") {
        fprintf(stderr, "Error: query may not begin with \"or\"\n");
        return NULL;
    }
    else if (words[size(words)] == "and") {
        fprintf(stderr, "Error: query may not end with \"and\"\n");
        return NULL;
    }
    else if (words[size(words)] == "or") {
        fprintf(stderr, "Error: query may not end with \"or\"\n");
        return NULL;
    }
    //check for double operators
    for (int i = 0; i < size(words); i++) {
        if (words[i] == "and" || words[i] == "or") {
            if (last == "and" || last == "or") {
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
static void getURL(int doc_id, dirname) {
    FILE *fp = NULL;
    char *doc = stringToInt(doc_id);
    char *filename = filenameCreator(dirname, doc);

    if ((fp = fopen(filename, "r")) != NULL) {
        char *ret = freadlinep(fp);
    }
    else {
        char *ret = malloc(sizeof(char)*20);
        ret = "No Url Available";
    }
    free(filename);
    free(doc);
    return ret;
}

/**************** COUNTER ITERATORS ***********************/

//intersection
//takes intersection of two counters and puts in result counter
static void and_iterator(void *arg, const int key, const int count1) {
    struct twoctr two  = arg;
    int count2 = 0; 
    if ((count2 = counters_get(two->first, key)) != 0) { //if key is in both counters
        if (count2 > count1) {
            counters_set(two->answer, key, count1); //set answer with minimum value
        }
        else {
            counters_set(two->answer, key, count2);
        }
    }

}

//union
//takes union of two counters and puts in result counter
static void or_iterator(void *arg, const int key, const int count) {
    struct twoctr two = arg;
    int count2 = 0;
    if ((count2 = counters_get(two->first, key)) != 0) { //if it's in both
        counters_set(two->answer, count2+count1); //set answer with the sum of both values
    }
    else {
        counters_set(two->answer, key, value); //else add it (it's in the union)
    }
}

//counts items in counters_t and adds to *arg (*nitem, an int ptr)
static void count_iterator(void *arg, const int key, const int count) {
    int *nitems = arg;
    if (arg != NULL && key != NULL && count != NULL) (*nitems)++;
}

//sorts counters and adds to an array of structs provided in *arg
static void sort_iterator(void *arg, const int key, const int count) {
    //initialize struct (with doc_id and value)
    struct ctrdata **sorted = arg;
    //add (key, count) pair to sorted array 
    //sort by count

    //compare val to each (k,v) pair in sorted array
    //insert when val >= (v)
}




