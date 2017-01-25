/*
 * 程序清单3-1 测试能否对标准输入设置偏移 
 * 
 */
 
#include "apue.h"
#define BUFFER_SIZE 2048

int main(void) {
	int n = 0;
	char buf[BUFFER_SIZE] = {0};
	while ((n = read(STDIN_FILENO, buf, BUFFER_SIZE)) > 0) {
		if (write(STDOUT_FILENO, buf, n) != n) {
			err_sys("write failed");
		}
	}
	if (n < 0) {
		err_sys("read failed");
	}

	return 0;
}
