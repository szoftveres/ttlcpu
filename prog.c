
/* Sample program for the c-like compiler */


/*
 * There's only one data type: 8-bit 'var'
 */
invert_out (var f) {
    out(~f);
}

add (var b1, var b2) {
    /*
     * There's no return statement; instead, the
     * value of the last expressions is returned.
     * Every function returns a 'var' type
     */
    b1 + b2;
}

main () {
    var i;

    /* binary operator precedence can be overriden with () */
    for (i = 10; (i - 1 != 255 - (1 == 2)) ? 1 : 0; i += add(1, 0)) {
        var i_p;
        /*
         * there's no separate 'pointer' type,
         * neither pointer arythmetics, everything
         * is 'var'
         */
        i_p = &i;
        invert_out(*i_p);
    }
}




