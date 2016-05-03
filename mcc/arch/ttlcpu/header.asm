
/*-------------------------------------------------------*/

/* Emulated x86-like CPU registers */

#undef BX
#define BX              0xff


/*-------------------------------------------------------*/

/* Emulated Stack implementation */

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
    st(SP, literal) lit(SP_BASE)

#undef call
#define call(l, u)                  \
    ld(to_acc, SP)                  \
    mov(to_ramaddr, frm_acc)        \
    mov(to_ram, literal) defh(u)    \
    dec()                           \
    mov(to_ramaddr, frm_acc)        \
    mov(to_ram, literal) defl(u)    \
    dec()                           \
    st(SP, frm_acc)                 \
    jp(l)                           \
lbl(u)

#undef ret
#define ret()                       \
    st(STACK_ACC, frm_acc)          \
    ld(to_acc, SP)                  \
    add(literal) lit(2)             \
    st(SP, frm_acc)                 \
    mov(to_ramaddr, frm_acc)        \
    mov(to_pch, frm_ram)            \
    dec()                           \
    mov(to_ramaddr, frm_acc)        \
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
    ld(to_ramaddr, STACK_TEMP)      \
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
    ld(to_ramaddr, SP)              \
    mov((d), frm_ram)

#undef dec_sp
#define dec_sp(i)                   \
    ld(to_acc, SP)                  \
    add(literal) lit(-(i))          \
    st(SP, frm_acc)                 \

#undef inc_sp
#define inc_sp(i)                   \
    st(STACK_ACC, frm_acc)          \
    ld(to_acc, SP)                  \
    add(literal) lit(i)             \
    st(SP, frm_acc)                 \
    ld(to_acc, STACK_ACC)




/*-------------------------------------------------------*/

    /* Entry point of main program */
    stackinit()
    out(literal) lit(0)
    call("main", "main_caller_ret")
    out(literal) lit(0)
lbl("haltcpu")
    jp("haltcpu")


/*-------------------------------------------------------*/

lbl("out")
    ld(to_acc, SP)    //mov ebp, esp
    add(literal) lit(3)
    mov(to_ramaddr, frm_acc)
    out(frm_ram)
    ret()

lbl("in")
    in(to_acc)
    ret()

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
    ld(to_acc, SP)          // local var value
    add(literal) lit(5)
    mov(to_ramaddr, frm_acc)
    inv(frm_ram)               // 1st compliment
    add(literal) lit(8)       // 2nd compliment
    push(frm_acc)    // var = acc
    ld(to_acc, SP)
    add(literal) lit(1+1)
    st(BX, frm_acc)
    pop(to_acc)
    mov(to_ramaddr, literal) lit(BX)
    mov(to_ramaddr, frm_ram)
    mov(to_ram, frm_acc)
lbl("bit_01_restart")      // teszt
    jz("bit_01_end")
lbl("bit_01_continue")       // ciklustorzs
    ld(to_acc, SP)          // local var value
    add(literal) lit(4)
    mov(to_ramaddr, frm_acc)
    mov(to_acc, frm_ram)
    shl()
    mov(to_ram, frm_acc)
lbl("bit_01_action")       // ciklusvaltozo noveles
    ld(to_acc, SP)          // local var value
    add(literal) lit(0+1)
    mov(to_ramaddr, frm_acc)
    mov(to_acc, frm_ram)
    dec()
    mov(to_ram, frm_acc)
    jp("bit_01_restart")
lbl("bit_01_end")
    ld(to_acc, SP)          // local var value
    add(literal) lit(4)
    mov(to_ramaddr, frm_acc)
    mov(to_acc, frm_ram)
    rol()
    mov(to_ram, frm_acc)
    ld(to_acc, SP)          // local var value
    add(literal) lit(5)
    mov(to_ramaddr, frm_acc)
    mov(to_acc, frm_ram)
    push(frm_acc)    // var = acc
    ld(to_acc, SP)
    add(literal) lit(1+1)
    st(BX, frm_acc)
    pop(to_acc)
    mov(to_ramaddr, literal) lit(BX)
    mov(to_ramaddr, frm_ram)
    mov(to_ram, frm_acc)
