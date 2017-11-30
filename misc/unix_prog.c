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


main (argc, argv) {
    auto i;

    msg("x86-64");

    puts("argc: "); putu(argc); puts("\n");

    for (i = 0; i != argc; i += 1) {
        puts("arg ");
        putu(i);
        puts(": ");
        puts(getarg(argv, i));
        puts("\n");
    }

    for (i = 30; i != 40; i += 1) {
        putu(i);
        puts(": ");
        putu(fibonacci(i));
        puts("\n");
    }
    0;
}


