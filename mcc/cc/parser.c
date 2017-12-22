#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "codegen.h"
#include "lex.h"
#include "sym.h"


void parser_error (char* s) {
    fprintf(stderr, "parser error: %s (token: %d)\n", s, token);
    exit(1);
}

void parser_warning (char* s) {
    fprintf(stderr, "parser warning: %s\n", s);
}

void program (void) {
    int vars;

    func_definitions();
    if (token != T_EOF) {
        parser_error("expected function definition");
    }
    while (vars--) {
        pop_var();
    }
    return;
}


int func_definitions (void) {
    if (!func_definition()) {
        return 0;
    }
    func_definitions();
    return 1;
}


int func_definition (void) {
    int args;
    int lbl;

    if (token != T_IDENTIFIER) {
        return 0;
    }
    CODE_func_definition_label(lexeme);
    next_token();
    if (!lex_get(T_LEFT_PARENTH, NULL)) {
        parser_error("expected '('");
    }
    scope_inc();
    args = arg_declarations();
    if (!lex_get(T_RIGHT_PARENTH, NULL)) {
        parser_error("expected ')'");
    }
    inc_var_pos(ARCH_code_pointer_size());
    /* The above one doesn't need a pair, (out of scope on return) */
    lbl = new_label();
    jmpstack_push(&returnstack, lbl);
    if (!block()) {
        parser_error("expected block");
    }
    jmpstack_pop(&returnstack);
    while (args--) {
        pop_var();
    }
    scope_dec();
    CODE_ret_jmppoint(lbl);
    return 1;
}


int arg_declarations (void) {
    int args = 0;

    args += arg_declaration();
    if (args) {
        if (lex_get(T_COMMA, NULL)) {
            int more_args;
            more_args = arg_declarations();
            if (!more_args) {
                parser_error("expected argument after ','");
            }
            args += more_args;
        }
    }
    return args;
}


int lcl_var_declarations (int* space) {
    int vars = 0;

    vars += var_declaration(space);
    if (vars) {
        if (!lex_get(T_SEMICOLON, NULL)) {
            parser_error("expected ';'");
        }
        vars += lcl_var_declarations(space);
    }
    return vars;

}


int block (void) {
    int vars;
    int var_space = 0;

    if (!lex_get(T_LEFT_BRACE, NULL)) {
        return 0;
    }
    vars = lcl_var_declarations(&var_space);
    CODE_var_declarations_space(var_space);
    statements();
    if (!lex_get(T_RIGHT_BRACE, NULL)) {
        parser_error("expected '}'");
    }
    CODE_stack_restore(var_space);
    while (vars--) {
        pop_var();
    }
    return 1;
}


int var_declaration (int* space) {
    char* name;
    int size;

    if (!lex_get(T_IDENTIFIER, "auto")) {
        return 0;
    }
    if (token != T_IDENTIFIER) {
        parser_error("expected identifier");
    }
    name = strdup(lexeme);
    next_token();
    if (find_var(name, 1)) {
        fprintf(stderr, "error : '%s' already defined\n", name);
        exit(1);
    }
    size = ARCH_word_size();
    push_var(name, size);
    free(name);
    if (space) {
        (*space) += size;
    }
    return 1; /* should be strictly 1 (number of declarations found) */
}

int arg_declaration (void) {
    char* name;

    if (token != T_IDENTIFIER) {
        return 0;
    }
    name = strdup(lexeme);
    next_token();
    if (find_var(name, 1)) {
        fprintf(stderr, "error : '%s' already defined\n", name);
        exit(1);
    }
    push_var(name, ARCH_word_size());
    free(name);
    return 1; /* should be strictly 1 (number of declarations found) */
}



int statements (void) {
    if (!statement()) {
        return 0;
    }
    statements();
    return 1;
}


int statement (void) {
    scope_inc();
    if (block()) {
        scope_dec();
        return 1;
    }
    scope_dec();
    if (keyword()) {
        return 1;
    }
    if (expression_statement()) {
        return 1;
    }
    return 0;
}


int keyword (void) {
    if (if_statement()) {
        return 1;
    }
    if (while_statement()) {
        return 1;
    }
    if (do_statement()) {
        return 1;
    }
    if (for_statement()) {
        return 1;
    }
    if (return_statement()) {
        return 1;
    }
    if (break_statement()) {
        return 1;
    }
    if (continue_statement()) {
        return 1;
    }
    return 0;
}


