OBJS = p2_2.o
DIR_OBJS = ../obj
DIR_BIN = ../bin
INCLUDE = -I ../include/
COMPILE.c += ${INCLUDE}

PROGS = ${DIR_BIN}/p2_4 ${DIR_BIN}/p2_2
SRCS=$(wildcard *.c)
OBJS=$(patsubst %.c,$(DIR_OBJS)/%.o,$(SRCS))


COMMON_OBJS = ../obj/error.o

vpath %.c .:../lib
vpath %.h ../include
vpath %.o ../obj

all: ${PROGS}

error.o: ../lib/error.c
	cd ../lib && make all 

${DIR_OBJS}/%.o: %.c
	${COMPILE.c} -o $@ $<
	
${DIR_BIN}/p2_2: ${DIR_OBJS}/p2_2.o error.o
	${LINK.c} -o $@ $^
	
p2_2.c: p2_1.awk
	awk -f $< > $@

${DIR_BIN}/p2_4: ${DIR_OBJS}/p2_4.o error.o 
	${LINK.c} -o $@ $^

include ../makefile
