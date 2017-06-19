/*
 * 程序清单7-4 Stack frames after cmd_add has been called
 * 
 */

#include "apue.h"
#include <setjmp.h>

#define TOK_ADD    5

jmp_buf jmpbuffer;

void    do_line(char *);
void    cmd_add(void);
int     get_token(void);

int main(void) {
	char    line[MAXLINE];
	if (setjmp(jmpbuffer) != 0)
	  printf("error!");
	while (fgets(line, MAXLINE, stdin) != NULL)
	  do_line(line);
	exit(0); 
}

char    *tok_ptr;

void do_line(char *ptr) {
	int cmd;
	/* global pointer for get_token() */
	/* process one line of input */
	tok_ptr = ptr;
	while ((cmd = get_token()) > 0) {
		switch (cmd) {  /* one case for each command */
			case TOK_ADD:
				cmd_add();
				break;
		} 
	}
}

void cmd_add(void) {
	int token;
	token = get_token();
	if (token < 0) /* an error has occurred */
	  longjmp(jmpbuffer, 1);
	/* rest of processing for this command */
}

int get_token(void) {
	/* fetch next token from line pointed to by tok_ptr */

	int ret = atoi(tok_ptr);
	printf(" token: %d, %s", ret, tok_ptr);

	while(*tok_ptr != ' ' && *tok_ptr != '\n')
	  tok_ptr++;


	printf(" rest: %s", tok_ptr);

	if (*tok_ptr == '\n') 
	  printf("line end\n");

	tok_ptr++;
	return ret;
}
