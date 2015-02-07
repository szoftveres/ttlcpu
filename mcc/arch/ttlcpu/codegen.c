#include "codegen.h"
#include <stdio.h>
#include <stdlib.h>



void unimplemented (const char* s) {
    fprintf(stderr, "[%s] unimplemented on this architecture\n", s);
    exit(1);
}


int SYM_integer_size (void) {
    return 1;
}


void CODE_func_definition_label (char* fn_name) {
    fprintf(stdout, "\n");
    fprintf(stdout, "lbl(\"%s\")\n", fn_name);
}

void CODE_func_definition_ret (void) {
    fprintf(stdout, "    ret()\n");
}

void CODE_stack_restore (void) {
    fprintf(stdout, "    inc_sp(%u)\n", SYM_integer_size());
}

void CODE_var_declarations_space (void) {
    fprintf(stdout, "    dec_sp(%u)\n", SYM_integer_size());
}

void CODE_if_statement_head (int lbl) {
    fprintf(stdout, "    jz(\"if_continue_%04d\")\n", lbl);
}

void CODE_if_statement_mid (int lbl) {
    fprintf(stdout, "    jp(\"endif_%04d\")\n", lbl);
    fprintf(stdout, "lbl(\"if_continue_%04d\")\n", lbl);
}

void CODE_if_statement_end_else (int lbl) {
    fprintf(stdout, "lbl(\"endif_%04d\")\n", lbl);
}

void CODE_if_statement_end_noelse (int lbl) {
    fprintf(stdout, "lbl(\"if_continue_%04d\")\n", lbl);
}


void CODE_while_statement_test (int lbl) {
    fprintf(stdout, "lbl(\"while_%04d_test\")\n", lbl);
}

void CODE_while_statement_evaluate (int lbl) {
    fprintf(stdout, "    jz(\"while_%04d_end\")\n", lbl);
}

void CODE_while_statement_end (int lbl) {
    fprintf(stdout, "    jp(\"while_%04d_test\")\n", lbl);
    fprintf(stdout, "lbl(\"while_%04d_end\")\n", lbl);
}


void CODE_do_statement_base (int lbl) {
    fprintf(stdout, "lbl(\"do_%04d_base\")\n", lbl);
}

void CODE_do_statement_test (int lbl) {
    fprintf(stdout, "    jz(\"do_%04d_end\")\n", lbl);
    fprintf(stdout, "    jp(\"do_%04d_base\")\n", lbl);
    fprintf(stdout, "lbl(\"do_%04d_end\")\n", lbl);
}


void CODE_for_statement_test (int lbl) {
    fprintf(stdout, "lbl(\"for_%04d_test\")\n", lbl);
}

void CODE_for_statement_evaluate (int lbl) {
    fprintf(stdout, "    jz(\"for_%04d_end\")\n", lbl);
}

void CODE_for_statement_jp_to_base (int lbl) {
    fprintf(stdout, "    jp(\"for_%04d_base\")\n", lbl);
}

void CODE_for_statement_action (int lbl) {
    fprintf(stdout, "lbl(\"for_%04d_action\")\n", lbl);
}

void CODE_for_statement_jp_to_test (int lbl) {
    fprintf(stdout, "    jp(\"for_%04d_test\")\n", lbl);
}

void CODE_for_statement_base (int lbl) {
    fprintf(stdout, "lbl(\"for_%04d_base\")\n", lbl);
}

void CODE_for_statement_end (int lbl) {
    fprintf(stdout, "    jp(\"for_%04d_action\")\n", lbl);
    fprintf(stdout, "lbl(\"for_%04d_end\")\n", lbl);
}


void CODE_asm_statement (char* s) {
    fprintf(stdout, "%s", s);
}


void CODE_logical_neg (int lbl) {
    fprintf(stdout, "    jz(\"log_neg_%04d\")\n", lbl);
    fprintf(stdout, "    mov(to_acc, literal) lit(0)\n");
    fprintf(stdout, "    jp(\"log_neg_%04d_end\")\n", lbl);
    fprintf(stdout, "lbl(\"log_neg_%04d\")\n", lbl);
    fprintf(stdout, "    mov(to_acc, literal) lit(1)\n");
    fprintf(stdout, "lbl(\"log_neg_%04d_end\")\n", lbl);
}

