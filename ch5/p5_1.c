/*
 * 程序清单5-1 Copy standard input to standard output using getc and putc 
 * 
 */
#include "apue.h"

int main(void) {
	int c;

	while ((c = getc(stdin)) != EOF)
	  if (putc(c, stdout) == EOF)
		err_sys("output error");

	if (ferror(stdin))
	  err_sys("input error");
	exit(0);
}