int if_statement (void) {
    int lbl;

    if (!lex_get(T_IDENTIFIER, "if")) {
        return 0;
    }
    if (!lex_get(T_LEFT_PARENTH, NULL)) {
        parser_error("expected '('");
    }
    if (!expressions()) {
        parser_error("expected expression");
    }
    if (!lex_get(T_RIGHT_PARENTH, NULL)) {
        parser_error("expected ')'");
    }
    lbl = new_label();
    CODE_if_statement_head(lbl);
    if (!statement()) {
        parser_error("expected statement");
    }
    if (lex_get(T_IDENTIFIER, "else")) {
        CODE_if_statement_mid(lbl);
        if (!statement()) {
            parser_error("expected statement");
        }
        CODE_if_statement_end_else(lbl);
    } else {
        CODE_if_statement_end_noelse(lbl);
    }
    return 1;
}


int break_statement (void) {
    if (!lex_get(T_IDENTIFIER, "break")) {
        return 0;
    }
    if (!lex_get(T_SEMICOLON, NULL)) {
        parser_error("expected ';'");
    }
    CODE_break_statement(jmpstack_lbl(&breakstack),
                         jmpstack_grow(&breakstack));

    return 1;
}


int continue_statement (void) {
    if (!lex_get(T_IDENTIFIER, "continue")) {
        return 0;
    }
    if (!lex_get(T_SEMICOLON, NULL)) {
        parser_error("expected ';'");
    }
    CODE_continue_statement(jmpstack_lbl(&continuestack),
                            jmpstack_grow(&continuestack));
    return 1;
}


int return_statement (void) {
    if (!lex_get(T_IDENTIFIER, "return")) {
        return 0;
    }
    expression(); /* optional return value */
    if (!lex_get(T_SEMICOLON, NULL)) {
        parser_error("expected ';'");
    }
    CODE_return_statement(jmpstack_lbl(&returnstack),
                          jmpstack_grow(&returnstack));
    return 1;
}


int while_statement (void) {
    int lbl;

    if (!lex_get(T_IDENTIFIER, "while")) {
        return 0;
    }
    if (!lex_get(T_LEFT_PARENTH, NULL)) {
        parser_error("expected '('");
    }
    lbl = new_label();
    CODE_while_statement_test(lbl);
    if (!expressions()) {
        parser_error("expected expression");
    }
    if (!lex_get(T_RIGHT_PARENTH, NULL)) {
        parser_error("expected ')'");
    }
    CODE_while_statement_evaluate(lbl);
    jmpstack_push(&breakstack, lbl);
    jmpstack_push(&continuestack, lbl);
    if (!statement()) {
        parser_error("expected statement");
    }
    jmpstack_pop(&breakstack);
    jmpstack_pop(&continuestack);
    CODE_continue_jmppoint(lbl);
    CODE_while_statement_end(lbl);
    CODE_break_jmppoint(lbl);
    return 1;
}


int do_statement (void) {
    int lbl;

    if (!lex_get(T_IDENTIFIER, "do")) {
        return 0;
    }
    lbl = new_label();
    CODE_do_statement_base(lbl);
    jmpstack_push(&breakstack, lbl);
    jmpstack_push(&continuestack, lbl);
    if (!statement()) {
        parser_error("expected statement");
    }
    jmpstack_pop(&breakstack);
    jmpstack_pop(&continuestack);
    CODE_continue_jmppoint(lbl);

    if (lex_get(T_IDENTIFIER, "while")) {
        if (!lex_get(T_LEFT_PARENTH, NULL)) {
            parser_error("expected '('");
        }
        if (!expressions()) {
            parser_error("expected expression");
        }
        if (!lex_get(T_RIGHT_PARENTH, NULL)) {
            parser_error("expected ')'");
        }
        if (!lex_get(T_SEMICOLON, NULL)) {
            parser_error("expected ';'");
        }
        CODE_do_statement_test(lbl);
    }
    CODE_break_jmppoint(lbl);
    return 1;
}


