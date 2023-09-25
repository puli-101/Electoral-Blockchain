#include <stdio.h>
#include "arithmetics.h"
#include "rsa.h"
#include "vote_handler.h"
#include "simulation.h"
#include "result_handler.h"

int main(int argc, char** argv) {
    //int nv = 15, nc = 5;
    //if (argc >= 3) {
    //    nv = atoi(argv[1]);
    //    nc = atoi(argv[2]);
    //}
    //generate_random_data(nv,nc);

    CellKey* lst = read_public_keys("candidates.txt");
    print_list_keys(lst);

    HashTable* t = create_hashtable(lst, 50);
    delete_hashtable(t);

    delete_cell_keys(lst);

    CellProtected* dec = read_protected();
    print_list_protected(dec);
    
    filter_fraud(&dec);
    print_list_protected(dec);

    delete_list_protected(dec);
    return 0;
}