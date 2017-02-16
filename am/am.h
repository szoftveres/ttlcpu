
#undef inst
#undef datal
#undef datah
#undef defl
#undef defh
#undef lbl

#ifndef _AM_H_
#define _AM_H_
/* ============================================================ */
/* First pass */

/* === Derived instructions === */

#define data(l)     datal(l)
#define def(l)      defl(l)

#define mov(d, s)   inst((d), (s), NULL)
#define movz(d, s)  inst((d), (s), "z")

#define add(s)      mov(to_acc_adder, (s))

#define inc()       do {add(progdata) data(1)} while(0);
#define dec()       do {add(progdata) data(-1)} while(0);
#define inv(s)      mov(to_acc_invert, (s))
#define shl()       add(frm_acc)
#define rol()       inst(to_acc_adder, frm_acc, "c")
#define out(s)      mov(to_port, (s))
#define in(d)       mov((d), frm_port)

#define nop()       mov(to_acc, frm_acc)

#define jp(a)       do {mov(to_pch, progdata) defh(a) mov(to_pc, progdata) defl(a)} while(0);
#define jz(a)       do {movz(to_pch, progdata) defh(a) movz(to_pc, progdata) defl(a)} while(0);

#define st(a, s)    do {mov(to_mar, progdata) data(a) mov(to_ram, (s))} while(0);
#define stz(a, s)   do {movz(to_mar, progdata) data(a) movz(to_ram, (s))} while(0);
#define ld(d, a)    do {mov(to_mar, progdata) data((a)) mov((d), frm_ram)} while(0);
#define ldz(d, a)   do {movz(to_mar, progdata) data((a)) movz((d), frm_ram)} while(0);

#define ror()       do {rol() rol() rol() rol() rol() rol() rol()} while(0);
#define shr()       do {ror() shl() ror()} while(0);

/* === Code organization === */

#define _org(addr)     while (pc != (addr)) {nop();};

#define _align()                                                            \
            do {                                                            \
                const unsigned int pagestart = ((pc & (~0xFF)) + 0x0100);   \
                while (pc != pagestart) {nop();}                            \
            } while (0);

#define _fitpage(size)                                                      \
            do {                                                            \
                const unsigned int pagestart = ((pc + (size)) & (~0xFF));   \
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

#define inst(d, s, m)   ++pc;
#define datal(l)        ++pc;
#define datah(l)        ++pc;
#define defl(n)         ++pc;
#define defh(n)         ++pc;
#define lbl(n)          lbladd(n);

#else
/* ============================================================ */
/* Second pass */

/*
 * In this pass we actually generate the bytecode, already knowing
 * the position of the labels
 */

#define inst(d, s, m)   instruction((d),(s),(m));
#define datal(v)        dataconst((unsigned int)(v), 0);
#define datah(v)        dataconst((unsigned int)(v), 1);
#define defl(n)         defaddr((n), 0);
#define defh(n)         defaddr((n), 1);
#define lbl(n)          lblprint(n);

#endif
/* ============================================================ */

#include "./program.asm"

