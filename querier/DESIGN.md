# Design Spec for Querier
## Uhuru Hashimoto CS50 21X

Description: 

Querier will perform two main tasks: 

1.) accept and categorize user input
2.) perform search queries to an index based on that input

The planned modular decomposition is as follows: 

# Querier Modules

main() 

    query_user()
        check_query()
        print_errors() (this may be multiple helpers)
    
    rank_query_args()
        product() (ANDs)
        sum() (ORs)

    search_query()
        OR_iterator()
        AND_iterator()

    printquery()

The modular breakdown is as follows: 

1.) ARGUMENT CHECKING/INPUT CATEGORIZATION

main: driver
- validate user args
- load index
- query_user() while !EOF
- exit with error status

query_user()
- get a line (1 query)
- check_query for errors; print_errors()
- rank_query_args(query)

rank_query_args()
- while ! \n
    - assemble hashtable of queries (each multiple char * words)
    - use sum and product 
- return hashtable

2.) SEARCH QUERY AND DISPLAY RESULTS

search_query() (single query searcher)
- OR_iterator  
    - for each set in hashtable (single query; set of ANDs/char ** word lists)
    - AND_iterator(set)
- print_query(result)


SORTING LOGIC

To sort, we pass an iterator a struct containing two things: (1) an array of countersdata objects, each containing doc_id and count info, and (2) an int pointer to keep track of the index (delineation between the sorted and unsorted sides of the list). For each counter in our result counter (k, v pair) we compare the score (v) to the score of each array entry below it, and move them over one place if their value is less than the score. When it isn't, we put the score in the newly open slot. This sorts the array in order from highest to lowest. 






