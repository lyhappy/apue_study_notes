/*
 * 程序清单8-13: Print real and effective user IDs
 * 
 */ 
#include "apue.h"

int main(void) {
	printf("real uid = %d, effective uid = %d\n", getuid(), geteuid());
	exit(0); 
}

