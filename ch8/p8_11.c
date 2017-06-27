/*
 * 程序清单8-11 The system function, without signal handling
 * 
 */
#include    <sys/wait.h> 
#include    <errno.h>
#include    <unistd.h>

int system(const char *cmdstring) {
	pid_t   pid;
	int     status;

	printf("self implemention system\n");
	/* version without signal handling */
	if (cmdstring == NULL)
	  return(1);      /* always a command processor with UNIX */

	if ((pid = fork()) < 0) {
		status = -1;    /* probably out of processes */
	} else if (pid == 0) {              /* child */
		execl("/bin/sh", "sh", "-c", cmdstring, (char *)0);
		_exit(127);     /* execl error */
	} else {                            /* parent */
		while (waitpid(pid, &status, 0) < 0) {
			if (errno != EINTR) {
				status = -1; /* error other than EINTR from waitpid() */
				break;
			} }
	}
	return(status);
}
