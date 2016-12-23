#ifndef _SYM_H_
#define _SYM_H_

#include "lex.h"


typedef struct var_s {
    char                name[MAX_TOKEN_SIZE];
    int                 pos;
    int                 size;   /* Size of this type */
    int                 num;    /* In case it represents an array */
    struct var_s        *child; /* In case it represents a structure */
    struct var_s        *next;
} var_t, *var_p;


/* local variables */
extern var_p            lcl_vars;
/* global variables */
extern var_p            glb_vars;



void sym_init (void);
void push_var (var_p* pvarp, char* name, int size, int num);
void pop_var (var_p* pvarp);
var_p find_var (var_p* pvarp, char* name);
void inc_var_pos (var_p *pvarp, int b);
void dec_var_pos (var_p *pvarp, int b);



int new_label (void);

#endif
