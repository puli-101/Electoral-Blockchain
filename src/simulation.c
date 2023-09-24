#include "simulation.h"

/*
 * The simulations does the following:
 * 1 Generates nv public-secret key pairs representing nv citizens
 * 2 Creates a file named keys.txt containing these pairs
 * 3 Randomly selects nc keys to define nc candidates
 * 4 Creates a file named candidates.txt containing these nc pairs
 * 5 Generates a signed voting declaration for each citizen (candidate chosen ranomdly)
 * 6 Creates a file named declarations.txt containing all signed declarations
*/
void generate_random_data(int nv, int nc) {
    srand(time(NULL));

    Key citizen[nv][2];
    int candidate_index[nc];
    FILE* fkeys = fopen("keys.txt","w");
    FILE* fcandidates = fopen("candidates.txt","w");
    FILE* fdeclarations = fopen("declarations.txt","w");

    if (fkeys == NULL || fdeclarations == NULL || fcandidates == NULL) {
        fprintf(stderr,"Fatal error: generate_random_data");
        exit(-1);
    }

    //1 Generation of nv public-secret key pairs
    for (int i = 0; i < nv; i++) {
        init_pair_keys(&citizen[i][0], &citizen[i][1],3,7);
    }
    //2 Creates a file named keys.txt containing these pairs
    for (int i = 0; i < nv; i++) {
        fprintf(fkeys,"%s %s\n",key_to_str(&citizen[i][0]), key_to_str(&citizen[i][1]));
    }
    //3 Randomly selects nc keys to define nc candidates
    //Omega(nc*nv)
    for (int i = 0; i < nc; i++) {
        int taken;
        int rand_index;

        do {
            rand_index = rand()%nv;
            taken = 0;
            for (int j = 0; j < i; j++) {
                if (candidate_index[j] == rand_index) {
                    taken = 1;
                    break;
                }
            }
        } while(taken);
        candidate_index[i] = rand_index;
    }
    //4 Creates a file named candidates.txt containing these nc pairs
    for(int i = 0; i < nc; i++) {
        int index = candidate_index[i];
        fprintf(fcandidates,"%s %s\n",
            key_to_str(&citizen[index][0]), key_to_str(&citizen[index][1]));
    }
    fclose(fkeys);
    fclose(fcandidates);
    fclose(fdeclarations);
}