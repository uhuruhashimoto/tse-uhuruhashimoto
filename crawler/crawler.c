/* 
* crawler program w driver - crawls webpages and outputs data to specified directory
* Error interpretation: 1 = command line arguments not verified, 2 = initialization failure, 3 = crawling error
* 
* Uhuru Hashimoto CS 50 21X
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

/*************************** DRIVER ************************************/
//Note: allocated url is freed automatically upon crawler call to webpage_delete()
int main(const int argc, char **argv) {
	int status = 0;

	/*------------------------------- CHECK ARGS ---------------------------*/
	//CHECK NUMBER
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

	//CHECK URL
	//internal check
	if (!isInternalURL(argv[1])) {
		fprintf(stderr, "Error: Seed URL %s is not internal\n", argv[1]);
		return ++status;
	}
	
	//copy and normalize
	char *url = copyURL(argv[1]);
	if (!NormalizeURL(url)) {
		fprintf(stderr, "Error: Seed URL %s cannot be normalized\n", url);
		return ++status;
	}

	//CHECK DIR
	char *dirname = argv[2];
	printf("Checking dir! TODO\n");

	//CHECK DEPTH
	//if non-numeric input, default to 0
	char *chardep = argv[3];
	intdep = strtoi(argv[3], NULL, 10); 

	/*------------------------------- DEPLOY CRAWLER ---------------------------*/
	status += crawler(url_copy, dirname, chardep, intdep);

	return status;
}


/*************************** HELPERS ************************************/
//crawler
int crawler(char *seedURL, char *dirname, char *chardep, int intdep) {
	//INITIALIZE DATA STRUCTURES
	int ret = 0;
	int *nitem = malloc(sizeof(int)); //counts items in ht
	int doc_id = 0; // doc id for saved info

	//bag (future pages)
	bag_t bag = bag_new();
	//hashtable (visited pages)
	hashtable_t table = hashtable_new();
	if (bag == NULL || table == NULL) {
		fprintf(stderr, "Failed to initalize crawler data structures.\n");
		return (ret+=2);
	}

	webpage_t *firstpage = webpage_new(seedURL, intdep, NULL);
	if (firstpage == NULL) {
		fprintf(stderr, "Seed webpage initalization failed. Cannot proceed\n");
		return (ret+=2);
	}

	//insert into bag and ht
	if (bag_insert(bag, firstpage) == NULL || hashtable_insert(bag, firstpage) == NULL) {
		fprintf(stderr, "Seed webpage insertion failed. Cannot proceed\n");
		return (ret+=2);
	}
	

	//CRAWL
	while (!isBagEmpty(bag, nitem)) {
		//get webpage from bag
		webpage_t *page = bag_extract(bag);
		if (page != NULL) {
			//fetch page from url
			if (pagefetcher(page)) {
				//save page
				pagesaver(page);
				//if we're still in depth constraint
				if (webpage_getDepth(page) < intdep) {
					//initalize tools for getting urls (allocation not necessary)
					int pos = 0;
					char *next_url;
					//get urls
					while ((next_url = webpage_getNextURL(page, &pos)) != NULL) {
						//make into a webpage
						webpage *newpage = webpage_new(next_url, ++webpage_getDepth(page), NULL);
						//put in bag
						bag_insert(bag, newpage);
					}
				}
			}
			webpage_delete(page); //page no longer in bag; delete
		}
	}

	//FREE STRUCTURES
	free(nitem);
	bag_delete(bag, webpage_delete());
	hashtable_delete(hashtable, webpage_delete());

	return ret;
}

//copy to allocate first url
//responsiblity of caller to free returned pointer
char *copyURL(char *url) {
	char *url_copy = assertp(calloc(sizeof(char), strlen(url)+1), "Memory insufficient");
	strcpy(url_copy, url);
	return url_copy;
}

//assumes non-null webpage with fetched html content
void pagesaver(webpage_t *webpage) {
	//saves and writes to file
	fprintf(stdout, "saving page! TODO\n");
}

//assumes null webpage html
//wrapper for clarity in modular decomposition
bool pagefetcher(webpage_t *webpage) {
	return webpage_fetch(webpage);
}

//wrapper to check if bag is empty
bool isBagEmpty(bag_t bag, int *counter) {
	bag_iterate(bag, counter, itemcount);
	if (*counter > 0) {
		return false;
	}
	return true;
}

//helper
//gets size of bag
//called as pointer to check if bag empty
static void itemcount(void *arg, const char *key, void *item)
{
  int *nitems = arg;

  if (nitems != NULL && key != NULL && item != NULL)
    (*nitems)++;
}