int for_statement (void) {
    int lbl;

    if (!lex_get(T_IDENTIFIER, "for")) {
        return 0;
    }
    if (!lex_get(T_LEFT_PARENTH, NULL)) {
        parser_error("expected '('");
    }
    expressions();
    if (!lex_get(T_SEMICOLON, NULL)) {
        parser_error("expected ';'");
    }
    lbl = new_label();
    CODE_for_statement_test(lbl);
    if (expressions()) {
        /* for (;;) empty condition is always true */
        CODE_for_statement_evaluate(lbl);
    }
    CODE_for_statement_jp_to_base(lbl);
    if (!lex_get(T_SEMICOLON, NULL)) {
        parser_error("expected ';'");
    }
    CODE_for_statement_action(lbl);
    expressions();
    CODE_for_statement_jp_to_test(lbl);
    if (!lex_get(T_RIGHT_PARENTH, NULL)) {
        parser_error("expected ')'");
    }
    CODE_for_statement_base(lbl);
    jmpstack_push(&breakstack, lbl);
    jmpstack_push(&continuestack, lbl);
    if (!statement()) {
        parser_error("expected statement");
    }
    jmpstack_pop(&breakstack);
    jmpstack_pop(&continuestack);
    CODE_continue_jmppoint(lbl);
    CODE_for_statement_end(lbl);
    CODE_break_jmppoint(lbl);
    return 1;
}


int expression_statement (void) {
    int expr;

    expr = expressions();
    if (lex_get(T_SEMICOLON, NULL)) {
        return 1;
    }
    if (expr) {
        parser_error("expected ';'");
    }
    return 0;
}


int expressions (void) {
    if (!expression()) {
        return 0;
    }
    if (lex_get(T_COMMA, NULL)) {
        if (!expressions()) {
            parser_error("expected expression after ','");
        }
    }
    return 1;
}


int expression (void) {
    if (primary_expression()) {
        binary_operation(0);
        ternary_cond();
        return 1;
    }
    return 0;
}


int primary_expression (void) {
    if (parentheses()) {
        return 1;
    }
    if (prefixop()) {
        return 1;
    }
    if (const_expression()) {
        return 1;
    }
    if (object_expression()) {
        return 1;
    }
    return 0;
}


int binary_operation (int precedence) {
    int op;

    op = token;
    switch (precedence) {
      case 9:
        switch (op) {
          case T_MUL :              /* * */
          case T_DIV :              /* / */
          case T_MOD :              /* % */
          break;
          default:
          return (binary_operation(precedence + 1));
        }
        break;

      case 8:
        switch (op) {
          case T_PLUS :             /* + */
          case T_MINUS :            /* - */
          break;
          default:
          return (binary_operation(precedence + 1));
        }
        break;

      case 7:
        switch (op) {
          case T_SLEFT :            /* << */
          case T_SRIGHT :           /* >> */
          break;
          default:
          return (binary_operation(precedence + 1));
        }
        break;

      case 6:
        switch (op) {
          case T_LESS :             /* < */
          case T_GREATER :          /* > */
          case T_LEQ :              /* <= */
          case T_GREQ :             /* >= */
          break;
          default:
          return (binary_operation(precedence + 1));
        }
        break;

      case 5:
        switch (op) {
          case T_EQ :               /* == */
          case T_NEQ :              /* != */
          break;
          default:
          return (binary_operation(precedence + 1));
        }
        break;

      case 4:
        switch (op) {
          case T_BWAND :            /* & */
          break;
          default:
          return (binary_operation(precedence + 1));
        }
        break;

      case 3:
        switch (op) {
          case T_BWXOR :            /* ^ */
          break;
          default:
          return (binary_operation(precedence + 1));
        }
        break;

      case 2:
        switch (op) {
          case T_BWOR :             /* | */
          break;
          default:
          return (binary_operation(precedence + 1));
        }
        break;

      case 1:
        switch (op) {
          case T_LAND :             /* && */
          break;
          default:
          return (binary_operation(precedence + 1));
        }
        break;

      case 0:
        switch (op) {
          case T_LOR :              /* || */
          break;
          default:
          return (binary_operation(precedence + 1));
        }
        break;

      default: return 0;
    }

    next_token();

    CODE_push_unsafe();
    inc_var_pos(ARCH_word_size());
    /* matching dec_var_pos in do_operations */

    if (!primary_expression()) {
        parser_error("expected expression after operator");
    }

    /* Subsequent higher precedence operations */
    binary_operation(precedence + 1);

    do_operations(op); /* inc_var_pos is in do_operations */

    /* Subsequent same or lower precedence operations */
    binary_operation(0);
    return 1;
}

/*
 * 1st operand in Stack
 * 2nd operand in Acc
 *
 * Each op_fn can store 2nd operand in BX if needed
 *
 */
