/*
* driver for index datatype: contains modules to build an index from crawler output
* Usage: 
* ./indexer pageDirectory indexFilename
*
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
index_t *index_build(char *dirname);
static void index_page(index_t *index, FILE *fp, int doc_id);
static void itemdelete(counters_t *item);


/******************** DRIVER **************************/
// driver
int main (const int argc, char **argv) {
	int status = 0;

	// check arguments
	if (argc < 3 || argc > 3) {
		fprintf(stderr, "Error: wrong number of arguments. Usage:\n
			./indexer pageDirectory indexFilename");
		return ++status;
	}
	
	if (!isCrawlerDirectory(argv[1])) {
		return ++status;
	}

	char *dirname = argv[1];
	char *filename = argv[2];

	//build
	index_t *index = index_build(dirname);
	//save
	index_save(index, filename);

	//delete
	index_delete(index, itemdelete);

	return status;
}


/******************** FUNCTIONS **************************/

// NEW: builds index from crawler output
// assumes directory has been checked
index_t *
	index_build(char *dirname) 
{
	index_t *index = index_new();
	//in directory, scan each file
	int doc_id = 1;
	char *fileid = intToString(doc_id);
	char *filename = filenameCreator(dirname, fileid);
	FILE *fp = NULL;

	while ((fp = fopen(filename, "r")) != NULL) {
		index_page(index, fp, doc_id);

		free(fileid);
		free(filename);

		//reset variables
		doc_id++;
		fileid = intToString(doc_id);
		filename = filenameCreator(dirname, fileid);
	} 

	free(fileid);
	free(filename);

	return index;
}

// helper
// handles one file of name (dirname/doc_id)
static void 
	index_page(index_t *index, FILE *fp, int doc_id) 
{
	// read words from webpage
	// for each word, add one to its counter with key (doc_id)
	char *word = NULL;
	while ((word = freadwordp(fp)) != NULL) {
		counters_t *counter = index_find(index, word);
		//if word is already in index
		if (counter != NULL) {
			counter_add(doc_id);
		}
		//free pointer for reuse in loop
		free(word); 
	}

}

//deletion helper
static void itemdelete(counters_t *item) {
	counter_delete(item);
}
