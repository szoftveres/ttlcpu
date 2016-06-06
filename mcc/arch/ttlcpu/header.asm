
/*-------------------------------------------------------*/

/* Emulated registers in data RAM */

#undef BX
#define BX              0xff
#undef SP
#define SP              0xfe
#undef STACK_ACC
#define STACK_ACC       0xfd
#undef STACK_TEMP
#define STACK_TEMP      0xfc
#undef PREVKEY
#define PREVKEY         0xfb

#undef SP_BASE
#define SP_BASE         0xfa

/*-------------------------------------------------------*/

#undef stackinit
#define stackinit()                 \
    st(SP, progdata) data(SP_BASE)

#undef call
#define call(l, u)                  \
    ld(to_acc, SP)                  \
    mov(to_mar, frm_acc)            \
    mov(to_ram, progdata) defh(u)   \
    dec()                           \
    mov(to_mar, frm_acc)            \
    mov(to_ram, progdata) defl(u)   \
    dec()                           \
    st(SP, frm_acc)                 \
    jp(l)                           \
lbl(u)

#undef ret
#define ret()                       \
    st(STACK_ACC, frm_acc)          \
    ld(to_acc, SP)                  \
    add(progdata) data(2)           \
    st(SP, frm_acc)                 \
    mov(to_mar, frm_acc)            \
    mov(to_pch, frm_ram)            \
    dec()                           \
    mov(to_mar, frm_acc)            \
    mov(to_acc, frm_ram)            \
    st(STACK_TEMP, frm_acc)         \
    ld(to_acc, STACK_ACC)           \
    ld(to_pc, STACK_TEMP)

#undef push
#define push(s)                     \
    st(STACK_ACC, frm_acc)          \
    ld(to_acc, SP)                  \
    st(STACK_TEMP, frm_acc)         \
    dec()                           \
    st(SP, frm_acc)                 \
    ld(to_acc, STACK_ACC)           \
    ld(to_mar, STACK_TEMP)          \
    mov(to_ram, (s))

#undef pop
#define pop(d)                      \
    if ((d) != to_acc) {            \
        st(STACK_ACC, frm_acc)      \
    }                               \
    ld(to_acc, SP)                  \
    inc()                           \
    st(SP, frm_acc)                 \
    if ((d) != to_acc) {            \
        ld(to_acc, STACK_ACC)       \
    }                               \
    ld(to_mar, SP)                  \
    mov((d), frm_ram)

#undef dec_sp
#define dec_sp(i)                   \
    ld(to_acc, SP)                  \
    add(progdata) data(-(i))        \
    st(SP, frm_acc)                 \

#undef inc_sp
#define inc_sp(i)                   \
    st(STACK_ACC, frm_acc)          \
    ld(to_acc, SP)                  \
    add(progdata) data(i)           \
    st(SP, frm_acc)                 \
    ld(to_acc, STACK_ACC)




/*-------------------------------------------------------*/

    /* Entry point of main program */
    stackinit()
    out(progdata) data(0)
    call("main", "__main_caller_ret")
    out(progdata) data(0)
lbl("__haltcpu")
    jp("__haltcpu")


/*-------------------------------------------------------*/

lbl("out")
    ld(to_acc, SP)    //mov ebp, esp
    add(progdata) data(3)
    mov(to_mar, frm_acc)
    out(frm_ram)
    ret()

lbl("in")
    in(to_acc)
    ret()
/*-------------------------------------------------------*/

/*
bit2(var a, var b) {
    var i;
    for (i=7-a ; i; i-=1);
    for (i=a ; i; i-=1);
}
*/
/* 
  bit (var <0-7>, var <data>) 

  return (data & (1 << var<0-7>));

*/

lbl("bit")
    dec_sp(1)       // var decl space
    // ld(to_acc, SP)          // local var value
    add(progdata) data(5)
    mov(to_mar, frm_acc)
    inv(frm_ram)               // 1st compliment
    add(progdata) data(8)       // 2nd compliment
    push(frm_acc)    // var = acc
    ld(to_acc, SP)
    add(progdata) data(1+1)
    st(BX, frm_acc)
    pop(to_acc)
    mov(to_mar, progdata) data(BX)
    mov(to_mar, frm_ram)
    mov(to_ram, frm_acc)
