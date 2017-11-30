
/* Print unsigned decimal integer */
putu (num) {
    if(num / 10){
        putu(num / 10);
    }
    putc((num % 10) + '0');
}

putx_internal (num) {
    if (num / 0x10) {
        putx_internal(num / 0x10);
    }
    putc((num % 0x10) + (((num % 0x10) >= 10) ? ('a' - 10) : ('0')));
}

/* Print hexadecimal integer */
putx (num) {
   putc('0');
   putc('x');
   putx_internal(num);
}

/* Print string */
puts (s) {
    while (*s & 0xFF) {
        putc(*s & 0xFF);
        s += 1;
    }
}


