#ifndef _ARITHMETICS_H_
#define _ARITHMETICS_H_

#include <math.h>
#include <stdlib.h>
#include <time.h>

/* Naive implementation in O(sqrt(n))*/
int is_sprime_naive(long p);

/* Naive implementation of modular exponentiation*/
long modpow_naive(long a, long m, long n);

/* Fast exponentiation in O(log2(m)) */
int modpow(long a, long m, long n);

/* este si a est un temoin de Miller pour p, pour un entier a donne. */
int witness (long a, long b, long d, long p);

/*generation of random number*/
long rand_long (long low, long up);

/*Miller Rabin primality test*/
int is_prime_miller (long p , int k);

/*
 * Random prime number generator
 * low_size: lower bound for the expected prime
 * up_size: upper bound for expected prime
 * k: number of miller rabin tests
*/
long random_prime_number(int low_size, int up_size, int k);

/* Euclid's gcd, returns gcd between s and t and (u,v) Bezout's coefficients */
long extended_gcd(long s, long t, long* u, long* v);

#endif