#ifndef _RESULT_HANDLER_H_
#define _RESULT_HANDLER_H_

#include "list.h"
#include "blockchain.h"

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

/* 
 * decl: list of declarations with valid signatures
 * candidates: list of candidates' keys
 * voters: list of approved citizens' keys
 * sizeC: size of hashtable containing the vote count for each candidate
 * sizeV: size of hashtable representing the amount of times each voter has voted
 * Returns: pk of the candidate with the most votes
 * since collisions are solved by probing, 
 *  we supposes that sizeC and sizeV are larger than the amount of citizens
 */
Key* compute_winner(CellProtected* decl, CellKey* candidates, CellKey* voters, int sizeC, int sizeV);

/*Same as computer_winner but using a blockchain*/
Key* compute_winner_BT(CellTree* tree, CellKey* candidates, CellKey* voters, int sizeC, int sizeV);

#endif