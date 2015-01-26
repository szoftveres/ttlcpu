#ifndef _SYM_H_
#define _SYM_H_

#include "lex.h"


typedef struct var_s {
    char                name[MAX_TOKEN_SIZE];
    int                 pos;
    struct var_s        *next;
} var_t, *var_p;


/* local variables */
extern var_p            lcl_vars;
/* global variables */
extern var_p            glb_vars;



void sym_init (void);
void push_var (var_p* pvarp, char* name);
void pop_var (var_p* pvarp);
var_p find_var (var_p* pvarp, char* name);
void inc_var_pos (var_p *pvarp);
void dec_var_pos (var_p *pvarp);



int new_label (void);

#endif
