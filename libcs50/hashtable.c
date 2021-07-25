/* 
* implements hash table along with provided hash implementatioin
*
* Uhuru Hashimoto CS50 21X
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "hashtable.h"
#include "memory.h"
#include "./jhash.h"
#include "set.h"


/************************ TYPE DEFINITIONS ***********************************/
//general
//(local is just a set)
typedef struct hashtable {
	set_t **table; //pointer to array of pointers (a set)
	int size; //size (for convenience)
} hashtable_t;

/************************ FUNCTIONS ***********************************/
//no local functions; using set's mods for all of them :)

/************************ CONSTRUCTORS ***********************************/
//allocate memory for table here
//general
hashtable_t *
hashtable_new(const int num_slots) 
{
	hashtable_t *table = count_malloc(sizeof(hashtable_t)); 
	if (table != NULL) {
		table->table = count_calloc(num_slots, sizeof(set_t *)); //allocates slots next to each other
		if (table->table == NULL) {
			return NULL; //no memory 
		} 
		else {
			table->size = num_slots; //just for easy access
		}
		return table;
	}
	else {
		return NULL;
	}
}

/************************ GETTERS/SETTERS ***********************************/
//also allocate memory here, upon insertion (done automatically by set)
//allocate for each set in table if necessary
//insertion
bool hashtable_insert(hashtable_t *ht, const char *key, void *item) 
{
	if (ht != NULL && key != NULL && item != NULL) {
		int index = JenkinsHash(key, ht->size); //get index for node
		if (ht->table[index] == NULL) { //if node is not initialized
			ht->table[index] = set_new(); //initalize node
		}
		//look for (k,v) in ht
		if (item != set_find(ht->table[index], key)) { 
			//if not there, insert
			bool isinserted = set_insert(ht->table[index], key, item);
			if (isinserted) { //add it to the table
				return true;
			}
			else {
				return false; //insertion failed
			}
		}
		else {
			return false; //if there, return false
		}
	}
	else {
		return false; //null checks fail
	}
}

//find
void *
hashtable_find(hashtable_t *ht, const char *key) 
{
	if (ht != NULL && key != NULL) {
		int index = JenkinsHash(key, ht->size); //go to slot
		return set_find(ht->table[index], key); //if it's null, then something went wrong
	}
	else {
		return NULL; //null input
	}
}

//print
void 
hashtable_print(hashtable_t *ht, FILE *fp, 
                     void (*itemprint)(FILE *fp, const char *key, void *item)) 
{
	if (fp != NULL) {	//check inputs
		if (ht != NULL) { 
			fputc('{', fp);
			for (int i = 0; i < ht->size; i++) { //for each set, print the set on a new line
				set_print(ht->table[i], fp, itemprint);
				fputc('\n', fp);
			}
			fputc('}', fp);
		}
		else {
			fputs("(null)", fp); //if ht is null
		}
	}
}

//iterate
void hashtable_iterate(hashtable_t *ht, void *arg,
                       void (*itemfunc)(void *arg, const char *key, void *item) ) {
	if (ht != NULL && itemfunc != NULL) { //if null inputs, do nothing
		for (int i = 0; i < ht->size; i++) { //otherwise, go through the table
			set_iterate(ht->table[i], arg, itemfunc); //apply func through set module
		}
	}
}

/************************ DELETION ***********************************/
//free memory here
void hashtable_delete(hashtable_t *ht, void (*itemdelete)(void *item) ) {
	if (ht != NULL) { //if null input, do nothing
		for (int i = 0; i < ht->size; i++) { //for each entry, delete and free
			set_delete(ht->table[i], itemdelete); //free strings and set
		}
		count_free(ht->table); //free the table itself
		count_free(ht); //free the ht data structure
	}
}

