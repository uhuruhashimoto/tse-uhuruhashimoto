/*
* implementation of set datatype
*
* Uhuru Hashimoto CS50 21X
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h> //for strcpy
#include "set.h"
#include "memory.h"

//iterator (optional)

/*********************** DATATYPES *******************************/

//local
//(k,v) pair and ptr to next list element
//Note: key is allocated in constructor and item is allocated by user
typedef struct setnode {
	char *key;
	void *item;
	struct setnode *next;
} setnode_t;

//general
//ptr to head of list
typedef struct set {
	setnode_t *head;
} set_t;

/*********************** function declarations *******************************/
//local constructor
static setnode_t *setnode_new(const char *key, void *item);
static setnode_t *findnode(set_t *set, const char *key);

/*********************** CONSTRUCTORS *******************************/
//allocate all memory (node and keys)

//local
//allocate memory for an individual node and its key
//if node is null or key < 0, return NULL
static setnode_t *
setnode_new(const char *key, void *item) 
{
	setnode_t *node = count_malloc(sizeof(setnode_t)); //0. allocate node

	if (node == NULL) {
		return NULL;
	} 
	else { 
		if (key != NULL) { //if key is valid
			char *newkey = count_malloc(sizeof(char)*strlen(key)); //1. allocate key (later freed in deletion function)
			newkey = strcpy(newkey, key); //2. copy key
			if (newkey != NULL) {
				node->key = newkey; //create new node with key and item 
		    	node->item = item;
		    	node->next = NULL;
		    	return node; //and return it
			}
			else {
				return NULL; //if key cannot be allocated (out of memory)
			}
		}
		else {
			return NULL; //if key is not valid, return null
		}
	}
}

//general
//allocate memory for a set (empty)
//returns null if mem alloc unsuccessful
//otherwise returns pointer to new set
set_t *
set_new(void) 
{
	set_t *set = count_malloc(sizeof(set_t));
	//if allocation is unsuccessful, return null
	if (set == NULL) {
		return NULL;
	}
	else { //if it works, return the set
		set->head = NULL; //initialize ptr as null for safety
		return set; //return pointer to set
	}

}

/*********************** GENERAL MODULES *******************************/

//helper; now with null check
//iteration
//checks if set is null; if so, returns null
//else returns pointer to specific node with keys
//if not found, returns null
static setnode_t *
findnode(set_t *set, const char *key) 
{
	bool found = 0; //set found flag
	if (set != NULL) {
		setnode_t *node = set->head; //start at the top
		while (!found && node != NULL) { //go through list and check for key (stop if found)
			if (strcmp(key, node->key) == 0) {
				found = 1;
			}
			if (!found) { //if not found, keep going
				node = node->next; 
			}
		}
		if (found) { 
			return node; //node was found!
		}
		else {
			return NULL; //node not found
		}
	}
	else { //if not found (or set is null), return null
		return NULL;
	}
}

//insertion
//returns false if null, duplicate, or error
//returns true if insertion successfull
bool 
set_insert(set_t *set, const char *key, void *item) 
{
	if (set != NULL && key != NULL && item != NULL) { //null checks
		//look through set for key
		setnode_t *node = findnode(set, key);
		if (node != NULL) {
			return false; //duplicate
		}
		else {
			setnode_t *new = setnode_new(key, item); //create a new node (including allocating keystring memory)
			if (new != NULL) { //if allocation works, insert the new node
				new->next = set->head;
				set->head = new;
				return true;
			} 
			else {
				return false; //no more memory to allocate/mem error
			}
		}
	}
	else {
		return false; //if any input is null, return false
	}
}

//find
//if nothing is null, search set for key
//if found, return item
//if not, return null
void *
set_find(set_t *set, const char *key) 
{
	if (set != NULL && key != NULL) { //null check
		setnode_t *node = findnode(set, key);
		if (node != NULL) {
			return node->item;
		}
		else {
			return NULL; //node not found
		}
	} 
	else {
		return NULL; //null inputs
	}

}

//print
//null if null, empty if empty or itemprint null
//format: {item, item, item}
void 
set_print(set_t *set, FILE *fp, 
               void (*itemprint)(FILE *fp, const char *key, void *item) ) 
{
	if (fp != NULL) { //if no file, do nothing
		if (set != NULL) {
			fputc('{', fp);
			for (setnode_t *node = set->head; node != NULL; node = node->next) {
				if (itemprint != NULL) {
					(*itemprint)(fp, node->key, node->item); //print the item
					if (node->next != NULL) {
						fputc(',', fp); //separate with commas
					}
				}
				//if itemprint is null, no set will be printed here
			}
			fputc('}', fp);
		}
		else {
			fputs("(null)", fp);
		}
	}

}

//iterate
void 
set_iterate(set_t *set, void *arg,
                 void (*itemfunc)(void *arg, const char *key, void *item) ) 
{
	if (set != NULL && itemfunc != NULL) { //null checks
		for (setnode_t *node = set->head; node != NULL; node = node->next) {
			(*itemfunc)(arg, node->key, node->item); //apply the function
		}
	}
}

/*********************** DELETION *******************************/
//free all memory (nodes, keys, and sets)
void 
set_delete(set_t *set, void (*itemdelete)(void *item) ) 
{
	if (set != NULL) {
		for (setnode_t *node = set->head; node != NULL; ) { //for each node
	      if (itemdelete != NULL) {        
	        (*itemdelete)(node->item);     //delete or free item
	      }
	      count_free(node->key); //free key (pointer to char array)
	      setnode_t *next = node->next;     //store next
	      count_free(node);                 //free whole node
	      node = next;                      
	    }
	    count_free(set); //free set
	}
}

