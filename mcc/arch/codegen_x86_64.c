#include "codegen.h"
#include <stdio.h>
#include <stdlib.h>



static void unimplemented (const char* s) {
    fprintf(stderr, "[%s] unimplemented on this architecture\n", s);
    exit(1);
}


static void print_debugs (const char* s) {
    fprintf(stdout, "# [%s]\n", s);
    ;
}


int SYM_integer_size (void) {
    return 8;                   /* 64 bit */
}

int SYM_data_pointer_size (void) {
    return 8;                   /* 64 bit */
}

int SYM_code_pointer_size (void) {
    return 8;                   /* 64 bit */
}

void CODE_func_definition_label (char* fn_name) {
    fprintf(stdout, "\n");
    print_debugs(__FUNCTION__);
    fprintf(stdout, "%s:\n", fn_name);
}

void CODE_func_definition_ret (char* fn_name) {
    print_debugs(__FUNCTION__);
    fprintf(stdout, "__%s__ret:\n", fn_name);
    fprintf(stdout, "    ret\n");
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
    fprintf(stdout, "    jz(\"if_continue_%04d\")\n", lbl);
}

void CODE_if_statement_mid (int lbl) {
    print_debugs(__FUNCTION__);
    fprintf(stdout, "    jp(\"endif_%04d\")\n", lbl);
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
    fprintf(stdout, "    jz(\"while_%04d_end\")\n", lbl);
}

void CODE_while_statement_end (int lbl) {
    print_debugs(__FUNCTION__);
    fprintf(stdout, "    jp(\"while_%04d_test\")\n", lbl);
    fprintf(stdout, "while_%04d_end:\n", lbl);
}


void CODE_do_statement_base (int lbl) {
    print_debugs(__FUNCTION__);
    fprintf(stdout, "do_%04d_base:\n", lbl);
}

void CODE_do_statement_test (int lbl) {
    print_debugs(__FUNCTION__);
    fprintf(stdout, "    jz(\"do_%04d_end\")\n", lbl);
    fprintf(stdout, "    jp(\"do_%04d_base\")\n", lbl);
    fprintf(stdout, "do_%04d_end:\n", lbl);
}


void CODE_for_statement_test (int lbl) {
    print_debugs(__FUNCTION__);
    fprintf(stdout, "for_%04d_test:\n", lbl);
}

void CODE_for_statement_evaluate (int lbl) {
    print_debugs(__FUNCTION__);
    fprintf(stdout, "    jz(\"for_%04d_end\")\n", lbl);
}

void CODE_for_statement_jp_to_base (int lbl) {
    print_debugs(__FUNCTION__);
    fprintf(stdout, "    jp(\"for_%04d_base\")\n", lbl);
}

void CODE_for_statement_action (int lbl) {
    print_debugs(__FUNCTION__);
    fprintf(stdout, "for_%04d_action:\n", lbl);
}

void CODE_for_statement_jp_to_test (int lbl) {
    print_debugs(__FUNCTION__);
    fprintf(stdout, "    jp(\"for_%04d_test\")\n", lbl);
}

void CODE_for_statement_base (int lbl) {
    print_debugs(__FUNCTION__);
    fprintf(stdout, "for_%04d_base:\n", lbl);
}

void CODE_for_statement_end (int lbl) {
    print_debugs(__FUNCTION__);
    fprintf(stdout, "    jp(\"for_%04d_action\")\n", lbl);
    fprintf(stdout, "for_%04d_end:\n", lbl);
}


void CODE_asm_statement (char* s) {
    print_debugs(__FUNCTION__);
    fprintf(stdout, "%s", s);
}

void CODE_return_statement (char* s, int d) {
    print_debugs(__FUNCTION__);
    if (d) {
        fprintf(stdout, "    inc_sp(%u)\n", SYM_integer_size() * d);
    }
    fprintf(stdout, "    jp(\"__%s__ret\")\n", s);
}

void CODE_logical_neg (int lbl) {
    print_debugs(__FUNCTION__);
    fprintf(stdout, "    jz(\"log_neg_%04d\")\n", lbl);
    fprintf(stdout, "    mov(to_acc, progdata) data(0)\n");
    fprintf(stdout, "    jp(\"log_neg_%04d_end\")\n", lbl);
    fprintf(stdout, "log_neg_%04d:\n", lbl);
    fprintf(stdout, "    mov(to_acc, progdata) data(1)\n");
    fprintf(stdout, "log_neg_%04d_end:\n", lbl);
}

void CODE_bitwise_neg (void) {
    print_debugs(__FUNCTION__);
    fprintf(stdout, "    inv(frm_acc)\n");
}

void CODE_push (void) {
    print_debugs(__FUNCTION__);
    fprintf(stdout, "    push(frm_acc)              // 1st operand push\n");
}

void CODE_push_unsafe (void) {
    print_debugs(__FUNCTION__);
    fprintf(stdout, "    push %%rax                 # 1st operand push\n");
}

void CODE_operand_pop (void) {
    print_debugs(__FUNCTION__);
    fprintf(stdout, "    st(BX, frm_acc)            // store 2nd operand\n");
    fprintf(stdout, "    pop(to_acc)                // 1st operand pop\n");
}

void CODE_do_operation_mul (int lbl) {
    unimplemented(__FUNCTION__);

    print_debugs(__FUNCTION__);

}

void CODE_do_operation_div (void) {
    unimplemented(__FUNCTION__);
}

void CODE_do_operation_mod (void) {
    unimplemented(__FUNCTION__);
}

