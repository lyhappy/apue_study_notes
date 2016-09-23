/*
 * 程序清单1-2 将标准输入复制到标准输出
 *
 */
#include "apue.h"

#define BUFFSIZE 4096

int main(void) {
	int n;
	char buf[BUFFSIZE] = {0};

	while ((n = read(STDIN_FILENO, buf, BUFFSIZE)) > 0)
		if (write(STDOUT_FILENO, buf, n) != n)
			err_sys("write error");

	if (n < 0)
		err_sys("read error");

	/*** for test ***/
	printf("/*** for test ***/\n");
	printf("stdin: %d\n", STDIN_FILENO);
	printf("stdout: %d\n", STDOUT_FILENO);
	printf("stderr: %d\n", STDERR_FILENO);
	return 0;
}
