#include "vote_handler.h"

/* mallocs and initializes a signature using content and size */
Signature* init_signature(unsigned long* content, int size) {
    Signature* sig = (Signature*)malloc(sizeof(Signature));
    if (sig == NULL) {
        fprintf(stderr,"Fatal Error: init_signature\n");
        exit(-1);
    }
    sig->content = (unsigned long*)malloc(sizeof(unsigned long)*size);
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
    unsigned long* tab = encrypt(mess,sKey->val, sKey->n);
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
    unsigned long * content = (unsigned long *) malloc ( sizeof (unsigned long) * len ) ;
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
    Signature* sgn = init_signature ( content , num );
    free(content);
    return sgn;
}

/* Initializes a Protected type */
Protected* init_protected(Key* pKey, char* mess, Signature* sgn) {
    Protected* prot = (Protected*)malloc(sizeof(Protected));
    if (prot == NULL) {
        fprintf(stderr,"Fatal Error: init_protected(pkey,%s,sgn)\n",mess);
        exit(-1);
    }
    prot->pKey = (Key*)malloc(sizeof(Key));
    if (prot->pKey == NULL) {
        fprintf(stderr,"Fatal Error: init_protected(pkey,%s,sgn)\n",mess);
        exit(-1);
    }
    prot->pKey->val = pKey->val;
    prot->pKey->n = pKey->n;
    prot->mess = strdup(mess); 
    prot->sgn = init_signature(sgn->content,sgn->size);
    return prot;
}

/* Verifies that the signature corresponds to the message on public key */
int verify(Protected* pr) {
    if (pr == NULL || pr->sgn == NULL || pr->pKey == NULL)
        return 0;
    int res;
    char* verif = decrypt(pr->sgn->content, pr->sgn->size, 
        pr->pKey->val, pr->pKey->n);
    
    res = strcmp(verif, pr->mess);
    if (res) {
        printf("!! %s (recalc from sgn)\n!! %s (original mess)\n",verif,pr->mess);
        printf("Info on sgn:\n");
        printf("Size : %d\n",pr->sgn->size);
        printf("Content : ");
        for (int i = 0; i < pr->sgn->size; i++) {
            printf("%lx ", pr->sgn->content[i]);
        }
        printf("\n");
    }
    free(verif);
    return !res;
}

//Output format: pKey msg signature
char* protected_to_str(Protected* pr) {
    char* str = (char*)malloc(sizeof(char)*500);
    if (str == NULL) {
        fprintf(stderr,"Fatal Error: protected_to_str(pr)\n");
        exit(-1);
    }
    str[0] = '\0';
    if (pr == NULL || pr->sgn == NULL || pr->pKey == NULL || pr->mess == NULL)
        return str;

    char* key = key_to_str(pr->pKey);
    char* sign = signature_to_str(pr->sgn);

    sprintf(str,"%s %s %s",key,pr->mess,sign);

    free(key);
    free(sign);
    return str;
}

//Input format: pKey msg signature
Protected* str_to_protected(char* str) {
    Protected* pr = (Protected*)malloc(sizeof(Protected));
    if (str == NULL) {
        fprintf(stderr,"Fatal Error: str_to_protected(str)\n");
        exit(-1);
    }

    char pt[3][300];
    int index = 0, i;
    int n = strlen(str);

    //splitting into 3 parts
    for (int q = 0; q < 3; q++) {
        for (i = 0; index <  n && str[index] != ' '; i++, index++) {
            pt[q][i] = str[index];
        }
        index++;
        pt[q][i] = '\0';
    }

    pr->pKey = str_to_key(pt[0]);
    pr->mess = strdup(pt[1]);
    pr->sgn = str_to_signature(pt[2]);

    return pr;
}

void free_signature(Signature* sgn) {
    free(sgn->content);
    free(sgn);
}

void free_protected(Protected* pr) {
    free_signature(pr->sgn);
    free(pr->mess);
    free(pr->pKey);
    free(pr);
}