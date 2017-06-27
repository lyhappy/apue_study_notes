DIR_OBJS = ../obj
DIR_BINS = ../bin


SRCS=$(wildcard *.c)
OBJS=$(patsubst %.c,$(DIR_OBJS)/%.o,$(SRCS))

INCLUDE = -I ../include
COMPILE.c += ${INCLUDE}

vpath %.c .:../lib
vpath %.h ../include
vpath %.o ../obj

all: ${OBJS}

${DIR_OBJS}/%.o: %.c
	${COMPILE.c} -o $@ $<
	
.PRECIOUS:${OBJS}

include ../makefile
