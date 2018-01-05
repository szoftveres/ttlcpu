#include "codegen.h"
#include <stdio.h>
#include <stdlib.h>

/*

static void unimplemented (const char* s) {
    fprintf(stderr, "[%s] unimplemented on this architecture\n", s);
    exit(1);
}
*/

static void print_debugs (const char* s) {
//    fprintf(stdout, "# [%s]\n", s);
    ;
}


int ARCH_word_size (void) {
    return 8;                   /* 64 bit */
}

int ARCH_code_pointer_size (void) {
    return 8;                   /* 64 bit */
}

int ARCH_stack_post_decrement (void) {
    return 0;
}


void CODE_func_definition_label (char* fn_name) {
    fprintf(stdout, "\n");
    print_debugs(__FUNCTION__);
    fprintf(stdout, "%s:\n", fn_name);
}

void CODE_ret_jmppoint (int lbl) {
    print_debugs(__FUNCTION__);
    fprintf(stdout, "__%04d__return:\n", lbl);
    fprintf(stdout, "    ret\n");
}

void CODE_break_jmppoint (int lbl) {
    print_debugs(__FUNCTION__);
    fprintf(stdout, "__%04d__break:\n", lbl);
}

void CODE_continue_jmppoint (int lbl) {
    print_debugs(__FUNCTION__);
    fprintf(stdout, "__%04d__continue:\n", lbl);
}

void CODE_stack_restore (int i) {
    print_debugs(__FUNCTION__);
    if (i) {
        fprintf(stdout, "    add  $%u,%%rsp\n", i);
    }
}

void CODE_var_declarations_space (int i) {
    print_debugs(__FUNCTION__);
    if (i) {
        fprintf(stdout, "    sub  $%u,%%rsp\n", i);
    }
}


void CODE_if_statement_head (int lbl) {
    print_debugs(__FUNCTION__);
    fprintf(stdout, "    cmp $0x0, %%rax\n");
    fprintf(stdout, "    jz  if_continue_%04d\n", lbl);
}

void CODE_if_statement_mid (int lbl) {
    print_debugs(__FUNCTION__);
    fprintf(stdout, "    jmp endif_%04d\n", lbl);
    fprintf(stdout, "if_continue_%04d:\n", lbl);
}

void CODE_if_statement_end_else (int lbl) {
    print_debugs(__FUNCTION__);
    fprintf(stdout, "endif_%04d:\n", lbl);
}

void CODE_if_statement_end_noelse (int lbl) {
    print_debugs(__FUNCTION__);
    fprintf(stdout, "if_continue_%04d:\n", lbl);
}


void CODE_while_statement_test (int lbl) {
    print_debugs(__FUNCTION__);
    fprintf(stdout, "while_%04d_test:\n", lbl);
}

void CODE_while_statement_evaluate (int lbl) {
    print_debugs(__FUNCTION__);
    fprintf(stdout, "    cmp $0x0, %%rax\n");
    fprintf(stdout, "    jz  while_%04d_end\n", lbl);
}

void CODE_while_statement_end (int lbl) {
    print_debugs(__FUNCTION__);
    fprintf(stdout, "    jmp while_%04d_test\n", lbl);
    fprintf(stdout, "while_%04d_end:\n", lbl);
}


void CODE_do_statement_base (int lbl) {
    print_debugs(__FUNCTION__);
    fprintf(stdout, "do_%04d_base:\n", lbl);
}

void CODE_do_statement_test (int lbl) {
    print_debugs(__FUNCTION__);
    fprintf(stdout, "    cmp $0x0, %%rax\n");
    fprintf(stdout, "    jz  do_%04d_end\n", lbl);
    fprintf(stdout, "    jmp do_%04d_base\n", lbl);
    fprintf(stdout, "do_%04d_end:\n", lbl);
}


void CODE_for_statement_test (int lbl) {
    print_debugs(__FUNCTION__);
    fprintf(stdout, "for_%04d_test:\n", lbl);
}

void CODE_for_statement_evaluate (int lbl) {
    print_debugs(__FUNCTION__);
    fprintf(stdout, "    cmp $0x0, %%rax\n");
    fprintf(stdout, "    jz  for_%04d_end\n", lbl);
}

void CODE_for_statement_jp_to_base (int lbl) {
    print_debugs(__FUNCTION__);
    fprintf(stdout, "    jmp for_%04d_base\n", lbl);
}

void CODE_for_statement_action (int lbl) {
    print_debugs(__FUNCTION__);
    fprintf(stdout, "for_%04d_action:\n", lbl);
}

void CODE_for_statement_jp_to_test (int lbl) {
    print_debugs(__FUNCTION__);
    fprintf(stdout, "    jmp for_%04d_test\n", lbl);
}

void CODE_for_statement_base (int lbl) {
    print_debugs(__FUNCTION__);
    fprintf(stdout, "for_%04d_base:\n", lbl);
}

