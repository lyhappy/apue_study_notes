/*
 * 程序清单3-4 对于指定的文件描述符打印文件标志
 * 
 */

#include "apue.h"
#include <fcntl.h>

int main(int argc, char *argv[]) {
	int val = 0;

	if (argc != 2)
		err_quit("usage: p3_4 <descriptor#>");

	if ((val = fcntl(atoi(argv[1]), F_GETFL, 0)) < 0)
		err_sys("fcntl failed! fd: %d", atoi(argv[1]));

	switch (val & O_ACCMODE) {
		case O_RDONLY:
			printf("read only\n");
			break;
		case O_WRONLY:
			printf("write only\n");
			break;
		case O_RDWR:
			printf("read write\n");
			break;

		default:
			err_quit("unknown access mode");
	}

	if (val & O_APPEND) 
		printf(", append");
	if (val & O_NONBLOCK)
		printf(", nonblocking");
#if defined(O_SYNC)
	if (val & O_SYNC)
		printf(", synchronous writes");
#endif
#if !defined(_POSIX_C_SOURCE) && defined(O_FSYNC)
	if (val & O_FSYNC)
		printf(", synchronous wirtes");
#endif
	putchar('\n');
	return 0;
}
