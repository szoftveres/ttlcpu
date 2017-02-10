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


disp_dec (num) {
    auto newnum;
    auto rem;

    newnum = div(num, 10, &rem);
    if (newnum) {
        disp_dec(newnum);
    }
    disp_push(hex2sym(rem));
}


disp_long (np, bytes) {
    auto ad;
    ad = np + bytes;
    do {
        ad += 0xFF;
        bytes += 0xFF;
        disp_push(hex2sym(*ad >> 4));
        disp_push(hex2sym(*ad));
    } while (bytes);
}


main () {
    auto memaddr;

    memaddr = 0;
//    msg();
    while (1) {
        auto c;

        c = das(); /* Display And Scan */
        if (c == 0x0F) {
            memset(0, 0, 8);
        } else if (c == 0x0E) {
            auto np;
            auto n;
            np = 0x10;
            memset(np,0,4); *np = msb(0xFF);
            for (n = 0; n != 32; n += 1) {
                /*
                memset(4, 0, 4);
                disp_dec(mul(n,n));
                */
                memset(0, 0, 8);
                disp_long(np, 4);
                disp(148);
                shl(np, 3);
            }
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

