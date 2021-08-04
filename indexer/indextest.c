/*
* indextest - tests load and save functionality of index
* Usage: 
* ./indextest oldIndex newIndex
*
* Note: no error checking of input args other than number
* Uhuru Hashimoto CS50 21X
*/

#include <stdio.h>
#include <stdlib.h>
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
#define NUM_SLOTS 300


/******************** FUNCTION DECLARATIONS **************************/
static void itemdelete(counters_t *item);


/******************** DRIVER **************************/
// driver
int main(const int argc, char **argv) {
	int status = 0;

	// check arguments: just the number
	if (argc < 3 || argc > 3) {
		fprintf(stderr, "Error: wrong number of arguments. Usage:\n"
			"./indexer pageDirectory indexFilename\n");
		return ++status;
	}

	char *oldfile = argv[1];
	char *newfile = argv[2];

	index_t *index = index_load(oldfile);
	if (index == NULL) {
		fprintf(stderr, "Error: created null index.\n");
	}

	//save
	if (!index_save(index, newfile)) {
		fprintf(stderr, "Error: failed to save index.\n");
		return ++status;
	}

	index_delete(index, itemdelete);

	return status;
}

// helper
static void itemdelete(counters_t *item) {
	counters_delete(item);
}
