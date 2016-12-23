OBJDIR = obj
SRCDIR = src
INCLUDE = -I include -I arch -I lex
ARCHDIR = arch/$(ARCH)
OUTDIR = output

## General Flags
PROGRAM = mcc
CC = gcc
LD = gcc
CFLAGS = -Wall -O0 $(INCLUDE)

## Objects that must be built in order to link
OBJECTS = $(OBJDIR)/main.o\
          $(OBJDIR)/parser.o\
          $(OBJDIR)/sym.o\


ASMHEADER = header.asm
ASMFOOTER = footer.asm

## Build both compiler and program
all: $(PROGRAM)

codegen:
	cd arch && make

lex:
	cd lex && make

.PHONY : lex codegen

## Compile source files
$(OBJDIR)/%.o : $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c -o $(OBJDIR)/$*.o $< 

## Link object files to build the compiler
$(PROGRAM): codegen lex $(OBJECTS) $(ARCHDIR)/$(ASMHEADER) $(ARCHDIR)/$(ASMFOOTER)
	cp $(ARCHDIR)/$(ASMHEADER) $(OUTDIR)/
	cp $(ARCHDIR)/$(ASMFOOTER) $(OUTDIR)/
	$(LD) $(OBJECTS) lex/lex.o arch/codegen_$(ARCH).o -o $(OUTDIR)/$(PROGRAM)

clean:
	-rm -rf $(OBJDIR)/* $(OUTDIR)/*

