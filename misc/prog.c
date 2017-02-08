#include "../lib/ttlcpuio.h"
#include "routines.c"


char d0;
char d1;
char d2;
char d3;
char d4;
char d5;
char d6;
char d7;

memset (char start, char c, char size) {
    char end;
    end = start + size;
    for (; start != end; start += 1) {
        *start = c;
    }
}


scroll (char c) {
    disp_push(c);
    disp(32);
}


msg () {
    char i;
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


mem_monitor (char addr) {
    d0 = SYM_LEFT_BRACKET;
    d1 = hex2sym(addr >> 4);
    d2 = hex2sym(addr);
    d3 = SYM_RIGHT_BRACKET;
    d4 = SYM_SPACE;
    d5 = SYM_SPACE;
    d6 = hex2sym(*addr >> 4);
    d7 = hex2sym(*addr) + SYM_DP;
}


main () {
    char memaddr;

    memaddr = 0;
    msg();
    while (1) {
        char c;

        c = das(); /* Display And Scan */
        if (c == 0x0F) {
            memset(0, 0, 8);
        } else if (c == 0x0E) {
            while (1) {
                msg();
            }
        } else if (c == 0x0D) {
            msg();
        } else if (c == 0x0C) {
            char ad;
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

