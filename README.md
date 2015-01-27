# ttlcpu

    Toolchain created to build programs for the
    'ttl-cpu' (http://digitarworld.uw.hu/ttlcpu.html)
    
    The toolchain contains:
    
     -  'mcc' A compiler which resembles to 'C' and implements
        a subset of the 'C' programming language
    
     -  'am' A very basic assembler implemented by using the
        capabilities of the gnu-gcc preprocessor
    
     -  'burner' Program that transfers assembled bytecode to
        the 'ttl-cpu' from a special programmer hardware based
        on the ATmega8 mcu


    To build a program, first edit 'prog.c' then run './b' shell script

    When you invoke './b' shell script:
     - It builds 'mcc' for 'ttl-cpu' architecture
     - Copies '/mcc/arch/ttlcpu/template.asm' to am/prog.asm
     - 'mcc' compiles 'prog.c' and extends am/prog.asm with the program output
     - 'am' takes 'am/prog.asm' and generates the bytecode
     - 'burner' takes bytecode and AVRDude burns it into the programmer

