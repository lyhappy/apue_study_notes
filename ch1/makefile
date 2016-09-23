include ../makefile
PROGS = p1_1 p1_2 p1_3 p1_4 p1_5
OBJS =  p1_1.o p1_2.o p1_3.o p1_4.o
OBJS_DIR = ../obj/
BIN_DIR = ../bin/
INCLUDE = -I ../include/


all: ${PROGS}

error.o: ../lib/error.c
	make ../lib/makefile

p1_1: p1_1.o ${OBJS_DIR}error.o 
	${LINK.c} -o ${BIN_DIR}$@ ${OBJS_DIR}/p1_1.o ${OBJS_DIR}/error.o

p1_1.o: p1_1.c 
	${COMPILE.c} -o ${OBJS_DIR}$@ p1_1.c ${INCLUDE}

p1_2: p1_2.o ${OBJS_DIR}error.o
	${LINK.c} -o ${BIN_DIR}$@ ${OBJS_DIR}/p1_2.o ${OBJS_DIR}/error.o

p1_2.o: p1_2.c
	${COMPILE.c} -o ${OBJS_DIR}$@ p1_2.c ${INCLUDE}

#${OBJS}: ch1/*.c include/apue.h
p1_3.o: p1_3.c
	${COMPILE.c} -o ${OBJS_DIR}$@ p1_3.c ${INCLUDE}
	
p1_3: p1_3.o ${OBJS_DIR}error.o
	${LINK.c} -o ${BIN_DIR}$@ ${OBJS_DIR}/p1_3.o ${OBJS_DIR}/error.o

p1_4.o: p1_4.c
	${COMPILE.c} -o ${OBJS_DIR}$@ p1_4.c ${INCLUDE}

p1_4: p1_4.o ${OBJS_DIR}error.o
	${LINK.c} -o ${BIN_DIR}$@ ${OBJS_DIR}/p1_4.o ${OBJS_DIR}/error.o
	
p1_5.o: p1_5.c
	${COMPILE.c} -o ${OBJS_DIR}$@ p1_5.c ${INCLUDE}

p1_5: p1_5.o ${OBJS_DIR}error.o
	${LINK.c} -o ${BIN_DIR}$@ ${OBJS_DIR}/p1_5.o ${OBJS_DIR}/error.o
