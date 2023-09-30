#include "decentralized_handler.h"

/* 
 * String (char*) containing an already calculated hash to hash (unsigned char*)
 * Each hash has 32 groups of 2 digit hexadec numbers
 * strlen(str) = 32
*/
unsigned char* hashstr_to_hash(char* str) {
    unsigned char* hash = (unsigned char*)malloc(sizeof(unsigned char*)
        *(SHA256_DIGEST_LENGTH+1));
    test_fatal_error(hash,"str_to_hash(str)");
    test_fatal_error(str,"str_to_hash(str)");
    char buffer[3];
    int j = 0;
    for(int i = 0; i < strlen(str); i+=2) {
        if (str[i] == '\n') continue;
        buffer[0] = str[i];
        buffer[1] = str[i+1];
        buffer[2] = '\0';
        //translates the characters into a base16 number
        hash[j] = strtol(buffer, NULL, 16); 
        j++;
    }
    hash[j] = '\0';
    return hash;
}

/* Converts a hash into a string */
char* hash_to_str(unsigned char* hash) {
    char* str = (char*)malloc(sizeof(char*)*(65));
    test_fatal_error(hash,"str_to_hash(str)");
    test_fatal_error(str,"str_to_hash(str)");
    str[0] = '\0';
    char buffer[5];
    for (int i = 0; i < SHA256_DIGEST_LENGTH ; i++) {
        sprintf(buffer,"%02x" , hash[i]);
        strcat(str,buffer);
    }
    return str;
}

/* 
 * Writes block 'bl' into a file named 'name'
 * Format: 
 * - Author
 * - hash
 * - previous hash
 * - nonce
 * - list of declarations
*/
void save_block_file(const char* name, Block* bl) {
    FILE *f = fopen(name, "w");
    test_fatal_error(f,"save_block_file(name,bl)");
    test_fatal_error(bl,"save_block_file(name,bl)");
    char* str_key = key_to_str(bl->author);
    fprintf(f,"%s\n",str_key);
    //casting hash->string
    char* hash = hash_to_str(bl->hash);
    fprintf(f,"%s\n",hash);
    char* previous_hash = hash_to_str(bl->previous_hash);
    fprintf(f,"%s\n",previous_hash);
    //proof of work
    fprintf(f,"%d\n",bl->nonce);
    //list of declarations
    for (CellProtected* iter = bl->votes; iter; iter = iter->next) {
        char* str_pr = protected_to_str(iter->data);
        fprintf(f,"%s\n",str_pr);
        free(str_pr);
    }
    free(str_key);
    free(hash);
    free(previous_hash);
    fclose(f);
}

/* 
 * Reads a block from a file named 'name'
 * Format: 
 * - Author
 * - hash
 * - previous hash
 * - nonce
 * - list of declarations
*/
Block* read_block_file(const char* name) {
    Block* bl = (Block*)malloc(sizeof(Block));
    test_fatal_error(bl,"read_block_file(name)");
    FILE *f = fopen(name, "r");
    test_fatal_error(f,"read_block_file(name)");
    char buffer[500];
    //pkey author
    fgets(buffer, 500, f);
    bl->author = str_to_key(buffer);
    //hash
    fgets(buffer, 500, f);
    bl->hash = hashstr_to_hash(buffer);
    //previous hash
    fgets(buffer, 500, f);
    bl->previous_hash = hashstr_to_hash(buffer);
    //proof of work
    fgets(buffer, 500, f);
    bl->nonce = atoi(buffer);
    //list of declarations
    bl->votes = NULL;
    //copy each protected
    while(fgets(buffer,500,f)) {
        Protected* pr = str_to_protected(buffer);
        insert_head_protected(&(bl->votes), pr);
        free_protected(pr);
    }
    fclose(f);
    return bl;
}

