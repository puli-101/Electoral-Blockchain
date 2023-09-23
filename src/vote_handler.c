#include "vote_handler.h"

/* mallocs and initializes a signature using content and size */
Signature* init_signature(long* content, int size) {
    Signature* sig = (Signature*)malloc(sizeof(Signature));
    if (sig == NULL) {
        fprintf(stderr,"Fatal Error: init_signature\n");
        exit(-1);
    }
    sig->tab = (long*)malloc(sizeof(long)*size);
    sig->size = size;

    //memset
    for (int i = 0; i < size; i++) {
        sig->tab[i] = content[i];
    }

    return sig;
}

/* Creates a signature from a given string message using the secret key sKey*/
Signature* sign(char* mess, Key* sKey) {
    int size = strlen(mess);
    long* tab = encrypt(mess,sKey->val, sKey->n);
    Signature* s = init_signature(tab, size);
    free(tab);

    return s;
}