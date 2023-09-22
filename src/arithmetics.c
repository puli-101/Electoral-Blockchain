#include "arithmetics.h"

/* Naive implementation in O(sqrt(n))*/
int is_sprime_naive(long p) {
    for (long i = 2; i < sqrt(p) + 1; i++) {
        if (p%i == 0)
            return 0;
    }
    return 1;
}

/* Naive implementation of modular exponentiation*/
long modpow_naive(long a, long m, long n) {
    long res = 1;
    for (int i = 0; i < m; i++) {
        res *= a;
        res %= n;
    }
    return res;
}

/* Fast exponentiation in O(log2(m)) */
int modpow(long a, long m, long n) {
    long b;
    long r;

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
int witness (long a, long b, long d, long p) {
    long x = modpow (a, d, p) ;
    if (x == 1) {
        return 0;
    }
    for ( long i = 0; i < b ; i++) {
        if (x == p-1) {
            return 0;
        }
        x = modpow (x ,2 , p ) ;
    }
    return 1;
}

/*generation of random number*/
long rand_long (long low, long up) {
    return rand() % (up - low +1) + low ;
}

/*Miller Rabin primality test*/
int is_prime_miller (long p , int k) {
    if ( p == 2) {
        return 1;
    }
    if (!( p & 1) || p <= 1) { //on verifie que p est impair et different de 1
        return 0;
    }
    //on determine b et d :
    long b = 0;
    long d = p - 1;
    while (!( d & 1) ) { //tant que d n’est pas impair
        d = d /2;
        b = b +1;
    }
    // On genere k valeurs pour a, et on teste si c’est un temoin :
    long a ;
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
 * low_size: lower bound for the expected prime (bitsize? au plus 7 bits)
 * up_size: upper bound for expected prime
 * k: number of miller rabin tests
*/
long random_prime_number(int low_size, int up_size, int k) {
    long n;
    do {
        n = rand_long(low_size,up_size);
    } while(!is_prime_miller(n,k));
    return n;
}
