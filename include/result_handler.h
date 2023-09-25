#ifndef _RESULT_HANDLER_H_
#define _RESULT_HANDLER_H_

#include "list.h"

typedef struct hashcell {
    Key * key ;
    int val ;
} HashCell ;

typedef struct hashtable {
    HashCell ** tab ;
    int size ;
} HashTable ;

/* Removes all the declarations with invalid signatures */
void filter_fraud(CellProtected** lst);

/* Mallocs a hash cell */
HashCell* create_hashcell(Key* key);

/* 
 *Returns the position of the element with the key 'key'
 * Size: size of the hash table 
*/
int hash_function(Key* key, int size);

/* 
 * Returns the index where the key 'key' is located
 * The hash table resolves conflicts using linear probing
 * If the key doesn't exist yet it returns the first open position
 */
int find_position(HashTable* t, Key* key);

/*
 * Initializes a HashTable of size 'size' from a list of keys 
*/
HashTable* create_hashtable(CellKey* keys, int size);

/*
 * frees HashTable
 * -
 * For each i 
 * Free each key
 * Free hashCell*
 * -
 * Free hashCell**
 * Free HashTable*
*/
void delete_hashtable(HashTable* t);

#endif