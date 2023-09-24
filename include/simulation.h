#ifndef _SIMULATION_H_
#define _SIMULATION_H_

#include "rsa.h"
#include "arithmetics.h"
#include "vote_handler.h"

/*
 * Generates nv public-secret key pairs representing nv citizens
 * Creates a file named keys.txt containing these pairs
 * Randomly selects nc keys to define nc candidates
 * Creates a file named candidates.txt containing these nc pairs
 * Generates a signed voting declaration for each citizen (candidate chosen ranomdly)
 * Creates a file named declarations.txt containing all signed declarations
*/
void generate_random_data(int nv, int nc);

#endif