lbl("bit_02_restart")
    jz("bit_02_end")
lbl("bit_02_continue")
    ld(to_acc, SP)          // local var value
    add(literal) lit(4)
    mov(to_ramaddr, frm_acc)
    mov(to_acc, frm_ram)
    shl()
    mov(to_ram, frm_acc)
lbl("bit_02_action")
    ld(to_acc, SP)          // local var value
    add(literal) lit(0+1)
    mov(to_ramaddr, frm_acc)
    mov(to_acc, frm_ram)
    dec()
    mov(to_ram, frm_acc)
    jp("bit_02_restart")
lbl("bit_02_end")
    ld(to_acc, SP)          // local var value
    add(literal) lit(4)
    mov(to_ramaddr, frm_acc)
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
    ld(to_acc, SP)
    add(literal) lit(0+1)
    mov(to_ramaddr, frm_acc)
    mov(to_ram, literal) lit(1)

// ret = 0;
    inc()
    mov(to_ramaddr, frm_acc)
    mov(to_ram, literal) lit(0)

// cc = 0;
    inc()
    mov(to_ramaddr, frm_acc)
    mov(to_ram, literal) lit(0)

    mov(to_acc, frm_ram)

// while(cc != 8)
lbl("bwand_rst_1")
    add(literal) lit(-8)
    jz("bwand_done")

// a_lcl = a;
    ld(to_acc, SP)
    add(literal) lit(9+1)
    mov(to_ramaddr, frm_acc)
    mov(to_acc, frm_ram)
    push(frm_acc)                  // 1st operand push
    ld(to_acc, SP)
    add(literal) lit(6+1)
    st(BX, frm_acc)                // store 2nd operand
    pop(to_acc)                      // 1st operand pop
    mov(to_ramaddr, literal) lit(BX)
    mov(to_ramaddr, frm_ram)
    mov(to_ram, frm_acc)

// b_lcl = b;
    ld(to_acc, SP)
    add(literal) lit(8+1)
    mov(to_ramaddr, frm_acc)
    mov(to_acc, frm_ram)
    push(frm_acc)                  // 1st operand push
    ld(to_acc, SP)
    add(literal) lit(5+1)
    st(BX, frm_acc)                // store 2nd operand
    pop(to_acc)                      // 1st operand pop
    mov(to_ramaddr, literal) lit(BX)
    mov(to_ramaddr, frm_ram)
    mov(to_ram, frm_acc)

// i = 7-cc;
    ld(to_acc, SP)
    add(literal) lit(2+1)
    mov(to_ramaddr, frm_acc)
    mov(to_acc_invert, frm_ram)
    inc()                        // 2nd cmpl      
    add(literal) lit(7)
    push(frm_acc)                  // 1st operand push
    ld(to_acc, SP)
    add(literal) lit(4+1)
    st(BX, frm_acc)                // store 2nd operand
    pop(to_acc)                      // 1st operand pop
    mov(to_ramaddr, literal) lit(BX)
    mov(to_ramaddr, frm_ram)
    mov(to_ram, frm_acc)

// while (i) {
lbl("bwand_sl1_test")
    jz("bwand_sl1_end")

    ld(to_acc, SP)
    add(literal) lit(5+1)
    mov(to_ramaddr, frm_acc)
    mov(to_acc, frm_ram)
    shl()
    mov(to_ram, frm_acc)

    ld(to_acc, SP)
    add(literal) lit(4+1)
    mov(to_ramaddr, frm_acc)
    mov(to_acc, frm_ram)
    shl()
    mov(to_ram, frm_acc)

    ld(to_acc, SP)
    add(literal) lit(3+1)
    mov(to_ramaddr, frm_acc)
    mov(to_acc, frm_ram)
    dec()
    mov(to_ram, frm_acc)

    jp("bwand_sl1_test")
lbl("bwand_sl1_end")

    ld(to_acc, SP)
    add(literal) lit(5+1)
    mov(to_ramaddr, frm_acc)
    mov(to_acc, frm_ram)
    rol()
    mov(to_ram, frm_acc)

    ld(to_acc, SP)
    add(literal) lit(4+1)
    mov(to_ramaddr, frm_acc)
    mov(to_acc, frm_ram)
    rol()
    mov(to_ram, frm_acc)

