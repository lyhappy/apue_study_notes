#ifndef APUE_H_H_
#define APUE_H_H_

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define MAXLINE 4096

void err_quit(const char* fmt, ...);
void err_sys(const char* fmt, ...);
void err_ret(const char* fmt, ...);
void err_dump(const char* fmt, ...);

char * path_alloc(int *size);
#define	FILE_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
					/* default file access permissions for new files */

#endif /* APUE_H_H_ */
