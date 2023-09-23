#ifndef _RSA_H_
#define _RSA_H_ 

#include "arithmetics.h"
#include <string.h>
#include <stdio.h>

typedef struct {
    unsigned long val;
    unsigned long n;
} Key;

/*
 * Permet de generer la cle publique pkey = (s, n) 
 * et la cle secrete skey = (u, n), a partir des nombres premiers 
 * p et q, en suivant le protocole RSA
*/
void generate_key_values(long p, long q, long* n, long *s, long *u); 

/* 
 * Encrypts the string chaine with the public key (s,n) 
 * The function converts each character into an int 
 * Returns a long array obtained by encrypting each character
*/
long* encrypt(char* chaine, long s, long n);

/* 
 * Decrypts a message using the secret key (u,n), knowing beforehand the size
 * of the integer array
 * This function returns a string corresponding to the original message
*/
char* decrypt(long* crypted, int size, long u, long n);

/* Initializes an already malloced */
void init_key(Key* key, long val, long n);

/* Initializes an already malloced pk and sk*/
void init_pair_keys(Key* pKey, Key* sKey, long low_size, long up_size);

/*translates a Key type to a string with the format (x,y) ; x,y in hexadecimal*/
char* key_to_str(Key* key);

/*translates a string with the format (x,y) in hexadecimal into a Key*/
Key* str_to_key(char* str);

#endif