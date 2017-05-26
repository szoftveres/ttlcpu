#include <stdio.h>
#include <stdlib.h>
#include "lex.h"
#include "parser.h"

int main (int argc, char* argv[]) {
    lex_init();
    program();
    exit(0);
}



