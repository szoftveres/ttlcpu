OBJDIR = .
SRCDIR = .
INCLDIR = .
OUTDIR = .

## General Flags
CC = gcc
LD = gcc
CFLAGS = -Wall -O0 -I $(INCLDIR)

## Objects that must be built in order to link
OBJECTS = $(OBJDIR)/logg.o


## Build both compiler and program
all: $(OBJECTS)

## Compile source files
$(OBJDIR)/%.o : $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c -o $(OBJDIR)/$*.o $< 

clean:
	-rm -rf $(OBJECTS)

