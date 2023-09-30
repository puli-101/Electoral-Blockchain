#include <stdio.h>
#include "arithmetics.h"
#include "rsa.h"
#include "vote_handler.h"
#include "simulation.h"
#include "result_handler.h"

#define POW 3

void submit_discrete(CellProtected* votes, CellKey* citizens, int nc);
Key* select_author(CellKey* cit, int n);

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
    CellProtected* votes = read_protected("declarations.txt");
    submit_discrete(votes, citizens, nc);
    /*
    //calculate the winner from all the declarations
    CellTree* tree = read_tree();
    print_tree(tree);

    winner = compute_winner_BT(tree, cand, voters, nc*2, nv*2); 
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

void submit_discrete(CellProtected* votes, CellKey* citizens, int nc) {
    CellTree* tree = NULL;
    CellProtected* iter = votes;
    char buffer[50];
    int i, c = 1;
    for (i = 1; iter; i++, iter = iter->next) {
        submit_vote(iter->data);
        if (i % 10 == 0) { 
            sprintf(buffer,"b%d.txt",c);
            printf("Reading tree...\n");
            tree = read_tree();
            printf("Creating block...\n");
            create_block(tree,select_author(citizens, rand()%nc),POW);
            printf("Adding block to blockchain/\n");
            add_block(POW, buffer);
            c++;
        }
    } 
    if (i%10 != 0) {
        submit_vote(iter->data);
        sprintf(buffer,"b%d.txt",c);
        printf("Reading tree...\n");
        tree = read_tree();
        printf("Creating block...\n");
        create_block(tree,select_author(citizens, rand()%nc),POW);
        printf("Adding block to blockchain/\n");
        add_block(POW, buffer);
    }
}

Key* select_author(CellKey* cit, int n) {
    CellKey* iter = cit;
    for (int i = 0; i < n && iter != NULL; iter = iter->next, i++);
    if (iter == NULL) return cit->data;
    else return iter->data;
}