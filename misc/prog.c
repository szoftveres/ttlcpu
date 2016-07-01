
#define SEGMENT_A       (0x10)
#define SEGMENT_B       (0x08)
#define SEGMENT_C       (0x40)
#define SEGMENT_D       (0x02)
#define SEGMENT_E       (0x80)
#define SEGMENT_F       (0x04)
#define SEGMENT_G       (0x01)
#define SEGMENT_DP      (0x20)


#define SYM_DP          (0x20)
#define SYM_HYPHEN      (0x01)
#define SYM_UNDERSCORE  (0x0D)
#define SYM_SPACE       (0x00)
#define SYM_COMMA       (0x80)

#define SYM_0           (0xDE)
#define SYM_1           (0x48)
#define SYM_2           (0x9B)
#define SYM_3           (0x5B)
#define SYM_4           (0x4D)
#define SYM_5           (0x57)
#define SYM_6           (0xD7)
#define SYM_7           (0x58)
#define SYM_8           (0xDF)
#define SYM_9           (0x5F)
#define SYM_A           (0xDD)
#define SYM_B           (0xC7)
#define SYM_C           (0x96)
#define SYM_D           (0xCB)
#define SYM_E           (0x97)
#define SYM_F           (0x95)

#define SYM_G           (0xD6)
#define SYM_H           (0xCD)
#define SYM_I           (0x48)
#define SYM_J           (0xCA)
#define SYM_K           (0x00)  /* unimplemented */
#define SYM_L           (0x86)
#define SYM_M           (0x00)  /* unimplemented */
#define SYM_N           (0xC1)
#define SYM_O           (0xDE)
#define SYM_P           (0x9D)
#define SYM_Q           (0x5D)
#define SYM_R           (0x81)
#define SYM_S           (0x57)
#define SYM_T           (0x87)
#define SYM_U           (0xCE)
#define SYM_V           (0x00)  /* unimplemented */
#define SYM_W           (0x00)  /* unimplemented */
#define SYM_X           (0x00)  /* unimplemented */
#define SYM_Y           (0x4F)
#define SYM_Z           (0x00)  /* unimplemented */






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
        *(7) = (hex2sym((i, asm("inv(frm_acc) rol() rol() rol() rol() \n"))));
        *(6) = (hex2sym((i, asm("inv(frm_acc)"))) + SYM_DP);
        i += 1;
        disp(32);
    } while (i);
//    for (i = 0; i!= 8; i += 1) {disp_push(0);}
}


scroll (var a) {
    disp_push(a);
    disp(64);
}

main () {
    while (1) {
        var i;
        for (i=0; i!= 2; i+=1) {
            scroll(SYM_H);
            scroll(SYM_E);
            scroll(SYM_L);
            scroll(SYM_L);
            scroll(SYM_O);
            scroll(SYM_COMMA);

            scroll(SYM_T);
            scroll(SYM_H);
            scroll(SYM_I);
            scroll(SYM_S);
            scroll(SYM_SPACE);
            scroll(SYM_I);
            scroll(SYM_S);
            scroll(SYM_SPACE);
            scroll(SYM_A);
            scroll(SYM_N);
            scroll(SYM_SPACE);

            scroll(SYM_8);
            scroll(SYM_HYPHEN);
            scroll(SYM_B);
            scroll(SYM_I);
            scroll(SYM_T);
 
            scroll(SYM_SPACE);

            scroll(SYM_T);
            scroll(SYM_T);
            scroll(SYM_L);
            scroll(SYM_SPACE);
            scroll(SYM_C);
            scroll(SYM_P);
            scroll(SYM_U);
            scroll(SYM_SPACE);
            scroll(SYM_SPACE);
            scroll(SYM_SPACE);
        }
        count();
        while (1) {
            put_scan(das());
            disp(64);
        }
    }
}

