# README for Querier - Lab 6
## Uhuru Hashimoto CS50 21X

# Querier
The querier is a program, containing a driver, modules, and unittests, that returns a ranked list of documents/web pages to the user based on stdin input. 

# Assumptions 
It is assumed that the crawler-produced files and index match, and no further checking is done besides permissions verification. Each query follows the format specified in the IMPLEMENTATION doc.

# Usage
To run the querier, run 
`./querier pageDirectory indexFilename`
- where `pageDirectory` is the location of a crawler-produced directory
- and `indexFilename` is the location of the corresponding index of the crawler data.

# Build
To build the querier, run `make`. To build all modules, run `make` from the parent directory. 

# Unit Tests
To execute unit tests, run `make unittest` (which will run tests automatically and display output to stdout). This creates an executable, which may be run by `./querier` with no further arguments (any given will be ignored). 

*Please note: to run the normal querier after unit testing, `make clean` must be run before `make`, because of the identical names of the executable files.*

# Clean
To clean the querier, run `make clean`. 