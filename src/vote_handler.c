#include "vote_handler.h"

/* mallocs and initializes a signature using content and size */
Signature* init_signature(long* content, int size) {
    Signature* sig = (Signature*)malloc(sizeof(Signature));
    if (sig == NULL) {
        fprintf(stderr,"Fatal Error: init_signature\n");
        exit(-1);
    }
    sig->content = (long*)malloc(sizeof(long)*size);
    sig->size = size;

    //memset
    for (int i = 0; i < size; i++) {
        sig->content[i] = content[i];
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

/*
 * Translates a signature into a string of the following format
 * x1#x2#...#xn
 * where each xi is the ith element of sgn.content
*/
char* signature_to_str(Signature* sgn) {
    char* result = malloc (10* sgn -> size * sizeof ( char ) ) ;
    result [0]= '#' ;
    int pos = 1;
    char buffer [156];
    for ( int i =0; i < sgn -> size ; i ++) {
        sprintf ( buffer , "%lx" , sgn -> content [ i ]) ;
        for ( int j =0; j < strlen ( buffer ) ; j ++) {
            result [ pos ] = buffer [ j ];
            pos = pos +1;
        }
        result [ pos ] = '#';
        pos = pos +1;
    }
    result [ pos ] = '\0';
    result = realloc ( result , ( pos +1) * sizeof ( char ) ) ;
    return result ;
}

/*
 * Translates a string of the following format
 * x1#x2#...#xn
 * into a signature
*/
Signature* str_to_signature(char* str) {
    int len = strlen ( str ) ;
    long * content = ( long *) malloc ( sizeof ( long ) * len ) ;
    int num = 0;
    char buffer [256];
    int pos = 0;
    for ( int i =0; i < len ; i ++) {
        if ( str [ i ] != '#' ) {
            buffer [ pos ] = str [ i ];
            pos = pos +1;
        } else {
            if ( pos != 0) {
                buffer [ pos ] = '\0' ;
                sscanf ( buffer , "%lx" , &( content [ num ]) ) ;
                num = num + 1;
                pos = 0;
            }
        }
    }
    content = realloc ( content , num * sizeof ( long ) ) ;
    return init_signature ( content , num ) ;
}

