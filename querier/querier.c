/*
* querier - asks user for input and displays ranked results from index
*
* Uhuru Hashimoto CS50 21X
*/

//standard macros
#include <stdio.h>
#include <stdlib.h>
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

/*************** STATIC/LOCAL FUNCTIONS ***********************/


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

    //query user
    //use readlinep

    return ret;
}
