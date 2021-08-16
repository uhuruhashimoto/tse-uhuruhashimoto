# Implementation of Querier - Lab 6
## Uhuru Hashimoto CS50 21X

Querier is a program that provides a user with search results for a specific query, as input to stdin in the following format: 
- One query per line
- A query is a selection of alphabetic words, with each word separated by spaces and (optionally) the operators 'and' and 'or'.
- Empty queries are accepted.
- Capitalized queries are normalized. 
After recieving a query, the query searches through its index (specified from `indexFilename` in command-line arguments), and prints a ranked list of documents that match the query. 

# Handling User Input

Each line of user input is split into a series of words by the use of two sliding pointers (one to track the beginnings of words, and one to track the ends). The beginning pointer looks for characters (matching the ctype isalpha command), and the end pointer looks for spaces (matching the ctype isspace command). By inserting null terminators, these pointers delineate the line into a series of characters without requiring the allocation of new memory. 

This functionality uses the `query user` and `get_words` modules, as well as the `hasCorrectOperators` helper to check for operators (syntax checking is carried out character-by-character in the `get_words` module).

Note: empty strings or space-only strings are handled by printing a delineator and accepting another query. 

# Searching

To search, this program utilizes a double-counter struct. It creates two of these, a `prodholder` and `resultholder` (corresponding to the arithmetic approach outlined in the provided class notes). 

- When dealing with the (implicit or explicit) 'and' operator, the program takes the intersection of the countersets from provided words and stores the results in `prodholder`.

- When dealing with the (explicit) 'or' operator, the program takes the union of the prod result counterset and the previous result and stores the result in `resultholder`. 

*Note: a successful union requires two cycles of the union iterator, to include the unique counterset entries in both sets. This overwrites the entries that overlap between the sets, but sets them to the same thing during both iterations.*

*Note: Only the two structs described are modified by this loop. The counters in the index are left purposefully untouched.* 

This functionality uses the `run_query` module and iterators (described below in "Iterators" section).

# Sorting

To sort, this program uses two separate structs, one to store ctrsnode data (`ctrsdata`) and one to store an array of the previous struct along with an index (`arraywithindex`). It creates a ctrsdata object, copies in the result (a counter) into this object. It then iterates over the result counter and uses an iteration-sort approach to sort the array (passed in an `arraywithindex` struct). In this way, the results are sorted without changing the specific result counter. 

This functionality uses the `display_results` module and iterators (described below in "Iterators" section).

# Iterators
The functions of iterators are described here: 
- intersection_iterator: takes the intersection of two provided countersets and stores the result in a different counterset. 
- union_iterator: takes the union of two provided countersets and stores the result in a different counterset. (Note: must be called twice.)
- count_iterator: counts the counters in a counterset
- sort_iterator: sorts an array of ctrsdata objects with insertion-sort approach (Note: must be passed an index)
- copy_iterator: copies a counterset to another counterset (used during search)
- copy_struct_iterator: copies a counterset to an array of struct ctrsdata objects (used before sorting)

# Unit Testing

User input handling (`get_words`), intersection (`intersection_iterator`), union (`union_iterator`), sorting, and display (`sort_iterator` and `display_results`) are all handled by the unit tests. These create dummy countersets, take their union/intersection/sort them, and display the results. 