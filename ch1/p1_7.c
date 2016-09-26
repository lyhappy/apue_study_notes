/*
 *	程序清单1-7 打印用户id和组id
 *
 */
#include "apue.h"

int main(void) {
	printf("uid = %d, gid = %d\n", getuid(), getgid());
	return 0;
}
