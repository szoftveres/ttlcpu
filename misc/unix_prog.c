#include "unix_routines.c"


fibonacci (n) {
    if (n < 2) {
        return n;
    }
    return fibonacci(n - 1) + fibonacci(n - 2);  /* Arbitrary deep recursion */
}


main (argc, argv) {
    auto i;
    auto c;

    puts("Hello World!\n");
    puts("argc: "); putd(argc); puts("\n");

    for (i = 0; i != argc; i += 1) {
        puts("argv[");
        putd(i);
        puts("]: ");
        puts(getarg(argv, i));
        puts("\n");
    }

    c = fork();
    if (c) {
        auto code;
        code = 0;
        puts("parent: waiting\n");
        waitpid(c, &code);
        puts("parent: child exited with code (");
        putd(code / 256);
        puts(")\n");
    } else {
        execv(getarg(argv, 1), getargaddr(argv, 1));
        puts("child: error, could not execute [");
        puts(getarg(argv, 1));
        puts("]\n");
        exit(1);
    }

    for (i = 0; i != 100; i += 1) {
        switch (i) {
          case 30:
          case 32:
          case 34:
          case 36:
          case 38:
          case 40:
            puts("fibonacci(");
            putd(i);
            puts(") = ");
            putd(fibonacci(i));
            puts("\n");
            break;
        }
    }

    while ((c = getchar()) != EOF) {
        puts("\'");
        putchar(c);
        puts("\' ");
        putx(c);
        puts("\n");
    }

    0; /* return code of 'main' is the exit code of the program */
}

