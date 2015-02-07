
/*-------------------------------------------------------*/

/* Emulated x86-like CPU registers */

#undef FP
#define FP              0xff
#undef BX
#define BX              0xfe


/*-------------------------------------------------------*/

/* Emulated Stack implementation */

#undef SP
#define SP              0xfd
#undef STACK_ACC
#define STACK_ACC       0xfc
#undef STACK_TEMP
#define STACK_TEMP      0xfb
#undef SP_BASE
#define SP_BASE         0xfa


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
    call("main", "main_caller_ret")
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
    var cc;
    var ret;
    cc = 8;
    ret = 0;
    while(cc) {
        cc -= 1;
        if (bit(cc, a)) if (bit(cc, b)) {
            ret += 1 << cc;
        }
    }
    ret;
}
*/

lbl("bwand")
/*
  var cc;
  var ret;
*/
    dec_sp(2)

/* cc = 8; */
    ld(to_acc, SP)
    add(literal) lit(2+1)
    mov(to_ramaddr, frm_acc)
    mov(to_ram, literal) lit(8)

/* ret = 0; */
    ld(to_acc, SP)
    add(literal) lit(1+1)
    mov(to_ramaddr, frm_acc)
    mov(to_ram, literal) lit(0)

lbl("bwand_loop_restart")

/* 
    while(cc) 
    cc -= 1;
*/
    ld(to_acc, SP)
    add(literal) lit(1+1)
    mov(to_ramaddr, frm_acc)
    mov(to_acc, frm_ram)
    jz("bwand_done")
    dec()
    mov(to_ram, frm_acc)


/* if (bit(cc, a)) */
    ld(to_acc, SP)
    add(literal) lit(1+1)
    mov(to_ramaddr, frm_acc)
    mov(to_acc, frm_ram)
    push(frm_acc)                      // push fn arg
    ld(to_acc, SP)
    add(literal) lit(6+1)
    mov(to_ramaddr, frm_acc)
    mov(to_acc, frm_ram)
    push(frm_acc)                      // push fn arg
    call("bit", "bwand_call_1")
    inc_sp(2)
    jz("bwand_skip")

/* if (bit(cc, b)) */
    ld(to_acc, SP)
    add(literal) lit(1+1)
    mov(to_ramaddr, frm_acc)
    mov(to_acc, frm_ram)
    push(frm_acc)                      // push fn arg
    ld(to_acc, SP)
    add(literal) lit(5+1)
    mov(to_ramaddr, frm_acc)
    mov(to_acc, frm_ram)
    push(frm_acc)                      // push fn arg
    call("bit", "bwand_call_2")
    inc_sp(2)
    jz("bwand_skip")

/* ret += 1 << cc; */
    ld(to_acc, SP)
    add(literal) lit(0+1)
    mov(to_ramaddr, frm_acc)
    mov(to_acc, frm_ram)
    push(frm_acc)                  // 1st operand push
    mov(to_acc, literal) lit(1)      // const
    push(frm_acc)                  // 1st operand push
    ld(to_acc, SP)
    add(literal) lit(3+1)
    mov(to_ramaddr, frm_acc)
    mov(to_acc, frm_ram)
lbl("bwand_shift_start")
    jz("bwand_shift_end")
    dec()
    st(BX, frm_acc)                // store 2nd operand
    pop(to_acc)                      // 1st operand pop
    shl()
    push(frm_acc)                  // 1st operand push
    ld(to_acc, BX)
    jp("bwand_shift_start")
lbl("bwand_shift_end")
    pop(to_acc)
    st(BX, frm_acc)                // store 2nd operand
    pop(to_acc)                      // 1st operand pop
    mov(to_ramaddr, literal) lit(BX)  // addition
    add(frm_ram)

    push(frm_acc)                  // 1st operand push
    ld(to_acc, SP)
    add(literal) lit(1+1)
    st(BX, frm_acc)                // store 2nd operand
    pop(to_acc)                      // 1st operand pop
    mov(to_ramaddr, literal) lit(BX)
    mov(to_ramaddr, frm_ram)
    mov(to_ram, frm_acc)

lbl("bwand_skip")
    jp("bwand_loop_restart")
lbl("bwand_done")

/* ret; */
    ld(to_acc, SP)
    add(literal) lit(0+1)
    mov(to_ramaddr, frm_acc)
    mov(to_acc, frm_ram)
    inc_sp(2)
    ret()

/*-------------------------------------------------------*/

/*
bwand (var a, var b) {
    var i;
    var ret_add;

    var cc;
    var ret;
    cc = 8;
    ret = 0;
    while(cc) {
        cc -= 1;

        i = 7-cc;
        while (i) {
            a;
            asm ("    shl()\n");
            asm ("    mov(to_ram, frm_acc)\n");
            b;
            asm ("    shl()\n");
            asm ("    mov(to_ram, frm_acc)\n");

            i -= 1;
        }

        a;
        asm ("    rol()\n");
        asm ("    mov(to_ram, frm_acc)\n");
        b;
        asm ("    rol()\n");
        asm ("    mov(to_ram, frm_acc)\n");

        ret_add = 1;
        i = cc;
        while (i) {
            a;
            asm ("    shl()\n");
            asm ("    mov(to_ram, frm_acc)\n");
            b;
            asm ("    shl()\n");
            asm ("    mov(to_ram, frm_acc)\n");

            ret_add;
            asm ("    shl()\n");
            asm ("    mov(to_ram, frm_acc)\n");

            i -= 1;
        }

        if (a==b) {
            ret += ret_add;
        }
    }
    ret;
}

*/  
