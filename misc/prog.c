
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


put_scan (var code) {
    disp_push(hex2sym((code, asm("rol() rol() rol() rol() \n"))));
    disp_push(hex2sym(code) + SYM_DP);
}


count () {
    var i;
    i = 0;
    for (i = 0; i!= 8; i += 1) {disp_push(0);}
    *(3) = 1;
    *(4) = 1;
    do {
        *(1) = (hex2sym((i, asm("rol() rol() rol() rol() \n"))));
        *(0) = (hex2sym(i) + SYM_DP);
        *(7) = (hex2sym((0-i, asm("rol() rol() rol() rol() \n"))));
        *(6) = (hex2sym(0-i) + SYM_DP);
        i += 1;
        disp(32);
    } while (i);
//    for (i = 0; i!= 8; i += 1) {disp_push(0);}
}



main () {
    while (1) {
        var i;
        for (i=0; i!= 2; i+=1) {
            disp_push(205);    //H
            disp(64);
            disp_push(151);    //E
            disp(64);
            disp_push(134);    //L
            disp(64);
            disp_push(134);    //L
            disp(64);
            disp_push(222);    //O
            disp(64);
            disp_push(0);
            disp(64);
            disp_push(0);
            disp(64);
            disp_push(0);
            disp(64);

            disp_push(223);
            disp(64);
            disp_push(1);
            disp(64);
            disp_push(199);
            disp(64);
            disp_push(72);
            disp(64);
            disp_push(135);
            disp(64);

            disp_push(0);
            disp(64);

            disp_push(135);    //T
            disp(64);
            disp_push(135);    //T
            disp(64);
            disp_push(134);    //L
            disp(64);
            disp_push(0);
            disp(64);
            disp_push(150);    //C
            disp(64);
            disp_push(157);    //P
            disp(64);
            disp_push(206);    //U
            disp(64);
            disp_push(0);
            disp(64);
            disp_push(0);
            disp(64);
            disp_push(0);
            disp(64);
        }
        count();
        while (1) {
            put_scan(das());
            disp(64);
        }
    }
}