lbl("__bit_01_restart")      // teszt
    jz("__bit_01_end")
lbl("__bit_01_continue")       // ciklustorzs
    ld(to_acc, SP)          // local var value
    add(progdata) data(4)
    mov(to_mar, frm_acc)
    mov(to_acc, frm_ram)
    shl()
    mov(to_ram, frm_acc)
lbl("__bit_01_action")       // ciklusvaltozo noveles
    ld(to_acc, SP)          // local var value
    add(progdata) data(0+1)
    mov(to_mar, frm_acc)
    mov(to_acc, frm_ram)
    dec()
    mov(to_ram, frm_acc)
    jp("__bit_01_restart")
lbl("__bit_01_end")
    ld(to_acc, SP)          // local var value
    add(progdata) data(4)
    mov(to_mar, frm_acc)
    mov(to_acc, frm_ram)
    rol()
    mov(to_ram, frm_acc)
    ld(to_acc, SP)          // local var value
    add(progdata) data(5)
    mov(to_mar, frm_acc)
    mov(to_acc, frm_ram)
    push(frm_acc)    // var = acc
    ld(to_acc, SP)
    add(progdata) data(1+1)
    st(BX, frm_acc)
    pop(to_acc)
    mov(to_mar, progdata) data(BX)
    mov(to_mar, frm_ram)
    mov(to_ram, frm_acc)
lbl("__bit_02_restart")
    jz("__bit_02_end")
lbl("__bit_02_continue")
    ld(to_acc, SP)          // local var value
    add(progdata) data(4)
    mov(to_mar, frm_acc)
    mov(to_acc, frm_ram)
    shl()
    mov(to_ram, frm_acc)
lbl("__bit_02_action")
    ld(to_acc, SP)          // local var value
    add(progdata) data(0+1)
    mov(to_mar, frm_acc)
    mov(to_acc, frm_ram)
    dec()
    mov(to_ram, frm_acc)
    jp("__bit_02_restart")
lbl("__bit_02_end")
    ld(to_acc, SP)          // local var value
    add(progdata) data(4)
    mov(to_mar, frm_acc)
    mov(to_acc, frm_ram)
    inc_sp(1)      // var decl restore
    ret()



/*-------------------------------------------------------*/

/*

bwand (var a, var b) {
    var a_lcl;
    var b_lcl;
    var i;
    var cc;
    var ret;
    var ret_add;

    ret_add = 1;
    ret = 0;
    cc = 0;

    while(cc != 8) {
        a_lcl = a;
        b_lcl = b;

        i = 7-cc;
        while (i) {
            a_lcl;
            asm ("    shl()\n");
            asm ("    mov(to_ram, frm_acc)\n");
            b_lcl;
            asm ("    shl()\n");
            asm ("    mov(to_ram, frm_acc)\n");

            i;
            asm ("    dec()\n");
            asm ("    mov(to_ram, frm_acc)\n");
        }

        a_lcl;
        asm ("    rol()\n");
        asm ("    mov(to_ram, frm_acc)\n");
        b_lcl;
        asm ("    rol()\n");
        asm ("    mov(to_ram, frm_acc)\n");

        i = cc;
        while (i) {
            a_lcl;
            asm ("    shl()\n");
            asm ("    mov(to_ram, frm_acc)\n");
            b_lcl;
            asm ("    shl()\n");
            asm ("    mov(to_ram, frm_acc)\n");

            i;
            asm ("    dec()\n");
            asm ("    mov(to_ram, frm_acc)\n");
        }

        if (a_lcl) if(b_lcl) {
            ret += ret_add;
        }
        
        ret_add;
        asm ("    shl()\n");
        asm ("    mov(to_ram, frm_acc)\n");
        
        cc;
        asm ("    inc()\n");
        asm ("    mov(to_ram, frm_acc)\n");
    }
    ret;

}

*/  

lbl("bwand")
    dec_sp(6)

// ret_add = 1;
    // ld(to_acc, SP)
    add(progdata) data(0+1)
    mov(to_mar, frm_acc)
    mov(to_ram, progdata) data(1)

// ret = 0;
    inc()
    mov(to_mar, frm_acc)
    mov(to_ram, progdata) data(0)

// cc = 0;
    inc()
    mov(to_mar, frm_acc)
    mov(to_ram, progdata) data(0)

    mov(to_acc, frm_ram)