void do_operations (int op_type) {
    switch (op_type) {
      case T_MUL :
      case T_RECURMUL :
        CODE_do_operation_mul(new_label());
        break;
      case T_DIV :
      case T_RECURDIV :
        CODE_do_operation_div();
        break;
      case T_MOD :
        CODE_do_operation_mod();
        break;
      case T_PLUS :
      case T_RECURADD :
        CODE_do_operation_add();
        break;
      case T_MINUS :
      case T_RECURSUB :
        CODE_do_operation_sub();
        break;
      case T_SLEFT :
        CODE_do_operation_shl(new_label());
        break;
      case T_SRIGHT :
        CODE_do_operation_shr(new_label());
        break;
      case T_EQ :
        CODE_do_operation_compare_eq(new_label());
        break;
      case T_NEQ :
        CODE_do_operation_compare_neq(new_label());
        break;
      case T_LESS :
        CODE_do_operation_compare_less(new_label());
        break;
      case T_GREQ :
        CODE_do_operation_compare_greq(new_label());
        break;
      case T_BWAND :
      case T_RECURBWAND :
        CODE_do_operation_bwand(new_label());
        break;
      case T_BWXOR :
      case T_RECURBWXOR :
        CODE_do_operation_bwxor();
        break;
      case T_BWOR :
      case T_RECURBWOR :
        CODE_do_operation_bwor();
        break;
      default:
        parser_error("unknown / unimplemented operator");
        return;
    }
    dec_var_pos(ARCH_word_size());
    /* there's a POP in each operation */
    return;
}


int ternary_cond (void) {
    int lbl;

    if (!lex_get(T_QUESTIONMARK, NULL)) {
        return 0;
    }
    lbl = new_label();
    CODE_ternary_cond_test(lbl);
    if (!expressions()) {   /* multiple expressions within ?: */
        parser_error("expexcted expression");
    }
    if (!lex_get(T_COLON, NULL)) {
        parser_error("expexcted ':'");
    }
    CODE_ternary_cond_mid(lbl);
    if (!expression()) {    /* one expressions after : */
        parser_error("expexcted expression");
    }
    CODE_ternary_cond_end(lbl);
    return 1;
}


int parentheses (void) {
    if (!lex_get(T_LEFT_PARENTH, NULL)) {
        return 0;
    }
    if (!expressions()) {   /* multiple expressions within () */
        parser_error("expected expression after '('");
    }
    if (!lex_get(T_RIGHT_PARENTH, NULL)) {
        parser_error("expected ')'");
    }
    return 1;
}

int prefixop(void) {
    if (logical_neg()) {
        return 1;
    }
    if (bitwise_neg()) {
        return 1;
    }
    if (addressof()) {
        return 1;
    }

    return (0);
}


int logical_neg (void) {
    if (!lex_get(T_NEG, NULL)) {
        return 0;
    }
    if (!primary_expression()) {
        parser_error("expected primary expression after '!'");
    }
    CODE_logical_neg(new_label());
    return 1;
}

int bitwise_neg (void) {
    if (!lex_get(T_BWNEG, NULL)) {
        return 0;
    }
    if (!primary_expression()) {
        parser_error("expected primary expression after '~'");
    }
    CODE_bitwise_neg();
    return 1;
}


int numeric_const (int* value) {
    if (token != T_CHAR &&
        token != T_INTEGER &&
        token != T_OCTAL &&
        token != T_BINARY &&
        token != T_HEXA) {
        return 0;
    }
    *value = num_process();
    next_token();
    return 1;
}


int const_expression (void) {
    int val;

    if (token == T_STRING) {
        CODE_const_expression_str(new_label(), lexeme);
        next_token();
        return 1;
    }
    if (numeric_const(&val)) {
        CODE_const_expression_int(val);
        return 1;
    }
    return 0;
}


int assignment (void) {
    /* Addr of location in acc */
    if (lex_get(T_ASSIGN, NULL)) {
        CODE_push_unsafe();
        inc_var_pos(ARCH_word_size());
        if (!expression()) {
            parser_error("expected expression after '='");
        }
        CODE_pop_addr_and_store();
        dec_var_pos(ARCH_word_size());
        return 1;
    } else if (recursive_assignment()) {
        CODE_pop_addr_and_store();
        dec_var_pos(ARCH_word_size());
        return 1;
    }
    return 0;
}


