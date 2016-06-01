
#undef inst
#undef litl
#undef lith
#undef defl
#undef defh
#undef lbl

#ifndef _AM_H_
#define _AM_H_
/* ============================================================ */
/* First pass */

/* derived instructions */
#define lit(l)      litl(l)
#define def(l)      defl(l)

#define mov(d, s)   inst((d), (s), "")
#define movz(d, s)  inst((d), (s), "z")

#define add(s)      mov(to_acc_adder, (s))

#define inc()       add(literal) lit(1)
#define dec()       add(literal) lit(-1)
#define inv(s)      mov(to_acc_invert, (s))
#define shl()       add(frm_acc)
#define rol()       inst(to_acc_adder, frm_acc, "c")
#define out(s)      mov(to_port, (s))
#define in(d)       mov((d), frm_port)

#define nop()       mov(to_acc, frm_acc)

#define jp(a)       mov(to_pch, literal) defh(a) mov(to_pc, literal) defl(a)
#define jz(a)       movz(to_pch, literal) defh(a) movz(to_pc, literal) defl(a)

#define st(a, s)    mov(to_mar, literal) lit(a) mov(to_ram, (s))
#define stz(a, s)   movz(to_mar, literal) lit(a) movz(to_ram, (s))
#define ld(d, a)    mov(to_mar, literal) lit((a)) mov((d), frm_ram)
#define ldz(d, a)   movz(to_mar, literal) lit((a)) movz((d), frm_ram)

#define ror()       rol() rol() rol() rol() rol() rol() rol()
#define shr()       ror() shl() ror()

/* = Code organization = */

#define _org(n)     while (pc != (n)) {nop();};

#define _fitpage(n)                                                         \
            do {                                                            \
                const unsigned int pagestart = ((pc + (n)) & (~0xFF));      \
                if ((pc & (~0xFF)) != pagestart) {                          \
                    while (pc != pagestart) {nop();}                        \
                }                                                           \
            } while (0);                                                    \


/* ============================================================ */
/* First pass */

/*
 * In this pass we just calculate position of the labels and add
 * them to the symbol table
 */

#define inst(d, s, m)   do {++pc;} while(0);
#define litl(l)         do {++pc;} while(0);
#define lith(l)         do {++pc;} while(0);
#define defl(n)         do {++pc;} while(0);
#define defh(n)         do {++pc;} while(0);
#define lbl(n)          do {lbladd(n);} while(0);

#else
/* ============================================================ */
/* Second pass */

/*
 * In this pass we actually generate the bytecode, already knowing
 * the position of the labels
 */

#define inst(d, s, m)   do {instruction((d),(s),(m));} while(0);
#define litl(v)         do {dataconst((unsigned int)(v), 0);} while(0);
#define lith(v)         do {dataconst((unsigned int)(v), 1);} while(0);
#define defl(n)         do {defaddr((n), 0);} while(0);
#define defh(n)         do {defaddr((n), 1);} while(0);
#define lbl(n)          do {lblprint(n);} while(0);

#endif
/* ============================================================ */

#include "./prog.asm"

