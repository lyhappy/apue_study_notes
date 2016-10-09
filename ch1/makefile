DIR_OBJS = ../obj
DIR_BINS = ../bin

PROGS = ${DIR_BINS}/p1_1 	\
		${DIR_BINS}/p1_2 	\
		${DIR_BINS}/p1_3 	\
		${DIR_BINS}/p1_4 	\
		${DIR_BINS}/p1_5 	\
		${DIR_BINS}/p1_6 	\
		${DIR_BINS}/p1_7 	\
		${DIR_BINS}/p1_8
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