// while(cc != 8)
lbl("__bwand_rst_1")
    add(progdata) data(-8)
    jz("__bwand_done")

// a_lcl = a;
    ld(to_acc, SP)
    add(progdata) data(9+1)
    mov(to_mar, frm_acc)
    mov(to_acc, frm_ram)
    push(frm_acc)                  // 1st operand push
    ld(to_acc, SP)
    add(progdata) data(6+1)
    st(BX, frm_acc)                // store 2nd operand
    pop(to_acc)                      // 1st operand pop
    mov(to_mar, progdata) data(BX)
    mov(to_mar, frm_ram)
    mov(to_ram, frm_acc)

// b_lcl = b;
    ld(to_acc, SP)
    add(progdata) data(8+1)
    mov(to_mar, frm_acc)
    mov(to_acc, frm_ram)
    push(frm_acc)                  // 1st operand push
    ld(to_acc, SP)
    add(progdata) data(5+1)
    st(BX, frm_acc)                // store 2nd operand
    pop(to_acc)                      // 1st operand pop
    mov(to_mar, progdata) data(BX)
    mov(to_mar, frm_ram)
    mov(to_ram, frm_acc)

// i = 7-cc;
    ld(to_acc, SP)
    add(progdata) data(2+1)
    mov(to_mar, frm_acc)
    mov(to_acc_invert, frm_ram)
    inc()                        // 2nd cmpl      
    add(progdata) data(7)
    push(frm_acc)                  // 1st operand push
    ld(to_acc, SP)
    add(progdata) data(4+1)
    st(BX, frm_acc)                // store 2nd operand
    pop(to_acc)                      // 1st operand pop
    mov(to_mar, progdata) data(BX)
    mov(to_mar, frm_ram)
    mov(to_ram, frm_acc)

// while (i) {
lbl("__bwand_sl1_test")
    jz("__bwand_sl1_end")

    ld(to_acc, SP)
    add(progdata) data(5+1)
    mov(to_mar, frm_acc)
    mov(to_acc, frm_ram)
    shl()
    mov(to_ram, frm_acc)

    ld(to_acc, SP)
    add(progdata) data(4+1)
    mov(to_mar, frm_acc)
    mov(to_acc, frm_ram)
    shl()
    mov(to_ram, frm_acc)

    ld(to_acc, SP)
    add(progdata) data(3+1)
    mov(to_mar, frm_acc)
    mov(to_acc, frm_ram)
    dec()
    mov(to_ram, frm_acc)

    jp("__bwand_sl1_test")
lbl("__bwand_sl1_end")

    ld(to_acc, SP)
    add(progdata) data(5+1)
    mov(to_mar, frm_acc)
    mov(to_acc, frm_ram)
    rol()
    mov(to_ram, frm_acc)

    ld(to_acc, SP)
    add(progdata) data(4+1)
    mov(to_mar, frm_acc)
    mov(to_acc, frm_ram)
    rol()
    mov(to_ram, frm_acc)

// i = cc;
    ld(to_acc, SP)
    add(progdata) data(2+1)
    mov(to_mar, frm_acc)
    mov(to_acc, frm_ram)
    push(frm_acc)                  // 1st operand push
    ld(to_acc, SP)
    add(progdata) data(4+1)
    st(BX, frm_acc)                // store 2nd operand
    pop(to_acc)                      // 1st operand pop
    mov(to_mar, progdata) data(BX)
    mov(to_mar, frm_ram)
    mov(to_ram, frm_acc)

// while (i) {
lbl("__bwand_sl2_test")
    jz("__bwand_sl2_end")

    ld(to_acc, SP)
    add(progdata) data(5+1)
    mov(to_mar, frm_acc)
    mov(to_acc, frm_ram)
    shl()
    mov(to_ram, frm_acc)

    ld(to_acc, SP)
    add(progdata) data(4+1)
    mov(to_mar, frm_acc)
    mov(to_acc, frm_ram)
    shl()
    mov(to_ram, frm_acc)

    ld(to_acc, SP)
    add(progdata) data(3+1)
    mov(to_mar, frm_acc)
    mov(to_acc, frm_ram)
    dec()
    mov(to_ram, frm_acc)

    jp("__bwand_sl2_test")
lbl("__bwand_sl2_end")

