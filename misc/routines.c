/*
 * Returns '1' if (a & 0x80)
 */
negative (char a) {
    (a, asm("    rol()\n")) - (a, asm("    shl()\n"));
}


shl (char np, char bytes) {
    char adh;

    adh = np + bytes;
    np = adh + 0xFF;
    bytes += 0xFF;
    do {
        adh += 0xFF;
        np += 0xFF;
        if (bytes) {
        //    *adh = (*adh, asm("    shl()\n")) + ((*np, asm("    rol()\n")) - (*np, asm("    shl()\n")));
            *adh = (*adh, asm("    shl()\n")) + negative(*np);
        }
        *np = (*np, asm("    shl()\n"));
        bytes += 0xFF;
    } while (bytes);
}


/*
 * a [0-255]
 * b [0-255]
 * ret [0-255]
 */
mul (char a, char b) {
    char i;
    char res;
    res = 0;
    i = 8;
    do {
        i += 0xFF; /* -1 */
        res = res << 1;
        if (negative(b)) {
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
div (char a, char b, char rem_pt) {
    char i;
    char res;
    char dif;
    res = 0;
    i = 4;
    do {
        i += 0xFF; /* -1 */
        dif = a - (b << i);
        if (!negative(dif)) {
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
memset (char start, char c, char bytes) {
    char end;
    end = start + bytes;
    for (; start != end; start += 1) {
        *start = c;
    }
}


