#include <stdio.h>
#include "arithmetics.h"
#include "rsa.h"
#include "vote_handler.h"
#include "simulation.h"
#include "result_handler.h"

//Testing of centralized version
int main(int argc, char** argv) {
    int nv = 15, nc = 5;
    if (argc >= 3) {
        nv = atoi(argv[1]);
        nc = atoi(argv[2]);
    }
    generate_random_data(nv,nc);

    CellKey* cand = read_public_keys("candidates.txt");
    CellKey* citizens = read_public_keys("keys.txt");
    CellKey* voters = citizens; //we could limit voters to a subset of citizens
    Key* winner;
    char* str;

    printf("Candidates:\n");
    print_list_keys(cand);

    //filter out fraudulent delcarations
    CellProtected* dec = read_protected();
    filter_fraud(&dec);

    //calculate the winner from all the declarations
    winner = compute_winner(dec, cand, voters, nc*2, nv*2); 
    str = key_to_str(winner);
    printf("Winner: %s\n",str);

    free(str);
    free(winner);
    delete_cell_keys(cand);
    delete_cell_keys(citizens);
    delete_list_protected(dec);
    return 0;
}