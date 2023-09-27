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

#endif