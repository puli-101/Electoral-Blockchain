#include "arithmetics.h"

/* Naive implementation in O(sqrt(n))*/
int is_sprime_naive(ul p) {
    for (ul i = 2; i < sqrt(p) + 1; i++) {
        if (p%i == 0)
            return 0;
    }
    return 1;
}

/* Naive implementation of modular exponentiation*/
ul modpow_naive(ul a, ul m, ul n) {
    ul res = 1;
    for (int i = 0; i < m; i++) {
        res *= a;
        res %= n;
    }
    return res;
}

/* Fast exponentiation in O(log2(m)) */
ul modpow(ul a, ul m, ul n) {
    ul b;
    ul r;

    if (m == 0) {
        r = 1;
    } else {
        b = modpow(a, m/2, n) % n;
        r = b * b;
        if (m % 2 == 1)
            r *= a;
    }  
    return r%n;
}

/* este si a est un temoin de Miller pour p, pour un entier a donne. */
int witness (ul a, ul b, ul d, ul p) {
    ul x = modpow (a, d, p) ;
    if (x == 1) {
        return 0;
    }
    for ( ul i = 0; i < b ; i++) {
        if (x == p-1) {
            return 0;
        }
        x = modpow (x ,2 , p ) ;
    }
    return 1;
}

/*generation of random number*/
ul rand_long (ul low, ul up) {
    return rand() % (up - low +1) + low ;
}

/*Miller Rabin primality test*/
int is_prime_miller (ul p , int k) {
    if ( p == 2) {
        return 1;
    }
    if (!( p & 1) || p <= 1) { //on verifie que p est impair et different de 1
        return 0;
    }
    //on determine b et d :
    ul b = 0;
    ul d = p - 1;
    while (!( d & 1) ) { //tant que d n’est pas impair
        d = d /2;
        b = b +1;
    }
    // On genere k valeurs pour a, et on teste si c’est un temoin :
    ul a ;
    int i ;
    for ( i = 0; i < k ; i ++) {
        a = rand_long (2 , p -1) ;
        if ( witness (a ,b ,d , p ) ) {
            return 0;
        }
    }
    return 1;
}

/*
 * Random prime number generator
 * low_size: lower bitsize bound for the expected prime 
 * up_size: upper bitsize bound for expected prime (at most 7)
 * k: number of miller rabin tests
*/
ul random_prime_number(int low_size, int up_size, int k) {
    ul n;
    ul low = 1 << low_size;
    ul up = 1 << up_size;
    do {
        n = rand_long(low, up);
    } while(!is_prime_miller(n,k));
    return n;
}

/* Euclid's gcd, returns gcd between s and t and (u,v) Bezout's coefficients */
ul extended_gcd(ul s, ul t, ul* u, ul* v) {
    if (s == 0) {
        *u = 0;
        *v = 1;
        return t ;
    }
    ul uPrim , vPrim ;
    ul gcd = extended_gcd (t %s , s , & uPrim , & vPrim ) ;
    *u = vPrim -( t / s ) * uPrim ;
    *v = uPrim ;
    return gcd ;
}

