#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sym.h"
#include "codegen.h"




/* variable stack */
static var_t            *variables;

/* unique labels */
static int             lbl;

/* scope - 0 means global */
static int             scope;
static int             stc_pos;

static int             stack_grow;
static char            function_name[32];

void
sym_init (void) {
    lbl = 0;
    variables = NULL;
    scope = 0;
    stack_grow = 0;
    stc_pos = 0;
}


var_t *
find_var (char* name, int scope_loc) {
    var_t *it;
    for (it = variables; it && (scope_loc ? it->scope == scope : 1); it = it->next) {
        if (!strcmp(name, it->name)) {
            return (it);
        }
    }
    return NULL;
}


static var_t *
new_var (char* name) {
    var_t   *var;

    var = (var_t*) malloc (sizeof(var_t));
    if (!var) {
        fprintf(stderr, "[%s,%d] error \n", __FUNCTION__, __LINE__);
        exit(1);
    }
    memset (var, 0, sizeof(var_t));
    strcpy(var->name, name);
    return var;
}


static var_t *
add_var (var_t *var, int size, int num, int stc) {
    if (var) {
        var->next = variables;
        variables = var;
        var->size = size;
        var->num = num;
        var->scope = scope;
        var->stc = stc;
    }
    return var;
}


static int
del_var (void) {
    var_t *var;
    int space = 0;

    var = variables;
    if (var) {
        space = var->stc ? 0 : var->size * var->num;
        variables = variables->next;
        free(var);
    }
    return (space);
}


void
inc_var_pos (int b) {
    var_t *it;
    for (it = variables; it; it = it->next) {
        if (it->stc) continue;
        it->pos += b;
    }
    stack_grow += b;
}


void
dec_var_pos (int b) {
    var_t* it;
    for (it = variables; it; it = it->next) {
        if (it->stc) continue;
        it->pos -= b;
    }
    stack_grow -= b;
}

void
reset_stack_grow (char* fn_name) {
    stack_grow = 0;
    strcpy(function_name, fn_name);
}

int
get_stack_grow (char** fn_name) {
    *fn_name = function_name;
    return (stack_grow);
}

void
pop_var (void) {
    dec_var_pos(del_var());
}

void
push_var (char* name, int size, int num, int stc) {
    var_t *var;
    if (!stc) {
        inc_var_pos(size * num);
    }
    var = new_var(name);
    add_var(var, size, num, stc);
    if (stc) {
        var->pos = stc_pos;
        stc_pos += (size * num);
    }
}

int
new_label (void) {
    return (++lbl);
}

void
scope_inc (void) {
    scope += 1;
}

void
scope_dec (void) {
    scope -= 1;
}
