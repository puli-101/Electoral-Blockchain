#ifndef _RSA_H_
#define _RSA_H_ 

#include "arithmetics.h"
#include <string.h>
#include <stdio.h>

typedef struct {
    ul val;
    ul n;
} Key;

/*
 * Permet de generer la cle publique pkey = (s, n) 
 * et la cle secrete skey = (u, n), a partir des nombres premiers 
 * p et q, en suivant le protocole RSA
*/
void generate_key_values(ul p, ul q, ul* n, ul *s, ul *u); 

/* 
 * Encrypts the string chaine with the public key (s,n) 
 * The function converts each character into an int 
 * Returns a ul array obtained by encrypting each character
*/
ul* encrypt(char* chaine, ul s, ul n);

/* 
 * Decrypts a message using the secret key (u,n), knowing beforehand the size
 * of the integer array
 * This function returns a string corresponding to the original message
*/
char* decrypt(ul* crypted, int size, ul u, ul n);

/* Initializes an already malloced key */
void init_key(Key* key, ul val, ul n);

/* Initializes an already malloced pk and sk*/
void init_pair_keys(Key* pKey, Key* sKey, ul low_size, ul up_size);

/*translates a Key type to a string with the format (x,y) ; x,y in hexadecimal*/
char* key_to_str(Key* key);

/*translates a string with the format (x,y) in hexadecimal into a Key*/
Key* str_to_key(char* str);

#endif