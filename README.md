# ttlcpu

Toolchain for the 8-bit TTL CPU:
```
http://digitarworld.uw.hu/ttlcpu.html
```

Toolchain elements:

 *  mcc - A compiler which resembles to the 'C' programming language
    and implements a subset of it. (see 'example.c' for details)
 *  am - A very basic assembler implemented by using the
    capabilities of the gnu-gcc preprocessor
 *  burner - Program that transfers assembled bytecode to
    the 'ttl-cpu' from a special programmer hardware based
    on the ATmega8 AVR mcu
 *  fsk - A tool to create a raw audio from a data stream (e.g.
    a program) for the on-board 300 baud acoustic serial receiver

The ttl-cpu program can be built and transferred to ttl-cpu by running
the './b' shell script ( syntax: ./b <source_file.c> )

The './b' shell script:
 - builds the 'mcc' compiler for 'ttl-cpu' architecture
 - copies '/mcc/arch/ttlcpu/header.asm' to am/prog.asm
 - invokes 'mcc' to compile the specified source file, and extends
   am/prog.asm with the assembly program output
 - extends am/prog.asm with '/mcc/arch/ttlcpu/footer.asm'
 - 'am' takes 'am/prog.asm', compiles it and generates the ttl-cpu bytecode
 - 'burner' takes the bytecode and AVRDude burns it into the AVR programmer
 - the AVR programmer writes the bytecode to the program memory of
   the ttl-cpu


## Note

'mcc' has x86-64 support on Linux, try this:
```
./bx misc/unix_prog.c
./unix_prog.bin
```


