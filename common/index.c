/*
* index - a file that provides modules to build an index, which maps words to repetitions per document 
* (by use of a hashtable of words to counters). 
* 
* Basically a hashtable that maps to counters instead of sets 
*
* It maps (k, item): word -> counters (doc id -> repetitions in file)
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
#define TABLE_SIZE 500

typedef struct index {
    counters_t **table;
    int size;
} index_t;

/************************ HT WRAPPER FUNCTIONS *******************************/
// ht wrapper: new index
// creates new index object: a hashtable with counters as objects
index_t *index_new(const int num_slots) {
    index_t *index = count_malloc(sizeof(index_t)); 
	if (index != NULL) {
		index->table = count_calloc(num_slots, sizeof(counters_t *)); //allocates slots next to each other
		if (index->table == NULL) {
			return NULL; //no memory 
		} else {
			index->size = num_slots; //just for easy access
		}
		return index;
	} else {
		return NULL;
	}
}

// ht wrapper: insert new word to index (one new counters)
bool index_insert(index_t *index, const char *key, const int doc_id) {
    if (index != NULL && key != NULL && doc_id > 0) {
		int i = JenkinsHash(key, index->size); //get index for node
		if (index->table[i] == NULL) { //if node is not initialized
			index->table[i] = counters_new(); //initalize node
		}
        //regardless of whether the counters is in the table, use counters_add
        // to add or increment it
		if (counters_add(index->table[i], doc_id) != 0) { //if addition succeeds
                return true;
            } else {
                return false; //addition failed
            }
	} else {
		return false; //null checks fail
	}
}

// ht wrapper: find word
// returns pointer to counters object
// returns number associated with key and doc id
// zero if error
int index_find(index_t *index, const char *key, const int doc_id) {
    if (index != NULL && key != NULL) {
		int i = JenkinsHash(key, index->size); //go to slot
		return counters_get(index->table[i], doc_id); //counters error check (0 if error)
	}
	else {
		return 0; //null input
	}
}

//ht wrapper: delete index
// free each counter of the index
index_delete(index_t *index) {
    if (index != NULL) { //if null input, do nothing
		for (int i = 0; i < index->size; i++) {
			counters_delete(index->table[i]); //free counters
		}
		count_free(index->table); //free the table itself
		count_free(index); //free the index data structure
	}

}

/************************ NEW FUNCTIONALITY *******************************/

//NEW: saves index to external file 
bool index_save(index_t *index, const char *filename) {
    FILE *fp = openFile(filename);
    for (int i = 0; i < index->size; i++) {
        //for each word
        fprintf(fp, "%s", index->table[i]);
        printWordNumbers(index->table[i], fp); //TODO: crap, which key is the word? how t get it?
        fprintf(fp, "\n");
    }
    fclose(fp);
}

//save helper
void *openFile(const char *filename) {
    FILE *fp = NULL;
    if ((fp = fopen(filename, "w") != NULL) {
        return (void *)fp;
    } else {
        return NULL;
    }
}

//save helper
//assumes opened file
//iterates through counters and prints each number followed by a space
void printWordNumbers(counters_t *counter, FILE *fp) {
    
}

//NEW: loads index from external INDEX file
index_load() {

}