void CODE_for_statement_end (int lbl) {
    print_debugs(__FUNCTION__);
    fprintf(stdout, "    jmp for_%04d_action\n", lbl);
    fprintf(stdout, "for_%04d_end:\n", lbl);
}


void CODE_asm_statement (char* s) {
    print_debugs(__FUNCTION__);
    fprintf(stdout, "%s", s);
}

void CODE_return_statement (int lbl, int d) {
    print_debugs(__FUNCTION__);
    if (d) {
        fprintf(stdout, "    add  $%u,%%rsp\n", d);
    }
    fprintf(stdout, "    jmp __%04d__return\n", lbl);
}

void CODE_break_statement (int lbl, int d) {
    print_debugs(__FUNCTION__);
    if (d) {
        fprintf(stdout, "    add  $%u,%%rsp\n", d);
    }
    fprintf(stdout, "    jmp __%04d__break\n", lbl);
}

void CODE_continue_statement (int lbl, int d) {
    print_debugs(__FUNCTION__);
    if (d) {
        fprintf(stdout, "    add  $%u,%%rsp\n", d);
    }
    fprintf(stdout, "    jmp __%04d__continue\n", lbl);
}

void CODE_logical_neg (int lbl) {
    print_debugs(__FUNCTION__);
    fprintf(stdout, "    cmp $0x0, %%rax\n");
    fprintf(stdout, "    jz  log_neg_%04d\n", lbl);
    fprintf(stdout, "    mov $0x0,%%rax\n");
    fprintf(stdout, "    jmp log_neg_%04d_end\n", lbl);
    fprintf(stdout, "log_neg_%04d:\n", lbl);
    fprintf(stdout, "    mov $0x1,%%rax\n");
    fprintf(stdout, "log_neg_%04d_end:\n", lbl);
}

void CODE_bitwise_neg (void) {
    print_debugs(__FUNCTION__);
    fprintf(stdout, "    inv(frm_acc)\n");
}

void CODE_push (void) {
    print_debugs(__FUNCTION__);
    fprintf(stdout, "    push %%rax                 # 1st operand push\n");
}

void CODE_push_unsafe (void) {
    print_debugs(__FUNCTION__);
    fprintf(stdout, "    push %%rax                 # 1st operand push\n");
}

void CODE_operand_pop (void) {
    print_debugs(__FUNCTION__);
    fprintf(stdout, "    mov  %%rax, %%rbx          # store 2nd operand\n");
    fprintf(stdout, "    pop  %%rax                 # 1st operand pop\n");
}

void CODE_do_operation_mul (int lbl) {
    print_debugs(__FUNCTION__);
    CODE_operand_pop();
    fprintf(stdout, "    mul  %%rbx\n");
}

void CODE_do_operation_div (void) {
    print_debugs(__FUNCTION__);
    CODE_operand_pop();
    fprintf(stdout, "    xor  %%rdx, %%rdx\n");
    fprintf(stdout, "    div  %%rbx\n");
}

void CODE_do_operation_mod (void) {
    print_debugs(__FUNCTION__);
    CODE_operand_pop();
    fprintf(stdout, "    xor  %%rdx, %%rdx\n");
    fprintf(stdout, "    div  %%rbx\n");
    fprintf(stdout, "    mov  %%rdx, %%rax\n");
}

void CODE_do_operation_add (void) {
    print_debugs(__FUNCTION__);
    CODE_operand_pop();
    fprintf(stdout, "    add  %%rbx, %%rax\n");
}

void CODE_do_operation_sub (void) {
    print_debugs(__FUNCTION__);
    CODE_operand_pop();
    fprintf(stdout, "    sub  %%rbx, %%rax\n");
}

void CODE_do_operation_shl (int lbl) {
    print_debugs(__FUNCTION__);
    CODE_operand_pop();
    fprintf(stdout, "    shl  %%rbx, %%rax\n");
}

void CODE_do_operation_shr (int lbl) {
    print_debugs(__FUNCTION__);
    CODE_operand_pop();
    fprintf(stdout, "    shr  %%rbx, %%rax\n");
}


void CODE_do_operation_compare_eq (int lbl) {
    print_debugs(__FUNCTION__);
    CODE_do_operation_sub();
    fprintf(stdout, "    cmp $0x0,%%rax\n");
    fprintf(stdout, "    jz  eq_%04d\n", lbl);
    fprintf(stdout, "    mov $0x0,%%rax\n");
    fprintf(stdout, "    jmp eq_%04d_end\n", lbl);
    fprintf(stdout, "eq_%04d:\n", lbl);
    fprintf(stdout, "    mov $0x1,%%rax\n");
    fprintf(stdout, "eq_%04d_end:\n", lbl);
}

void CODE_do_operation_compare_neq (int lbl) {
    print_debugs(__FUNCTION__);
    CODE_do_operation_sub();
    fprintf(stdout, "    cmp $0x0, %%rax\n");
    fprintf(stdout, "    jz  neq_%04d\n", lbl);
    fprintf(stdout, "    mov $0x1,%%rax\n");
    fprintf(stdout, "    jmp neq_%04d_end\n", lbl);
    fprintf(stdout, "neq_%04d:\n", lbl);
    fprintf(stdout, "    mov $0x0,%%rax\n");
    fprintf(stdout, "neq_%04d_end:\n", lbl);
}

