#ifndef _ARITHMETICS_H_
#define _ARITHMETICS_H_

#include <math.h>
#include <stdlib.h>
#include <time.h>

typedef unsigned long ul;

/* Naive implementation in O(sqrt(n))*/
int is_sprime_naive(ul p);

/* Naive implementation of modular exponentiation*/
ul modpow_naive(ul a, ul m, ul n);

/* Fast exponentiation in O(log2(m)) */
ul modpow(ul a, ul m, ul n);

/* este si a est un temoin de Miller pour p, pour un entier a donne. */
int witness (ul a, ul b, ul d, ul p);

/*generation of random number*/
ul rand_long(ul low, ul up);

/*Miller Rabin primality test*/
int is_prime_miller (ul p , int k);

/*
 * Random prime number generator
 * low_size: lower bound for the expected prime
 * up_size: upper bound for expected prime
 * k: number of miller rabin tests
*/
ul random_prime_number(int low_size, int up_size, int k);

/* Euclid's gcd, returns gcd between s and t and (u,v) Bezout's coefficients */
ul extended_gcd(ul s, ul t, ul* u, ul* v);

#endif