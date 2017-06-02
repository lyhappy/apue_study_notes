#include "apue.h"

extern char** environ;
int main(int argc, char *argv[]) {
	int i;
	for (i = 0; i < argc; i++)			/* echo all command-line args */
	  printf("argv[%d]: %s\n", i, argv[i]);
	for (i = 0; environ[i] != NULL; i++)
	  printf("env[%d]: %s\n", i, environ[i]);
	exit(0);
}
