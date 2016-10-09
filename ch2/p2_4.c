/*
 *	程序清单2-4 确定文件描述符数
 *
 */
#include "apue.h"
#include <errno.h>
#include <limits.h>

#ifdef OPEN_MAX
static int openmax = OPEN_MAX;
#else
static int openmax = 0;
#endif

/*
 * If OPEN_MAX is indeterminate, we're not guaranteed that this is adequate.
 *
 */

#define OPEN_MAX_GUESS	256

long open_max(void)
{
	if (openmax == 0) {		/* first time through */
		errno = 0;
		if ((openmax = sysconf(_SC_OPEN_MAX)) < 0) {
			if (errno == 0)
				openmax = OPEN_MAX_GUESS;	/* it's indeterminate */
			else 
				err_sys("sysconf error for _SC_OPEN_MAX");
		}
	}

	return(openmax);
}

int main(void) {
	printf("unit test, return value of open_max() is %ld\n", open_max());
	return 0;
}
