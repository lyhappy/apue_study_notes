DIR_OBJS = ../obj
DIR_BINS = ../bin

PROGS = ${DIR_BINS}/p7_2		\
		${DIR_BINS}/p7_3		\
		${DIR_BINS}/p7_5		\
		${DIR_BINS}/p7_6		\
		${DIR_BINS}/p7_6_O		\
		${DIR_BINS}/p7_8


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

${DIR_BINS}/p7_6_O: ${DIR_OBJS}/p7_6_O.o
	${LINK.c} -O -o $@ $<
${DIR_OBJS}/p7_6_O.o: p7_6.c
	${COMPILE.c} -O -o $@ $<


include ../makefile
