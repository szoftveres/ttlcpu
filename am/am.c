#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ============================================================ */
/* data sources and destinations */

#define to_acc          (0b000)
#define to_acc_invert   (0b100)
#define to_acc_adder    (0b010)
#define to_port         (0b110)
#define to_ramaddr      (0b001)
#define to_ram          (0b101)
#define to_pc           (0b011)
#define to_pch          (0b111)

#define frm_acc         (0b000)
#define frm_ram         (0b101)
#define literal         (0b100)
#define frm_port        (0b110)

struct lbl_s {
    char            *name;
    unsigned int    addr;
    struct lbl_s    *next;
};

unsigned int    progcnt;
struct lbl_s    *head;

static void lbladd(char* n) {
    struct lbl_s *lbl;
    for (lbl = head; lbl; lbl=lbl->next) {
        if (!strcmp(lbl->name, (n))) {
            fprintf(stderr, "ERROR: label %s already defined\n", (n));
            exit(1);
        }
    }
    lbl = (struct lbl_s*) malloc(sizeof(struct lbl_s));
    lbl->name = (char*) malloc(strlen((n))+1);
    strcpy(lbl->name, (n));
    lbl->addr = progcnt;
    lbl->next = head;
    head = lbl;
    return;
}

static void lblprint(char* n) {
    fprintf(stderr, "%s:\n", n);
}

static void instruction (int d, int s, char* m) {
    char* mod = m;
    char* de = "?", *so = "?";
    int i;
    unsigned char ic = ((d << 5) | (s << 2) | 1);
    while (*mod) {
      switch (*mod) {
        case 'Z' : case 'z' :
            ic &= ~((unsigned char)1);
            break;
        case 'C' : case 'c' :
            ic |= 2;
            break;
        }
        mod++;
    }
    printf(" 0x%02X,\n", ic);
    fprintf(stderr, "  %04X:  ", progcnt);
    for (i = 0; i < 8; i++) {
        fprintf(stderr, (ic & 0x80) ? "1" : "0");
        ic = ic << 1;
    }
    switch (d) {
      case to_acc : de = "acc"; break;
      case to_acc_invert : de = "acc_invert"; break;
      case to_acc_adder : de = "acc_adder"; break;
      case to_port : de = "port"; break;
      case to_ramaddr : de = "ramaddr"; break;
      case to_ram : de = "ram"; break;
      case to_pc : de = "pc"; break;
      case to_pch : de = "pch_latch"; break;
      default: fprintf(stderr, "         %d <- %d %s\n", d, s, m); exit(1); break;
    }
    switch (s) {
      case frm_acc : so = "acc"; break;
      case literal : so = "literal"; break;
      case frm_port : so = "port"; break;
      case frm_ram : so = "ram"; break;
      default: fprintf(stderr, "         %d <- %d %s\n", d, s, m); exit(1); break;
    }
    if ((s == frm_ram) && (d == to_ram)) {
        fprintf(stderr, "   ILLEGAL ram <- ram \n"); exit(1);
    }
    fprintf(stderr, "         %12s <- %0.12s %s\n", de, so, m);
    progcnt += 1;
    return;
}


static int defaddr (char* n, int high) {
    int i;
    unsigned char ic;
    unsigned char ic2;
    struct lbl_s *lbl = head;

    while (lbl) {
        if (!strcmp(lbl->name, (n))) {break;}
        lbl = lbl->next;
    }
    if (!lbl) {
        fprintf(stderr, "ERROR: label %s not found\n", (n));
        exit(1);
    }
    ic = (unsigned char) (lbl->addr >> (high ? 8 : 0));
    printf(" 0x%02X,\n", ic);
    fprintf(stderr, "  %04X:  ", progcnt);
    for (i = 0, ic2 = ic; i < 8; i++, ic2 = ic2 << 1) {
        fprintf(stderr, (ic2 & 0x80) ? "1" : "0");
    }
    fprintf(stderr, "         %08s(%s): 0x%02X\n",
            lbl->name,
            (high ? "Hi" : "Lo"),
            ic);
    progcnt += 1;
    return;
}

static int dataconst (unsigned int v, int shift) {
    int i;
    unsigned char ic = (unsigned char)(v >> (shift ? 8 : 0));
    unsigned char ic2;

    printf(" 0x%02X,\n", ic);
    fprintf(stderr, "  %04X:  ", progcnt);
    for (i = 0, ic2 = ic; i < 8; i++, ic2 = ic2 << 1) {
        fprintf(stderr, (ic2 & 0x80) ? "1" : "0");
    }
    fprintf(stderr, "          lit: 0x%02X\n", ic); 
    progcnt += 1;
    return;
}

int
main (int argc, char** argv) {
    head = NULL;
    
    progcnt = 0;
    #include "am.h"
    progcnt = 0;
    #include "am.h"

    return 0;
}
