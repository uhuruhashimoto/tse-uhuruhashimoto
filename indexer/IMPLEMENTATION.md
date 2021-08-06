# Implementation of Indexer
## Uhuru Hashimoto CS50 21X

## Note on Index
The index used is a hashtable of countersets. That is, each hashtable slot contains a set of (key, item) pairs, and each (item) in the set is itself a counterset of (counters) objects. These contain (k, v) pairs of (doc_id, frequency) values for the words in the hashtable.

## Indexer - Implementation 

My indexer builds indexes with the `index_build` module, which simply contains a loop to 
The modular breakdown of indexer is as follows:

- `main` checks command-line arguments (including verifying directory)
- `index_build` creates an index

The `index_build` function also references two static helpers, one of which is an iterator and the other is `index_page`, which deals with one webpage. `index_page` creates a webpage object (with provided html data) from the crawler data on a single file (of number doc_id), and uses `webpage_getNextWord` to find the words on the page. For each word, and attempts to normalize it and checks that its length is greater than the minimum word length. If the word meets these criteria, then it is added to the index.

Addition to the index occurs differently for two possible cases. If the index already contains the word, then the counterset associated with it is modified to add an entry of key 'doc_id'. If the index does not contain the word, then a new counter is created for it and it is added to the index (which is a hashtable).

## Indexer - Testing Plan

The test cases for the indexer focus on two main areas of note:
1.) input argument testing
2.) testing actual crawler directories

For the first case, multiple incorrect input tests are included in the testing.sh file. These should print a specific error and exit with a nonzero return value (but the program should not crash).

For the second case, the test file creates a directory `test`, under which it stores the created indexes in pertinent subdirectories (i.e. letter indexes under `letter`) with file format `index.letters-#` where `letters` is the source of the data, and `#` represents the depth of the data. 

To verify these indexes, I compared them to the class data provided at http://cs50tse.cs.dartmouth.edu/tse-output/

Note: The test file also creates copies (for indextest) under the same subdirectories, with fileformat `copyindex.letters-#` (simply adding `copy` to the filenames). It then compares these to the original indexes using the `diff` command. 