// if (a_lcl) if(b_lcl)
    ld(to_acc, SP)
    add(progdata) data(5+1)
    mov(to_mar, frm_acc)
    mov(to_acc, frm_ram)
    jz("__bwand_neg")
    ld(to_acc, SP)
    add(progdata) data(4+1)
    mov(to_mar, frm_acc)
    mov(to_acc, frm_ram)
    jz("__bwand_neg")

// ret += ret_add;
    ld(to_acc, SP)
    add(progdata) data(1+1)
    mov(to_mar, frm_acc)
    mov(to_acc, frm_ram)
    push(frm_acc)                  // 1st operand push
    ld(to_acc, SP)
    add(progdata) data(1+1)
    mov(to_mar, frm_acc)
    mov(to_acc, frm_ram)
    st(BX, frm_acc)                // store 2nd operand
    pop(to_acc)                      // 1st operand pop
    mov(to_mar, progdata) data(BX)  // addition
    add(frm_ram)
    push(frm_acc)                  // 1st operand push
    ld(to_acc, SP)
    add(progdata) data(2+1)
    st(BX, frm_acc)                // store 2nd operand
    pop(to_acc)                      // 1st operand pop
    mov(to_mar, progdata) data(BX)
    mov(to_mar, frm_ram)
    mov(to_ram, frm_acc)
lbl("__bwand_neg")

    ld(to_acc, SP)
    add(progdata) data(0+1)
    mov(to_mar, frm_acc)
    mov(to_acc, frm_ram)
    shl()
    mov(to_ram, frm_acc)

    ld(to_acc, SP)
    add(progdata) data(2+1)
    mov(to_mar, frm_acc)
    mov(to_acc, frm_ram)
    inc()
    mov(to_ram, frm_acc)

    jp("__bwand_rst_1")
lbl("__bwand_done")
    ld(to_acc, SP)
    add(progdata) data(1+1)
    mov(to_mar, frm_acc)
    mov(to_acc, frm_ram)
    inc_sp(6)
    ret()

/*-------------------------------------------------------*/

/* Display And Scan */

/*

das2 () {
    var col;
    var inp;
    var row;
    var scancode;

    do {
        col = 0;
        scancode = 255;
        do {
            // Display
            col;
            asm("    mov(to_pch, frm_acc)\n");
            *col;
            asm("    out(frm_acc)\n");

            // Scan
            if (col, asm("    add(progdata) data(-8) // != 8\n")) { // col != 8
                inp = (asm("    in(to_acc)\n"));  // to_acc_invert ha 74HC541
                asm("jz(\"__das2_nokey_atall\")"); // found
                asm("    out(progdata) data(0)  // blank \n");

                row = 0;
                while (row, asm("add(progdata) data(-8) // != 8\n")) {
                    inp;
                    asm("add(progdata) data(128)");  // 0b10000000   MSB Xor
                    asm("jz(\"__das2_key\")"); // found
                    asm("jp(\"__das2_nokey\")");
                    asm("lbl(\"__das2_key\")");
                    scancode = (col , asm("shl() shl() shl()")) + row; // ((col << 3) + row)
                    row = 7;
                    col = 7;
                    asm("lbl(\"__das2_nokey\")");
                    inp = (inp, asm("   rol()\n"));
                    row = (row, asm("   inc()\n"));
                }
                asm("lbl(\"__das2_nokey_atall\")");
            }
            col = (col, asm("inc()"));
        } while (col, asm("    add(progdata) data(-9) // != 9\n")); // col != 9
        if (scancode, asm("    add(progdata) data(1) // scancode == 255\n"));
            *(PREVKEY) = scancode;     // release, 255 mindenhol
        }
    } while (scancode == (*(PREVKEY)));
    *(PREVKEY) = scancode;
}

*/



lbl("das")
    dec_sp(4)
lbl("__das_do_1_base")

    ld(to_acc, SP)
    add(progdata) data(3+1)
    mov(to_mar, frm_acc)
    mov(to_ram, progdata) data(0)

    ld(to_acc, SP)
    add(progdata) data(0+1)
    mov(to_mar, frm_acc)
    mov(to_ram, progdata) data(255)

