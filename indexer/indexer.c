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
#include "../common/index.h"
#include "../common/word.h"
#define NUM_SLOTS 300


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
		fprintf(stderr, "Error: wrong number of arguments. Usage:\n"
			"./indexer pageDirectory indexFilename\n");
		return ++status;
	}
	
	if (!isCrawlerDirectory(argv[1])) {
		fprintf(stderr, "Error: non-crawler directory provided.\n");
		return ++status;
	}

	char *dirname = argv[1];
	char *filename = argv[2];

	//build
	index_t *index = index_build(dirname);
	if (index == NULL) {
		fprintf(stderr, "Error: created null index.\n");
	}
	//save
	if (!index_save(index, filename)) {
		fprintf(stderr, "Error: failed to save index.\n");
		return ++status;
	}

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
	index_t *index = index_new(NUM_SLOTS);
	//in directory, scan each file
	int doc_id = 1;
	char *fileid = intToString(doc_id);
	char *filename = filenameCreator(dirname, fileid);
	FILE *fp = NULL;

	while ((fp = fopen(filename, "r")) != NULL) {
		index_page(index, fp, doc_id);

		free(fileid);
		free(filename);
		fclose(fp);

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
	int pos = 0;
	char *url = freadlinep(fp);
	char *chardep = freadlinep(fp);
	int depth = stringToInt(chardep);
	char *html = freaduntil(fp, NULL);

	webpage_t *webpage = webpage_new(url, depth, html);

	while ((word = webpage_getNextWord(webpage, &pos)) != NULL) {
		if (NormalizeWord(word) && strlen(word) >= 3) { // if word can be normalized and is longer than 3 chars
			counters_t *counter = index_find(index, word);
			// if word is not already in index, make a new counter and add it
			if (counter == NULL) {
				counter = counters_new();
				counters_add(counter, doc_id);
				index_insert(index, word, counter);
			}
			else { // if it is, just add to the counter
				counters_add(counter, doc_id);
			}
		}
		// free pointer for reuse in loop
		free(word); 
  	}

	free(chardep);
	webpage_delete(webpage); //frees url and html strings autmatically

}

//deletion helper
static void itemdelete(counters_t *item) {
	counters_delete(item);
}
