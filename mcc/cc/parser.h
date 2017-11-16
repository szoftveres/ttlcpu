#ifndef _PARSER_H_
#define _PARSER_H_

int program (void);

int func_definitions (void);

int func_definition (void);


int var_declaration (int* space, int stc);

int glb_var_declarations (int* space);
int lcl_var_declarations (int* space);

int arg_declaration (void);
int arg_declarations (void);



int block (void);

int statements (void);

int keyword (void);

int if_statement (void);

int while_statement (void);
int do_statement (void);
int for_statement (void);
int return_statement (void);

int statement (void);

int expression_statement (void);

int expressions (void);

int expression (void);

int prefixop(void);

int parentheses (void);

int logical_neg (void);
int bitwise_neg (void);
int addressof (void);

int assignment (void);

int recursive_assignment (void);

int primary_expression(void);

int ternary_cond (void);

int numeric_const (int* value);

int const_expression (void);

int function_expression (char* identifier);

int asm_expression (char* identifier);

int fn_call (char* identifier);

int fn_call_args (void);

int binary_operation(int precedence);

void do_operations (int op_type);

int dereference (void);
int object_identifier (void);
int object_expression (void);


#endif
