#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grammar.h"
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
        grammar_error("expected function declaration");
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
    get_token();
    if (token != T_LEFT_PARENTH) {
        grammar_error("expected '('");
    }
    get_token();
    i = arg_declarations();
    if (token != T_RIGHT_PARENTH) {
        grammar_error("expected ')'");
    }
    for (j = 0; j != SYM_code_pointer_size(); j++) {
        inc_var_pos(&(lcl_vars));
    }
    get_token();
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
        if (token == T_COMMA) {
            int more_args;
            get_token();
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
        if (token != T_SEMICOLON) {
            grammar_error("expected ';'");
        }
        get_token();
        vars += var_declarations();
    }
    return vars;

}


int block (void) {
    int vars;

    if (token != T_LEFT_BRACE) {
        return 0;
    }
    get_token();
    
    vars = var_declarations();
    CODE_var_declarations_space(vars);
    statements();
    if (token != T_RIGHT_BRACE) {
        grammar_error("expected '}'");
    }
    get_token();
    CODE_stack_restore(vars);
    while (vars--) {
        pop_var(&(lcl_vars));
    }

    return 1;
}

int var_declaration (void) {
    if (token != T_IDENTIFIER) {
        return 0;
    }
    if (strcmp(lexeme, "var")) {
        return 0;
    }
    get_token();
    if (token != T_IDENTIFIER) {
        grammar_error("expected identifier");
    }
    if (find_var(&(lcl_vars), lexeme)) {
        fprintf(stderr, "error : '%s' already defined\n", lexeme);
        exit(1);
    }
    push_var(&(lcl_vars), lexeme);
    get_token();
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
    if (asm_statement()) {
        return 1;
    }
    return 0;
}


