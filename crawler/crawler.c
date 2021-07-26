/* 
* crawler program w driver - crawls webpages and outputs data to specified directory
* Error interpretation: 1 = command line arguments not verified, 2 = initialization failure, 3 = crawling error
* 
* Uhuru Hashimoto CS 50 21X
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h> //for parsing command line args
//#include "../libcs50/*"
#define NUM_SLOTS 200

/*************************** FUNCTION DECLARATIONS ************************************/

static void itemcount(void *arg, const char *key, void *item);
void pagefetcher();
void pagescanner();

/*************************** DRIVER ************************************/
int main(const int argc, char **argv) {

	/*------------------------------- CHECK ARGS ---------------------------*/
	//check command-line args
	//note: strings for arguments declared below
	int max_depth; //depth to crawl

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

	/*------------------------------- DEPLOY CRAWLER ---------------------------*/
	int status = crawler(seedURL, dirname, max_depth);

	return status;
}

/*************************** HELPERS ************************************/
//crawler
int crawler(char *seedURL, char *dirname, int max_depth) {
	/*------------------------------- INITIALIZE MODS ---------------------------*/
	//crawler and its data structures
	//all allocation here
	int *nitem; //items in ht
	int doc_id = 0; // doc id for saved info
	bag_t *bag = bag_new();
	hashtable_t *table = table_new(NUM_SLOTS);

	if (bag == NULL || table == NULL) {
		fprinf(stderr, "Failed to initialize modules. Memory allocation error.\n");
		return 2;
	}

	nitem = malloc(sizeof(int));


	/*------------------------------- CRAWL ---------------------------*/

	//make the first webpage object (seed)
	webpage_t *first = webpage_new(seedURL, max_depth, NULL); //no html yet
	//put it in bag
	if (bag_insert(bag, first) == NULL) {
		fprintf(stderr, "Initialization/insertion error: bag not empty\n");
		return 2;
	}

	//crawling loop
	//get size of bag
	hashtable_iterate(table, nitem, itemcount);
	while (*nitem > 0) {
		//get webpage
		webpage_t *webpage = bag_extract(bag);
		if (webpage == NULL) {
			fprintf(stderr, "Bag extraction error while crawling\n");
			return 3;
		} //pause built in 

		//write page data to directory
			//pagefetcher, pagescanner (docid++)

		//if webpage is < depth, get urls and store them
		if (webpage->depth < max_depth) {
			//use pagescanner to get links from page
			if (!pagefetcher(webpage)) {
				fprintf(stderr, "Failed to find html data\n");
				return 3;
			}
			else {

			}
			char *html = webpage_getHTML(page);

			//for each page
				//normalize urls
				//check if seen: insert into ht (seen)
				//if not seen yet, make webpage object (maxdepth++)
				//insert into bag 
		}

		hashtable_iterate(table, nitem, itemcount); //recheck if ht is empty
	}


	/*------------------------------- FREE MODS ---------------------------*/
	//all deallocation here
	free(nitem);
	bag_delete(bag, webpage_delete());
	hashtable_delete(hashtable, webpage_delete());

	return 0;
}

//assumes null webpage html
//wrapper for clarity in modular decomposition
bool pagefetcher(webpage_t *webpage) {
	return webpage_fetch(webpage);
}

void pagescanner() {
	 webpage_fetch()
}

void pagesaver() {

}


//gets size of bag
static void itemcount(void *arg, const char *key, void *item)
{
  int *nitems = arg;

  if (nitems != NULL && key != NULL && item != NULL)
    (*nitems)++;
}
