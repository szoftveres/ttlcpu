#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sym.h"
#include "codegen.h"




/* unique labels */
static int      lbl;

sym_t           *variables;
sym_t           *returns;
sym_t           *breaks;
sym_t           *continues;


int
new_label (void) {
    return (++lbl);
}

void
sym_init (void) {
    lbl = 0;
    variables = NULL;
    returns = NULL;
    breaks = NULL;
    continues = NULL;
}

void
sym_dist_inc (int b) {
    if (variables) {variables->dist += b;}
    if (returns) {returns->dist += b;}
    if (breaks) {breaks->dist += b;}
    if (continues) {continues->dist += b;}
}

void
sym_dist_dec (int b) {
    if (variables) {variables->dist -= b;}
    if (returns) {returns->dist -= b;}
    if (breaks) {breaks->dist -= b;}
    if (continues) {continues->dist -= b;}
}

sym_t* sym_create_lbl (int lbl) {
    sym_t      *sym;

    sym = (sym_t*) malloc(sizeof(sym_t));
    memset(sym, 0, sizeof(sym_t));
    sym->lbl = lbl;
    sym->dist = 0;
    return sym;
}

sym_t* sym_create_name (const char* name) {
    sym_t      *sym;

    sym = (sym_t*) malloc(sizeof(sym_t));
    memset(sym, 0, sizeof(sym_t));
    strncpy(sym->name, name, MAX_TOKEN_SIZE);
    sym->dist = 0;
    return sym;
}

void
sym_push (sym_t** stack, sym_t *sym) {
    sym->next = *stack;
    *stack = sym;
}

void
sym_pop (sym_t** stack) {
    sym_t *sym;

    sym = *stack;
    *stack = (*stack)->next;
    free(sym);
    return;
}

int
sym_dist_top (sym_t** stack) {
    return ((*stack)->dist);
}

int
sym_dist_name (sym_t** stack, const char* name, int *dist) {
    sym_t *sym;

    if (dist) {
        *dist = 0;
    }
    for (sym = *stack; sym; sym = sym->next) {
        if (dist) {
            *dist += sym->dist;
        }
        if (!strcmp(sym->name, name)) {
            return 1;
        }
    }
    return 0;
}

int
sym_lbl_top (sym_t** stack) {
    return ((*stack)->lbl);
}

