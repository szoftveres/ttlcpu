
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
#define inst(d, s, m)   ++progcnt;
#define litl(l)         ++progcnt;
#define lith(l)         ++progcnt;
#define defl(n)         ++progcnt;
#define defh(n)         ++progcnt;
#define lbl(n)          lbladd(n);

#else
/* ============================================================ */
/* Second pass */

#define inst(d,s,m)     instruction((d),(s),(m));
#define litl(v)         dataconst((unsigned int)(v), 0);
#define lith(v)         dataconst((unsigned int)(v), 1);
#define defl(n)         defaddr((n), 0);
#define defh(n)         defaddr((n), 1);
#define lbl(n)          lblprint(n);

#endif
/* ============================================================ */

#include "./prog.asm"

