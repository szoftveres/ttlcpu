# ttlcpu

    Toolchain created to build programs for the
    'ttl-cpu' (http://digitarworld.uw.hu/ttlcpu.html)
    
    The toolchain contains:
    
     -  'mcc' A compiler which resembles to the 'C' programming language
        and implements a subset of it.
     -  'am' A very basic assembler implemented by using the
        capabilities of the gnu-gcc preprocessor
     -  'burner' Program that transfers assembled bytecode to
        the 'ttl-cpu' from a special programmer hardware based
        on the ATmega8 AVR mcu

    The main ttl-cpu program is in 'prog.c' file. It can be built and
    transferred to ttl-cpu by running the './b' shell script

    The './b' shell script:
     - builds the 'mcc' compiler for 'ttl-cpu' architecture
     - copies '/mcc/arch/ttlcpu/header.asm' to am/prog.asm
     - invokes 'mcc' to compile 'prog.c', and extends am/prog.asm with the
       assembly program output
     - extends am/prog.asm with '/mcc/arch/ttlcpu/footer.asm'
     - 'am' takes 'am/prog.asm', compiles it and generates the ttl-cpu bytecode
     - 'burner' takes the bytecode and AVRDude burns it into the AVR programmer
     - the AVR programmer writes the bytecode to the program memory of
       the ttl-cpu

