/*
 * 程序清单8-13：Execute the command-line argument using system
 * 
 */ 
#include "apue.h"

int main(int argc, char *argv[]) {
	int status;
	if (argc < 2)
	  err_quit("command-line argument required");
	if ((status = system(argv[1])) < 0)
	  err_sys("system() error");
	pr_exit(status);
	exit(0); 
}

