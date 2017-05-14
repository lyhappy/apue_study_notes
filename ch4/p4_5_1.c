/*
 * 程序清单4-5 Open a file and then unlink it
 * 
 */

#include "apue.h"
#include <fcntl.h>
#include <stdio.h>

#define	MAX_SIZE 512


int main(void) {
	int fd = 0;
	char buf[MAX_SIZE] = {0};
	int retn = 0;

	if ((fd = open("tempfile", O_RDWR)) < 0)
	  err_sys("open error");

	retn = read(fd, buf, MAX_SIZE);
	if (retn > 0) {
		printf("%s\n", buf);
	} else {
		printf("return of read: %d\n", retn);
	}

	if (unlink("tempfile") < 0)
	  err_sys("unlink error");
	printf("file unlinked\n");
	printf("done\n");

	lseek(fd, 0, SEEK_SET);
	retn = write(fd, "Hello World!\n", sizeof("Hello World!")+1);
	if (retn > 0) { 
		printf("write success!\n");
	}

	lseek(fd, 0, SEEK_SET);
	retn = read(fd, buf, MAX_SIZE);
	if (retn > 0) {
		printf("%s\n", buf);
	} else {
		printf("return of read: %d\n", retn);
	}

	close(fd);

	exit(0);
}
