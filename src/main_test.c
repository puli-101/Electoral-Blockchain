#include <stdio.h>
#include "arithmetics.h"

int main() {
    for (int i = 1; i < 100; i++) {
        for (int j = 1; j < 100; j++) {
            for (int k = 2; k < 100; k++) {
                long a = modpow_naive(i,j,k);
                long b = modpow(i,j,k);
                if (a != b) {
                    fprintf(stderr,"Error : %ld != %ld (%d,%d,%d)\n",a,b,i,j,k);
                    exit(-1);
                }
            }
        }
    }
    for (int i = 0; i < 100; i++) {
        printf("is_prime %d %d\n", i, is_prime_miller(i,20));
    }
    return 0;
}