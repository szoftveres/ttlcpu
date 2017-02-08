
/*
 * 8-bit multiplication
 */
mul (char a, char b) {
    char i;
    char res;
    res = 0;
    i = 8;
    do {
        i += 255; /* -1 */
        res = res << 1;
        if (b & 0x80) {
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
        i += 255; /* -1 */
        dif = a - (b << i);
        if (!(dif & 0x80)) {
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
memset (char start, char c, char size) {
    char end;
    end = start + size;
    for (; start != end; start += 1) {
        *start = c;
    }
}


