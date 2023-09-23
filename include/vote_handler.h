#ifndef _VOTE_HANDLER_H_
#define _VOTE_HANDLER_H_

#include "rsa.h"

typedef struct {
    long* content;
    int size;
} Signature;

/* mallocs and initializes a signature using content and size */
Signature* init_signature(long* content, int size);

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

#endif 