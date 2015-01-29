
#undef litl
#undef lith
#undef lbl
#undef defl
#undef defh
#undef inst

#ifndef _AM_H_
#define _AM_H_
/* ============================================================ */
/* First pass */

/* derived instructions */
#define lit(l) litl(l)
#define def(l) defl(l)

#define mov(d, s) inst((d), (s), "")
#define movz(d, s) inst((d), (s), "z")

#define add(s) mov(to_acc_adder, (s))

#define inc() add(literal) lit(1)
#define dec() add(literal) lit(-1)
#define inv(s) mov(to_acc_invert, (s))
#define shl() add(frm_acc)
#define rol() inst(to_acc_adder, frm_acc, "c")
#define out(s) mov(to_port, (s))
#define in(d) mov((d), frm_port)

#define jp(a) mov(to_pch, literal) defh(a) mov(to_pc, literal) defl(a)
#define jz(a) movz(to_pch, literal) defh(a) movz(to_pc, literal) defl(a)

#define st(a, s) mov(to_ramaddr, literal) lit(a) mov(to_ram, (s))
#define stz(a, s) movz(to_ramaddr, literal) lit(a) movz(to_ram, (s))
#define ld(d, a) mov(to_ramaddr, literal) lit((a)) mov((d), frm_ram)
#define ldz(d, a) movz(to_ramaddr, literal) lit((a)) movz((d), frm_ram)

/* Base */
#define inst(d, s, m)   do {++progcnt;} while(0);
#define litl(l)         do {++progcnt;} while(0);
#define lith(l)         do {++progcnt;} while(0);
#define defl(n)         do {++progcnt;} while(0);
#define defh(n)         do {++progcnt;} while(0);
#define lbl(n)          do {lbladd(n);} while(0);

#else
/* ============================================================ */
/* Second pass */

#define inst(d,s,m)     do {instruction((d),(s),(m));} while(0);
#define litl(v)         do {dataconst((unsigned int)(v), 0);} while(0);
#define lith(v)         do {dataconst((unsigned int)(v), 1);} while(0);
#define defl(n)         do {defaddr((n), 0);} while(0);
#define defh(n)         do {defaddr((n), 1);} while(0);
#define lbl(n)          do {lblprint(n);} while(0);

#endif
/* ============================================================ */

#include "./prog.asm"

