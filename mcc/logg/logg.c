#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "logg.h"


static FILE* fl;



void
logg (const char* fmt, ...) {
    va_list ap;

    if (!fl) {
        return;
    }
    va_start(ap, fmt);
    vfprintf(fl, fmt, ap);
    va_end(ap);
}



void
logg_init (int argc, char **argv) {
    int opt;
    while ((opt = getopt(argc, argv, "l:")) > 0) {
        switch (opt) {
          case 'l':
            fl = fopen(optarg, "w");
            if (!fl) {
                fprintf(stderr, "Unable to open [-l %s]\n", optarg);
                exit(-1);
            }
            break;
        }
    }
}

