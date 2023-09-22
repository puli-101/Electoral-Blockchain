#ifndef _RSA_H_
#define _RSA_H_ 

#include "arithmetics.h"
#include <string.h>
#include <stdio.h>

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

#endif