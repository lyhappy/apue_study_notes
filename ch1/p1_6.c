/*
 * 程序清单1-6 示例strerror和perror
 *
 */
#include "apue.h"
#include <errno.h>

int main(int argc, char* argv[]) {
	fprintf(stderr, "EACCES: %s\n", strerror(EACCES));
	errno = ENOENT;
	perror(argv[0]);
	return 0;
}
