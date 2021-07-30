/* 
* crawler program w driver - crawls webpages and outputs data to specified directory
* Error interpretation: 1 = command line arguments not verified, 2 = initialization failure, 3 = crawling error
* 
* Uhuru Hashimoto CS 50 21X
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
//all of library
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
#define NUM_SLOTS 200
#define MAX_DEPTH 50

/*************************** FUNCTION DECLARATIONS **********************/
int crawler(char *seedURL, char *dirname, int depth);
char *copyURL(char *url);
bool pagefetcher(webpage_t *webpage);
bool isBagEmpty(bag_t *bag, int *counter);
static void itemcount(void *arg, void *item);

/*************************** DRIVER ************************************/
// Note: allocated url is freed automatically upon crawler call to webpage_delete()
int main(const int argc, char **argv) {
	int status = 0;

	/*------------------------------- CHECK ARGS ---------------------------*/
	// CHECK NUMBER
	if (argc < 4) {
		fprintf(stderr, "Error: fewer than mininmum number of arguments.\n "
			"Usage: ./crawler seedURL pageDirectory maxDepth\n");
		return ++status;
	}
	else if (argc > 4) {
		fprintf(stderr, "Error: exceeded max number of arguments.\n "
			"Usage: ./crawler seedURL pageDirectory maxDepth\n");
		return ++status;
	}

	// CHECK URL
	// internal check
	if (!IsInternalURL(argv[1])) {
		fprintf(stderr, "Error: Seed URL %s is not internal\n", argv[1]);
		return ++status;
	}
	
	// copy and normalize
	char *url = copyURL(argv[1]);
	if (!NormalizeURL(url)) {
		fprintf(stderr, "Error: Seed URL %s cannot be normalized\n", url);
		if (url != NULL) free(url);
		return ++status;
	}

	// CHECK DIR
	char *dirname = argv[2];
	bool dirWritable = checkDir(dirname);
	if (!dirWritable) {
		fprintf(stderr, "Error: directory %s does not exist or lacks write permissions\n", dirname);
		free(url);
		return ++status;
	}

	// CHECK DEPTH
	// if non-numeric input, default to 0
	int intdep = strtol(argv[3], NULL, 10);  // assumes no overflow
	if (intdep < 0) {
		fprintf(stderr, "Error: A negative depth %d was provided.\n", intdep);
		return ++status;
	}
	else if (intdep > MAX_DEPTH) {
		fprintf(stderr, "Error: Depth %d exceeded maximum depth %d.\n", intdep, MAX_DEPTH);
		return ++status;
	}

	/*------------------------------- DEPLOY CRAWLER ---------------------------*/
	status += crawler(url, dirname, intdep);

	return status;
}


/*************************** HELPERS ************************************/
// crawler
int crawler(char *seedURL, char *dirname, int depth) {
	// INITIALIZE DATA STRUCTURES
	int ret = 0;
	int *nitem = malloc(sizeof(int)); //counts items in ht
	*nitem = 0; // initialize to 0
	int doc_id = 0; // doc id for saved info

	// bag (future pages)
	bag_t *bag = bag_new();
	// hashtable (visited pages)
	hashtable_t *table = hashtable_new(NUM_SLOTS);
	if (bag == NULL || table == NULL) {
		fprintf(stderr, "Failed to initalize crawler data structures.\n");
		return (ret+=2);
	}

	webpage_t *firstpage = webpage_new(seedURL, 0, NULL);
	if (firstpage == NULL) {
		fprintf(stderr, "Seed webpage initalization failed. Cannot proceed\n");
		if (seedURL != NULL) free(seedURL);
		return (ret+=2);
	}

	// insert into bag and ht
	// (bag insertion takes pointer to webpage
	// ht insertion copies allocated url string; left to us to free)
	bag_insert(bag, firstpage); 
	if (!hashtable_insert(table, webpage_getURL(firstpage), "")) {
		fprintf(stderr, "Seed webpage insertion failed. Cannot proceed\n");
		return (ret+=2);
	}
	

	// CRAWL
	while (!isBagEmpty(bag, nitem)) {
		// get webpage from bag
		webpage_t *page = bag_extract(bag);
		if (page != NULL) {
			// fetch page from url
			if (pagefetcher(page)) {
				//save page
				pagesaver(page, webpage_getDepth(page), dirname, ++doc_id);
				// if we're still in depth constraint
				if (webpage_getDepth(page) < depth) {
					// initalize tools for getting urls (allocation not necessary)
					int pos = 0;
					char *next_url;
					// get urls
					while ((next_url = webpage_getNextURL(page, &pos)) != NULL) {
						//if the url is internal
						if (IsInternalURL(next_url)) {
							// try to insert it into the hashtable
							if (hashtable_insert(table, next_url, "")) {
								// if it hasn't been seen before (insertion successful)
								// make into a webpage
								int ndepth = webpage_getDepth(page);
								webpage_t *newpage = webpage_new(next_url, ++ndepth, NULL);
								// put in bag
								bag_insert(bag, newpage);
							}
						}
					}
				}
			}
			webpage_delete(page); // page no longer in bag; delete
		}
		*nitem = 0; // reset counter for next iteration
	}

	// FREE STRUCTURES
	free(nitem);
	bag_delete(bag, webpage_delete);
	hashtable_delete(table, NULL); // no itemdelete needed, since values are not allocated

	return ret;
}

// copy to allocate first url
// responsiblity of caller to free returned pointer
char *copyURL(char *url) {
	char *url_copy = count_malloc(sizeof(char)*(strlen(url)+1)); // first copy; second is in set line 54
	strcpy(url_copy, url); // (destination, source)
	url_copy[strlen(url)] = '\0'; // set nul terminator just in case
	return url_copy;
}

// assumes null webpage html
// wrapper for clarity in modular decomposition
bool pagefetcher(webpage_t *webpage) {
	return webpage_fetch(webpage);
}

// wrapper to check if bag is empty
bool isBagEmpty(bag_t *bag, int *counter) {
	bag_iterate(bag, counter, itemcount);
	if (*counter > 0) {
		return false;
	}
	return true;
}

// helper
// gets size of bag
// called as pointer to check if bag empty
static void itemcount(void *arg, void *item)
{
  int *nitems = arg;

  if (nitems != NULL && item != NULL)
    (*nitems)++;
}
