# README for Crawler - Lab 4
## Uhuru Hashimoto CS50 21X

# Crawler
The crawler is a program, containing a driver and associated modules, which crawls webpages to a specified depth and saves their associated html data to a series of files identified by a specific id ("1", "2", etc.)

To build the crawler, run `make` (this also builds associated libraries).

To run, use `./crawler URL dirname depth` where:
-   'URL' is a specified *internal* url beginning with http://cs50tse.cs.dartmouth.edu/
-   'dirname' is the name of/path to a directory with write permissions
-   'depth' is a non-negative integer specifying the depth at which the crawler
    should crawl.

To clean, use `make clean` (which also cleans libraries).