void CODE_do_operation_add (void) {
    print_debugs(__FUNCTION__);
    CODE_operand_pop();
    fprintf(stdout, "    mov(to_mar, progdata) data(BX)  // addition\n");
    fprintf(stdout, "    add(frm_ram)\n");
}

void CODE_do_operation_sub (void) {
    print_debugs(__FUNCTION__);
    fprintf(stdout, "    inv(frm_acc)             // 1st cmpl\n");
    fprintf(stdout, "    inc()                        // 2nd cmpl      \n");
    CODE_operand_pop();
    fprintf(stdout, "    mov(to_mar, progdata) data(BX)\n");
    fprintf(stdout, "    add(frm_ram)\n");
}

void CODE_do_operation_shl (int lbl) {
    print_debugs(__FUNCTION__);
}

void CODE_do_operation_shr (int lbl) {
    print_debugs(__FUNCTION__);
}


void CODE_do_operation_compare_eq (int lbl) {
    print_debugs(__FUNCTION__);
    CODE_do_operation_sub();
    fprintf(stdout, "    jz(\"eq_%04d\")\n", lbl);
    fprintf(stdout, "    mov(to_acc, progdata) data(0)\n");
    fprintf(stdout, "    jp(\"eq_%04d_end\")\n", lbl);
    fprintf(stdout, "eq_%04d:\n", lbl);
    fprintf(stdout, "    mov(to_acc, progdata) data(1)\n");
    fprintf(stdout, "eq_%04d_end:\n", lbl);
}

void CODE_do_operation_compare_neq (int lbl) {
    print_debugs(__FUNCTION__);
    CODE_do_operation_sub();
    fprintf(stdout, "    jz(\"neq_%04d\")\n", lbl);
    fprintf(stdout, "    mov(to_acc, progdata) data(1)\n");
    fprintf(stdout, "    jp(\"neq_%04d_end\")\n", lbl);
    fprintf(stdout, "neq_%04d:\n", lbl);
    fprintf(stdout, "    mov(to_acc, progdata) data(0)\n");
    fprintf(stdout, "neq_%04d_end:\n", lbl);
}

void CODE_do_operation_compare_less (int lbl) {
    print_debugs(__FUNCTION__);
    CODE_do_operation_sub();
    CODE_push_unsafe();
    fprintf(stdout, "    call(\"msb\", \"compare_less_call_%04d\")\n", lbl);
    fprintf(stdout, "    inc_sp(%d)\n", SYM_integer_size()); // 1 function argument
}

void CODE_do_operation_compare_greq (int lbl) {
    print_debugs(__FUNCTION__);
    CODE_do_operation_sub();
    fprintf(stdout, "    inv(frm_acc)\n");
    CODE_push_unsafe();
    fprintf(stdout, "    call(\"msb\", \"compare_greq_call_%04d\")\n", lbl);
    fprintf(stdout, "    inc_sp(%d)\n", SYM_integer_size()); // 1 function argument
}


void CODE_do_operation_bwand (int lbl) {
    print_debugs(__FUNCTION__);
    CODE_push_unsafe();
    fprintf(stdout, "    call(\"bwand\", \"bwand_call_%04d\")\n", lbl);
    fprintf(stdout, "    inc_sp(%d)\n", SYM_integer_size() * 2); // 2 function arguments
}

void CODE_do_operation_bwxor (void) {
    unimplemented(__FUNCTION__);
}

void CODE_do_operation_bwor (void) {
    unimplemented(__FUNCTION__);
}



void CODE_ternary_cond_test (int lbl) {
    print_debugs(__FUNCTION__);
    fprintf(stdout, "    jz(\"ternary_second_%04d\")\n", lbl);
}

void CODE_ternary_cond_mid (int lbl) {
    print_debugs(__FUNCTION__);
    fprintf(stdout, "    jp(\"ternary_end_%04d\")\n", lbl);
    fprintf(stdout, "ternary_second_%04d:\n", lbl);
}

void CODE_ternary_cond_end (int lbl) {
    print_debugs(__FUNCTION__);
    fprintf(stdout, "ternary_end_%04d:\n", lbl);
}


void CODE_pop_addr_and_store (void) {
    print_debugs(__FUNCTION__);
    fprintf(stdout, "    pop(to_mar)\n");
    fprintf(stdout, "    mov(to_ram, frm_acc)\n");
}

void CODE_dereference (void) {
    print_debugs(__FUNCTION__);
    fprintf(stdout, "    mov  (%%rax),%%rax\n");
}


void CODE_load_eff_addr_auto (int pos) {
    print_debugs(__FUNCTION__);
    fprintf(stdout, "    mov  %%rsp,%%rax\n");
    fprintf(stdout, "    sub  $%d,%%rax\n", pos);
}


void CODE_const_expression_str (int lbl, char* c) {
    print_debugs(__FUNCTION__);
    fprintf(stdout, ".data\n");
    fprintf(stdout, "str_const_%04d:\n", lbl);
    fprintf(stdout, "    .string  %s\n", c);
    fprintf(stdout, ".text\n");
    fprintf(stdout, "    mov  $str_const_%04d,%%rax    # str const\n", lbl);
}

void CODE_const_expression_int (int i) {
    print_debugs(__FUNCTION__);
    fprintf(stdout, "    mov  $0x%X,%%rax    # const\n", i);
}

void CODE_fn_call (int lbl, char* fn_name) {
    print_debugs(__FUNCTION__);
    fprintf(stdout, "    call(\"%s\", \"call_%04d\")\n", fn_name, lbl);
}

void CODE_fn_call_args (void) {
    print_debugs(__FUNCTION__);
    fprintf(stdout, "    push_unsafe(frm_acc)                      // push fn arg\n");
}


