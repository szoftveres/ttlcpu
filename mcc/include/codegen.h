#ifndef _CODEGEN_H_
#define _CODEGEN_H_

int SYM_integer_size (void);


void CODE_func_definition_label (char* fn_name);
void CODE_func_definition_ret (void);

void CODE_stack_restore (void);

void CODE_var_declarations_space (void);

void CODE_if_statement_head (int lbl);
void CODE_if_statement_mid (int lbl);
void CODE_if_statement_end_else (int lbl);
void CODE_if_statement_end_noelse (int lbl);

void CODE_while_statement_test (int lbl);
void CODE_while_statement_evaluate (int lbl);
void CODE_while_statement_end (int lbl);

void CODE_do_statement_base (int lbl);
void CODE_do_statement_test (int lbl);

void CODE_for_statement_test (int lbl);
void CODE_for_statement_evaluate (int lbl);
void CODE_for_statement_jp_to_base (int lbl);
void CODE_for_statement_action (int lbl);
void CODE_for_statement_jp_to_test (int lbl);
void CODE_for_statement_base (int lbl);
void CODE_for_statement_end (int lbl);

void CODE_asm_statement (char* s);

void CODE_logical_neg (int lbl);
void CODE_bitwise_neg (void);

void CODE_push (void);
void CODE_operand_pop (void);

void CODE_do_operation_mul (int lbl);
void CODE_do_operation_div (void);
void CODE_do_operation_mod (void);
void CODE_do_operation_add (void);
void CODE_do_operation_sub (void);
void CODE_do_operation_shl (int lbl);
void CODE_do_operation_shr (int lbl);
void CODE_do_operation_compare_eq (int lbl);
void CODE_do_operation_compare_neq (int lbl);
void CODE_do_operation_bwand (int lbl);
void CODE_do_operation_bwxor (void);
void CODE_do_operation_bwor (void);


void CODE_ternary_cond_test (int lbl);
void CODE_ternary_cond_mid (int lbl);
void CODE_ternary_cond_end (int lbl);


void CODE_pop_addr_and_store (void);
void CODE_dereference (void);

void CODE_load_eff_addr (int pos);


void CODE_const_expression_str (int lbl, char* c);
void CODE_const_expression_int (char* c);



void CODE_fn_call (int lbl, char* fn_name);

void CODE_fn_call_args (void);



#endif
