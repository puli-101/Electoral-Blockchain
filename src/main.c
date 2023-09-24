#include <stdio.h>
#include "arithmetics.h"
#include "rsa.h"
#include "vote_handler.h"
#include "simulation.h"

int main(int argc, char** argv) {
    int nv = 15, nc = 5;
    if (argc >= 3) {
        nv = atoi(argv[1]);
        nc = atoi(argv[2]);
    }
    generate_random_data(nv,nc);
    return 0;
}