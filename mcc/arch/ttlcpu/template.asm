
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
/* bit (var <0-7>, var <data>) */

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

    
