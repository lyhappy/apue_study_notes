/*
 * 程序清单4-5 Example of futimens function
 *
 */
#include "apue.h"
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>


int main(int argc, char *argv[])
{
	int i, fd;
	struct stat statbuf;
#if !defined(_POSIX_C_SOURCE) || defined(_DARWIN_C_SOURCE)
	struct timeval times[2];
#else 
	struct timespec times[2];
#endif

	for (i = 1; i < argc; i++) {
		if (stat(argv[i], &statbuf) < 0) {  /* fetch current times */
			err_ret("%s: stat error", argv[i]);
			continue; }
		if ((fd = open(argv[i], O_RDWR | O_TRUNC)) < 0) { /* truncate */
			err_ret("%s: open error", argv[i]);
			continue;
		}
#if !defined(_POSIX_C_SOURCE) || defined(_DARWIN_C_SOURCE)
		times[0].tv_sec = statbuf.st_atimespec.tv_sec;
		times[0].tv_usec = statbuf.st_atimespec.tv_nsec / 1000;
		times[1].tv_sec = statbuf.st_mtimespec.tv_sec;
		times[1].tv_usec = statbuf.st_mtimespec.tv_nsec / 1000;
#else
		times[0] = statbuf.st_atim;
		times[1] = statbuf.st_mtim;
#endif

#if !defined(_POSIX_C_SOURCE) || defined(_DARWIN_C_SOURCE)
		if (futimes(fd, times) < 0)
		  err_ret("%s: futimes error", argv[i]);
#else
		if (futimens(fd, times) < 0)        /* reset times */
		  err_ret("%s: futimens error", argv[i]);
#endif
		close(fd);
	}
	exit(0); 
}