/* Similar to save block file except it doesnt include current hash */
char* block_to_str(Block* bl) {
    char* str = (char*)malloc(sizeof(char)*1500);
    test_fatal_error(str,"block_to_str(bl)");
    test_fatal_error(bl,"save_block_file(name,bl)");
    str[0] = '\0';
    char buffer[50];
    //pkey author
    char* str_key = key_to_str(bl->author);
    strcat(str,str_key);
    strcat(str,"\n");
    //previous hash
    char* str_prev_hash = hash_to_str(bl->previous_hash);
    strcat(str,str_prev_hash);
    strcat(str,"\n");
    //proof of work
    sprintf(buffer,"%d",bl->nonce);
    strcat(str,buffer); //nonce
    strcat(str,"\n");
    //list of declarations
    for (CellProtected* iter = bl->votes; iter; iter = iter->next) {
        char* str_pr = protected_to_str(iter->data);
        strcat(str,str_pr);
        strcat(str,"\n");
        free(str_pr);
    }
    free(str_prev_hash);
    free(str_key);
    return str;
}

/* 
 * Calculates the hash value of a string hash 
 * and saves it into a new (unsigned char*) tab
*/
unsigned char* str_to_hash(char* str) {
    test_fatal_error(str,"str_to_hash(str)");
    //replace SHA256 (it takes too much space in DATA segment while doing proof of work)
    unsigned char* s_hash = SHA256((unsigned char*)str, strlen(str),0);
    unsigned char* d_hash = (unsigned char*)malloc(sizeof(unsigned char*)*SHA256_DIGEST_LENGTH);
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
        d_hash[i] = s_hash[i];
    return d_hash;
}

/* 
 * Brute force proof of work algorithm, the hash of the block B has to begin by d 0's
 * (or 4d zeroes in the binary representation since the hash is in hexadecimal)
*/
void compute_proof_of_work(Block *B, int d) {
    B->nonce = 0;
    int valid = 0;
    do {
        //we calc the hash of the block (very slow)
        char* str = block_to_str(B);
        unsigned char* hash = str_to_hash(str);
        char* str_hash = hash_to_str(hash);
        valid = 1;
        //check if we match a prefix of d zeroes
        for (int i = 0; i < d; i++) {
            if (str_hash[i] != '0') {
                valid = 0;
            }
        }
        if (!valid) {
            (B->nonce)++;
            free(hash);
        } else {
            free(B->hash);
            B->hash = hash;
        }
        free(str);
        free(str_hash);
    } while(!valid && ((B->nonce) > 0)); //prevent overflow
}

/* Checks if the hash of the block b starts with d zeroes */
int verify_block(Block* b, int d) {
    //we calc the hash of the block
    char* str = block_to_str(b);
    unsigned char* hash = str_to_hash(str);
    char* str_hash = hash_to_str(hash);
    int valid = 1;
    //check if we match a prefix of d zeroes
    for (int i = 0; i < d; i++) {
        if (str_hash[i] != '0') {
            valid = 0;
        }
    }
    free(hash);
    free(str);
    free(str_hash);
    return valid;
}

/* Frees a block b */
void delete_block(Block* b) {
    //DO NOT FREE author NOR data FIELD OF EACH CELLPROTECTED* VOTES
    free(b->hash);
    free(b->previous_hash);
    CellProtected* tmp;
    while(b->votes != NULL) {
        tmp = b->votes->next;
        free(b->votes);
        b->votes = tmp;
    }
    free(b);
}

/* Frees a block b and all of its fields*/
void destroy_block(Block* b) {
    free(b->hash);
    free(b->previous_hash);
    free(b->author);
    delete_list_protected(b->votes);
    free(b);
}

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

/*Adds a vote declaration to the file pending votes.txt*/
void submit_vote(Protected* p) {
    FILE* f = fopen("blockchain/pending_votes.txt", "a");
    char* str = protected_to_str(p);
    fprintf(f,"%s\n",str);
    free(str);
    fclose(f);
}

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
    valid_block->votes = read_protected("blockchain/pending_votes.txt");
    system("rm blockchain/pending_votes.txt ; touch blockchain/pending_votes.txt");

    //nonce
    compute_proof_of_work(valid_block,d);

    save_block_file("blockchain/pending_block.txt",valid_block);

    destroy_block(valid_block);
}