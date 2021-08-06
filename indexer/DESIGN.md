# Design Spec for Index, Indexer, and Indextest
## Uhuru Hashimoto CS50 21X

Note: index is contained in common directory, and indexer and indextest under indexer directory. While the spec for this document only focuses on indexer, all are mentioned here because the logic for each translates most naturally with context. 

## Note on Index
Description: Index is a collection of modules and helpers that provide the functionality of a ht-like
data structure with counters as items. It maps a word -> doc id -> number of repetitions in document. The main APIs are: 

- new: creation and allocation of a hashtable of (k, v) -> (word, counters) 
- insert: insert a new word into ht
- find: return a counters associated with a word
- delete: delete ht and call function on each item
- *load: loads index from external file 
- *save: writes index to external file in format per line: word doc1 num1 [doc1 num2] ...
* New APIs (added to hashtable implementation)

## Design Specs: Indexer
* driver
Description: The indexer references index modules to build, and save a single index (provided by one crawler directory). It follows the following usage: 

`./indexer pageDirectory indexFilename`

where pageDirectory is a crawler-produced directory and indexFilename is an external file with write permissions to which to save the output. 

The indexer shall: 
- check inputs
- create an index object
- build it with the directory
- save it to the external file
- close file and delete index/free memory
- exit with a status indicating any errors

Note: no header file provided

## Note on Indextest
* driver
Description: Indextest loads an index from an index output file and compare the result to the file (testing the index_load and index_save functionality).
It follows the following usage: 

`./indextest oldIndex newIndex`

where oldIndex is the output file of a run of indexer (building and writing an index) and newIndex is the name of a new file with write permissions to which to save the resulting index. Note: after execution, these files should be identical. To check this, run bash `diff oldIndex newIndex` to check that it returns 0.

Indextest shall: 
- check inputs (or not)
- load index from oldIndex 
- save index to newIndex
- delete index 
- exit with a status indicating any errors

Note: no header file provided