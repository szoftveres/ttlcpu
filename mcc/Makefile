OBJDIR = obj
SRCDIR = src
INCLDIR = include
ARCHDIR = arch/$(ARCH)
OUTDIR = output

## General Flags
PROGRAM = mcc
CC = gcc
LD = gcc
CFLAGS = -Wall -O0 -I $(INCLDIR)

## Objects that must be built in order to link
OBJECTS = $(OBJDIR)/lex.o\
          $(OBJDIR)/main.o\
          $(OBJDIR)/grammar.o\
          $(OBJDIR)/sym.o\
          $(OBJDIR)/codegen.o\


ASMHEADER = header.asm

## Build both compiler and program
all: $(PROGRAM)

## Compile target architecture dependent source files
$(OBJDIR)/%.o : $(ARCHDIR)/%.c
	$(CC) $(CFLAGS) -c -o $(OBJDIR)/$*.o $< 

## Compile source files
$(OBJDIR)/%.o : $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c -o $(OBJDIR)/$*.o $< 

## Link object files to build the compiler
$(PROGRAM): $(OBJECTS) $(ARCHDIR)/$(ASMHEADER)
	cp $(ARCHDIR)/$(ASMHEADER) $(OUTDIR)/
	$(LD) $(OBJECTS) -o $(OUTDIR)/$(PROGRAM)

clean:
	-rm -rf $(OBJDIR)/* $(OUTDIR)/*

