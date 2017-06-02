DIR_OBJS = ../obj
DIR_BINS = ../bin

PROGS = ${DIR_BINS}/p7_3 	


SRCS=$(wildcard *.c)
OBJS=$(patsubst %.c,$(DIR_OBJS)/%.o,$(SRCS))

INCLUDE = -I ../include/
COMPILE.c += ${INCLUDE}

vpath %.c .:../lib
vpath %.h ../include
vpath %.o ../obj

all: ${PROGS}
	
${DIR_OBJS}/%.o: %.c
	${COMPILE.c} -o $@ $<

${DIR_BINS}/%: ${DIR_OBJS}/%.o error.o	pathalloc.o 
	${LINK.c} -o $@ $^

include ../makefile