// i = cc;
    ld(to_acc, SP)
    add(literal) lit(2+1)
    mov(to_ramaddr, frm_acc)
    mov(to_acc, frm_ram)
    push(frm_acc)                  // 1st operand push
    ld(to_acc, SP)
    add(literal) lit(4+1)
    st(BX, frm_acc)                // store 2nd operand
    pop(to_acc)                      // 1st operand pop
    mov(to_ramaddr, literal) lit(BX)
    mov(to_ramaddr, frm_ram)
    mov(to_ram, frm_acc)

// while (i) {
lbl("bwand_sl2_test")
    jz("bwand_sl2_end")

    ld(to_acc, SP)
    add(literal) lit(5+1)
    mov(to_ramaddr, frm_acc)
    mov(to_acc, frm_ram)
    shl()
    mov(to_ram, frm_acc)

    ld(to_acc, SP)
    add(literal) lit(4+1)
    mov(to_ramaddr, frm_acc)
    mov(to_acc, frm_ram)
    shl()
    mov(to_ram, frm_acc)

    ld(to_acc, SP)
    add(literal) lit(3+1)
    mov(to_ramaddr, frm_acc)
    mov(to_acc, frm_ram)
    dec()
    mov(to_ram, frm_acc)

    jp("bwand_sl2_test")
lbl("bwand_sl2_end")

// if (a_lcl) if(b_lcl)
    ld(to_acc, SP)
    add(literal) lit(5+1)
    mov(to_ramaddr, frm_acc)
    mov(to_acc, frm_ram)
    jz("bwand_neg")
    ld(to_acc, SP)
    add(literal) lit(4+1)
    mov(to_ramaddr, frm_acc)
    mov(to_acc, frm_ram)
    jz("bwand_neg")

// ret += ret_add;
    ld(to_acc, SP)
    add(literal) lit(1+1)
    mov(to_ramaddr, frm_acc)
    mov(to_acc, frm_ram)
    push(frm_acc)                  // 1st operand push
    ld(to_acc, SP)
    add(literal) lit(1+1)
    mov(to_ramaddr, frm_acc)
    mov(to_acc, frm_ram)
    st(BX, frm_acc)                // store 2nd operand
    pop(to_acc)                      // 1st operand pop
    mov(to_ramaddr, literal) lit(BX)  // addition
    add(frm_ram)
    push(frm_acc)                  // 1st operand push
    ld(to_acc, SP)
    add(literal) lit(2+1)
    st(BX, frm_acc)                // store 2nd operand
    pop(to_acc)                      // 1st operand pop
    mov(to_ramaddr, literal) lit(BX)
    mov(to_ramaddr, frm_ram)
    mov(to_ram, frm_acc)
lbl("bwand_neg")

    ld(to_acc, SP)
    add(literal) lit(0+1)
    mov(to_ramaddr, frm_acc)
    mov(to_acc, frm_ram)
    shl()
    mov(to_ram, frm_acc)

    ld(to_acc, SP)
    add(literal) lit(2+1)
    mov(to_ramaddr, frm_acc)
    mov(to_acc, frm_ram)
    inc()
    mov(to_ram, frm_acc)

    jp("bwand_rst_1")
lbl("bwand_done")
    ld(to_acc, SP)
    add(literal) lit(1+1)
    mov(to_ramaddr, frm_acc)
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
    var scan;

    do {
        col = 0;
        scan = 255;
        do {
            col;
            asm("    mov(to_pch, frm_acc)\n");


            *col;
            asm("    out(frm_acc)\n");

            if (col, asm("    add(literal) lit(-8) // != 8\n")) { // col != 8
                inp = (asm("    in(to_acc)\n"));
                asm("jz(\"das2_nokey_atall\")"); // found
                asm("    out(literal) lit(0)  // blank \n");

                row = 0;
                while (row, asm("add(literal) lit(-8) // != 8\n")) {
                    inp;
                    asm("inv(frm_acc)"); // ez nem kell ha 74HC541
                    asm("add(literal) lit(128)");
                    asm("inv(frm_acc)");
                    asm("jz(\"das2_key\")"); // found
                    asm("jp(\"das2_nokey\")");
                    asm("lbl(\"das2_key\")");
                    scan = (col , asm("shl() shl() shl()")) + row;
                    row = 7;
                    col = 7;
                    asm("lbl(\"das2_nokey\")");
                    inp = (inp, asm("   rol()\n"));
                    row = (row, asm("   inc()\n"));
                }
                asm("lbl(\"das2_nokey_atall\")");
            }
            col = (col, asm("inc()"));
        } while (col, asm("    add(literal) lit(-9) // != 9\n")); // col != 9
        if (scan, asm("    add(literal) lit(1) // != 255\n")) { } else {
            *(16) = scan;     // release, 255 mindenhol
        }
    } while (scan == (*(16)));
    *(16) = scan;
}
*/



