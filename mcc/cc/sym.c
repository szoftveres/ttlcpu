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

void
sym_init (void) {
    lbl = 0;
    variables = NULL;
    scope = 0;
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
add_var (var_t *var, int size, int num) {
    if (var) {
        var->next = variables;
        variables = var;
        var->size = size;
        var->num = num;
        var->scope = scope;
    }
    return var;
}


static int
del_var (void) {
    var_t *var;
    int space = 0;

    var = variables;
    if (var) {
        space = var->size * var->num;
        variables = variables->next;
        free(var);
    }
    return (space);
}


void
inc_var_pos (int b) {
    var_t *it;
    for (it = variables; it && it->scope; it = it->next) {
        it->pos += b;
    }
}


void
dec_var_pos (int b) {
    var_t* it;
    for (it = variables; it && it->scope; it = it->next) {
        it->pos -= b;
    }
}

void
pop_var (void) {
    dec_var_pos(del_var());
}

void
push_var (char* name, int size, int num) {
    var_t *var;
    inc_var_pos(size * num);
    var = new_var(name);
    add_var(var, size, num);
    if (!scope) {
        if (!var->next) {
            var->pos = 0;
        } else {
            var->pos = var->next->pos + (var->next->size * var->next->num);
        }
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
