
#define SEGMENT_A       (0x10)
#define SEGMENT_B       (0x08)
#define SEGMENT_C       (0x40)
#define SEGMENT_D       (0x02)
#define SEGMENT_E       (0x80)
#define SEGMENT_F       (0x04)
#define SEGMENT_G       (0x01)
#define SEGMENT_DP      (0x20)

#define SYM_0           (222)
#define SYM_1           (72)
#define SYM_2           (155)
#define SYM_3           (91)
#define SYM_4           (77)
#define SYM_5           (87)
#define SYM_6           (215)
#define SYM_7           (88)
#define SYM_8           (223)
#define SYM_9           (95)
#define SYM_A           (221)
#define SYM_B           (199)
#define SYM_C           (150)
#define SYM_D           (203)
#define SYM_E           (151)
#define SYM_F           (149)

#define SYM_DP          (32)

mem_init () {
    *(0) = 222;    //O
    *(1) = 134;    //L
    *(2) = 134;    //L
    *(3) = 151;    //E
    *(4) = 205;    //H
    *(5) = 2;
    *(6) = 1;
    *(7) = 16;
    *(8) = 1;
    *(9) = 2;      //_
}


m_shift () {
    var i;
    var p;

    p = *(9);
    for (i = 9; i != 0; i +=255) {
        *(i) = (*(i-1));
    }
    *(0) = p;
}

/**
 * Push one symbol from the right
 */
disp_push (var s) {
    var i;
    i = 7;
    while (i) {
        *(i) = (*(i, asm("add(progdata) data(255)")));
        i = (i, asm("add(progdata) data(255)"));
    }
    *(0) = s;
}


put_scan (var code) {
    disp_push(hex2sym((code, asm("rol() rol() rol() rol() \n"))));
    disp_push(hex2sym(code) + SYM_DP);
}


main () {
    while (1) {
        mem_init();
        while (1) {
            disp(250);
            m_shift();
        }
        while (1) {
            put_scan(das());
        }
    }
}



