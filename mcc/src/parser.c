#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "codegen.h"
#include "lex.h"
#include "sym.h"


void grammar_error (char* s) {
    fprintf(stderr, "error : %s\n", s);
    exit(1);
}


int program (void) {
    func_definitions();
    if (token != T_EOF) {
        grammar_error("expected function definition");
    }
    exit(0);
}


int func_definitions (void) {
    if (!func_definition()) {
        return 0;
    }
    func_definitions();
    return 1;
}


int func_definition (void) {
    int i;
    int j;

    if (token != T_IDENTIFIER) {
        return 0;
    }
    CODE_func_definition_label(lexeme);
    lex_consume();
    if (!lex_get(T_LEFT_PARENTH, NULL)) {
        grammar_error("expected '('");
    }
    i = arg_declarations();
    if (!lex_get(T_RIGHT_PARENTH, NULL)) {
        grammar_error("expected ')'");
    }
    for (j = 0; j != SYM_code_pointer_size(); j++) {
        inc_var_pos(&(lcl_vars));
    }
    if (!block()) {
        grammar_error("expected block");
    }
    while (i--) {
        pop_var(&(lcl_vars));
    }
    CODE_func_definition_ret();
    return 1;
}


int arg_declarations (void) {
    int args = 0;

    args += var_declaration();
    if (args) {
        if (lex_get(T_COMMA, NULL)) {
            int more_args;
            more_args = arg_declarations();
            if (!more_args) {
                grammar_error("expected argument after ','");
            }
            args += more_args;
        }    
    }
    return args;
}


int var_declarations (void) {
    int vars = 0;

    vars += var_declaration();
    if (vars) {
        if (!lex_get(T_SEMICOLON, NULL)) {
            grammar_error("expected ';'");
        }
        vars += var_declarations();
    }
    return vars;

}


int block (void) {
    int vars;

    if (!lex_get(T_LEFT_BRACE, NULL)) {
        return 0;
    }
    
    vars = var_declarations();
    CODE_var_declarations_space(vars);
    statements();
    if (!lex_get(T_RIGHT_BRACE, NULL)) {
        grammar_error("expected '}'");
    }
    CODE_stack_restore(vars);
    while (vars--) {
        pop_var(&(lcl_vars));
    }

    return 1;
}

