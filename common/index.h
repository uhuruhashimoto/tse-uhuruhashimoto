/*
* header file for index modules
*
* Uhuru Hashimoto CS50 21X */


// typedef defintion for index
typedef struct hashtable_t index_t;

// creator; ht wrapper
index_t *index_new(const int num_slots);

// insertion; ht wrapper
bool index_insert(index_t *index, const char *key, counters_t *item);

// finder; ht wrapper
counters_t *index_find(index_t *index, const char *key);

// deletion; ht wrapper
void index_delete(index_t *index, void (*itemdelete)(counters_t *item) );

// NEW FUNCTIONALITY: saves index to external file
// with one word per line. Format: 
// 		word doc_id1 num1 [doc_id2 num2]...
bool index_save(index_t *index, const char *filename);

// loads index from index-created file
// of format (per line):
// 		word doc_id1 num1 [doc_id2 num2]...
index_t *index_load(char *filename);