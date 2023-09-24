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

    Protected* declaration[nv];
    Signature* signature[nv];
    char* msg[nv];
    //[][0] : public key , [][1] : secret key
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
        char* pk = key_to_str(&citizen[i][0]);
        char* sk = key_to_str(&citizen[i][1]);
        fprintf(fkeys,"%s %s\n",pk, sk);
        free(pk);
        free(sk);
    }
    //3 Randomly selects nc keys to define nc candidates
    //Omega(nc*nv)
    for (int i = 0; i < nc; i++) {
        int taken;
        int rand_index;
        //selects random citizen and checks if it isn't a candidate already
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
        char* pk = key_to_str(&citizen[index][0]);
        char* sk = key_to_str(&citizen[index][1]);
        fprintf(fcandidates,"%s %s\n", pk, sk);
        free(pk);
        free(sk);
    }
    //5 Generates a signed voting declaration for each citizen (candidate chosen ranomdly)
    for (int i = 0; i < nv; i++) {
        int choice = rand()%nc;
        //We select the pk of the chosen candidate and parse it to string
        msg[i] = key_to_str(&citizen[candidate_index[choice]][0]);
        //We sign with the current sk the msg of the chosen candidate
        signature[i] = sign(msg[i],&citizen[i][1]);
        //We declare the vote of the citizen with their pk, choice and signature
        declaration[i] = init_protected(&citizen[i][0], msg[i], signature[i]);
    }
    //6 Creates a file named declarations.txt containing all signed declarations
    for (int i = 0; i < nv; i++) {
        char* str = protected_to_str (declaration[i]); 
        fprintf(fdeclarations, "%s\n",str);
        free(str);
    }
    fclose(fkeys);
    fclose(fcandidates);
    fclose(fdeclarations);
    for (int i = 0; i < nv; i++) {
        free_signature(signature[i]);
        free(msg[i]);
        free_protected(declaration[i]);
    }

}