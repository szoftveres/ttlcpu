#include <stdio.h>
#include <stdlib.h>
#include "lex.h"
#include "parser.h"
#include "logg.h"

int main (int argc, char* argv[]) {
    logg_init(argc, argv);
    lex_init();
    program();
    exit(0);
}



