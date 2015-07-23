#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SAMPLE_RATE     (44100)

static unsigned char data[] = {
    'a',
    'G',
    '3',
    'd',
    'v',
    'V',
    'e',
    'G',
    'o',
    'O',
    '6',
    '7',
    '5',
};


void
gen_freq (int smp_rate, int freq, int period) {
    int halfcycle;
    int i;
    int p;
    
    halfcycle = ((((smp_rate * 10) / freq) + 5) / 20);

//    fprintf(stderr, "smp:%u, freq:%u, period:%u, halfcycle:%u\n", smp_rate, freq, period, halfcycle);

    for (p = 0; p != period; p++) {
        for (i = 0; i != halfcycle; i++) {
            putchar(0x70);
        }
    
        for (i = 0; i != halfcycle; i++) {
            putchar(0x00);
        }
    }

}

#define BIT_ONE() gen_freq(SAMPLE_RATE, 3300, 11)
#define BIT_ZERO() gen_freq(SAMPLE_RATE, 2700, 9)

void
put_byte (unsigned char b) {
    int i;

    for (i = 0; i != 8; i++) {
        if (b & 0x01) {
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

    for (i = 0; i != 900; i++) {
        BIT_ONE();              /* 3 sec lead-up */
    }
    for (i = 0; i != 256; i++) {
        BIT_ZERO();             /* Start bit */
        put_byte(i);
        BIT_ONE();              /* Stop bit */
    } 
    for (i = 0; i != 900; i++) {
        BIT_ONE();              /* 3 sec ramp-down */
    }
}


