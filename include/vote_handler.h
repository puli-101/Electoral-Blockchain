#ifndef _VOTE_HANDLER_H_
#define _VOTE_HANDLER_H_

#include "rsa.h"

typedef struct {
    unsigned long* content;
    int size;
} Signature;

typedef struct {
    Key* pKey; //voters public key
    char* mess; //vote declaration
    Signature* sgn; //signature of vote
} Protected;

/* mallocs and initializes a signature using content and size */
Signature* init_signature(unsigned long* content, int size);

/* Creates a signature from a given string message using the secret key sKey*/
Signature* sign(char* mess, Key* sKey);

/*
 * Translates a signature into a string of the following format
 * x1#x2#...#xn
 * where each xi is the ith element of sgn.content
*/
char* signature_to_str(Signature* sgn);

/*
 * Translates a string of the following format
 * x1#x2#...#xn
 * into a signature
*/
Signature* str_to_signature(char* str);

/* Initializes a Protected type */
Protected* init_protected(Key* pKey, char* mess, Signature* sgn);

/* Verifies that the signature corresponds to the message on public key */
int verify(Protected* pr);

//Output format: pKey msg signature
char* protected_to_str(Protected* pr);

//Input format: pKey msg signature
Protected* str_to_protected(char* str);

void free_signature(Signature* sgn);
void free_protected(Protected* pr);

/*Adds a vote declaration to the file pending votes.txt*/
void submit_vote(Protected* p);

#endif 