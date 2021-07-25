/*
* counters.c - file containing modules to implement a counter
* (set-like data structure which incrememnts a value associated with a specific key)
*
* Uhuru Hashimoto CS50 21X
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "counters.h"
#include "memory.h"

/******************* TYPE DEFINITIONS *************************/
//two structs

//1. general counter
typedef struct counters {
	struct countersnode *head; //pointer to head of list (initially null)
} counters_t;

//2. counter node
typedef struct countersnode {
	//each node contains a pointer to next, and a value (counter)
	struct countersnode *next; //pointer to next element in list
	int key; //key for key/val pair (no malloc here!)
	int count; //count for each key
} countersnode_t;

/******************* FUNCTION HANDLES *************************/

//function handle: local counter
static countersnode_t *countersnode_new(const int key);
//function handle: helper function (search "iteration" keyword for usage)
static countersnode_t *findnode(counters_t *ctrs, const int key);

/******************* CONSTRUCTORS *************************/
//build counter (general)
//ALLOCATE MEMORY HERE (1 node)
counters_t *
counters_new(void) 
{
	counters_t *new = count_malloc(sizeof(countersnode_t));
	if (new == NULL) { //if pointer does not work
		return NULL;
	}
	else { //if the pointer works
		new->head = NULL; //set the head to null for now
		return new; //return pointer to counters datatype
	}
}

//build counter node (local/static)
//ALLOCATE MEMORY HERE (+1 node)
static countersnode_t *
countersnode_new(const int key) 
{
	countersnode_t *node = count_malloc(sizeof(countersnode_t));

	if (node == NULL) {
		//no more memory or memory error
		return NULL;
	}
	else {
		if (key >= 0) {
			node->key = key;
			node->count = 1;
			node->next = NULL;
		}
		else {
			return NULL; //negative key
		}
		return node;
	}
}

/******************* GETTERS, SETTERS, ITERATORS *************************/

//helper iteration function
//returns pointer to node if found, NULL if not found
static countersnode_t *
findnode(counters_t *ctrs, const int key) 
{
	bool found = 0;
	if (ctrs != NULL) {
		countersnode_t *node = ctrs->head; //initialize starting node
		while (!found && node != NULL) {
			if (node->key == key) {
				found = 1;
			}
			else {
				node = node->next;
			}
		}
		if (!found) {
			return NULL; 
		}
		else {
			return node;
		}
	}
	else {
		return NULL; //null ctrs
	}
	
}

//addition
//look for key
//if found, increment counter
//if not found, make a new node in ctrs
int counters_add(counters_t *ctrs, const int key) 
{
	if (ctrs != NULL) { //null checks and key checks
		if (key >= 0) {
			//look for key
			countersnode_t *node = findnode(ctrs, key); //iteration
			if (node != NULL) { //if a node is returned (it found one!)
				node->count++;
				return node->count;
			}
			else { //if the node does not exist in the ctrs
				//if not found, create new node
				countersnode_t *new = countersnode_new(key);
				if (new != NULL) {
					new->next = ctrs->head;
					ctrs->head = new;
					return new->count;
				}
				else {
					return 0; //node creation failed (or key was <0)
				}
			}
		}
		else { //end null check; report error and return 0
			return 0;
		}
	}
	else { //end key check; report negative-key error and return 0
  		return 0;
	}

}

//getter
//look through counters for key
//if found, return it
//if not found (or counters is null), return 0
int counters_get(counters_t *ctrs, const int key) 
{
	if (ctrs != NULL) {
		countersnode_t *node = findnode(ctrs, key); //iteration
		if (node != NULL) {
			return node->count;
		}
		else {
			return 0;
		}
	}
	else {
		return 0;
	}

}

//setter
//search through the counters
//if key is found, update the value
//if key is not found, make a new node with value initialized as count
bool counters_set(counters_t *ctrs, const int key, const int count) 
{
	if (ctrs != NULL && key >= 0 && count >=0) { //null/vals check
		countersnode_t *node = findnode(ctrs, key);
		if (node == NULL) { //if not found, create a new node and initialize its value to count
			countersnode_t *new = countersnode_new(key);
				if (new != NULL) {
					new->count = count; //change val from 1 to count
					new->next = ctrs->head;
					ctrs->head = new;
					return true;
				}
				else { //out of memory
					return false;
				}
		}
		else { //if found, update counter
			node->count = count;
			return true;
		}
	}
	else {
		return false;
	}

}

//printing
//prints counters within {}
//each node is printed {k,v}
//spaces are for cowards
void counters_print(counters_t *ctrs, FILE *fp) 
{
	if (fp != NULL) { //if no file, do nothing
		if (ctrs != NULL) {
			fputc('{', fp);
			for (countersnode_t *node = ctrs->head; node != NULL; node = node->next) {
				//format: {[firstval],[secondval]} for each pair
				fputc('{', fp);
				fprintf(fp, "%d", node->key);
				fputc(',', fp);
				fprintf(fp, "%d", node->count);
				fputc('}', fp);
			}
			fputc('}', fp);
		}
		else {
			fputs("(null)", fp);
		}
	}
}

//iterator
//different from helper
//go through list and apply function (of pointer) to each one
void counters_iterate(counters_t *ctrs, void *arg, 
                      void (*itemfunc)(void *arg, 
                                       const int key, const int count)) 
{
	if (ctrs != NULL && itemfunc != NULL) { //null check
		for (countersnode_t *node = ctrs->head; node != NULL; node = node->next) { //for each node in ctrs
			(*itemfunc)(arg, node->key, node->count); //apply the function
		}
	}

}

/******************* DELETION *************************/
//delete function 
//FREE MEMORY HERE
//1. free each node
//2. free general counter
void counters_delete(counters_t *ctrs) 
{
	if (ctrs != NULL) { //null check
		for (countersnode_t *node = ctrs->head; node != NULL;) { //for each node in ctrs (3rd for term happens in loop itself)
			countersnode_t *next = node->next; //store next
			count_free(node); //free node
			node = next; //step
		}
		count_free(ctrs); 
	}
}

