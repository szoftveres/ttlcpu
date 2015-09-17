
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
#undef SP_BASE
#define SP_BASE         0xfb


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







