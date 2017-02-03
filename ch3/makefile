DIR_OBJS = ../obj
DIR_BINS = ../bin

PROGS = ${DIR_BINS}/p3_1 	\
		${DIR_BINS}/p3_2	\
		${DIR_BINS}/p3_3	\
		${DIR_BINS}/p3_4

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

${DIR_BINS}/%: ${DIR_OBJS}/%.o error.o
	${LINK.c} -o $@ $^

include ../makefile
