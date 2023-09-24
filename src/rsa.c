#include "rsa.h"

/*
 * Permet de generer la cle publique pkey = (s, n) 
 * et la cle secrete skey = (u, n), a partir des nombres premiers 
 * p et q, en suivant le protocole RSA
*/
void generate_key_values(ul p, ul q, ul* n, ul *s, ul *u) {
    *n = p*q;
    ul t = (p-1)*(q-1);
    ul new_s = 3;
    ul filler;
    ul pgcd = extended_gcd(new_s,t,u,&filler);

    //if new_s isnt invertible mod t then we try another random num
    while (pgcd != 1) {
        new_s = rand_long(3,t-1);
        pgcd = extended_gcd(new_s,t,u,&filler);
    }

    *s = new_s;
}

/* 
 * Encrypts the string chaine with the public key (s,n) 
 * The function converts each character into an int 
 * Returns a ul array obtained by encrypting each character
*/
ul* encrypt(char* chaine, ul s, ul n) {
    int size = strlen(chaine);
    ul* tab = (ul*)malloc(sizeof(ul) * size);
    if (tab == NULL) {
        fprintf(stderr,"Fatal Error: malloc encrypt(%s,%ld,%ld)\n",chaine,s,n);
        exit(-1);
    }

    for (int i = 0; i < size; i++) {
        tab[i] = modpow(chaine[i],s,n);
        //printf("!%lx",tab[i]);
    }
    
    return tab;
}

/* 
 * Decrypts a message using the secret key (u,n), knowing beforehand the size
 * of the integer array
 * This function returns a string corresponding to the original message
*/
char* decrypt(ul* crypted, int size, ul u, ul n) {
    char* str = (char*)malloc(sizeof(char)*(size+1));
    if (str == NULL) {
        fprintf(stderr,"Fatal Error: malloc decrypt(crypted,size,u,n)\n");
        exit(-1);
    }
    str[size] = '\0';

    for (int i = 0; i < size; i++) {
        str[i] = modpow(crypted[i],u,n);
    }
    return str;
}

/* Initializes an already malloced */
void init_key(Key* key, ul val, ul n) {
    key->val = val;
    key->n = n;
}

/* Initializes an already malloced pk and sk*/
void init_pair_keys(Key* pKey, Key* sKey, ul low_size, ul up_size) {
    ul p = random_prime_number(low_size, up_size, 100) ;
    ul q = random_prime_number(low_size, up_size, 100) ;
    while (p == q) {
        q = random_prime_number(low_size, up_size, 100) ;
    }

    ul n, s, u;
    generate_key_values(p, q, &n, &s, &u);

    pKey->val = s;
    pKey->n = n;

    sKey->val = u;
    sKey->n = n; 
}

/*translates a Key type to a string with the format (x,y) ; x,y in hexadecimal*/
char* key_to_str(Key* key) {
    char* str = (char*)malloc(sizeof(char)*100);
    if (str == NULL) {
        fprintf(stderr,"Fatal Error: key_to_str\n");
        exit(-1);
    }

    sprintf(str,"(%lx,%lx)",key->val,key->n);
    return str;
}

/*translates a string with the format (x,y) in hexadecimal into a Key*/
Key* str_to_key(char* str) {
    Key* k = (Key*)malloc(sizeof(Key));
    ul val,n; 
    if (k == NULL) {
        fprintf(stderr,"Fatal Error: str_to_key(%s)\n",str);
        exit(-1);
    }

    sscanf(str, " ( %lx , %lx ) ", &val, &n);
    k->val = val;
    k->n = n;
    return k;
}