/*
 * Sample (otherwise useless) program to demonstrate the capabilities
 * of the c-like compiler
 */


#define NUM_TEN     10
/* You can use gnu-gcc preprocessor directives */


/*
 * There's only one data type: 8-bit 'var'
 * Every function returns a 'var' type value, there's no 'void' function
 */
add (var b1, var b2) {
    /*
     * There's no return statement; the value
     * of the last expressions is returned.
     */
    b1 + b2;
    /* This function returns the value of b1 + b2 */
}

/*
 * Function arguments are passed via the (emulated) stack, the
 * accumulator contains the return value (which is the result of
 * the last expression).
 *
 * For stack emulation implementation details,
 * see mcc/arch/ttlcpu/header.asm
 */

invert_out (var f) {
    out(~f);
    /*
     * Cannot find out() definition? See
     * mcc/arch/ttlcpu/header.asm
     */
}

/* The main function, program execution starts here */
main () {
    /* Variable declarations at the beginning of each block */
    var i;

    /*
     * Built-in operator precedence can be overriden
     * with () parentheses
     */
    for (i = NUM_TEN; (i - 1 != 255 - (1 == 2)) ? 1 : 0; i += add(1, 0)) {
        var i_p;
        /*
         * there's no 'pointer' type, neither
         * pointer arythmetics, everything is 'var'
         */
        i_p = &i;
        invert_out(*i_p);
    }
    /*
     * Functions don't have to (and cannot) be declared
     * prior to call, 'out_series' function is defined below;
     * The assembler is responsible for collecting the symbols
     * and for figuring out which subroutine to call.
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

