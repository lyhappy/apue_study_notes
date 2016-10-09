
CC=gcc
COMPILE.c ?= $(CC) $(CFLAGS) $(CPPFLAGS) -c
LINK.c ?= $(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS)

VPATH = include:obj:lib


CFLAGS = -ansi -Wall 
CFLAGSO = -ansi -Wall -O

clean:
	-@rm obj/*.o ../obj/*.o ../bin/*
