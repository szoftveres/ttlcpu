/*
 * Sample (otherwise useless) program to demonstrate the capabilities
 * of the c-like compiler
 */


#define NUM_TEN     0x0A
#define OFFSET      0x01
/*
 * All the #preprocessor directives and comments are currently
 * handled by gcc (gcc -E)
 */

/*
 * This is a typeless language, the CPU word determines the size of the main
 * data type (8-bit for the ttl-cpu, 64 bit for x86-64). A CPU word may
 * represent data, as well as a pointer to data (or to another pointer).
 * the actual size of the CPU word can be retrieved by the 'sizeof()' function
 * (no arguments needed).
 * Function arguments are placed onto the stack in order(!) prior to function
 * call, the return value of a function is the data that is in the main data
 * register (accumulator, rax, eax, etc..) upon return.
 * Expressions are evaluated into the main data register, hence the assumed
 * return value of a function is the value of the last evaluated expression.
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
    b1 + b2;        /* this function returns the value of b1 + b2 */
}

myaddress () {
    0x10;           /* This function returns 0x10 */
}

cpu_word_in_bytes () {
    sizeof();       /* Doesn't need any arguemts */
}

/*
 * The below function returns, when the loop finishes with 'i != 5' expression
 * becoming 'false'. The return value of the function hence will be '0' (false),
 * because 'i != 5' was the last evaluated expression.
 */

#define TRUE    (1)

return_false () {
    auto i;
    for (i = 0; i != 5; i += 1) {
        TRUE;
    }
}

/*
 * The below function will return 'true' (1), because the loop will break with
 * an explicit 'return' statement, which is executed after loading the value of
 * the last evaluated expression (TRUE) into the main data register.
 */

return_true () {
    auto i;
    for (i = 0; i != 5; i += 1) {
        return TRUE;
    }
}

/*
 * A function begins with the 'name()' and a satement.
 * A '{block}' itself is a statement that contains multiple statements,
 * however functions can be written without involving a '{block}' as long
 * as the function body itself is a single statement.
 * Below are examples of legal function definitions:
 */

five (a) while (1) for (a = 0; a != 10; a += 1) if (a == 5) return a;
add2 (a, b) a + b;
sub2 (a, b) a - b;
nop (); /* This is the definition (not pre-declaration) of an empty function */

/*
 * In-line assembly, comma separated expressions.
 */
minus_one (a) {
    a, asm("add(progdata) data(0xFF)"); /* Quick two's complement addition */
}


/*
 * The main funcion.
 */
main () {
    /* Variable declarations at the beginning of each block */
    auto i;
    auto g;

    while (1) {
        /*
         * Built-in operator precedence can be overriden with () parentheses
         */
        for (i = NUM_TEN; (i - 1 != 0xFF - (1 == 2)) ? 1 : 0; i += addition(1, 0)) {
            auto i_p;
            auto i_pp;

            i_p = &i;  /* addressof */
            i_pp = &i_p;  /* addressof */
            **i_pp += 1; /* arbitrary deep dereferencing */
            out(*i_p);
            g += 1;

            switch (i) {
              case 10:
              case 11:
              case 12:
                continue;    /* continue 'for' cycle */
              default:
                break;       /* break from switch */
            }
            if (i == 13) {break;} /* break from 'for' cycle */

            /*
             * Any primary expression can be dereferenced, including the
             * return value of functions.
             */
            *myaddress() = *(0x10 + OFFSET);
            *0x01 = 0x05;  /* Direct write 0x05 to memory address 0x01 */
            *0x01 = *0x02; /* Copy data from 0x02 to 0x01 */

        }

        /* 'do {}' is a single cycle loop, equivalent to 'do {} while(0);' */
        do {
            if (i == 5)
                break;
            if (i == 6)
                continue; /* same effect as 'break' */
            out(i);
        }

        /*
         * Function forward declarations are not needed; function calls and
         * definitions directly translate to assembler subroutine calls and
         * assembler labels respectively.
         */
        do {
            out(fibonacci(addition(one(), two() << three())));
        } while (0);
    }
}

/* Arbitrary deep recursion */
fibonacci (n) {
    if (n < 2) {
        return n;
    }
    return fibonacci(n - 1) + fibonacci(n - 2);
}


/* Interesting stuff inside */
#include "misc/ttlcpu_routines.c"


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

