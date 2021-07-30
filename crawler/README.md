# README for Crawler - Lab 4
## Uhuru Hashimoto CS50 21X

# Crawler
The crawler is a program, containing a driver and associated modules, which crawls webpages to a specified depth and saves their associated html data to a series of files identified by a specific id ("1", "2", etc.)

## Build
To build the crawler, run `make` (this also builds associated libraries).

## Normal Operation
To run, use `./crawler URL dirname depth` where:
-   'URL' is a specified *internal* url beginning with http://cs50tse.cs.dartmouth.edu/
-   'dirname' is the name of/path to a directory with write permissions
-   'depth' is a non-negative integer specifying the depth at which the crawler
    should crawl.

## Testing
To test the crawler (output redirected to `testing.out`), run `make test` or directly test to stdout with `testing.sh`. 

*Note: this creates nested storage directories in the current directory of the user. Inital testing currently writes twice to THE SAME DIRECTORY for its first test (letters at depth 0), and then writes to successive directories.*

*Note: uncomment the commented test in testing.sh to test the wiki safe seed at depth 2. This test creates 1675 files under test/wiki/wiki-2 and takes ~30 minutes to run.*

To enable file overwrite checks when writing data, run `make safe` before testing. This will preserve data files within the test directory, and issue corresponding warnings within the testing output file if any attempts are made to overwrite existing files. 

*PLEASE NOTE: because common.a is a static library, running `make` after `make safe` will not recompile the object files, and the file overwrite check will remain in force during testing. To remove this, the user must run `make clean` before recompiling and testing.*

## Clean
To clean, use `make clean` (which also cleans libraries). 