#ifndef _PARSER_H_
#define _PARSER_H_

void program (void);

int func_definitions (void);

int func_definition (void);


int var_declaration (int* space);

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
int break_statement (void);
int continue_statement (void);

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
int sizeof_expression (char* identifier);

int fn_call (char* identifier);

int fn_call_args (void);

int binary_operation(int precedence);

void do_operations (int op_type);

int object_expression (void);

int dereference (void);

typedef enum id_obj_e {
    RC_NONE     = 0,
    RC_ADDRESS  = 1,
    RC_VARIABLE = 2,
    RC_FUNCTION = 3
} id_obj_t;


id_obj_t object_identifier (void);

int switch_statement (void);
int case_type_statements (int lbl, int *next_lbl);
int case_type_statement (int lbl, int *next_lbl);
int case_statement (int lbl, int *next_lbl);
int default_statement (int lbl, int *next_lbl);
int switch_block (int lbl, int *next_lbl);




#endif
