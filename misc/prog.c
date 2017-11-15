#include "../lib/ttlcpuio.h"


static d0;
static d1;
static d2;
static d3;
static d4;
static d5;
static d6;
static d7;


#include "routines.c"


scroll (c) {
    disp_push(c);
    disp(32);
}


msg () {
    auto i;
    memset(0, 0, 8);
    scroll(SYM_P);
    scroll(SYM_R);
    scroll(SYM_E);
    scroll(SYM_S);
    scroll(SYM_S);
    scroll(SYM_SPACE);
    scroll(SYM_D);
    for (i = 0; i != 8; i += 1) {
        scroll(SYM_SPACE);
    }
}


mem_monitor (addr) {
    d0 = SYM_LEFT_BRACKET;
    d1 = hex2sym(addr >> 4);
    d2 = hex2sym(addr);
    d3 = SYM_RIGHT_BRACKET;
    d4 = SYM_SPACE;
    d5 = SYM_SPACE;
    d6 = hex2sym(*addr >> 4);
    d7 = hex2sym(*addr) + SYM_DP;
}

buf_push (item, b_p, size) {
    while (size) {
        size = (size, asm("add(progdata) data(255)"));
        *(b_p + size) = (*((b_p + size), asm("add(progdata) data(255)")));
    }
    *(b_p) = item;
}

swp (a_p, b_p) {
    auto tmp;

    tmp = *a_p;
    *a_p = *b_p;
    *b_p = tmp;
    disp(24);
}

qst (arr_p, size) {
    auto idx;
    auto lp;
    auto arr_ep;

    if (size < 2) {
        return;
    }
    lp = 0;
    for (idx = 1; idx != size; idx += 1) {
        arr_ep = arr_p + idx;
        if (*arr_ep < *arr_p) {
            swp(arr_p + lp + 1, arr_p + idx);
            lp += 1;
        }
    }

    swp(arr_p + lp, arr_p);

    qst(arr_p, lp);
    qst(arr_p + lp + 1, size - lp - 1);
}



main () {
    auto memaddr;

    memaddr = 0;
    msg();
    while (1) {
        auto c;

        c = das(); /* Display And Scan */
        if (c == 0x0F) {
            memset(0, 0, 8);
        } else if (c == 0x0E) {
            qst(0,8);
        } else if (c == 0x0D) {
            msg();
        } else if (c == 0x0C) {
            auto ad;
            ad = 0;
            do {
                mem_monitor(ad);
                disp(24);
                ad += 1;
            } while (ad);
        } else if (c == 0x0B) {
            memaddr += 1;
            mem_monitor(memaddr);
        } else if (c == 0x0A) {
            memaddr -= 1;
            mem_monitor(memaddr);
        } else {
            disp_push(hex2sym(c));
        }
    }
}

