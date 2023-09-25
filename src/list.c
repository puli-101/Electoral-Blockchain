#include "list.h"

/* Mallocs and initializes a linked list */
CellKey* create_cell_key(Key* key) {
    CellKey* cell = (CellKey*)malloc(sizeof(CellKey));
    if (cell == NULL) {
        fprintf(stderr,"Fatal Error: create_cell_key(key)\n");
        exit(-1);
    }
    cell->data = (Key*)malloc(sizeof(Key));
    if (cell->data == NULL) {
        fprintf(stderr,"Fatal Error: create_cell_key(key)\n");
        exit(-1);
    }
    init_key(cell->data, key->val, key->n);
    cell->next = NULL;
    return cell;
}

/* Inserts element as head of the list */
void insert_head_key(CellKey** lst, Key* key) {
    CellKey* head = create_cell_key(key);
    head->next = *lst;
    *lst = head;
}

/* 
 * Reads file keys.txt or candidates.txt 
 * returns a linked list of the public keys
*/
CellKey* read_public_keys(char *option) {
    if (strcmp("keys.txt",option) != 0 && 
        strcmp("candidates.txt",option) != 0) {
        fprintf(stderr,"Invalid option %s\n",option);
        return NULL;
    }
    FILE* f = fopen(option,"r");
    if (f == NULL) {
        fprintf(stderr,"Fatal Error: read_public_keys(%s)\n",option);
        exit(-1);
    }
    CellKey* lst = NULL;
    char line_buffer[500];

    while(fgets(line_buffer, 500, f)) {
        Key* cpy = str_to_key(line_buffer);
        insert_head_key(&lst, cpy);
        free(cpy);
    }

    fclose(f);
    return lst;
}

/* Prints a list of keys */
void print_list_keys(CellKey* LCK) {
    for (;LCK != NULL; LCK = LCK->next) {
        char* str = key_to_str(LCK->data);
        printf("%s|",str);
        free(str);
    }
    printf("\n");
}

/* Deletes a single cell of a key linked list */
void delete_cell_key(CellKey* c) {
    free(c->data);
    free(c);
}

/* Deletes a key linked list*/
void delete_cell_keys(CellKey* c) {
    if (c == NULL)
        return;
    delete_cell_keys(c->next);
    delete_cell_key(c);
}

/* Creates and initializes a cell of type protected*/
CellProtected* create_cell_protected(Protected* pr) {
    CellProtected* cell = (CellProtected*)malloc(sizeof(CellProtected));
    if (cell == NULL || pr == NULL) {
        fprintf(stderr,"Fatal Error: create_cell_protected(pr)\n");
        exit(-1);
    }
    cell->data = init_protected(pr->pKey, pr->mess, pr->sgn);
    
    cell->next = NULL;
    return cell;
}

/* Inserts element as head of the list */
void insert_head_protected(CellProtected** lst, Protected* pr) {
    CellProtected* head = create_cell_protected(pr);
    head->next = *lst;
    *lst = head;
}

/* 
 * Reads file declarations.txt
 * returns a linked list of the public keys
*/
CellProtected* read_protected() {
    FILE* f = fopen("declarations.txt","r");
    if (f == NULL) {
        fprintf(stderr,"Fatal Error: read_protected()\n");
        exit(-1);
    }
    CellProtected* lst = NULL;
    char line_buffer[500];

    while(fgets(line_buffer, 500, f)) {
        Protected* cpy = str_to_protected(line_buffer);
        insert_head_protected(&lst, cpy);
        free_protected(cpy);
    }

    fclose(f);
    return lst;
}

/* Prints a list of protecteds */
void print_list_protected(CellProtected* LCK) {
    printf("- Declarations:\n");
    for (;LCK != NULL; LCK = LCK->next) {
        char* str = protected_to_str(LCK->data);
        printf("%s\n",str);
        free(str);
    }
}

/* Deletes a single cell of a protected linked list */
void delete_cell_protected(CellProtected* c) {
    free_protected(c->data);
    free(c);
}

/* Deletes a protected linked list*/
void delete_list_protected(CellProtected* c) {
    if (c == NULL)
        return;
    delete_list_protected(c->next);
    delete_cell_protected(c);
}