#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sym.h"
#include "codegen.h"


static int             lbl;


/* local variable list */
var_p            lcl_vars;
/* global variable list */
var_p            glb_vars;



void sym_init (void) {
    lbl=0;
    glb_vars = NULL;
    lcl_vars = NULL;
}


var_p find_var (var_p* pvarp, char* name) {
    var_p it;
    for (it = *pvarp; it; it=it->next) {
        if (!strcmp(name, it->name)) {
            return it;
        }
    }
    return NULL;
}


static var_p new_var (char* name) {
    var_p   var;

    var = (var_p) malloc (sizeof(var_t));
    if (!var) {
        fprintf(stderr, "[%s,%d] error \n", __FUNCTION__, __LINE__);
        exit(1);
    }
    memset (var, 0, sizeof(var_t));
    strcpy(var->name, name);
    return var;
}


static var_p add_var (var_p* pvarp, var_p var) {
    if (var) {
        var->next = *pvarp;
        *pvarp = var;
    }
    return var;
}


static var_p del_var (var_p* pvarp) {
    var_p var;
    if (!pvarp) {
        fprintf(stderr, "[%s,%d] error \n", __FUNCTION__, __LINE__);
        exit(1);
    }
    var = *pvarp;
    if (var) {
        *pvarp = (*pvarp)->next;
        free(var);
    }
    return var;
}


void inc_var_pos (var_p *pvarp) {
    var_p it;
    for (it = *pvarp; it; it=it->next) {
        it->pos += SYM_integer_size();
    }
}


void dec_var_pos (var_p *pvarp) {
    var_p it;
    for (it = *pvarp; it; it=it->next) {
        it->pos -= SYM_integer_size();
    }
}

void pop_var (var_p* pvarp) {
    dec_var_pos(pvarp);
    del_var(pvarp);
}

void push_var (var_p* pvarp, char* name) {
    inc_var_pos(pvarp);
    add_var(pvarp, new_var(name));
}

int new_label (void) {
    return ++lbl;
}
