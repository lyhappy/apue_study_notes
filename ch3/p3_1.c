/*
 * 程序清单3-1 测试能否对标准输入设置偏移 
 * 
 */
 
#include <stdio.h>
#include <unistd.h>

int main(void) {
	if (lseek(STDIN_FILENO, -2, SEEK_SET) == -1)
	  printf("can not seek!\n");
	else
	  printf("seek ok!\n");
	return 0;
}
