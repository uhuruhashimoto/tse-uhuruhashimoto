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


/******************** FUNCTION DECLARATIONS **************************/
static void itemdelete(counters_t *item);


/******************** DRIVER **************************/
// driver
int main(const int argc, char **argv) {
	int status = 0;

	// check arguments: just the number
	if (argc < 3 || argc > 3) {
		fprintf(stderr, "Error: wrong number of arguments. Usage:\n
			./indexer pageDirectory indexFilename");
		return ++status;
	}

	char *oldfile = argv[1];
	char *newfile = argv[2];

	index_t *index = index_new();
	index_load(index, oldfile);
	index_save(index, newfile);

	index_delete(index, itemdelete);

	return status;
}

// helper
static void itemdelete(counters_t *item) {
	counter_delete(item);
}
