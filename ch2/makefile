include ../makefile
PROGS = p2_2
OBJS = p2_2.o
DIR_OBJS = ../obj/
DIR_BIN = ../bin/
INCLUDE = -I ../include/

all: ${PROGS}

error.o: ../lib/error.c
	make ../lib/makefile

p2_2: p2_2.o error.o
	${LINK.c} -o ${DIR_BIN}$@ ${DIR_OBJS}$< ${DIR_OBJS}/error.o

p2_2.o: p2_2.c
	${COMPILE.c} -o ${DIR_OBJS}$@ $< ${INCLUDE}
	
p2_2.c: p2_1.awk
	awk -f p2_1.awk > $@

