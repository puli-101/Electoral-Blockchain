#include "result_handler.h"

/* Removes all the declarations with invalid signatures */
//needs optimization
void filter_fraud(CellProtected** lst) {
    if (lst == NULL) return;
    CellProtected* new_lst = NULL;
    CellProtected* iter = *lst;
    while(iter != NULL) {
        if (verify(iter->data)) {
            insert_head_protected(&new_lst, iter->data);
        }
        iter = iter->next;
    }
    
    delete_list_protected(*lst);
    *lst = new_lst;
    
}

/* Mallocs a hash cell */
HashCell* create_hashcell(Key* key) {
    HashCell* hc = (HashCell*)malloc(sizeof(HashCell));
    if (hc == NULL) {
        fprintf(stderr,"Fatal Error: create_hashcell(key)\n");
        exit(-1);
    }
    hc->key = (Key*)malloc(sizeof(Key));
    if (hc->key == NULL) {
        fprintf(stderr,"Fatal Error: create_hashcell(key)\n");
        exit(-1);
    }
    init_key(hc->key, key->val, key->n);
    hc->val = 0;
    return hc;
}

/* 
 *Returns the position of the element with the key 'key'
 * Size: size of the hash table 
 * http://www.cse.yorku.ca/~oz/hash.html
*/
int hash_function(Key* key, int size) {
    char* str = key_to_str(key);
    test_fatal_error(str,"hash_function(key,size)");
    unsigned long hash = 5381;
    for (int i = 0; str[i] != '\0'; i++) {
        hash = ((hash << 5) + hash) + str[i]; /* hash * 33 + c */
    }
    free(str);
    return hash%size;
}

int cmp_keys(Key* k1, Key* k2) {
    return k1 != NULL && k2 != NULL 
            && k1->val == k2->val && k1->n == k2->n;
}

/* 
 * Returns the index where the key 'key' is located
 * The hash table resolves conflicts using linear probing
 * If the key doesn't exist yet it returns the first open position
 */
int find_position(HashTable* t, Key* key) {
    int index = hash_function(key,t->size);
    //
    if (t->tab[index] != NULL && !cmp_keys(key, t->tab[index]->key)) {
        int i = (index + 1)%t->size;
        //we go around the table looking for the first empty position/key's position
        while (i != index) {
            if (t->tab[i] == NULL || cmp_keys(key, t->tab[i]->key)) {
                index = i;
                break;
            }
            i = (i+1)%t->size;
        }
    }
    return index;
}

/*
 * Initializes a HashTable of size 'size' from a list of keys 
*/
HashTable* create_hashtable(CellKey* keys, int size) {
    HashTable* table = (HashTable*)malloc(sizeof(HashTable));
    test_fatal_error(table,"create_hashtable(keys,size)");
    table->size = size;
    table->tab = (HashCell**)malloc(sizeof(HashCell*)*size);
    test_fatal_error(table->tab,"create_hashtable(keys,size)");
    //initialization de la table
    for (int i = 0; i < size; i++) {
        table->tab[i] = NULL;
    }
    for (; keys!=NULL; keys = keys->next) {
        Key* key = keys->data;
        Key* key_cpy = (Key*)malloc(sizeof(Key));
        test_fatal_error(key_cpy,"create_hashtable(keys,size)");
        int index = find_position(table, key);
        //initialization de la position index
        //copy of current key into a new heap space
        init_key(key_cpy,key->val, key->n);
        //malloc of actual cell
        table->tab[index] = (HashCell*)malloc(sizeof(HashCell));
        test_fatal_error(table->tab[index],"create_hashtable(keys,size)");
        table->tab[index]->val = 0;
        table->tab[index]->key = key_cpy;
    }
    return table;
}

/*
 * frees HashTable
 * -
 * For each i 
 * Free each key
 * Free hashCell*
 * -
 * Free hashCell**
 * Free HashTable*
*/
void delete_hashtable(HashTable* t) {
    for (int i = 0; i < t->size; i++) {
        if (t->tab[i] != NULL) {
            free(t->tab[i]->key);
            free(t->tab[i]);
        }
    }
    free(t->tab);
    free(t);
}

/* 
 * decl: list of declarations with valid signatures
 * candidates: list of candidates' keys
 * voters: list of approved citizens' keys
 * sizeC: size of hashtable containing the vote count for each candidate
 * sizeV: size of hashtable representing the amount of times each voter has voted
 * Returns: pk of the candidate with the most votes
 * since collisions are solved by probing, 
 *  we supposes that sizeC and sizeV are larger than the amount of citizens
 */
Key* compute_winner(CellProtected* decl, CellKey* candidates, CellKey* voters, int sizeC, int sizeV) {
    HashTable* cand_count = create_hashtable(candidates, sizeC); //Hc
    HashTable* voters_count = create_hashtable(voters,sizeV); //Hv
    Key* winner = (Key*)malloc(sizeof(Key));
    test_fatal_error(winner, "compute_winner(decl,candidates,voters,sizeC,sizeV)");
    int max_count = -1;

    //for each declaration we check:
    //  that the citizen is an allowed voter
    //  that the citizen hasn't voted already
    //  that the person they vote for is an enlisted candidate
    for(; decl != NULL; decl = decl->next) {
        int index_cand, val;
        Key* citizen = decl->data->pKey;
        Key* voting_for = str_to_key(decl->data->mess);
        int index = find_position(voters_count, citizen);
        //check if it's an allowed voter i.e. already registered i.e. in voters list
        if (voters_count->tab[index] == NULL)
            continue;
        //check if they haven't voted yet
        if (voters_count->tab[index]->val > 0)
            continue;
        //check that the person they're voting for is an enlisted candidate
        index_cand = find_position(cand_count, voting_for);
        if (cand_count->tab[index_cand] == NULL)
            continue;
            
        //update Hc and Hv
        //increase the number of votes for 'voting_for'
        val = (++(cand_count->tab[index_cand]->val)); 
        //mark that the voter 'citizen' has already voted
        (voters_count->tab[index]->val)++;

        //update candidate with the most votes
        if (val > max_count) {
            max_count = val;
            init_key(winner, voting_for->val, voting_for->n);
        }
        free(voting_for);
    }

    delete_hashtable(cand_count);
    delete_hashtable(voters_count);
    return winner;
}

/*Same as computer_winner but using a blockchain*/
Key* compute_winner_BT(CellTree* tree, CellKey* candidates, CellKey* voters, int sizeC, int sizeV) {
    //extraction of the vote declaration list out of the longest blockchain branch
    CellProtected* votes = list_decl_longest_branch(tree);
    filter_fraud(&votes);

    Key* winner = compute_winner(votes, candidates, voters,sizeC, sizeV);

    delete_list_protected(votes);
    return winner;
}