/*
* index.c - an implementation of hashtable specific to the indexer
* hashtable maps a key to an item, where (key) is a word, and (item) is a counter of (doc id, num) objects
* the total data structure is an index, used by the indexer and querier
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

//rename hashtable_t to index_t
typedef hashtable_t index_t; 

/******************** FUNCTIONS **************************/

/******************** WRAPPER MODULES **************************/

//creator
index_t *index_new(const int num_slots) {
	return hashtable_new(num_slots);
}

//insertion
bool index_insert(index_t *index, const char *key, counters_t *item) {
	return hashtable_insert(index, key, item);
}

//find (returns a counter)
counters_t *index_find(index_t *index, const char *key) {
	return hashtable_find(index, key);
}

//delete (deletes counters)
void index_delete(index_t *index, void (*itemdelete)(counters_t *item) ) {
	hashtable_delete(index, itemdelete);
}


/******************** NEW MODULES **************************/
// (called by indexer to create an index)

void hashtable_iterate(hashtable_t *ht, void *arg,
                       void (*itemfunc)(void *arg, const char *key, void *item) )

//--------------------- SAVE -------------------------------//
// save index to external file 
bool index_save(index_t *index, const char *filename) {
	//check permissions
	FILE *fp = NULL;
    if ((fp = fopen(filename, "w")) != NULL) {
        return false;
    } 
    
    //iterate through index and write to file
    //for each set in ht, print the word and iterate through the set to print numbers
    hashtable_iterate(index, fp, setprint);

    fclose(fp);
    return true;
}

// HELPERS
// on each line, write a word, pass counters a print function, and write \n
static void setprint(FILE *fp, const char *name, counters_t *counter) {
	fprintf(fp, "%s ", name); //print word
	counters_iterate(counter, fp, counterprint); //print counters
	fprintf(fp, "\n"); //print newline char
	
}

// print doc id - num pairs
static void counterprint(FILE *fp, const int key, const int item) {
	fprintf(fp, "%d %d ", key, item);
}


//--------------------- LOAD -------------------------------//
// load from external index-created file
// file format is [word] [doc_id num] [...]
index_t *index_load() {
	//get number of lines in file using file apis
}