void CODE_do_operation_compare_less (int lbl) {
    print_debugs(__FUNCTION__);
    CODE_operand_pop();
    fprintf(stdout, "    cmp %%rbx, %%rax\n");
    fprintf(stdout, "    jc  less_%04d\n", lbl);
    fprintf(stdout, "    mov $0x0,%%rax\n");
    fprintf(stdout, "    jmp less_%04d_end\n", lbl);
    fprintf(stdout, "less_%04d:\n", lbl);
    fprintf(stdout, "    mov $0x1,%%rax\n");
    fprintf(stdout, "less_%04d_end:\n", lbl);
}

void CODE_do_operation_compare_greq (int lbl) {
    print_debugs(__FUNCTION__);
    CODE_operand_pop();
    fprintf(stdout, "    cmp %%rbx, %%rax\n");
    fprintf(stdout, "    jc  greq_%04d\n", lbl);
    fprintf(stdout, "    mov $0x1,%%rax\n");
    fprintf(stdout, "    jmp greq_%04d_end\n", lbl);
    fprintf(stdout, "greq_%04d:\n", lbl);
    fprintf(stdout, "    mov $0x0,%%rax\n");
    fprintf(stdout, "greq_%04d_end:\n", lbl);
}


void CODE_do_operation_bwand (int lbl) {
    print_debugs(__FUNCTION__);
    CODE_operand_pop();
    fprintf(stdout, "    and  %%rbx, %%rax\n");
}

void CODE_do_operation_bwxor (void) {
    print_debugs(__FUNCTION__);
    CODE_operand_pop();
    fprintf(stdout, "    xor  %%rbx, %%rax\n");
}

void CODE_do_operation_bwor (void) {
    print_debugs(__FUNCTION__);
    CODE_operand_pop();
    fprintf(stdout, "    or   %%rbx, %%rax\n");
}



void CODE_ternary_cond_test (int lbl) {
    print_debugs(__FUNCTION__);
    fprintf(stdout, "    cmp $0x0, %%rax\n");
    fprintf(stdout, "    jz  ternary_second_%04d\n", lbl);
}

void CODE_ternary_cond_mid (int lbl) {
    print_debugs(__FUNCTION__);
    fprintf(stdout, "    jmp ternary_end_%04d\n", lbl);
    fprintf(stdout, "ternary_second_%04d:\n", lbl);
}

void CODE_ternary_cond_end (int lbl) {
    print_debugs(__FUNCTION__);
    fprintf(stdout, "ternary_end_%04d:\n", lbl);
}


void CODE_pop_addr_and_store (void) {
    print_debugs(__FUNCTION__);
    fprintf(stdout, "    pop  %%rdx\n");
    fprintf(stdout, "    mov  %%rax,(%%rdx)\n");
}

void CODE_dereference (void) {
    print_debugs(__FUNCTION__);
    fprintf(stdout, "    mov  (%%rax),%%rax\n");
}


void CODE_load_eff_addr_auto (int dist) {
    print_debugs(__FUNCTION__);
    fprintf(stdout, "    mov  %%rsp,%%rax\n");
    fprintf(stdout, "    add  $%d,%%rax\n", dist);
}


void CODE_const_expression_str (int lbl, char* c) {
    print_debugs(__FUNCTION__);
    fprintf(stdout, ".data\n");
    fprintf(stdout, "str_const_%04d:\n", lbl);
    fprintf(stdout, ".string        %s\n", c);
    fprintf(stdout, ".text\n");
    fprintf(stdout, "    mov  $str_const_%04d,%%rax    # str const\n", lbl);
}

void CODE_const_expression_int (int i) {
    print_debugs(__FUNCTION__);
    fprintf(stdout, "    mov  $0x%X,%%rax    # const\n", i);
}

void CODE_fn_call (int lbl, char* fn_name) {
    print_debugs(__FUNCTION__);
    fprintf(stdout, "    call %s\n", fn_name);
}


void CODE_caseblock_start (int lbl) {
    print_debugs(__FUNCTION__);
    fprintf(stdout, "    jmp case_eval_%04d\n", lbl);

}

void CODE_caseblock (int lbl, int next_lbl, int data_const) {
    print_debugs(__FUNCTION__);
    fprintf(stdout, "    jmp case_start_%04d\n", lbl);
    fprintf(stdout, "case_eval_%04d:\n", lbl);
    fprintf(stdout, "    cmp $0x%X, (%%rsp)   # case data\n", data_const);
    fprintf(stdout, "    jnz case_eval_%04d\n", next_lbl);
    fprintf(stdout, "case_start_%04d:\n", lbl);
}

void CODE_caseblock_end (int lbl) {
    print_debugs(__FUNCTION__);
    fprintf(stdout, "case_eval_%04d:\n", lbl);
}