lbl("das")
    dec_sp(4)
lbl("das_do_1_base")

    ld(to_acc, SP)
    add(literal) lit(3+1)
    mov(to_ramaddr, frm_acc)
    mov(to_ram, literal) lit(0)

    ld(to_acc, SP)
    add(literal) lit(0+1)
    mov(to_ramaddr, frm_acc)
    mov(to_ram, literal) lit(255)

lbl("das_do_2_base")
    ld(to_acc, SP)
    add(literal) lit(3+1)
    mov(to_ramaddr, frm_acc)
    mov(to_pch, frm_ram)

    ld(to_acc, SP)
    add(literal) lit(3+1)
    mov(to_ramaddr, frm_acc)
    mov(to_acc, frm_ram)
    mov(to_ramaddr, frm_acc)
    out(frm_ram)

    ld(to_acc, SP)
    add(literal) lit(3+1)
    mov(to_ramaddr, frm_acc)
    mov(to_acc, frm_ram)
    add(literal) lit(-8) // != 8
    jz("das_if_continue_1")

    ld(to_acc, SP)
    add(literal) lit(2+1)
    mov(to_ramaddr, frm_acc)
    in(to_acc)
    mov(to_ram, frm_acc)

jz("das_nokey_atall")    
    out(literal) lit(0)  // blank 

    ld(to_acc, SP)
    add(literal) lit(1+1)
    mov(to_ramaddr, frm_acc)
    mov(to_ram, literal) lit(0)

lbl("das_while_1_test")
    ld(to_acc, SP)
    add(literal) lit(1+1)
    mov(to_ramaddr, frm_acc)
    mov(to_acc, frm_ram)
    add(literal) lit(-8) // != 8
    jz("das_while_1_end")

    ld(to_acc, SP)
    add(literal) lit(2+1)
    mov(to_ramaddr, frm_acc)
    inv(frm_ram)
    add(literal) lit(128)
    inv(frm_acc)

    jz("das_key")
    jp("das_nokey")
lbl("das_key")
    ld(to_acc, SP)
    add(literal) lit(3+1)
    mov(to_ramaddr, frm_acc)
    mov(to_acc, frm_ram)
    shl()
    shl()
    shl()
    st(BX, frm_acc)
    ld(to_acc, SP)
    add(literal) lit(1+1)
    mov(to_ramaddr, frm_acc)
    mov(to_acc, frm_ram)
    mov(to_ramaddr, literal) lit(BX)
    add(frm_ram)
    st(BX, frm_acc)
    ld(to_acc, SP)
    add(literal) lit(0+1)
    push(frm_acc)
    ld(to_acc, BX)
    pop(to_ramaddr)
    mov(to_ram, frm_acc)


    ld(to_acc, SP)
    add(literal) lit(1+1)
    mov(to_ramaddr, frm_acc)
    mov(to_ram, literal) lit(7)

    ld(to_acc, SP)
    add(literal) lit(3+1)
    mov(to_ramaddr, frm_acc)
    mov(to_ram, literal) lit(7)


