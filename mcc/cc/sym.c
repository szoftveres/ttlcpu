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


static var_p add_var (var_p* pvarp, var_p var, int size, int num) {
    if (var) {
        var->next = *pvarp;
        *pvarp = var;
        var->size = size;
        var->num = num;
    }
    return var;
}


static int del_var (var_p* pvarp) {
    var_p var;
    int space = 0;

    if (!pvarp) {
        fprintf(stderr, "[%s,%d] error \n", __FUNCTION__, __LINE__);
        exit(1);
    }
    var = *pvarp;
    if (var) {
        space = var->size * var->num;
        *pvarp = (*pvarp)->next;
        free(var);
    }
    return space;
}


void inc_var_pos (var_p *pvarp, int b) {
    var_p it;
    for (it = *pvarp; it; it=it->next) {
        it->pos += b;
    }
}


void dec_var_pos (var_p *pvarp, int b) {
    var_p it;
    for (it = *pvarp; it; it=it->next) {
        it->pos -= b;
    }
}

void pop_var (var_p* pvarp) {
    dec_var_pos(pvarp, del_var(pvarp));
}

void push_var (var_p* pvarp, char* name, int size, int num) {
    inc_var_pos(pvarp, size * num);
    add_var(pvarp, new_var(name), size, num);
}

int new_label (void) {
    return ++lbl;
}
