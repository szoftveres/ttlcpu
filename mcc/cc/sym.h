#ifndef _SYM_H_
#define _SYM_H_

#include "lex.h"


typedef struct var_s {
    char                name[MAX_TOKEN_SIZE];
    int                 scope;
    int                 pos;
    int                 size;   /* Size of this type */
    int                 num;    /* In case it represents an array */
    struct var_s        *child; /* In case it represents a structure */
    struct var_s        *next;
} var_t;



void sym_init (void);
void push_var (char* name, int size, int num);
void pop_var (void);
var_t *find_var (char* name, int scope_loc);
void inc_var_pos (int b);
void dec_var_pos (int b);

int new_label (void);

void scope_inc (void);
void scope_dec (void);

#endif
