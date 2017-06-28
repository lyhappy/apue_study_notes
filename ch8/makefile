DIR_OBJS = ../obj
DIR_BINS = ../bin

PROGS = ${DIR_BINS}/p8_1		\
		${DIR_BINS}/p8_2		\
		${DIR_BINS}/p8_4		\
		${DIR_BINS}/p8_5		\
		${DIR_BINS}/p8_6		\
		${DIR_BINS}/p8_7		\
		${DIR_BINS}/p8_8		\
		${DIR_BINS}/p8_9		\
		${DIR_BINS}/p8_12		\
		${DIR_BINS}/p8_13		\
		${DIR_BINS}/p8_14		\
		${DIR_BINS}/p8_15		\
		${DIR_BINS}/p8_16		\
		${DIR_BINS}/p8_18		\
		${DIR_BINS}/p8_17		\
		echoall		\
		echoarg		\
		tsys		\
		printuids



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

${DIR_BINS}/p8_4: ${DIR_OBJS}/p8_4.o ${DIR_OBJS}/p8_3.o error.o

${DIR_BINS}/p8_13: ${DIR_OBJS}/p8_13.o ${DIR_OBJS}/p8_3.o error.o

${DIR_BINS}/p8_18: ${DIR_OBJS}/p8_18.o ${DIR_OBJS}/p8_3.o error.o

${DIR_BINS}/p8_12: ${DIR_OBJS}/p8_12.o ${DIR_OBJS}/p8_11.o ${DIR_OBJS}/p8_3.o ${DIR_OBJS}/error.o
	${LINK.c} -o $@ $^

echoall:${DIR_BINS}/p8_8 
	mv ${DIR_BINS}/p8_8 ${DIR_BINS}/echoall
echoarg:${DIR_BINS}/echoall
	cp ${DIR_BINS}/echoall ${DIR_BINS}/echoarg
tsys:${DIR_BINS}/p8_13
	mv ${DIR_BINS}/p8_13 ${DIR_BINS}/tsys
printuids:${DIR_BINS}/p8_14
	mv ${DIR_BINS}/p8_14 ${DIR_BINS}/printuids

.INTERMEDIATE:${OBJS}

include ../makefile
