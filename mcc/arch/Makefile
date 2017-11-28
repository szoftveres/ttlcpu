OBJDIR = .
SRCDIR = .
INCLDIR = .
OUTDIR = .

## General Flags
CC = gcc
LD = gcc
CFLAGS = -Wall -O0 -I $(INCLDIR)

## Objects that must be built in order to link
OBJECTS = $(OBJDIR)/codegen_i386.o\
          $(OBJDIR)/codegen_ttlcpu.o\
          $(OBJDIR)/codegen_x86_64.o


## Build both compiler and program
all: $(OBJECTS)

## Compile source files
$(OBJDIR)/%.o : $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c -o $(OBJDIR)/$*.o $< 

clean:
	-rm -rf $(OBJECTS)

