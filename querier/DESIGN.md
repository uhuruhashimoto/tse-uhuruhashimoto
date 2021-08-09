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


RANK QUERY LOGIC

This will use the outlined system, with a two-counter structure to hold both the result (malloc'ed and added to), and the comparison counter (part of index; not modified during copy process).



