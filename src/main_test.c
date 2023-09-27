#include <stdio.h>
#include "arithmetics.h"
#include "rsa.h"
#include "vote_handler.h"
#include "simulation.h"
#include "decentralized_handler.h"

void print_long_vector (unsigned long* result, int size) {
    printf ("Vector: [ ") ;
        for ( int i = 0; i < size ; i++) {
        printf ("%lx \t " , result [ i ]) ;
    }
    printf ( " ] \n" ) ;
}

int main (int argc, char** argv) {
    char* str1, *str2;
    int nv = 15, nc = 5;
    int POW = 3;

    if (argc == 3) {
        nv = atoi(argv[1]);
        nc = atoi(argv[2]);
    } 
    if (argc >= 4) {
        POW = atoi(argv[3]);
    }

    generate_random_data(nv,nc);
    Block* b = read_block_file("tests/block1.txt");

    str1 = block_to_str(b);
    printf("Block1: \n%s\n",str1);

    

    compute_proof_of_work(b, POW);
    str2 = block_to_str(b);
    printf("After pow: \n%s\n",str2);
    if (!verify_block(b,POW)) {
        printf("ERROR: invalid proof of work\n");
    } else {
        printf("OK\n");
    }
    save_block_file("tests/block2.txt",b);
    free(str2);
    free(str1);
    destroy_block(b);

    return 0;
}