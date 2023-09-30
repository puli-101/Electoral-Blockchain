#include "blockchain.h"


/*Creates a node of the blockchain tree (the block b isn't recopied)*/
CellTree* create_node(Block* b) {
    test_fatal_error(b, "create_node(b)");
    CellTree* cell = (CellTree*)malloc(sizeof(CellTree));
    test_fatal_error(cell, "create_node(b)");

    cell->height = 0;
    cell->father = NULL;
    cell->firstChild = NULL;
    cell->nextBro = NULL;
    cell->block = b;

    return cell;
}

/* 
 * Updates the height of the father 
 * Returns 1 if the father's height changes
*/
int update_height(CellTree* father, CellTree* child) {
    if (child->height+1 > father->height) {
        father->height = child->height + 1;
        return 1;
    } else {
        return 0;
    }
}

/* Adds a child to the list of children for a certain father
 * Updates the father's height and all the ascending family
*/
void add_child(CellTree* father, CellTree* child) {
    if (father->firstChild == NULL) {
        father->firstChild = child;
        
    } else {
        CellTree* iter = father->firstChild;
        while(iter->nextBro != NULL) {
            iter = iter->nextBro;
        }  
        iter->nextBro = child;
    }
    CellTree* f1 = father, *c1 = child;
    while(update_height(f1,c1)) {
        c1 = f1;
        f1 = f1->father;
    }
}

/* Supprime un noeud de l'arbre */
void delete_node(CellTree* node) {
    delete_block(node->block); //destroy insted??
    free(node);
}

/*
 * Prints the tree ab
 * for each node we print the current hash and height of the block
*/
void print_tree(CellTree* ab) {
    print_tree(ab->nextBro);
    char* str = hash_to_str(ab->block->hash);
    printf("hash: %s\n",str);
    printf("height: %d\n-\n",ab->height);
    free(str);
    print_tree(ab->firstChild);
}

/*Deletes a tree*/
void delete_tree(CellTree* ab) {
    if (ab == NULL) return;
    delete_tree(ab->firstChild);
    delete_tree(ab->nextBro);
    delete_node(ab);
}

/* Returns pointer of the child of the cell with the maximal height*/
CellTree* highest_child(CellTree* cell) {
    if (cell == NULL) return NULL;

    int max = -1;
    CellTree* highest = NULL;
    for (CellTree* iter = cell->firstChild;
        iter; iter = iter->nextBro) {
            if (iter->height > max) {
                max = iter->height;
                highest = iter;
            }
    }
    return highest;
}

/*Returns the hash of the deepest block */
CellTree* last_node(CellTree* cell) {
    if (cell == NULL) return NULL;

    if (cell->firstChild == NULL) {
        return cell;
    }
    return last_node(highest_child(cell));
}

/*Returns the list of all the vote declarations linked to the longest branch*/
CellProtected* list_decl_longest_branch(CellTree* tree) {
    if (tree == NULL) return NULL;

    CellTree* depth = last_node(tree);
    CellProtected* lst = NULL;
    while(depth != NULL) {
        fuse_declarations(&lst, depth->block->votes);
        depth = depth->father;
    }
    return lst;
}

/* 
 * Writes a block into pending block.txt from 
 * the status of the current tree, the author 
 * and a certain proof of work size
 *
*/
void create_block(CellTree* tree, Key* author, int d) {
    Block* valid_block = (Block*)malloc(sizeof(Block));
    test_fatal_error(valid_block,"create_block(tree, author,d)");

    //Extraction of author
    //may have to be deleted
    Key* key = (Key*)malloc(sizeof(Key));
    test_fatal_error(key, "create_block(tree,author,d)");
    init_key(key, author->val, author->n);
    //

    valid_block->author = key; //author
    ////

    //Extraction of the previous hash
    CellTree* last = last_node(tree);

    unsigned char* hash_copy = (unsigned char*) malloc(sizeof(unsigned char)*SHA256_DIGEST_LENGTH);
    test_fatal_error(hash_copy,"create_block(tree,author,d)");
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
        hash_copy[i] = 0;
    if (last != NULL) {
        for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
            hash_copy[i] = last->block->hash[i];
    }
    valid_block->previous_hash = hash_copy;
    ////    

    //Allocation of current hash
    valid_block->hash = (unsigned char*) malloc(sizeof(unsigned char)*SHA256_DIGEST_LENGTH);
    test_fatal_error(valid_block->hash,"create_block(tree,author,d)");

    //Extract pending vote declarations
    //maybe check if pending_votes.txt exists???
    valid_block->votes = read_protected("preprocess/pending_votes.txt");
    system("rm preprocess/pending_votes.txt");

    //nonce
    compute_proof_of_work(valid_block,d);

    save_block_file("preprocess/pending_block.txt",valid_block);

    destroy_block(valid_block);
}

/*Add a pending block to the blockchain if it's valid*/
void add_block(int d, char* name) {
    Block* b = read_block_file("preprocess/pending_block.txt");

    if (verify_block(b,d)) {
        char buffer[500];
        sprintf(buffer,"blockchain/%s",name);
        save_block_file(buffer,b);
    }

    system("rm preprocess/pending_block.txt");
    destroy_block(b);
}

/* Compares if two hashes are equal (without using strings)*/
int cmp_hash(unsigned char* h1, unsigned char* h2) {
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        if (h1[i] != h2[i])
            return 0;
    }
    return 1;
}

/*
 * Creates a tree node for each file in blockchain/
 * For each node T[j] in blockchain/ we extract all of T[j]'s children (using add_child)
 * Considering that each of its children has previous_hash set to T[j]'s hash
 * We iterate a final time over T to find the root (node whose parent is null) and we return it
*/
CellTree* read_tree() {
    int c = 0;
    CellTree** T = (CellTree**)malloc(sizeof(CellTree*)*500);
    test_fatal_error(T,"read_tree()");
    CellTree* root = NULL;
    char buffer[500];

    DIR * rep = opendir ("./blockchain/" ) ;
    if ( rep != NULL ) {
        struct dirent * dir ;
        while (( dir = readdir ( rep ) ) ) {
            if ( strcmp ( dir-> d_name , "." ) !=0 && strcmp ( dir -> d_name , ".." ) !=0) {
                //for each file we extract the block and add it to the block array
                sprintf(buffer,"blockchain/%s",dir->d_name);
                T[c] = (CellTree*)malloc(sizeof(CellTree));
                test_fatal_error(T[c],"read_tree() : T[c]");
                T[c]->block = read_block_file(buffer);
                T[c]->father = T[c]->firstChild = T[c]->nextBro = NULL;
                c++;
                //avoid overflow
                if (c%500 == 0) {
                    T = (CellTree**)realloc(T,c+500);
                    test_fatal_error(T,"read_tree()");
                }
            }
        }
        closedir ( rep ) ;
    }

    //set parental relationships
    for (int i = 0; i < c; i++) {
        for (int j = 0; j < c; j++) {
            if (i == j) continue;
            if (cmp_hash(T[i]->block->hash, T[j]->block->previous_hash)) {
                T[j]->father = T[i];
                add_child(T[i],T[j]);
            }
        }
    }

    //set root
    for (int i = 0; i < c; i++) {
        if (T[i]->father == NULL)
            root = T[i];
    }

    free(T);
    return root;
}