int recursive_assignment (void) {
    int op_type = token;
    switch (token) {
      case T_RECURADD :
      case T_RECURSUB :
      case T_RECURMUL :
      case T_RECURDIV :
      case T_RECURBWAND :
      case T_RECURBWOR :
      case T_RECURBWXOR :
        next_token();
        break;
      default:
        return 0;
    }

    CODE_push();               /* Push the address where we store back */
    inc_var_pos(ARCH_word_size());  /* matching dec_var_pos */

    CODE_dereference();
    CODE_push_unsafe();
    inc_var_pos(ARCH_word_size());
    /* matching dec_var_pos in do_operations */

    if (!expression()) {    /* only one expression after assignment */
        parser_error("expected expression after assignment operator");
    }
    do_operations(op_type);  /* inc_var_pos is in do_operations */

    return 1;
}


int function_expression (char* identifier) {
    if (asm_expression(identifier)) {
        return 1;
    }
    if (sizeof_expression(identifier)) {
        return 1;
    }
    if (fn_call(identifier)) {
        return 1;
    }
    return 0;
}


int asm_expression (char* identifier) {
    if (strcmp(identifier, "asm")) {
        return 0;
    }
    if (!lex_get(T_LEFT_PARENTH, NULL)) {
        parser_error("expected '('");
    }
    do {
        if (token != T_STRING) {
            parser_error("expected string");
        }
        str_process();
        CODE_asm_statement(lexeme);
        next_token();
    } while (token == T_STRING);
    if (!lex_get(T_RIGHT_PARENTH, NULL)) {
        parser_error("expected ')'");
    }
    return 1;
}

int sizeof_expression (char* identifier) {
    if (strcmp(identifier, "sizeof")) {
        return 0;
    }
    if (!lex_get(T_LEFT_PARENTH, NULL)) {
        parser_error("expected '('");
    }
    expressions(); /* doesn't matter */
    if (!lex_get(T_RIGHT_PARENTH, NULL)) {
        parser_error("expected ')'");
    }
    CODE_const_expression_int(ARCH_word_size());
    return 1;
}

int fn_call (char* identifier) {
    int i;

    if (!lex_get(T_LEFT_PARENTH, NULL)) {
        return 0;
    }
    i = fn_call_args();
    if (!lex_get(T_RIGHT_PARENTH, NULL)) {
        parser_error("expected ')'");
    }
    CODE_fn_call(new_label(), identifier);
    CODE_stack_restore(i * ARCH_word_size());
    dec_var_pos(i * ARCH_word_size());
    return 1;
}


int fn_call_args (void) {
    int args = 0;
    args += expression();
    if (args) {
        CODE_fn_call_args();
        inc_var_pos(ARCH_word_size());
        if (lex_get(T_COMMA, NULL)) {
            int more_args;
            more_args = fn_call_args();
            if (!more_args) {
                parser_error("expected expression after ','");
            }
            args += more_args;
        }
    }
    return args;
}


/* This handles functions too */
int object_expression (void) {
    if (!dereference()) {
        id_obj_t rc;

        rc = object_identifier();
        if (!rc) {
            return 0;
        }
        if (rc == RC_FUNCTION) {
            return 1; /* Done */
        }
    }
    /* Object address in acc */
    if (!assignment()) {
        CODE_dereference();
    }
    return 1;
}


int dereference (void) {
    id_obj_t rc = RC_ADDRESS;
    if (!lex_get(T_MUL, NULL)) {
        return 0;
    }
    if (!dereference()) {
        rc = object_identifier();
    } else {
        CODE_dereference();
    }

    if (rc == RC_VARIABLE) {
        CODE_dereference();
        return 1;
    }
    if (!rc) {
        if (!primary_expression()) {
            parser_error("expected object address after '*'");
        }
    }
    return 1;
}


int addressof (void) {
    if (!lex_get(T_BWAND, NULL)) {
        return 0;
    }
    if (object_identifier() != RC_VARIABLE) {
        parser_error("expected variable after '&'");
    }
    return 1;
}


id_obj_t object_identifier (void) {
    char* id;
    var_t *var;

    if (token != T_IDENTIFIER) {
        return 0;
    }
    id = strdup(lexeme);
    next_token();
    if (function_expression(id)) {
        free(id);
        /* Function return value in acc */
        return RC_FUNCTION;
    }
    var = find_var(id, 0);
    if (!var) {
        fprintf(stderr, "error : '%s' not defined in this scope\n", id);
        free(id);
        exit(1);
    }
    CODE_load_eff_addr_auto(var->pos +
                           (ARCH_stack_post_decrement() ?
                            ARCH_word_size() : 0));
    free(id);
    /* Variable address in acc */
    return RC_VARIABLE;
}


