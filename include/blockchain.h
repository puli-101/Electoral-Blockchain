#ifndef _BLOCKCHAIN_H_
#define _BLOCKCHAIN_H_
# include <dirent.h>
#include "block.h"

typedef struct block_tree_cell {
    Block* block ;
    struct block_tree_cell * father ;
    struct block_tree_cell * firstChild ;
    struct block_tree_cell * nextBro ;
    int height ;
} CellTree ;

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

/* 
 * Writes a block into pending block.txt from 
 * the status of the current tree, the author 
 * and a certain proof of work size
 *
*/
void create_block(CellTree* tree, Key* author, int d);


/*Add a pending block to the blockchain if it's valid*/
void add_block(int d, char* name);

/*
 * Creates a tree node for each file in blockchain/
 * For each node T[j] in blockchain/ we extract all of T[j]'s children (using add_child)
 * Considering that each of its children has previous_hash set to T[j]'s hash
 * We iterate a final time over T to find the root (node whose parent is null) and we return it
*/
CellTree* read_tree();

void destroy_node(CellTree* node);

void destroy_tree(CellTree* ab) ;

#endif