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