void CODE_bitwise_neg (void) {
    fprintf(stdout, "    inv(frm_acc)\n");
}

void CODE_operand_push (void) {
    fprintf(stdout, "    push(frm_acc)                  // 1st operand push\n");
}

void CODE_operand_pop (void) {
    fprintf(stdout, "    st(BX, frm_acc)                // store 2nd operand\n");
    fprintf(stdout, "    pop(to_acc)                      // 1st operand pop\n");
}

void CODE_do_operation_mul (int lbl) {
    unimplemented(__FUNCTION__);

    fprintf(stdout, "lbl(\"mul_%04d_start\")\n", lbl);
    fprintf(stdout, "    jz(\"mul_%04d_end\")\n", lbl);
    fprintf(stdout, "    dec()\n");
    CODE_operand_pop();
    fprintf(stdout, "    shl()\n");
    CODE_operand_push();
    fprintf(stdout, "    ld(to_acc, BX)\n");
    fprintf(stdout, "    jp(\"mul_%04d_start\")\n", lbl);
    fprintf(stdout, "lbl(\"mul_%04d_end\")\n", lbl);
    fprintf(stdout, "    pop(to_acc)\n");

}

void CODE_do_operation_div (void) {
    unimplemented(__FUNCTION__);
}

void CODE_do_operation_mod (void) {
    unimplemented(__FUNCTION__);
}

void CODE_do_operation_add (void) {
    CODE_operand_pop();
    fprintf(stdout, "    mov(to_ramaddr, literal) lit(BX)  // addition\n");
    fprintf(stdout, "    add(frm_ram)\n");
}

void CODE_do_operation_sub (void) {
    fprintf(stdout, "    inv(frm_acc)             // 1st cmpl\n");
    fprintf(stdout, "    inc()                        // 2nd cmpl      \n");
    CODE_operand_pop();
    fprintf(stdout, "    mov(to_ramaddr, literal) lit(BX)\n");
    fprintf(stdout, "    add(frm_ram)\n");
}

void CODE_do_operation_shl (int lbl) {
    fprintf(stdout, "lbl(\"sleft_%04d_start\")\n", lbl);
    fprintf(stdout, "    jz(\"sleft_%04d_end\")\n", lbl);
    fprintf(stdout, "    dec()\n");
    CODE_operand_pop();
    fprintf(stdout, "    shl()\n");
    CODE_operand_push();
    fprintf(stdout, "    ld(to_acc, BX)\n");
    fprintf(stdout, "    jp(\"sleft_%04d_start\")\n", lbl);
    fprintf(stdout, "lbl(\"sleft_%04d_end\")\n", lbl);
    fprintf(stdout, "    pop(to_acc)\n");
}

void CODE_do_operation_shr (int lbl) {
    fprintf(stdout, "lbl(\"sright_%04d_start\")\n", lbl);
    fprintf(stdout, "    jz(\"sright_%04d_end\")\n", lbl);
    fprintf(stdout, "    dec()\n");
    CODE_operand_pop();
    fprintf(stdout, "    rol()\n");
    fprintf(stdout, "    rol()\n");
    fprintf(stdout, "    rol()\n");
    fprintf(stdout, "    rol()\n");
    fprintf(stdout, "    rol()\n");
    fprintf(stdout, "    rol()\n");
    fprintf(stdout, "    rol()\n");
    fprintf(stdout, "    shl()\n");
    fprintf(stdout, "    rol()\n");
    fprintf(stdout, "    rol()\n");
    fprintf(stdout, "    rol()\n");
    fprintf(stdout, "    rol()\n");
    fprintf(stdout, "    rol()\n");
    fprintf(stdout, "    rol()\n");
    fprintf(stdout, "    rol()\n");
    CODE_operand_push();
    fprintf(stdout, "    ld(to_acc, BX)\n");
    fprintf(stdout, "    jp(\"sright_%04d_start\")\n", lbl);
    fprintf(stdout, "lbl(\"sright_%04d_end\")\n", lbl);
    fprintf(stdout, "    pop(to_acc)\n");
}


