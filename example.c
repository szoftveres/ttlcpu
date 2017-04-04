/*
 * Sample (otherwise useless) program to demonstrate the capabilities
 * of the c-like compiler
 */


#define NUM_TEN     0x0A
/* All the #preprocessor directives are currently handled by gcc (gcc -E) */


/* Global variables */
static var_glb;
/* are reset to zero before main function execution begins */


/*
 * There are no data types, everything is an 8-bit CPU-word.
 * Every function is assumed to return a value, there's
 * no 'void' function.
 * Function arguments are passed via the (emulated) stack.
 * The result of the expressions are stored in the accumulator;
 * also, the accumulator is used to pass the return value of the
 * functions to the caller. Hence, the return value is
 * whatever that's in the accumulator when the function returns.
 * The 'return' statement is available, however it needn't to be used.
 */
return_arg (a) {
    a;      /* This function returns the value of its argument */
}


/*
 * The retrun statment can have a value, however it just works as if
 * it was a 'goto' statement that jumps to the end of
 * the function; its only purpose is program flow control.
 * The return statement can be omitted in the below case, the body
 * of the function can be replaced with a 'b1 + b2;' statment,
 * the result would be the same.
 */
addition (b1, b2) {
    return b1 + b2;
}


/*
 * In-line assembly:
 * Expressions which are separated with commas, are being evaluated
 * sequentially after each other.
 * The first expression (a) loads the value of 'a' into the
 * accumulator. An 'a-1' statement would form the 2nd complement
 * of '1' and would add it to 'a'.
 * We can do it with a signle assembly instruction, the 2nd complement of
 * '1' is '255'; code execution speed can be significantly boosted with these tricks.
 */
dec (a) {
    a, asm("add(progdata) data(0xFF)");
}


/* The main function, program execution starts here */
main () {
    /* Variable declarations at the beginning of each block */
    auto i;
    static g;

    while (1) {
        /*
         * Built-in operator precedence can be overriden
         * with () parentheses
         */
        for (i = NUM_TEN; (i - 1 != 0xFF - (1 == 2)) ? 1 : 0; i += addition(1, 0)) {
            auto i_p;
            /*
             * there's neither 'pointer' type, nor
             * pointer arythmetics; everything is a 8-bit 'word'
             */
            i_p = &i;  /* addressof */
            *i_p += 1; /* dereference */
            out(*i_p);
            var_glb += 1;
        }
        /*
         * No function prototypes, each function translates
         * to an assembler subroutine.
         */
        do {
            out_series(add(NUM_TEN, 2 << 4));
        } while (0);
    }
}


out_series (num) {
    out(num);
    if (!num) {
        return;
    }
    /* Arbitrary deep recursion */
    out_series(num - 1);
}


/* Interesting stuff inside */
#include "misc/routines.c"


/* TTL-CPU specific library functions */
implemented_library_functions () {
    auto a;
    auto b;
    auto c;
    auto d;

    /* memset: a:addr, b:data, c:length */
    memset(a,b,c);

    /* memcpy: a:dst, b:src, c:lenght */
    memcpy(a,b,c);

    /*
     * display and scan
     * waits for a keystroke while displaying the display mem
     * on the display. The return value is the scan code of the
     * depressed key. Handles long key depression case.
     */
    d = das();

    /* disp: displays the display mem on the display for 'a' cycles*/
    disp(a);

    /* disp_push: push a character from the right */
    disp_push(0xD4);

    /*
     * add with carry: 1 byte full addition with carry
     * a: pointer to data1
     * b: data2
     * c: carry in (1 or 0)
     * result is stored at the address of 'a'
     * return value: carry_out (1 or 0)
     */
    d = adc(a,b,c);

    /* msb: returns (a & 0x80) */
    b = msb(a);

    /*
     * add: full addition on arbitrary long (max 256 bit) data
     * a: pointer to data1
     * b: pointer to data2
     * c: length in bytes
     * result is stored at the address of 'a'
     */
    add(a,b,c);

    /*
     * sub: subtraction on arbitrary long (max 256 bit) data
     * a: pointer to data1
     * b: pointer to data2
     * c: length in bytes
     * result is stored at the address of 'a'
     */
    sub(a,b,c);
}

