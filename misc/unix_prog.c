#include "unix_routines.c"


msg (s) {
    puts("\nThis program runs on ");
    puts(s);
    puts(" architecture.\n");
}


fibonacci (n) {
    if (!n) {
        return n;
    } else if (n == 1) {
        return n;
    }
    return fibonacci(n - 1) + fibonacci(n - 2);  /* Arbitrary deep recursion */
}


main () {
    auto i;

    msg("x86-64");

    for (i = 0; i != 10; i += 1) {
        putu(i);
        puts(": ");
        putu(fibonacci(i));
        puts("\n");
    }
    0;
}



