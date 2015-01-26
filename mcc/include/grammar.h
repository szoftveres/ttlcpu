#ifndef _GRAMMAR_H_
#define _GRAMMAR_H_

int program (void);

int program_base (void);

int func_definitions (void);

int func_definition (void);


int var_declaration (void);

int var_declarations (int *i);

int arg_declarations (int *i);



int block (void);

int block_base (void);

int statements (void);

int keyword (void);

int if_statement (void);
int else_statement (void);

int while_statement (void);
int do_statement (void);
int for_statement (void);
int asm_statement (void);

int statement (void);

int expression_statement (void);

int expressions (void);

int expression (void);

int prefixop(void);

int parentheses (void);

int logical_neg (void);
int bitwise_neg (void);

int dereference (void);

int addressof (void);

int identifier_expression (void);

int recursive_assignment (void);

int primary_expression(void);

int ternary_cond (void);

int const_expression (void);

int fn_call (char* identifier);

int fn_call_args (int *i);

int binary_operation(int precedence);

void do_operations (int op_type);
#endif
