/*
 * Sample (otherwise useless) program to demonstrate the capabilities
 * of the c-like compiler
 */


#define NUM_TEN     0x0A
/* You can use gnu-gcc preprocessor directives */


/* Global variables */
static var_glb;
/* are reset to zero before main function execution begins */

/*
 * There are no data types, everything is an 8-bit word
 * Every function returns a value, there's no 'void' function
 */
addition (b1, b2) {
    return b1 + b2;
    /* The retrun statment can have a value, however it just works as if
     * it was a 'goto' statement that jumps to the end of
     * the function; it's used purely for program flow control.
     * The return value of a function is always the value
     * of the last evaluated expression.
     * The return statement can be omitted in this case, the body of the function
     * could be rewritten as a single 'b1 + b2;' statment, the result would be the same.
     * See the 'dec' function below, which doesn't utilize the return statement;
     */
}

/*
 * Function arguments are passed via the (emulated) stack, the
 * accumulator contains the return value (which is the result of
 * the last expression).
 *
 * For stack emulation implementation details,
 * see mcc/arch/ttlcpu/header.asm
 */


/*
 * In-line assembly for faster execution
 */
dec (a) {
    /*
     * Expressions which are separated with commas, are being evaluated
     * sequentially after each other.
     * The first expression (a) does nothing but loads the
     * value of 'a' into the accumulator register. In order
     * to decrement its value, there's no need to have the CPU
     * form the 2nd complement of '1' and add the two numbers
     * together; instead, we can do it with one assembly instruction,
     * the accumulator will contain the return value.
     * Code execution speed can be significantly boosted with these tricks.
     */
    a, asm("add(progdata) data(0xFF)");
}


invert_out (f) {
    out(~f);
    /*
     * Cannot find out() definition? See
     * mcc/arch/ttlcpu/header.asm
     */
}

/* The main function, program execution starts here */
main () {
    /* Variable declarations at the beginning of each block */
    auto i;
    static g;

    /*
     * Built-in operator precedence can be overriden
     * with () parentheses
     */
    for (i = NUM_TEN; (i - 1 != 0xFF - (1 == 2)) ? 1 : 0; i += addition(1, 0)) {
        auto i_p;
        /*
         * there's neither 'pointer' type, nor
         * pointer arythmetics; remember, everything is a word
         */
        i_p = &i;
        *i_p += 1;
        invert_out(*i_p);
        var_glb += 1;
    }
    /*
     * Functions don't have to (and cannot) be declared
     * prior to call, 'out_series' function is defined below;
     * The assembler is responsible for collecting the symbols
     * and for figuring out which subroutine to call.
     */
    out_series(add(NUM_TEN, 2 << 4));

}


out_series (num) {
    out(num);
    if (!num) {
        return;
    }
    /* Arbitrary deep recursion */
    out_series(num - 1);
}

