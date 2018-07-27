#ifndef _SYM_H_
#define _SYM_H_

#include "lex.h"


typedef struct sym_s {
    union {
        char                name[MAX_TOKEN_SIZE];
        int                 lbl;
    };
    int                 dist;
    struct sym_s        *next;
} sym_t;


extern sym_t       *variables;
extern sym_t       *returns;
extern sym_t       *breaks;
extern sym_t       *continues;


int new_label (void);

void sym_init (void);
void sym_dist_inc (int b);
void sym_dist_dec (int b);
sym_t* sym_create_lbl (int lbl);
sym_t* sym_create_name (const char* name);
void sym_push (sym_t** stack, sym_t *sym);
void sym_pop (sym_t** stack);
int sym_dist_top (sym_t** stack);
int sym_dist_name (sym_t** stack, const char* name, int *dist);
int sym_lbl_top (sym_t** stack);

#endif
