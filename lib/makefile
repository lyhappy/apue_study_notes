OBJS = ../obj/error.o
INCLUDE = -I ../include
OBJ_DIR = ../obj/

all: ${OBJS}

../obj/error.o: error.c 
	${COMPILE.c} -o $@ ${INCLUDE} $<

include ../makefile
