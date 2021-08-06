# README for Indexer - Lab 5
## Uhuru Hashimoto CS50 21X

Indexer is a file that creates an index of normalized words and their frequencies from provided crawler webpage data. Its files follow the per-line format `word doc_id num` where `word` is a word in the file, and the one or more `doc_id  num` pairs are the id of the file and number of times the word occurs (1 or more).
Note: words may be in any order, but doc_id's are sorted from least to greatest on the lines. 

Usage: 

`./indexer dirname filename` where `dirname` is a crawler-created directory (will be verified) and `filename` is the name of a file to which to save the data. 

To build, run `make` or `make all`. 

To test for memory leaks, run `make valgrind`. 

To run a test, run `make test` (which directs output to testing.out) or simply `./testing.sh`. (Note: testing requires the use of both targets, so running `make indexer` and `./testing.sh` will fail.)

To clean, run `make clean` (note: this also cleans libraries).