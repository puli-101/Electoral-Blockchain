#include <stdio.h>
#include "arithmetics.h"
#include "rsa.h"
#include "vote_handler.h"
#include "simulation.h"
#include "result_handler.h"

#define POW 3

void submit_discrete(CellProtected* votes);

int main(int argc, char** argv) {
    int nv = 1000, nc = 5;
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

    //start discrete vote submissions every 10 votes
    CellProtected* votes = read_protected();
    submit_discrete(votes);

    /*calculate the winner from all the declarations
    CellTree* tree = read_tree();

    winner = compute_winner_BC(tree, cand, voters, nc*2, nv*2); 
    str = key_to_str(winner);
    printf("Winner: %s\n",str);

    free(str);
    free(winner);*/
    delete_cell_keys(cand);
    delete_cell_keys(citizens);
    delete_list_protected(votes);
    //delete_tree(tree);
    return 0;
}

void submit_discrete(CellProtected* votes) {
    CellProtected* iter = votes;
    char buffer[50];
    int i, c = 1;
    for (i = 0; iter; i++, iter = iter->next) {
        submit_vote(votes->data);
        if (i % 10 == 0) { 
            sprintf(buffer,"b%d.txt",c);
            add_block(POW, buffer);
            c++;
        }
    } 
    if (i%10 != 0) {
        sprintf(buffer,"b%d.txt",c);
        add_block(POW, buffer);
    }
}