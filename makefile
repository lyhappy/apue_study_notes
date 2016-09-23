
CC=gcc
COMPILE.c=$(CC) $(CFLAGS) $(CPPFLAGS) -c
LINK.c=$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS)


CFLAGS = -ansi -Wall -I ../include
CFLAGSO = -ansi -Wall -O

clean:
	rm obj/*