lbl("__das_do_2_base")
    ld(to_acc, SP)
    add(progdata) data(3+1)
    mov(to_mar, frm_acc)
    mov(to_pch, frm_ram)

    ld(to_acc, SP)
    add(progdata) data(3+1)
    mov(to_mar, frm_acc)
    mov(to_mar, frm_ram)
    out(frm_ram)

    ld(to_acc, SP)
    add(progdata) data(3+1)
    mov(to_mar, frm_acc)
    mov(to_acc, frm_ram)
    add(progdata) data(-8) // != 8
    jz("__das_if_continue_1")

    ld(to_acc, SP)
    add(progdata) data(2+1)
    mov(to_mar, frm_acc)
    in(to_acc) // Could go directly to RAM
    mov(to_ram, frm_acc)

jz("__das_nokey_atall")    
    out(progdata) data(0)  // blank 

    ld(to_acc, SP)
    add(progdata) data(1+1)
    mov(to_mar, frm_acc)
    mov(to_ram, progdata) data(0)

lbl("__das_while_1_test")
    ld(to_acc, SP)
    add(progdata) data(1+1)
    mov(to_mar, frm_acc)
    mov(to_acc, frm_ram)
    add(progdata) data(-8) // != 8
    jz("__das_while_1_end")

    ld(to_acc, SP)
    add(progdata) data(2+1)
    mov(to_mar, frm_acc)
    mov(to_acc, frm_ram)
    add(progdata) data(128) // 0b10000000   MSB Xor

    jz("__das_key")
    jp("__das_nokey")
lbl("__das_key")
    ld(to_acc, SP)
    add(progdata) data(3+1)
    mov(to_mar, frm_acc)
    mov(to_acc, frm_ram)
    shl()
    shl()
    shl()
    st(BX, frm_acc)
    ld(to_acc, SP)
    add(progdata) data(1+1)
    mov(to_mar, frm_acc)
    mov(to_acc, frm_ram)
    mov(to_mar, progdata) data(BX)
    add(frm_ram)
    st(BX, frm_acc)
    ld(to_acc, SP)
    add(progdata) data(0+1)
    push(frm_acc)
    ld(to_acc, BX)
    pop(to_mar)
    mov(to_ram, frm_acc)


    ld(to_acc, SP)
    add(progdata) data(1+1)
    mov(to_mar, frm_acc)
    mov(to_ram, progdata) data(7)

    ld(to_acc, SP)
    add(progdata) data(3+1)
    mov(to_mar, frm_acc)
    mov(to_ram, progdata) data(7)


lbl("__das_nokey")

    ld(to_acc, SP)
    add(progdata) data(2+1)
    push(frm_acc)                  // 1st operand push
    ld(to_acc, SP)
    add(progdata) data(3+1)
    mov(to_mar, frm_acc)
    mov(to_acc, frm_ram)
    rol()
    pop(to_mar)
    mov(to_ram, frm_acc)

    ld(to_acc, SP)
    add(progdata) data(1+1)
    push(frm_acc)                  // 1st operand push
    ld(to_acc, SP)
    add(progdata) data(2+1)
    mov(to_mar, frm_acc)
    mov(to_acc, frm_ram)
    inc()
    pop(to_mar)
    mov(to_ram, frm_acc)

    jp("__das_while_1_test")

lbl("__das_while_1_end")
lbl("__das_nokey_atall")
lbl("__das_if_continue_1")

    ld(to_acc, SP)
    add(progdata) data(3+1)
    push(frm_acc)                  // 1st operand push
    ld(to_acc, SP)
    add(progdata) data(4+1)
    mov(to_mar, frm_acc)
    mov(to_acc, frm_ram)
    inc()
    pop(to_mar)
    mov(to_ram, frm_acc)

    ld(to_acc, SP)
    add(progdata) data(3+1)
    mov(to_mar, frm_acc)
    mov(to_acc, frm_ram)
    add(progdata) data(-9) // != 9

    jz("__das_do_2_end")
    jp("__das_do_2_base")
lbl("__das_do_2_end")
    ld(to_acc, SP)
    add(progdata) data(0+1)
    mov(to_mar, frm_acc)
    mov(to_acc, frm_ram)
    add(progdata) data(1) // == 255
    jz("__das_if_continue_2")
    jp("__das_endif_2")
lbl("__das_if_continue_2")
    
    ld(to_acc, SP)
    add(progdata) data(0+1)
    mov(to_mar, frm_acc)
    mov(to_acc, frm_ram)
    mov(to_mar, progdata) data(PREVKEY)
    mov(to_ram, frm_acc)