void CODE_do_operation_compare_eq (int lbl) {
    CODE_do_operation_sub();
    fprintf(stdout, "    jz(\"eq_%04d\")\n", lbl);
    fprintf(stdout, "    mov(to_acc, literal) lit(0)\n");
    fprintf(stdout, "    jp(\"eq_%04d_end\")\n", lbl);
    fprintf(stdout, "lbl(\"eq_%04d\")\n", lbl);
    fprintf(stdout, "    mov(to_acc, literal) lit(1)\n");
    fprintf(stdout, "lbl(\"eq_%04d_end\")\n", lbl);
}

void CODE_do_operation_compare_neq (int lbl) {
    CODE_do_operation_sub();
    fprintf(stdout, "    jz(\"neq_%04d\")\n", lbl);
    fprintf(stdout, "    mov(to_acc, literal) lit(1)\n");
    fprintf(stdout, "    jp(\"neq_%04d_end\")\n", lbl);
    fprintf(stdout, "lbl(\"neq_%04d\")\n", lbl);
    fprintf(stdout, "    mov(to_acc, literal) lit(0)\n");
    fprintf(stdout, "lbl(\"neq_%04d_end\")\n", lbl);
}

void CODE_do_operation_bwand (int lbl) {
    CODE_operand_push();
    fprintf(stdout, "    call(\"bwand\", \"bwand_%04d\")\n", lbl);
    fprintf(stdout, "    inc_sp(2)\n");
}

void CODE_do_operation_bwxor (void) {
    unimplemented(__FUNCTION__);
}

void CODE_do_operation_bwor (void) {
    unimplemented(__FUNCTION__);
}



void CODE_ternary_cond_test (int lbl) {
    fprintf(stdout, "    jz(\"ternary_second_%04d\")\n", lbl);
}

void CODE_ternary_cond_mid (int lbl) {
    fprintf(stdout, "    jp(\"ternary_end_%04d\")\n", lbl);
    fprintf(stdout, "lbl(\"ternary_second_%04d\")\n", lbl);
}

void CODE_ternary_cond_end (int lbl) {
    fprintf(stdout, "lbl(\"ternary_end_%04d\")\n", lbl);
}


void CODE_dereference_assign_push (void){
    fprintf(stdout, "    push(frm_acc)\n");
}

void CODE_dereference_assign_pop (void) {
    fprintf(stdout, "    pop(to_ramaddr)\n");
    fprintf(stdout, "    mov(to_ram, frm_acc)\n");
}

void CODE_dereference (void) {
    fprintf(stdout, "    mov(to_ramaddr, frm_acc)\n");
    fprintf(stdout, "    mov(to_acc, frm_ram)\n");
}


void CODE_addressof (int pos) {
    fprintf(stdout, "    ld(to_acc, SP)\n");
    fprintf(stdout, "    add(literal) lit(%d+1)\n", pos);
}


void CODE_const_expression_str (int lbl, char* c) {
    unimplemented(__FUNCTION__);
}

void CODE_const_expression_int (char* c) {
    fprintf(stdout, "    mov(to_acc, literal) lit(%s)      // const\n", c);
}


void CODE_store_to_address (void) {
    fprintf(stdout, "    mov(to_ramaddr, literal) lit(BX)\n");
    fprintf(stdout, "    mov(to_ramaddr, frm_ram)\n");
    fprintf(stdout, "    mov(to_ram, frm_acc)\n");
}


void CODE_fn_call (int lbl, char* fn_name) {
    fprintf(stdout, "    call(\"%s\", \"call_%04d\")\n", fn_name, lbl);
}

void CODE_fn_call_args (void) {
    fprintf(stdout, "    push(frm_acc)                      // push fn arg\n");
}


