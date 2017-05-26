DIR_OBJS = ../obj
DIR_BINS = ../bin

PROGS = ${DIR_BINS}/p4_1 	\
		${DIR_BINS}/p4_2	\
		${DIR_BINS}/p4_3	\
		${DIR_BINS}/p4_4	\
		${DIR_BINS}/p4_5	\
		${DIR_BINS}/p4_5_1	\
		${DIR_BINS}/p4_6	\
		${DIR_BINS}/p4_7	\
		${DIR_BINS}/p4_8	\
		${DIR_BINS}/p4_9	\
		${DIR_BINS}/p4_10	


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
