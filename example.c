/*
 * Sample (otherwise useless) program to demonstrate the capabilities
 * of the c-like compiler
 */


#define NUM_TEN     0x0A
/* All the #preprocessor directives are currently handled by gcc (gcc -E) */

/*
#инцлуд <стдио.х>

инт маин () {
    принтф ("хелло, ворлд!");
    ретурн 0;
}
*/

/* Global variables */
static var_glb;
/* are reset to zero before main function execution begins */


/*
 * There are no data types, everything is an 8-bit CPU-word.
 * Every function is assumed to return a value, there's
 * no 'void' function.
 * Function arguments are passed via the (emulated) stack.
 * The result of the expressions are stored in the accumulator;
 * also, the accumulator is used to transfer the return value of
 * functions to the caller. Hence, the return value of every function is
 * the value that's stored in the accumulator just when the function returns.
 * The 'retrun' statment is available and can have a value, however it
 * just works as if it was a 'goto' statement that jumps to the end of
 * the function; its only purpose is program flow control.
 */
one () {
    1;              /* This function returns '1' */
}

two () {
    return 2;       /* This function returns '2' */
}

three () {
    3;
    return;         /* This function returns '3' */
}

addition (b1, b2) {
    b1 + b2;        /* this function returns b1 + b2 */
}

/*
 * The below case is tricky: In order to break the loop, the program needs to
 * evaluate the 'i != 5' expression after each cycle; hence the last expression
 * that's being evaluated is not '1', but 'i != 5'. The same is true for
 * 'while' and 'do-while' cycles.
 */
return_false () {
    auto i;
    for (i = 0; i != 5; i += 1) {
        1;
    }
}

return_true () {
    auto i;
    for (i = 0; i != 5; i += 1) {
        return 1;   /* Got it? :) */
    }
}


/*
 * In-line assembly:
 * Expressions which are separated with commas, are being evaluated
 * sequentially after each other.
 * The first expression (a) loads the value of 'a' into the
 * accumulator. An 'a-1' statement would form the 2nd complement
 * of '1' and would add it to 'a'.
 * We can do it with a signle assembly instruction, the 2nd complement of
 * '1' is '255'; code execution speed can be significantly boosted
 * with these tricks.
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
            auto i_pp;
            /*
             * there's neither 'pointer' type, nor
             * pointer arythmetics; everything is a 8-bit 'word'
             * It's up to you how you use the variables.
             */
            i_p = &i;  /* addressof */
            i_pp = &i_p;  /* addressof */
            **i_pp += 1; /* arbitrary deep dereferencing */
            out(*i_p);
            var_glb += 1;
        }
        /*
         * No function prototypes, each function translates
         * to an assembler subroutine.
         */
        do {
            out(fibonacci(addition(one(), two() << three())));
        } while (0);
    }
}


fibonacci (n) {
    if (!n) {
        return n;
    } else if (n == 1) {
        return n;
    }
    return fibonacci(n - 1) + fibonacci(n - 2);  /* Arbitrary deep recursion */
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

    /*
     * msb: returns 1 if ((a & 0x80) != 0)
     * The CPU itself doesn't have bitwise AND facilities; however
     * it can shift and rotate bits to the left.
     * The MSB can be obtained by this tiny 'algorithm':
     * rol(a) - shl(a);
     */
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

