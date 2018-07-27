OBJDIR = .
SRCDIR = .
INCLUDE = -I mcc -I mcc/lex
OUTDIR = .

## General Flags
PROGRAM = cc
CC = gcc
LD = gcc
CFLAGS = -Wall -O0 $(INCLUDE)

## Objects that must be built in order to link
OBJECTS = $(OBJDIR)/main.o          \
          $(OBJDIR)/codegen.o       \


SUBDIRS = mcc
.PHONY : $(SUBDIRS)

## Build both compiler and program
all: $(PROGRAM)

$(SUBDIRS):
	git submodule update --init
	$(MAKE) -C $@

## Compile source files
$(OBJDIR)/%.o : $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c -o $(OBJDIR)/$*.o $< 

## Link object files to build the compiler
$(PROGRAM): $(SUBDIRS) $(OBJECTS)
	$(LD) $(OBJECTS) mcc/mcc.a -o $(OUTDIR)/$(PROGRAM)

clean:
	-rm -rf $(OBJECTS) $(OUTDIR)/$(PROGRAM) 
	for dir in $(SUBDIRS); do $(MAKE) clean -C $$dir; done


