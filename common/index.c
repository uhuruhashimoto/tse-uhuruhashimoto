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

//save index to external file
bool index_save(index_t *index, const char *filename) {

}

//load from external index-created file
index_t *index_load() {

}
