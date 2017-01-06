
#include "../lib/ttlcpuio.h"


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
    memset(0, 0, 8);
    disp_push(SYM_LEFT_BRACKET);
    disp_push(hex2sym(addr >> 4));
    disp_push(hex2sym(addr));
    disp_push(SYM_RIGHT_BRACKET);
    disp_push(SYM_SPACE);
    disp_push(SYM_SPACE);
    disp_push(hex2sym(*addr >> 4));
    disp_push(hex2sym(*addr) + SYM_DP);
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
            disp_push(SYM_SPACE);
        } else if (c == 0x0D) {
            msg();
        } else if (c == 0x0C) {
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

