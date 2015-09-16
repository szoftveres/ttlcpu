#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

const unsigned char program[] PROGMEM = {

#include "../am/bytecode_part.h"

};

#define SETDDR0  do {DDRD  |=  (1<<5);} while (0);
#define RESDDR0  do {DDRD  &= ~(1<<5);} while (0);
#define SETD0    do {PORTD |=  (1<<5);} while (0);
#define RESD0    do {PORTD &= ~(1<<5);} while (0);

#define SETDDR1  do {DDRB  |=  (1<<7);} while (0);
#define RESDDR1  do {DDRB  &= ~(1<<7);} while (0);
#define SETD1    do {PORTB |=  (1<<7);} while (0);
#define RESD1    do {PORTB &= ~(1<<7);} while (0);

#define SETDDR2  do {DDRB  |=  (1<<6);} while (0);
#define RESDDR2  do {DDRB  &= ~(1<<6);} while (0);
#define SETD2    do {PORTB |=  (1<<6);} while (0);
#define RESD2    do {PORTB &= ~(1<<6);} while (0);

#define SETDDR3  do {DDRD  |=  (1<<4);} while (0);
#define RESDDR3  do {DDRD  &= ~(1<<4);} while (0);
#define SETD3    do {PORTD |=  (1<<4);} while (0);
#define RESD3    do {PORTD &= ~(1<<4);} while (0);

#define SETDDR4  do {DDRD  |=  (1<<3);} while (0);
#define RESDDR4  do {DDRD  &= ~(1<<3);} while (0);
#define SETD4    do {PORTD |=  (1<<3);} while (0);
#define RESD4    do {PORTD &= ~(1<<3);} while (0);

#define SETDDR5  do {DDRD  |=  (1<<2);} while (0);
#define RESDDR5  do {DDRD  &= ~(1<<2);} while (0);
#define SETD5    do {PORTD |=  (1<<2);} while (0);
#define RESD5    do {PORTD &= ~(1<<2);} while (0);

#define SETDDR6  do {DDRD  |=  (1<<1);} while (0);
#define RESDDR6  do {DDRD  &= ~(1<<1);} while (0);
#define SETD6    do {PORTD |=  (1<<1);} while (0);
#define RESD6    do {PORTD &= ~(1<<1);} while (0);

#define SETDDR7  do {DDRD  |=  (1<<0);} while (0);
#define RESDDR7  do {DDRD  &= ~(1<<0);} while (0);
#define SETD7    do {PORTD |=  (1<<0);} while (0);
#define RESD7    do {PORTD &= ~(1<<0);} while (0);



#define SETDDWR  do {DDRB  |=  (1<<0);} while (0);
#define RESDDWR  do {DDRB  &= ~(1<<0);} while (0);
#define SETWR    do {PORTB |=  (1<<0);} while (0);
#define RESWR    do {PORTB &= ~(1<<0);} while (0);

#define SETDDRP  do {DDRD  |=  (1<<7);} while (0);
#define RESDDRP  do {DDRD  &= ~(1<<7);} while (0);
#define SETP     do {PORTD |=  (1<<7);} while (0);
#define RESP     do {PORTD &= ~(1<<7);} while (0);



/*
 * Set all outputs to initial state
 */

/* Data bits */
void arm_d (void) {
    SETDDR0;
    SETDDR1;
    SETDDR2;
    SETDDR3;
    SETDDR4;
    SETDDR5;
    SETDDR6;
    SETDDR7;
}

/* Control bits */
void arm_c (void) {
    SETWR;        /* wr active low */
    RESP;         /* pc active high */

    SETDDWR;
    SETDDRP;

    SETWR;        /* wr active low */
    RESP;         /* pc active high */
}



/*
 * Set all outputs to initial state
 */

/* Data bits */
void unarm_d (void) {
    RESDDR0;
    RESDDR1;
    RESDDR2;
    RESDDR3;
    RESDDR4;
    RESDDR5;
    RESDDR6;
    RESDDR7;
}

/* Control bits */
void unarm_c (void) {
    SETWR;        /* wr active low */
    RESP;         /* pc active high */

    RESDDWR;
    RESDDRP;

    RESWR;        /* release pull-ups */
    RESP;         /* release pull-ups */

}

#define PULSELEN (2)

/*
 * write the data
 */
void write (unsigned char data) {
    int i;

    if (data & (0x01 << 0)) {SETD0;} else {RESD0;}
    if (data & (0x01 << 1)) {SETD1;} else {RESD1;}
    if (data & (0x01 << 2)) {SETD2;} else {RESD2;}
    if (data & (0x01 << 3)) {SETD3;} else {RESD3;}
    if (data & (0x01 << 4)) {SETD4;} else {RESD4;}
    if (data & (0x01 << 5)) {SETD5;} else {RESD5;}
    if (data & (0x01 << 6)) {SETD6;} else {RESD6;}
    if (data & (0x01 << 7)) {SETD7;} else {RESD7;}

    arm_d();

    for (i = 0; i < PULSELEN; i++) {SETWR;} /* data setup time */
    for (i = 0; i < PULSELEN; i++) {RESWR;}
    for (i = 0; i < PULSELEN; i++) {SETWR;}

    unarm_d();

    for (i = 0; i < PULSELEN; i++) {SETP;}
    for (i = 0; i < PULSELEN; i++) {RESP;}

    /* Verify byte here if necessary */

    for (i = 0; i < PULSELEN; i++) {SETP;}
    for (i = 0; i < PULSELEN; i++) {RESP;}

}

int main (void){
    int i;

    for (i = 0; i < 30000; i++) {
        SFIOR |= (1 << PUD);
    }

    arm_c();

    for (i = 0; i < (sizeof(program)/sizeof(program[0])); i++) {
        write(pgm_read_byte(&program[i]));
    }

    unarm_c();

    while (1) {

        MCUCR = (MCUCR & ((~((1<<SM0) | (1<<SM2)))) | (1<<SM1));
        MCUCR |= (1<<SE);    // Enable sleep

        asm volatile (
            "\n\t cli"
            "\n\t sleep"
            "\n\t");
    }

    return 0;
}
