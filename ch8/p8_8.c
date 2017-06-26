/*
 * 程序清单8-8： Echo all command-line arguments and all environment strings
 * 
 */ 
#include "apue.h"

int main(int argc, char *argv[]) {
	int         i;
	char        **ptr;
	extern char **environ;

	for (i = 0; i < argc; i++)
	  printf("argv[%d]: %s\n", i, argv[i]);

	for (ptr = environ; *ptr != 0; ptr++)   /* and all env strings */
	  printf("%s\n", *ptr);

	exit(0); 
}