lbl("__das_endif_2")

    ld(to_acc, SP)
    add(progdata) data(0+1)
    mov(to_mar, frm_acc)
    inv(frm_ram)             // 1st cmpl
    inc()                        // 2nd cmpl      
    mov(to_mar, progdata) data(PREVKEY)      // const
    add(frm_ram)

    jz("__das_do_1_base")
    
    ld(to_acc, SP)
    add(progdata) data(0+1)
    mov(to_mar, frm_acc)
    mov(to_acc, frm_ram)
    mov(to_mar, progdata) data(PREVKEY)
    mov(to_ram, frm_acc)
    inc_sp(4)
    ret()


/*-------------------------------------------------------*/



/*

disp (var c) {
    var g;
    while (c) {
        g = 0;
        do {
            g;
            asm("    mov(to_pch, frm_acc)\n");
            *g;
            asm("    out(frm_acc)\n");
            g += 1;
        } while (g, asm("add(progdata) data(-9) // != 9\n")); // g != 9
        c = (c, asm("add(progdata) data(255)"));
    }
}


*/



lbl("disp")
    dec_sp(1)

lbl("__disp_while_test")
    ld(to_acc, SP)
    add(progdata) data(3+1)
    mov(to_mar, frm_acc)
    mov(to_acc, frm_ram)
    jz("__disp_while_end")

    ld(to_acc, SP)
    add(progdata) data(0+1)
    mov(to_mar, frm_acc)
    mov(to_ram, progdata) data(0)

lbl("__disp_do_base")
    ld(to_acc, SP)
    add(progdata) data(0+1)
    mov(to_mar, frm_acc)
    mov(to_pch, frm_ram)

    ld(to_acc, SP)
    add(progdata) data(0+1)
    mov(to_mar, frm_acc)
    mov(to_mar, frm_ram)
    out(frm_ram)

    // += 1
    ld(to_acc, SP)
    add(progdata) data(0+1)
    mov(to_mar, frm_acc)
    mov(to_acc, frm_ram)
    add(progdata) data(1)
    mov(to_ram, frm_acc)

     // != 9
    add(progdata) data(-9)
    jz("__disp_do_end")
    jp("__disp_do_base")
lbl("__disp_do_end")
    ld(to_acc, SP)
    add(progdata) data(3+1)
    mov(to_mar, frm_acc)
    mov(to_acc, frm_ram)
    add(progdata) data(255)  // -1
    mov(to_ram, frm_acc)

    jp("__disp_while_test")
lbl("__disp_while_end")
    inc_sp(1)
    ret()

/*-------------------------------------------------------*/

/*
 * Push one symbol from the right

disp_push (var s) {
    var i;
    i = 7;
    while (i) {
        *(i) = (*(i, asm("add(progdata) data(255)")));
        i = (i, asm("add(progdata) data(255)"));
    }
    *(0) = s;
}

 */

lbl("disp_push")
    dec_sp(1)
    // ld(to_acc, SP)
    add(progdata) data(0+1)
    mov(to_mar, frm_acc)
    mov(to_ram, progdata) data(7)      // const
    mov(to_acc, frm_ram)
lbl("__disp_push_loop_base")

    ld(to_acc, SP)
    add(progdata) data(0+1)
    mov(to_mar, frm_acc)
    mov(to_acc, frm_ram)
    st(BX, frm_acc)
    add(progdata) data(255)
    mov(to_mar, frm_acc)
    mov(to_acc, frm_ram)
    ld(to_mar, BX)
    mov(to_ram, frm_acc)

    ld(to_acc, SP)
    add(progdata) data(0+1)
    mov(to_mar, frm_acc)
    mov(to_acc, frm_ram)
    add(progdata) data(255)
    mov(to_ram, frm_acc)
    jz("__disp_push_loop_end")
    jp("__disp_push_loop_base")
lbl("__disp_push_loop_end")
    ld(to_acc, SP)
    add(progdata) data(3+1)
    mov(to_mar, frm_acc)
    mov(to_acc, frm_ram)
    mov(to_mar, progdata) data(0)
    mov(to_ram, frm_acc)
    inc_sp(1)
    ret()

/*-------------------------------------------------------*/

