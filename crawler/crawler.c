/* 
* crawler program w driver - crawls webpages and outputs data to specified directory
* Error interpretation: 1 = command line arguments not verified, 2 = initialization failure, 3 = crawling error
* 
* Uhuru Hashimoto CS 50 21X
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h> //for parsing command line args
#define MAX_WEBPAGES_TO_VISIT
//TODO: include bag and ht and set
//TODO: and webpagefetcher, pagedir, etc. 

//TODO: what should I do about numslots to initialize ht? (currently #defined as MAX)
// if need to dynamically allocate, must calloc, copy, and free every cycle to add a new webpage
// ew

/*************************** TYPEDEF ************************************/
typedef struct crawler {
	bag_t *bag; //urls to visit
	hashtable_t *table; //urls visited
} crawler_t;

/*************************** FUNCTION DECLARATIONS ************************************/

static void itemcount(void *arg, const char *key, void *item)
void pagefetcher();
void pagescanner();

/*************************** DRIVER ************************************/
int main(const int argc, char **argv) {
	/*------------------------------- CHECK ARGS ---------------------------*/
	//check command-line args
	//note: strings for arguments declared below
	int max_depth; //depth to crawl
	int *nitem; //items in ht
	int doc_id = 0; // doc id for saved info

	if (argc < 4) {
		fprintf(stderr, "Error: fewer than mininmum number of arguments.\n "
			"Usage: ./crawler seedURL pageDirectory maxDepth\n");
		return 1;
	}
	else if (argc > 4) {
		fprintf(stderr, "Error: exceeded max number of arguments.\n "
			"Usage: ./crawler seedURL pageDirectory maxDepth\n");
		return 1;
	}
	//assign arguments
	char *seedURL = argv[1];

	//verify directory
	char *dirname = argv[2];
	//TODO use pagedir here

	//assign maxDepth
	max_depth = strtoi(argv[3], NULL, 10); //if non-numeric input, default to 0


	/*------------------------------- INITIALIZE MODS ---------------------------*/
	//crawler and its data structures
	//all allocation here
	crawler_t crawler = malloc(sizeof(crawler_t));

	if (crawler == NULL) {
		fprinf(stderr, "Failed to initialize modules. Memory allocation error.\n");
		return 2;
	}

	crawler->bag = bag_new();
	crawler->table = hashtable_new(MAX_WEBPAGES_TO_VISIT); 

	if (crawler->bag == NULL || crawler -> table == NULL) {
		fprinf(stderr, "Failed to initialize modules. Memory allocation error.\n");
		return 2;
	}

	nitem = malloc(sizeof(int));


	/*------------------------------- CRAWL ---------------------------*/
	//
	//make the first webpage object (seed)
	webpage_t *first = webpage_new(seedURL, max_depth, html);
	//put it in bag
	if (bag_insert(crawler->bag, first) == NULL) {
		fprintf(stderr, "Initialization/insertion error: bag not empty\n");
		return 2;
	}

	//crawling loop
	//get size of bag
	hashtable_iterate(ht, nitem, itemcount);
	while (*nitem > 0) {
		//get webpage
		webpage_t *webpage = bag_extract();
		if (webpage == NULL) {
			fprintf(stderr, "Bag extraction error while crawling\n");
			return 3;
		} //pause built in 

		//write page data to directory
			//pagefetcher, pagescanner (docid++)

		//if webpage is < depth, get urls and store them
		if (webpage->depth < max_depth) {
			//normalize urls
			//check if seen: insert into ht (seen)
			//if not seen yet, make webpage object (maxdepth++)
			//insert into bag 
		}

		hashtable_iterate(ht, nitem, itemcount); //recheck if ht is empty
	}


	/*------------------------------- FREE MODS ---------------------------*/
	//all deallocation here
	free(nitem);
	bag_delete(crawler->bag, webpage_delete());
	hashtable_delete(crawler->hashtable, webpage_delete());
	free(crawler);

	return 0;
}

/*************************** HELPERS ************************************/

//gets size of bag
static void itemcount(void *arg, const char *key, void *item)
{
  int *nitems = arg;

  if (nitems != NULL && key != NULL && item != NULL)
    (*nitems)++;
}

void pagefetcher() {
	//use (webpage-fetch())
}

void pagescanner() {

}

