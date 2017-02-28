OBJDIR = cc
SRCDIR = cc
INCLUDE = -I cc -I arch -I lex -I logg
ARCHDIR = arch/$(ARCH)
OUTDIR = .

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

SUBDIRS = arch lex logg
.PHONY : $(SUBDIRS)

## Build both compiler and program
all: $(PROGRAM)

$(SUBDIRS):
	$(MAKE) -C $@

## Compile source files
$(OBJDIR)/%.o : $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c -o $(OBJDIR)/$*.o $< 

## Link object files to build the compiler
$(PROGRAM): $(SUBDIRS) $(OBJECTS)
	$(LD) $(OBJECTS) lex/lex.o arch/codegen_$(ARCH).o logg/logg.o -o $(OUTDIR)/$(PROGRAM)

clean:
	-rm -rf $(OBJECTS) $(OUTDIR)/$(PROGRAM) 
	for dir in $(SUBDIRS); do $(MAKE) clean -C $$dir; done


