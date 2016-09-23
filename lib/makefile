include ../makefile

OBJS = error.o
INCLUDE = -I ../include
OBJ_DIR = ../obj/

all: ${OBJS}

error.o: error.c 
	${COMPILE.c} -o ${OBJ_DIR}$@ ${INCLUDE} error.c
