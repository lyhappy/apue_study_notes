/*
 * 程序清单 8-9： A program that execs an interpreter file
 * 
 */ 
#include "apue.h"
#include <sys/wait.h>

char    *env_init[] = { "USER=unknown", "PATH=/tmp", NULL };

int main(void) {
	pid_t pid;

	if ((pid = fork()) < 0) {
		err_sys("fork error");
	} else if (pid == 0) {
		if (execle("./testinterp",
						"testinterp", "myarg1", "MY ARG2", (char *)0, env_init) < 0)
		  err_sys("execl error");
	}

	if (waitpid(pid, NULL, 0) < 0)  /* parent */
	  err_sys("waitpid error");

	exit(0);
}
