# ttlcpu

Toolchain for the 8-bit TTL CPU:
```
http://digitarworld.uw.hu/ttlcpu.html
https://hackaday.io/project/11703-8-bit-ttl-cpu
```

Toolchain elements:

 *  [mcc](https://github.com/szoftveres/ttlcpu/tree/master/mcc) - A compiler that implements a subset the
    'C' programming language (see [example.c](https://github.com/szoftveres/ttlcpu/blob/master/example.c)).
 *  [am](https://github.com/szoftveres/ttlcpu/tree/master/am) - A basic assembler implemented in gnu-gcc
    preprocessor macros.
 *  [burner](https://github.com/szoftveres/ttlcpu/tree/master/burner) - AtMega8 firmware for loading bytecode
    into the program memory of the ttl-cpu.

The ttl-cpu program can be built and transferred to ttl-cpu by running
the './b' script ( syntax: ./b <source_file.c> )

The './b' shell script:
 - Builds the 'mcc' compiler for 'ttl-cpu' architecture.
 - Copies '/mcc/arch/ttlcpu/header.asm' to am/prog.asm.
 - Invokes 'mcc' to compile the specified source file, and
   extends am/prog.asm with the output assembly program.
 - Extends am/prog.asm with '/mcc/arch/ttlcpu/footer.asm'.
 - Runs 'am', which assemles 'am/prog.asm'  and generates
   the ttl-cpu bytecode.
 - Builds 'burner', which includes the ttl-cpu bytecode.
 - Invokes AvrDude to transfer 'burner' to the Atmega8
   burner board, which finally transfers the bytecode to
   the ttl-cpu.


## Note

'mcc' has x86-64 support, try this on a 64-bit Linux PC:
```
./bx misc/unix_prog.c
echo -n Hello World | ./unix_prog.bin /bin/ls -l .
```
See [mcc/arch/x86_64_Linux/header.s](https://github.com/szoftveres/ttlcpu/blob/master/mcc/arch/x86_64_Linux/header.s) for implemented system calls,
and [misc/unix_routines.c](https://github.com/szoftveres/ttlcpu/blob/master/misc/unix_routines.c) for helper functions.

