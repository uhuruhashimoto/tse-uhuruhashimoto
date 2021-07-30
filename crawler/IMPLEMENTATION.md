# Implementation of Crawler 
## Uhuru Hashimoto CS50 21X

# General Description

My crawler leverages the counters, set, and hashtable modules that I created for Lab 4. Specifically, the crawler.c file contains the modular layout specified in the assignment prompt: 

-   a driver, `main`, that checks command line arguments
-   the `crawler` module, that contains the crawler loop (see below)
-   the `copyURL` function to allocate the url of the first webpage (others
    are allocated by `webpage_getNextURL`)
-   the `pagefetcher` module, a simple wrapper for `webpage_fetch` 
-   the `isBagEmpty` module, which uses the static helper `itemcount` to 
    iterate through the bag and return a size so that our crawler can check
    if the bag is empty. 

# Crawler Breakdown

The crawler works by referencing two data structures, a bag and a hashtable. The bag contains `webpage_t *` pointers to webpage objects, and the hashtable contains allocated keystrings of specific webpage URLs (with empty strings as objects). The bag is the future list of webpages to search, and the hashtable is a record of past pages visited. Therefore, the crawler uses `hashtable_insert` to simultaneously see if a page has been visited before and if not, create a record that it was just seen. 

The most important part of the crawler architecture is its loop (beginning at crawler.c line 131). While the bag is not empty, the crawler takes a single webpage from it, fetches its html content, and saves all data in a file. It then examines the hyperlinks on the page itself by attempting to insert them into the hashtable. If insertion is successful (meaning the crawler has not previously encountered the page), it adds it to the bag of pages to visit. This continues until the bag is empty, at which point the crawler frees allocated memory and terminates. 

A note on string allocation: my crawler uses the copyURL module to copy the url of the homepage (seed url) so it can later be freed by hashtable_delete. Otherwise, all keystrings are allocated by webpage_getURL() and freed by hashtable_delete().

# Testing

To test the crawler, various edge cases were used: 

## Invalid input arguments
The driver of the crawler is used to check arguments, both by number and content. It checks that 
- the correct number of arguments is present 
- the seed url provided is internal (leveraging isInternalURL)
- the directory name provided exists with write permissions (leveraging checkDir)
- the depth provided is non-negative and less than the macro MAX_DEPTH (currently set to 50)

Assumption: By using strtol to convert an argument to an integer, crawler defaults to a depth of 0 
if it encounters non-numeric input. However, floating point (decimal) input produces undefined behavior. 

## Letters: closed internal seed
The Letters seed tested the crawler on a closed system, where internal checks on html links were not necessary. It provided the correct files in the correct order, verifying that the crawler loop functioned successfully.

## Wiki: open internal seed
Testing on the Wikipedia seed checked not only that the crawler loop was functioning, but also that it contained a check that the urls it encountered were internal. This seed also tested scalability: when crawling up to hundreds of webpages at depth 2, any memory leaks (such as insufficient allocation of memory) could cause a core dump. 

## ToScrape: open internal seed
Testing on the ToScrape seed provided further verification that our crawler functioned in a variety of working environments. The adaptability of our crawler design is perhaps its greatest strength, and ToScrape provided the means to test this. 