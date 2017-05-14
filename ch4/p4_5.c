/*
 * 程序清单4-5 Open a file and then unlink it
 * 
 */

#include "apue.h"
#include <fcntl.h>


int main(void) {
	if (open("tempfile", O_RDWR) < 0)
	  err_sys("open error");
	if (unlink("tempfile") < 0)
	  err_sys("unlink error");
	printf("file unlinked\n");
	sleep(15);
	printf("done\n");
	exit(0);
}
