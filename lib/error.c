#include "apue.h"
#include <errno.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
	SYS_CALL,
	NONE_SYS_CALL
} CALL_TYPE;

static void do_error(CALL_TYPE type, const char* fmt, va_list args);

void err_quit(const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);
	do_error(NONE_SYS_CALL, fmt, args);
	va_end(args);

	exit(1);
}

void err_ret(const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);
	do_error(NONE_SYS_CALL, fmt, args);
	va_end(args);

}

void err_sys(const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);
	do_error(SYS_CALL, fmt, args);
	va_end(args);

	exit(1);
}

void do_error(CALL_TYPE type, const char* fmt, va_list args) {
	int errno_save = errno;
	char buf[MAXLINE] = {0};

	sprintf(buf, fmt, args);
	if (type == SYS_CALL) {
		sprintf(buf + strlen(buf), ": %s", strerror(errno_save));
	}
	strcat(buf, "\n");
	fflush(stdout);
	fputs(buf, stderr);
	fflush(NULL);
	return;
}

/* Fatal error related to a system call.
 * Print a message, dump core, and terminate.
 */

void err_dump(const char *fmt, ...)
{
	va_list		ap;

	va_start(ap, fmt);
	do_error(1, fmt, ap);
	va_end(ap);
	abort();		/* dump core and terminate */
	exit(1);		/* shouldn't get here */
}

