#ifndef _VOTE_HANDLER_H_
#define _VOTE_HANDLER_H_

#include "rsa.h"

typedef struct {
    long* tab;
    int size;
} Signature;

/* mallocs and initializes a signature using content and size */
Signature* init_signature(long* content, int size);

/* Creates a signature from a given string message using the secret key sKey*/
Signature* sign(char* mess, Key* sKey);

#endif 