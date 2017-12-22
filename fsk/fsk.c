#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SAMPLE_RATE     (30000)
#define BAUD_RATE       (300)

#define CYCLE_LO        (9)
#define CYCLE_HI        (11)

static unsigned char data[] = {

#include "../am/bytecode.h"

};

static signed int filterbuf[CYCLE_LO];
static int  idx;

void filterout (char c) {
    int i;
    signed int avg = 0;
    filterbuf[idx++] = (signed int)c;
    if (idx >= (sizeof(filterbuf)/sizeof(filterbuf[0]))) {
        idx = 0;
    }
    for (i = 0; i != (sizeof(filterbuf)/sizeof(filterbuf[0])); i++) {
        avg += filterbuf[i];
    }
    avg /= (signed int)(sizeof(filterbuf)/sizeof(filterbuf[0]));
    putchar((char)avg);
}


void
gen_freq (int smp_rate, int baud, int cycle) {
    int num_smp;
    int freq;
    int i;
    int p;

    freq = baud * cycle;
    num_smp = ((((smp_rate * 10) / freq) + 5) / 10);

    for (p = 0; p != cycle; p++) {
        for (i = 0; i != (num_smp / 2); i++) {
            filterout(-96);
        }

        for (i = 0; i != (num_smp / 2); i++) {
            filterout(96);
        }
        if (num_smp % 2) {
            filterout(0);
        }
    }
    return;
}

void
baud_stat (int smp_rate, int baud, int cycle) {
    int num_smp;
    int freq;
    int achieved_baud;

    freq = baud * cycle;
    num_smp = ((((smp_rate * 10) / freq) + 5) / 10);
    achieved_baud = (((smp_rate * 10) / (num_smp) / cycle) + 5 ) / 10;

    fprintf(stderr, "Statistics:\n  %d-bit signed PCM\n  desired_baud:%u\n  smp_rate:%uHz\n  "
            "freq:%uHz\n  cycle:%u\n  num_smp:%u\n  achieved_baud:%u\n  "
            "deviation:%d\%\n\n\n",
            (sizeof(data[0]) * 8),
            baud,
            smp_rate,
            freq,
            cycle,
            num_smp,
            achieved_baud,
            ((((achieved_baud * 1000) / baud) + 5)/10)-100);

}


#define BIT_ZERO() gen_freq(SAMPLE_RATE, BAUD_RATE, CYCLE_LO)
#define BIT_ONE() gen_freq(SAMPLE_RATE, BAUD_RATE, CYCLE_HI)

#define STAT_ZERO() baud_stat(SAMPLE_RATE, BAUD_RATE, CYCLE_LO)
#define STAT_ONE() baud_stat(SAMPLE_RATE, BAUD_RATE, CYCLE_HI)



void
put_byte (unsigned char b) {
    int i;

    for (i = 0; i != 8; i++) {
        if (b & 1) {
            BIT_ONE();
        } else {
            BIT_ZERO();
        }
        b = b >> 1;
    }
}


int
main (int argc, char** argv) {
    int i;

    STAT_ZERO();
    STAT_ONE();

    for (i = 0; i != (2 * BAUD_RATE); i++) {
        BIT_ONE();              /* 2 sec lead-in */
    }
    for (i = 0; i != (sizeof(data)/sizeof(data[0])); i++) {
        if (!(i % 256)) {       /* Page verify break */
            int k;
            for (k = 0; k != (BAUD_RATE / 4); k++) {
                BIT_ONE();
            }
        }

        BIT_ZERO();             /* Start bit */
        put_byte(data[i]);
        BIT_ONE();              /* Stop bit */
    }
    for (i = 0; i != (2 * BAUD_RATE); i++) {
        BIT_ONE();              /* 2 sec lead-out */
    }
}


