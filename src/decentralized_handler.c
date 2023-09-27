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
        hash[j] = strtol(buffer, NULL, 16); 
        j++;
    }
    hash[j] = '\0';
    return hash;
}

char* hash_to_str(unsigned char* hash) {
    char* str = (char*)malloc(sizeof(char*)*(SHA256_DIGEST_LENGTH+1));
    test_fatal_error(hash,"str_to_hash(str)");
    test_fatal_error(str,"str_to_hash(str)");
    char buffer[5];
    for (int i = 0; i < SHA256_DIGEST_LENGTH ; i++) {
        sprintf(buffer,"%02x" , hash[i]);
        strcat(str,buffer);
    }
    return str;
}

/* Writes block 'bl' into a file named 'name' */
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
    fprintf(f,"%s\n",hash);
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

/* Reads a block from a file named 'name' */
Block* read_block_file(const char* name) {
    Block* bl = (Block*)malloc(sizeof(Block));
    test_fatal_error(bl,"read_block_file(name)");
    FILE *f = fopen(name, "r");
    test_fatal_error(f,"read_block_file(name)");
    char buffer[500];
    //pkey author
    fgets(buffer, 500, f);
    bl->author = str_to_key(buffer);
    fgets(buffer, 500, f);
    //hash
    bl->hash = hashstr_to_hash(buffer);
    fgets(buffer, 500, f);
    //previous hash
    bl->previous_hash = hashstr_to_hash(buffer);
    fgets(buffer, 500, f);
    //proof of work
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

/* Similar to save block file */
char* block_to_str(Block* bl) {
    char* str = (char*)malloc(sizeof(char)*1000);
    test_fatal_error(str,"block_to_str(bl)");
    test_fatal_error(bl,"save_block_file(name,bl)");
    str[0] = '\0';
    char buffer[50];
    //pkey author
    char* str_key = key_to_str(bl->author);
    strcat(str,str_key);
    strcat(str,"\n");
    //hash
    char* str_hash = hash_to_str(bl->hash);
    strcat(str,str_hash);
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
    free(str_hash);
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
    return SHA256((unsigned char*)str, strlen(str),0);
}