int if_statement (void) {
    int lbl;
    if (token != T_IDENTIFIER) {
        return 0;
    }
    if (strcmp(lexeme, "if")) {
        return 0;
    }
    get_token();
    if (token != T_LEFT_PARENTH) {
        grammar_error("expected '('");
    }
    get_token();
    if (!expressions()) {
        grammar_error("expected expression");
    }
    if (token != T_RIGHT_PARENTH) {
        grammar_error("expected ')'");
    }
    get_token();
    lbl = new_label();
    CODE_if_statement_head(lbl);
    if (!statement()) {
        grammar_error("expected statement");
    }
    if (else_statement()) {
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


int else_statement (void) {
    if (token != T_IDENTIFIER) {
        return 0;
    }
    if (strcmp(lexeme, "else")) {
        return 0;
    }
    get_token();
    return 1;
}


int while_statement (void) {
    int lbl;
    if (token != T_IDENTIFIER) {
        return 0;
    }
    if (strcmp(lexeme, "while")) {
        return 0;
    }
    get_token();
    if (token != T_LEFT_PARENTH) {
        grammar_error("expected '('");
    }
    get_token();
    lbl = new_label();
    CODE_while_statement_test(lbl);
    if (!expressions()) {
        grammar_error("expected expression");
    }
    if (token != T_RIGHT_PARENTH) {
        grammar_error("expected ')'");
    }
    get_token();
    CODE_while_statement_evaluate(lbl);
    if (!statement()) {
        grammar_error("expected statement");
    }
    CODE_while_statement_end(lbl);
    return 1;
}


int do_statement (void) {
    int lbl;
    if (token != T_IDENTIFIER) {
        return 0;
    }
    if (strcmp(lexeme, "do")) {
        return 0;
    }
    get_token();
    lbl = new_label();
    CODE_do_statement_base(lbl);
    if (!statement()) {
        grammar_error("expected statement");
    }
    if (token != T_IDENTIFIER || strcmp(lexeme, "while")) {
        grammar_error("expected 'while'");
    }
    get_token();
    if (token != T_LEFT_PARENTH) {
        grammar_error("expected '('");
    }
    get_token();
    if (!expressions()) {
        grammar_error("expected expression");
    }
    if (token != T_RIGHT_PARENTH) {
        grammar_error("expected ')'");
    }
    get_token();
    if (token != T_SEMICOLON) {
        grammar_error("expected ';'");
    }
    get_token();
    CODE_do_statement_test(lbl);
    return 1;
}


int for_statement (void) {
    int lbl;
    if (token != T_IDENTIFIER) {
        return 0;
    }
    if (strcmp(lexeme, "for")) {
        return 0;
    }
    get_token();
    if (token != T_LEFT_PARENTH) {
        grammar_error("expected '('");
    }
    get_token();
    expressions();
    if (token != T_SEMICOLON) {
        grammar_error("expected ';'");
    }
    get_token();
    lbl = new_label();
    CODE_for_statement_test(lbl);
    if (expressions()) {
        /* for (;;) empty condition is always true */
        CODE_for_statement_evaluate(lbl);
    }
    CODE_for_statement_jp_to_base(lbl);
    if (token != T_SEMICOLON) {
        grammar_error("expected ';'");
    }
    get_token();
    CODE_for_statement_action(lbl);
    expressions();
    CODE_for_statement_jp_to_test(lbl);
    if (token != T_RIGHT_PARENTH) {
        grammar_error("expected ')'");
    }
    get_token();
    CODE_for_statement_base(lbl);
    if (!statement()) {
        grammar_error("expected statement");
    }
    CODE_for_statement_end(lbl);
    return 1;
}


int asm_statement (void) {
    if (token != T_IDENTIFIER) {
        return 0;
    }
    if (strcmp(lexeme, "asm")) {
        return 0;
    }
    get_token();
    if (token != T_LEFT_PARENTH) {
        grammar_error("expected '('");
    }
    get_token();

    do {
        if (token != T_STRING) {
            grammar_error("expected asm instructions");
        }
        str_process();
        CODE_asm_statement(lexeme);
        get_token();
    } while (token == T_STRING);
    if (token != T_RIGHT_PARENTH) {
        grammar_error("expected ')'");
    }
    get_token();
    if (token != T_SEMICOLON) {
        grammar_error("expected ';'");
    }
    get_token();
    return 1;
}


int expression_statement (void) {
    int expr;

    expr = expressions();
    if (token == T_SEMICOLON) {
        get_token();
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
    if (token == T_COMMA) {
        get_token();
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

    get_token();

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
        return;
    }
    dec_var_pos(&(lcl_vars));       /* there's a POP in each operation */
    return;
}


int ternary_cond (void) {
    int lbl;
    if (token != T_QUESTIONMARK) {
        return 0;
    }
    get_token();
    lbl = new_label();
    CODE_ternary_cond_test(lbl);
    if (!expressions()) {   /* multiple expressions within ?: */
        grammar_error("expexcted expression");
    }
    if (token != T_COLON) {
        grammar_error("expexcted ':'");
    }
    get_token();
    CODE_ternary_cond_mid(lbl);
    if (!expression()) {    /* only one expression after : */
        grammar_error("expexcted expression");
    }
    CODE_ternary_cond_end(lbl);
    return 1;
}


int parentheses (void) {
    if (token != T_LEFT_PARENTH) {
        return 0;
    }
    get_token();
    if (!expressions()) {   /* multiple expressions within () */
        grammar_error("expected expression after '('");
    }
    if (token != T_RIGHT_PARENTH) {
        grammar_error("expected ')'");
    }
    get_token();
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
    if (token != T_NEG) { /* '!' Logical Not */
        return 0;
    }
    get_token();
    if (!primary_expression()) {
        grammar_error("expected primary expression after '!'");
    }
    CODE_logical_neg(new_label());
    return 1;
}

int bitwise_neg (void) {
    if (token != T_BWNEG) { /* '~' Bitwise Not */
        return 0;
    }
    get_token();
    if (!primary_expression()) {
        grammar_error("expected primary expression after '~'");
    }
    CODE_bitwise_neg();
    return 1;
}


int dereference (void) {
    if (token != T_MUL) { /* Dereference */
        return 0;
    }
    get_token();
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

    if (token != T_BWAND) { /* addressof operator */
        return 0;
    }
    get_token();

    if (token != T_IDENTIFIER) {
        grammar_error("expected identifier after '&'");
    }
    id = strdup(lexeme);
    get_token();

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
        get_token();
        return 1;
    }
    if (token == T_CHAR || token == T_INTEGER) {
        CODE_const_expression_int(lexeme);
        get_token();
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
    get_token();
    if (fn_call(id)) {
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
    if (token == T_ASSIGN) {
        get_token();
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
        get_token();
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


int fn_call (char* identifier) {
    int i;
    if (token != T_LEFT_PARENTH) {
        return 0;
    }
    get_token();
    i = fn_call_args();
    if (token != T_RIGHT_PARENTH) {
        grammar_error("expected ')'");
    }
    get_token();
    CODE_fn_call(new_label(), identifier);
    CODE_stack_restore(i);
    while(i--) {
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
        if (token == T_COMMA) {
            int more_args;
            get_token();
            more_args = fn_call_args();
            if (!more_args) {
                grammar_error("expected expression after ','");
            }
            args += more_args;
        }
    }
    return args;
}