int var_declaration (void) {
    if (!lex_get(T_IDENTIFIER, "var")) {
        return 0;
    }
    if (token != T_IDENTIFIER) {
        grammar_error("expected identifier");
    }
    if (find_var(&(lcl_vars), lexeme)) {
        fprintf(stderr, "error : '%s' already defined\n", lexeme);
        exit(1);
    }
    push_var(&(lcl_vars), lexeme);
    lex_consume();
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
    if (keyword()) {
        return 1;
    }
    if (block()) {
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
    return 0;
}


int if_statement (void) {
    int lbl;

    if (!lex_get(T_IDENTIFIER, "if")) {
        return 0;
    }
    if (!lex_get(T_LEFT_PARENTH, NULL)) {
        grammar_error("expected '('");
    }
    if (!expressions()) {
        grammar_error("expected expression");
    }
    if (!lex_get(T_RIGHT_PARENTH, NULL)) {
        grammar_error("expected ')'");
    }
    lbl = new_label();
    CODE_if_statement_head(lbl);
    if (!statement()) {
        grammar_error("expected statement");
    }
    if (lex_get(T_IDENTIFIER, "else")) {
        CODE_if_statement_mid(lbl);
        if (!statement()) {
            grammar_error("expected statement");
        }
        CODE_if_statement_end_else(lbl);
    } else {
        CODE_if_statement_end_noelse(lbl);
    }
    return 1;
}


int while_statement (void) {
    int lbl;

    if (!lex_get(T_IDENTIFIER, "while")) {
        return 0;
    }
    if (!lex_get(T_LEFT_PARENTH, NULL)) {
        grammar_error("expected '('");
    }
    lbl = new_label();
    CODE_while_statement_test(lbl);
    if (!expressions()) {
        grammar_error("expected expression");
    }
    if (!lex_get(T_RIGHT_PARENTH, NULL)) {
        grammar_error("expected ')'");
    }
    CODE_while_statement_evaluate(lbl);
    if (!statement()) {
        grammar_error("expected statement");
    }
    CODE_while_statement_end(lbl);
    return 1;
}


int do_statement (void) {
    int lbl;

    if (!lex_get(T_IDENTIFIER, "do")) {
        return 0;
    }
    lbl = new_label();
    CODE_do_statement_base(lbl);
    if (!statement()) {
        grammar_error("expected statement");
    }
    if (!lex_get(T_IDENTIFIER, "while")) {
        grammar_error("expected 'while'");
    }
    if (!lex_get(T_LEFT_PARENTH, NULL)) {
        grammar_error("expected '('");
    }
    if (!expressions()) {
        grammar_error("expected expression");
    }
    if (!lex_get(T_RIGHT_PARENTH, NULL)) {
        grammar_error("expected ')'");
    }
    if (!lex_get(T_SEMICOLON, NULL)) {
        grammar_error("expected ';'");
    }
    CODE_do_statement_test(lbl);
    return 1;
}


int for_statement (void) {
    int lbl;

    if (!lex_get(T_IDENTIFIER, "for")) {
        return 0;
    }
    if (!lex_get(T_LEFT_PARENTH, NULL)) {
        grammar_error("expected '('");
    }
    expressions();
    if (!lex_get(T_SEMICOLON, NULL)) {
        grammar_error("expected ';'");
    }
    lbl = new_label();
    CODE_for_statement_test(lbl);
    if (expressions()) {
        /* for (;;) empty condition is always true */
        CODE_for_statement_evaluate(lbl);
    }
    CODE_for_statement_jp_to_base(lbl);
    if (!lex_get(T_SEMICOLON, NULL)) {
        grammar_error("expected ';'");
    }
    CODE_for_statement_action(lbl);
    expressions();
    CODE_for_statement_jp_to_test(lbl);
    if (!lex_get(T_RIGHT_PARENTH, NULL)) {
        grammar_error("expected ')'");
    }
    CODE_for_statement_base(lbl);
    if (!statement()) {
        grammar_error("expected statement");
    }
    CODE_for_statement_end(lbl);
    return 1;
}


int expression_statement (void) {
    int expr;

    expr = expressions();
    if (lex_get(T_SEMICOLON, NULL)) {
        return 1;
    }
    if (expr) {
        grammar_error("expected ';'");
    }
    return 0;
}


int expressions (void) {
    if (!expression()) {
        return 0;
    }
    if (lex_get(T_COMMA, NULL)) {
        if (!expressions()) {
            grammar_error("expected expression after ','");
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
    if (identifier_expression()) {
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

    lex_consume();

    CODE_push();
    inc_var_pos(&(lcl_vars)); /* matching dec_var_pos in do_operations */

    if (!primary_expression()) {
        grammar_error("expected expression after operator");
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
        grammar_error("unknown / unimplemented operator");
        return;
    }
    dec_var_pos(&(lcl_vars));       /* there's a POP in each operation */
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
        grammar_error("expexcted expression");
    }
    if (!lex_get(T_COLON, NULL)) {
        grammar_error("expexcted ':'");
    }
    CODE_ternary_cond_mid(lbl);
    if (!expression()) {    /* one expressions after : */
        grammar_error("expexcted expression");
    }
    CODE_ternary_cond_end(lbl);
    return 1;
}


int parentheses (void) {
    if (!lex_get(T_LEFT_PARENTH, NULL)) {
        return 0;
    }
    if (!expressions()) {   /* multiple expressions within () */
        grammar_error("expected expression after '('");
    }
    if (!lex_get(T_RIGHT_PARENTH, NULL)) {
        grammar_error("expected ')'");
    }
    return 1;
}

int prefixop(void) {
    if (dereference()) {
        return 1;
    }
    if (addressof()) {
        return 1;
    }
    if (logical_neg()) {
        return 1;
    }
    if (bitwise_neg()) {
        return 1;
    }
    return (0);
}


int logical_neg (void) {
    if (!lex_get(T_NEG, NULL)) {
        return 0;
    }
    if (!primary_expression()) {
        grammar_error("expected primary expression after '!'");
    }
    CODE_logical_neg(new_label());
    return 1;
}

int bitwise_neg (void) {
    if (!lex_get(T_BWNEG, NULL)) {
        return 0;
    }
    if (!primary_expression()) {
        grammar_error("expected primary expression after '~'");
    }
    CODE_bitwise_neg();
    return 1;
}


int dereference (void) {
    if (!lex_get(T_MUL, NULL)) {
        return 0;
    }
    if (!primary_expression()) {
        grammar_error("expected primary expression after '*'");
    }
    if (!assignment()) {
        CODE_dereference();
    }
    return 1;
}


int addressof (void) {
    char* id;
    var_p var;

    if (!lex_get(T_BWAND, NULL)) {
        return 0;
    }
    if (token != T_IDENTIFIER) {
        grammar_error("expected identifier after '&'");
    }
    id = strdup(lexeme);
    lex_consume();
    var = find_var(&(lcl_vars), id);
    if (!var) {
        fprintf(stderr, "error : '%s' not defined in this scope\n", id);
        exit(1);
    }
    CODE_load_eff_addr(var->pos);
    free(id);
    return 1;
}


int const_expression (void) {
    if (token == T_STRING) {
        CODE_const_expression_str(new_label(), lexeme);
        lex_consume();
        return 1;
    }
    if (token == T_CHAR || token == T_INTEGER || token == T_OCTAL || token == T_BINARY || token == T_HEXA) {
        CODE_const_expression_int(lexeme);
        lex_consume();
        return 1;
    }
    return 0;
}


int identifier_expression (void) {
    char* id;
    var_p var;

    if (token != T_IDENTIFIER) {
        return 0;
    }
    id = strdup(lexeme);
    lex_consume();
    if (function_expression(id)) {
        free(id);
        return 1;
    }
    var = find_var(&(lcl_vars), id);
    if (!var) {
        fprintf(stderr, "error : '%s' not defined in this scope\n", id);
        exit(1);
    }
    CODE_load_eff_addr(var->pos);
    if (!assignment()) {
        CODE_dereference();
    }
    free(id);
    return 1;
}


int assignment (void) {
    /* Addr of location in acc */
    if (lex_get(T_ASSIGN, NULL)) {
        CODE_push();
        inc_var_pos(&(lcl_vars));
        if (!expression()) {
            grammar_error("expected expression after '='");
        }
        CODE_pop_addr_and_store();
        dec_var_pos(&(lcl_vars));
    } else if (recursive_assignment()) {
        CODE_pop_addr_and_store();
        dec_var_pos(&(lcl_vars));
    } else {
        return 0;
    }
    return 1;
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
        lex_consume();
        break;
      default:
        return 0;
    }

    CODE_push();               /* Push the address where we store back */
    inc_var_pos(&(lcl_vars));  /* matching dec_var_pos */

    CODE_dereference();
    CODE_push();
    inc_var_pos(&(lcl_vars));  /* matching dec_var_pos in do_operations */

    if (!expression()) {    /* only one expression after assignment */
        grammar_error("expected expression after assignment operator");
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


int sizeof_expression (char* identifier) {
    if (strcmp(identifier, "sizeof")) {
        return 0;
    }
    if (!lex_get(T_LEFT_PARENTH, NULL)) {
        grammar_error("expected '('");
    }

    if (!expression()) {
        grammar_error("expected expression");
    }
    if (!lex_get(T_RIGHT_PARENTH, NULL)) {
        grammar_error("expected ')'");
    }
    grammar_error("'sizeof' unimplemented");
    return 1;
}


int asm_expression (char* identifier) {
    if (strcmp(identifier, "asm")) {
        return 0;
    }
    if (!lex_get(T_LEFT_PARENTH, NULL)) {
        grammar_error("expected '('");
    }
    do {
        if (token != T_STRING) {
            grammar_error("expected string");
        }
        str_process();
        CODE_asm_statement(lexeme);
        lex_consume();
    } while (token == T_STRING);
    if (!lex_get(T_RIGHT_PARENTH, NULL)) {
        grammar_error("expected ')'");
    }
    return 1;
}


int fn_call (char* identifier) {
    int i;

    if (!lex_get(T_LEFT_PARENTH, NULL)) {
        return 0;
    }
    i = fn_call_args();
    if (!lex_get(T_RIGHT_PARENTH, NULL)) {
        grammar_error("expected ')'");
    }
    CODE_fn_call(new_label(), identifier);
    CODE_stack_restore(i);
    while (i--) {
        dec_var_pos(&(lcl_vars));
    }
    return 1;
}


int fn_call_args (void) {
    int args = 0;

    args += expression();
    if (args) {
        CODE_fn_call_args();
        inc_var_pos(&(lcl_vars));
        if (lex_get(T_COMMA, NULL)) {
            int more_args;
            more_args = fn_call_args();
            if (!more_args) {
                grammar_error("expected expression after ','");
            }
            args += more_args;
        }
    }
    return args;
}


/* ================================= */


int object (void) {
    if (object_parentheses()) {
        return 1;
    }
    if (object_identifier()) {
        return 1;
    }
    return 0;  
}


int object_parentheses (void) {
    if (!lex_get(T_LEFT_PARENTH, NULL)) {
        return 0;
    }
    if (!object()) {   /* multiple expressions within () */
        grammar_error("expected expression after '('");
    }
    if (!lex_get(T_RIGHT_PARENTH, NULL)) {
        grammar_error("expected ')'");
    }
    return 1;
}


int object_identifier (void) {
    char* id;
    var_p var;

    if (token != T_IDENTIFIER) {
        return 0;
    }
    id = strdup(lexeme);
    lex_consume();
    var = find_var(&(lcl_vars), id);
    if (!var) {
        fprintf(stderr, "error : '%s' not defined in this scope\n", id);
        exit(1);
    }
    CODE_load_eff_addr(var->pos);
    free(id);
    return 1;
}

