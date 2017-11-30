/*
 * SHL operation on arbitrary (bytes) long big-endian integer (n_p)
 * Returns the carry (0 or 1)
 */
shl (n_p, bytes) {
    auto cyn;
    auto cy;
    cy = 0;
    while (bytes) {
        asm("    dec() mov(to_ram, frm_acc)\n");
        cyn = msb(*n_p);
        *n_p = (*n_p, asm("    shl()\n")) + cy;
        cy = cyn;
        n_p = (n_p, asm("    inc()\n"));
    }
    cy;
}

/*
 * res = a * b
 * bytes [0-32]
 */
mul (a_p, b_p, res_p, bytes) {
    auto bits;
    bits = (bytes, asm("    shl() shl() shl()  // x8\n"));
    memset(res_p, 0x00, bytes);
    while (bits) {
        asm("    dec() mov(to_ram, frm_acc)\n");
        shl(res_p, bytes);
        if (shl(b_p, bytes)) {
            add(res_p, a_p, bytes);
            *b_p += 1; // rol, reconstruct b
        }
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

