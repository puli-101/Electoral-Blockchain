#ifndef _LIST_H_
#define _LIST_H_
#include "vote_handler.h"

//Linked lists of type key and protected

/* Results will first be saved as a linked list */
typedef struct cellKey {
    Key * data ;
    struct cellKey * next ;
} CellKey;

typedef struct cellProtected {
    Protected * data ;
    struct cellProtected * next ;
} CellProtected;

/* Mallocs and initializes a linked list */
CellKey* create_cell_key(Key* key);

/* Inserts element as head of the list */
void insert_head_key(CellKey** lst, Key* key);

/* 
 * Reads file keys.txt or candidates.txt 
 * returns a linked list of the public keys
*/
CellKey* read_public_keys(char *option);

/* Prints a list of keys */
void print_list_keys(CellKey* LCK);

/* Deletes a single cell of a key linked list */
void delete_cell_key(CellKey* c);

/* Deletes a key linked list*/
void delete_cell_keys(CellKey* c);

/* Creates and initializes a cell of type protected*/
CellProtected* create_cell_protected(Protected* pr);

/* Inserts element as head of the list */
void insert_head_protected(CellProtected** lst, Protected* key);

/* 
 * Reads file declarations.txt
 * returns a linked list of the public keys
*/
CellProtected* read_protected();

/* Prints a list of protecteds */
void print_list_protected(CellProtected* LCK);

/* Deletes a single cell of a protected linked list */
void delete_cell_protected(CellProtected* c);

/* Deletes a protected linked list*/
void delete_list_protected(CellProtected* c);

/* Checks if obj was alloced properly */
void test_fatal_error(void* obj, char* msg);

/*Concatenates l2 at the end of l1: */
void fuse_declarations(CellProtected* l1, CellProtected* l2);

#endif