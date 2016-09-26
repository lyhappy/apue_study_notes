/*
 * 程序清单1-5 从标准输入读命令并执行 
 *
 */
#include "apue.h"
#include <sys/wait.h>

#define MAX_CMD 1024

int main(void) {
	char cmd_buf[MAX_CMD] = {0};
	int pid = 0;
	int child_status = 0;

	printf("%%: ");	/* prompt */
	while(fgets(cmd_buf, MAX_CMD, stdin) != NULL) {
		if(cmd_buf[strlen(cmd_buf) - 1] == '\n') {
			cmd_buf[strlen(cmd_buf) - 1] = '\0';
		}

		if ((pid = fork()) < 0) {
			err_sys("fork error");
		} else if (pid == 0) { /* child */
			printf("child pid: %d\n", getpid());
			execlp(cmd_buf, cmd_buf, (char *) 0);
			err_quit("could not execute: %s\n", cmd_buf);
			return 127;
		}

		if ((pid = waitpid(pid, &child_status, 0)) < 0) 
			err_sys("wait error");
		printf("%%: ");
	}

	return 0;
}
