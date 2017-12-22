#ifndef _SYM_H_
#define _SYM_H_

#include "lex.h"


typedef struct var_s {
    char                name[MAX_TOKEN_SIZE];
    int                 scope;
    int                 pos;
    int                 size;   /* Size of this type */
    struct var_s        *child; /* In case it represents a structure */
    struct var_s        *next;
} var_t;


typedef struct jmpstack_s {
    int                 lbl;
    int                 stack_grow;
    struct jmpstack_s   *next;
} jmpstack_t;


extern jmpstack_t       *returnstack;
extern jmpstack_t       *breakstack;
extern jmpstack_t       *continuestack;


void sym_init (void);
void push_var (char* name, int size);
void pop_var (void);
var_t *find_var (char* name, int scope_loc);
void inc_var_pos (int b);
void dec_var_pos (int b);

int new_label (void);

void scope_inc (void);
void scope_dec (void);

void jmpstack_push (jmpstack_t** stack, int lbl);
int jmpstack_pop (jmpstack_t** stack);
int jmpstack_grow (jmpstack_t** stack);
int jmpstack_lbl (jmpstack_t** stack);

#endif
