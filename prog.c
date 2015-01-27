
/* Sample program for the c-like compiler */


/*
 * There's only one data type: 8-bit 'var'
 */
invert_out (var f) {
    out(~f);
    /*
     * Cannot find out() definition? See
     * mcc/arch/ttlcpu/template.asm
     */
}

add (var b1, var b2) {
    /*
     * There's no return statement; instead, the
     * value of the last expressions is returned.
     * Every function returns a 'var' type
     */
    b1 + b2;
}


#define NUM_TEN     10
/* You can use gnu-gcc preprocessor directives */


main () {
    var i;

    /* binary operator precedence can be overriden with () */
    for (i = NUM_TEN; (i - 1 != 255 - (1 == 2)) ? 1 : 0; i += add(1, 0)) {
        var i_p;
        /*
         * there's no separate 'pointer' type, neither
         * pointer arythmetics, everything is 'var'
         */
        i_p = &i;
        invert_out(*i_p);
    }
    /*
     * Functions doesn't have to (and cannot) be declared
     * prior to call. The assembler takes care of function
     * names
     */
    out_series(add(NUM_TEN, 2 << 4));

}


out_series (var num) {
    out(num);
    if (num) {
        /* Arbitrary deep recursion */
        out_series(num - 1);
    }
}



