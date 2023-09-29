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

typedef struct block_tree_cell {
    Block * block ;
    struct block_tree_cell * father ;
    struct block_tree_cell * firstChild ;
    struct block_tree_cell * nextBro ;
    int height ;
} CellTree ;

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

/*Creates a node of the blockchain tree (the block b isn't recopied)*/
CellTree* create_node(Block* b);

/* 
 * Updates the height of the father 
 * Returns 1 if the father's height changes
*/
int update_height(CellTree* father, CellTree* child);

/* Adds a child to the list of children for a certain father
 * Updates the father's height and all the ascending family
*/
void add_child(CellTree* father, CellTree* child);

/* Supprime un noeud de l'arbre */
void delete_node(CellTree* node);

/*
 * Prints the tree ab
 * for each node we print the current hash and height of the block
*/
void print_tree(CellTree* ab);

/*Deletes a tree*/
void delete_tree(CellTree* ab);

/* Returns pointer of the child of the cell with the maximal height*/
CellTree* highest_child(CellTree* cell);

/*Returns the hash of the deepest block */
CellTree* last_node(CellTree* cell);

/*Returns the list of all the vote declarations linked to the longest branch*/
CellProtected* list_decl_longest_branch(CellTree* tree);

#endif