lbl("das_nokey")

    ld(to_acc, SP)
    add(literal) lit(2+1)
    push(frm_acc)                  // 1st operand push
    ld(to_acc, SP)
    add(literal) lit(3+1)
    mov(to_ramaddr, frm_acc)
    mov(to_acc, frm_ram)
    rol()
    pop(to_ramaddr)
    mov(to_ram, frm_acc)

    ld(to_acc, SP)
    add(literal) lit(1+1)
    push(frm_acc)                  // 1st operand push
    ld(to_acc, SP)
    add(literal) lit(2+1)
    mov(to_ramaddr, frm_acc)
    mov(to_acc, frm_ram)
    inc()
    pop(to_ramaddr)
    mov(to_ram, frm_acc)

    jp("das_while_1_test")

lbl("das_while_1_end")
lbl("das_nokey_atall")
lbl("das_if_continue_1")

    ld(to_acc, SP)
    add(literal) lit(3+1)
    push(frm_acc)                  // 1st operand push
    ld(to_acc, SP)
    add(literal) lit(4+1)
    mov(to_ramaddr, frm_acc)
    mov(to_acc, frm_ram)
    inc()
    pop(to_ramaddr)
    mov(to_ram, frm_acc)

    ld(to_acc, SP)
    add(literal) lit(3+1)
    mov(to_ramaddr, frm_acc)
    mov(to_acc, frm_ram)
    add(literal) lit(-9) // != 9

    jz("das_do_2_end")
    jp("das_do_2_base")
lbl("das_do_2_end")
    ld(to_acc, SP)
    add(literal) lit(0+1)
    mov(to_ramaddr, frm_acc)
    mov(to_acc, frm_ram)
    add(literal) lit(1) // != 255
    jz("das_if_continue_2")
    jp("das_endif_2")
lbl("das_if_continue_2")
    
    ld(to_acc, SP)
    add(literal) lit(0+1)
    mov(to_ramaddr, frm_acc)
    mov(to_acc, frm_ram)
    mov(to_ramaddr, literal) lit(PREVKEY)
    mov(to_ram, frm_acc)

lbl("das_endif_2")

    ld(to_acc, SP)
    add(literal) lit(0+1)
    mov(to_ramaddr, frm_acc)
    inv(frm_ram)             // 1st cmpl
    inc()                        // 2nd cmpl      
    mov(to_ramaddr, literal) lit(PREVKEY)      // const
    add(frm_ram)

    jz("das_do_1_base")
    
    ld(to_acc, SP)
    add(literal) lit(0+1)
    mov(to_ramaddr, frm_acc)
    mov(to_acc, frm_ram)
    mov(to_ramaddr, literal) lit(PREVKEY)
    mov(to_ram, frm_acc)
    inc_sp(4)
    ret()





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
        } while (g, asm("add(literal) lit(-9) // != 9\n")); // g != 9
        c = (c, asm("add(literal) lit(255)"));
    }
}


*/



lbl("disp")
    dec_sp(1)

lbl("disp_while_test")
    ld(to_acc, SP)
    add(literal) lit(3+1)
    mov(to_ramaddr, frm_acc)
    mov(to_acc, frm_ram)
    jz("disp_while_end")

    ld(to_acc, SP)
    add(literal) lit(0+1)
    mov(to_ramaddr, frm_acc)
    mov(to_ram, literal) lit(0)

lbl("disp_do_base")
    ld(to_acc, SP)
    add(literal) lit(0+1)
    mov(to_ramaddr, frm_acc)
    mov(to_pch, frm_ram)

    ld(to_acc, SP)
    add(literal) lit(0+1)
    mov(to_ramaddr, frm_acc)
    mov(to_acc, frm_ram)
    mov(to_ramaddr, frm_acc)
    out(frm_ram)

    // += 1
    ld(to_acc, SP)
    add(literal) lit(0+1)
    mov(to_ramaddr, frm_acc)
    mov(to_acc, frm_ram)
    add(literal) lit(1)
    mov(to_ram, frm_acc)

     // != 9
    add(literal) lit(-9)
    jz("disp_do_end")
    jp("disp_do_base")
lbl("disp_do_end")
    ld(to_acc, SP)
    add(literal) lit(3+1)
    mov(to_ramaddr, frm_acc)
    mov(to_acc, frm_ram)
    add(literal) lit(255)  // -1
    mov(to_ram, frm_acc)

    jp("disp_while_test")
lbl("disp_while_end")
    inc_sp(1)
    ret()

/*-------------------------------------------------------*/







