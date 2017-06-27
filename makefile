
CC=gcc
COMPILE.c ?= $(CC) $(CFLAGS) $(CPPFLAGS) -c -ansi
LINK.c ?= $(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS)

VPATH = include:obj:lib


CFLAGS = -Wall 
CFLAGSO = -Wall -O

.INTERMEDIATE:${OBJS} obj/*

clean:
	-@rm ../obj/*.o ../bin/*
