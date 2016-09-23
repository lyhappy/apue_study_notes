#ifndef APUE_H_H_
#define APUE_H_H_

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINE 4096

void err_quit(const char* fmt, ...);
void err_sys(const char* fmt, ...);


#endif /* APUE_H_H_ */
