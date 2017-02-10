/*
 * Returns '1' if (a & 0x80)
 */
msb (a) {
    (a, asm("    rol()\n")) - (a, asm("    shl()\n"));
}


/*
 * SHL operation on arbitrary (bytes) long big-endian integer (n_p)
 */
shl (n_p, bytes) {
    n_p += bytes;
    do {
        auto carry;
        n_p += 0xFE;
        bytes += 0xFF;
        carry = bytes ? msb(*n_p) : 0;
        n_p += 1;
        *n_p = (*n_p, asm("    shl()\n")) + carry;
    } while (bytes);
}


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
        i += 0xFF; /* -1 */
        res = res << 1;
        if (msb(b)) {
            res += a;
        }
        b = b << 1;
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
        i += 0xFF; /* -1 */
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

/*
 * Memset
 */
memset (start, c, bytes) {
    auto end;
    end = start + bytes;
    for (; start != end; start += 1) {
        *start = c;
    }
}


