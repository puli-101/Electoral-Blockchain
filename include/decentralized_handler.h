#ifndef _DECENTRALIZED_HANDLER_H_
#define _DECENTRALIZED_HANDLER_H_

#include "list.h"
#include "vote_handler.h"
#include <openssl/sha.h>

typedef struct block {
    Key * author ; //pk of creator
    CellProtected * votes ; //list of vote decl.
    unsigned char * hash ; //hash value of block
    int nonce ; //proof of work
    unsigned char * previous_hash ; //hash of prev block
} Block ;

/* Writes block 'bl' into a file named 'name' */
void save_block_file(const char* name, Block* bl);

/* Reads a block from a file named 'name' */
Block* read_block_file(const char* name);

/* Similar to save block file */
char* block_to_str(Block* bl);

/* 
 * Calculates the hash value of a string hash 
 * and saves it into a new (unsigned char*) tab
*/
unsigned char* str_to_hash(char* str);

/* Converts a hash into a string */
char* hash_to_str(unsigned char* hash);

/* 
 * char* str containing an already calculated hash to hash (unsigned char*)
 * Each hash has 32 groups of 2 digit hexadec numbers
 * strlen(str) = 32
*/
unsigned char* hashstr_to_hash(char* str);

/* Brute force proof of work algorithm, the hash of the block B has to begin by d 0's*/
void compute_proof_of_work(Block *B, int d);

/* Frees a block b */
void delete_block(Block* b);

/* Checks if the hash of the block b starts with d zeroes */
int verify_block(Block* b, int d);

/* Frees a block b and all of its fields*/
void destroy_block(Block* b);

#endif