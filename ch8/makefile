DIR_OBJS = ../obj
DIR_BINS = ../bin

PROGS = ${DIR_BINS}/p8_1		\
		${DIR_BINS}/p8_2		\
		${DIR_BINS}/p8_4		\
		${DIR_BINS}/p8_5		\
		${DIR_BINS}/p8_6		\
		${DIR_BINS}/p8_7		\
		${DIR_BINS}/p8_8		\
		echoall


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

${DIR_BINS}/%: ${DIR_OBJS}/%.o error.o	pathalloc.o ${DIR_OBJS}/p8_3.o
	${LINK.c} -o $@ $^

echoall:${DIR_BINS}/p8_8 
	mv ${DIR_BINS}/p8_8 ${DIR_BINS}/echoall

include ../makefile
