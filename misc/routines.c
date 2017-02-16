

/*
 * SHL operation on arbitrary (bytes) long big-endian integer (n_p)
 */
shl (n_p, bytes) {
    auto cyn;
    auto cy;
    cy = 0;
    while (bytes) {
        asm("    dec()\n");
        asm("    mov(to_ram, frm_acc)\n");
        cyn = msb(*n_p);
        *n_p = (*n_p, asm("    shl()\n")) + cy;
        cy = cyn;
        n_p = (n_p, asm("    inc()\n"));
    }
}

#if 0
/*
 * a [0-255]
 * b [0-255]
 * ret [0-255]
 */
mul (a, b) {
    auto i;
    auto res;
    res = 0;
    i = 8;
    do {
        i = (i, asm("    dec()\n"));
        res = (res, asm("    shl()\n"));
        if (msb(b)) {
            res += a;
        }
        b = (b, asm("    shl()\n"));
    } while (i);
    res;
}

/*
 * Divide
 * a [0-255] b [0-15]
 */
div (a, b, rem_pt) {
    auto i;
    auto res;
    auto dif;
    res = 0;
    i = 4;
    do {
        i = (i, asm("    dec()\n"));
        dif = a - (b << i);
        if (!msb(dif)) {
            res += 1 << i;
            a = dif;
        }
    } while (i);
    if (rem_pt) {
        *rem_pt = a;
    }
    res;
}

#endif

