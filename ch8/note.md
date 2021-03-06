进程控制
===

进程标识符
---

进程ID是一个进程的唯一标识符，进程结束后，ID会重复利用。

进程ID为0的进程通常为调度进程(swapper)，该进程属于内核的一部分。<br>
进程ID为1的进程为init进程，在自举过程结束后，由内核调用。init进程运行在用户空间，具有超级用户权限。

进程具有的其他ID

```c
#include <unistd.h>
pid_t getpid(void);
		//	Returns: process ID of calling process
pid_t getppid(void);
		//	Returns: parent process ID of calling process
uid_t getuid(void);
		//	Returns: real user ID of calling process
uid_t geteuid(void);
		//	Returns: effective user ID of calling process
gid_t getgid(void);
		//	Returns: real group ID of calling process
gid_t getegid(void);
		//	Returns: effective group ID of calling process
```

fork 函数
---

通过调用fork函数，可以创建一个新进程
```c
#include <unistd.h>
pid_t fork(void);
		//	Returns: 0 in child, process ID of child in parent, −1 on error
```

这个函数调用一次会返回两次，返回值为0是，表示位于子进程中，返回值为其他值时，表示位于父进程中。

fork表示子进程是从父进程复制出来的，子进程将获取到父进程的数据空间，堆和栈的副本。实际系统实现中，并不是直接复制上述所有数据区，而是采用了所谓的“写时复制（copy on write）”。这些数据区由父子进程共享，且内核将它们的访问权限设置为只读，当父子进程中的任何一个试图修改这些区域，则内核只为修改区域的那块内存制作一个副本，通常是虚拟内存器系统中的“一页”。

[p8_1.c](p8_1.c)

> $ ./a.out
> a write to stdout
> before fork
> pid = 430, glob = 7, var = 89				*child’s variables were changed* 
> pid = 429, glob = 6, var = 88				*parent’s copy was not changed*
> $ ./a.out > temp.out
> $ cat temp.out
> a write to stdout
> before fork
> pid = 432, glob = 7, var = 89 
> before fork
> pid = 431, glob = 6, var = 88

write是不带缓冲的；输出到标准输出设备时，printf为行缓冲；输出到文件时，printf为全缓冲。

##### 文件共享

在重定向父进程的标准输出时，子进程的标准输出也被重定向。实际上，fork的特性是，所有父进程打开的文件描述符，都被复制到了子进程中。
如果父子进程操作同一个输出文件描述符，但又没有任何同步处理，它们的输出很可能发生混乱。

一般fork之后，对文件描述的处理有以下两种情况：
* 父进程等待子进程结束，这种情况下，父进程不需要对文件描述符做任何特殊处理。
* 父子进程执行不同的程序段，这种情况下，父子进程关闭各自不需要的文件描述符。这种方法是网络服务进程中经常使用的。

fork执行失败的两个可能原因：
* 系统中有太多的进程
* 该实际用户ID的进城总数超过了系统限制。

fork的两种用法：
* 当一个进程想复制自己，使父子进程执行不同的代码段。这在网络服务中很常见，通常是父进程监听网络套接字，得到客户端的链接请求后，fork出子进程来完成具体的服务。父进程则继续监听后续的请求。
* 一个进程要执行一个不同的程序。常见于shell的实现，通常在fork之后，立即调用exec

vfork
---

vfork 用于创建一个新进程，新进程的目的是用于exec一个新程序。与fork一样都是创建一个新进程，但vfork不复制父进程的地址空间，因为子进程会立即调用exec或exit
；另一个区别是，vfork保证子进程先执行，在子进程调用exec或exit后才会唤醒父进程。

[p8_2.c](p8_2.c)

exit
---

如上一章所述，进程有8种终止方式。

父进程如果在子进程结束之前终止，子进程的父进程会变成init进程，称为init领养。领养过程大致如下：<br>
系统在结束一个进程时，会先扫描所有活动的进程，如果发现有属于被结束进程的子进程，会将其父进程ID修改为1，即init进程的ID。

如果子进程在父进程结束之前终止，内核会为每个要终止的子进程维护一些信息，当父进程调用wait或waitpid时，会获取到这信息，包括进程ID，进程的终止状态，以及该进程使用的CPU时间总量。

在UNIX术语中，一个已经终止，但父进程未进行善后处理（调用wait函数）的进程将处于僵死状态（Zombie），ps(1)命令将僵死进程的状态打印为Z。如果一个程序长期运行产生很多子进程，又未对子进程做善后处理，将产生很多僵死进程，太多的僵死进程会耗尽系统资源导致异常。

被init进程领养的进程，会由init进程对其进行善后处理，init进程始终会对其子进程调用wait函数。这样就保证了系统不会有太多的僵死进程。

wait和waitpid
---

当一个进程正常终止或发生异常时，内核会向其父进程发送SIGCHLD信号。

父进程调用wait或waitpid时，可能会发生的情况：
* 如果所有的子进程都还在运行，则阻塞父进程;
* 如果一个子进程已终止，则获取其终止状态，并立即返回；
* 如果没有任何子进程，则立即出错返回。

```c
#include <sys/wait.h>
pid_t wait(int *statloc);
pid_t waitpid(pid_t pid, int *statloc, int options);
		//	Both return: process ID if OK, 0 (see later), or −1 on error
```

这两个函数的区别是：
* 在一个进程终止前，wait使其调用者阻塞，而waitpid的参数options可使调用者不阻塞。
* waitpid可以指定要等待的进程id，wait始终等待的是第一个结束的进程，不论是哪个。

参数statloc用于返回子进程的终止状态。

POSIX.1规定终止状态用定义在`<sys/wait.h>`中的宏来查看.

Macro	| Description
--- | ---
WIFEXITED(status) | True if status was returned for a child that terminated normally. In this case, we can execute<br> WEXITSTATUS(status)<br> to fetch the low-order 8 bits of the argument that the child passed to exit, `_exit,` or `_Exit`.
WIFSIGNALED(status) | True if status was returned for a child that terminated abnormally, by receipt of a signal that it didn’t catch. In this case, we can execute<br> WTERMSIG(status)<br> to fetch the signal number that caused the termination.<br> Additionally, some implementations (but not the Single UNIX Specification) define the macro
WCOREDUMP(status) | that returns true if a core file of the terminated process was generated.<br> WIFSTOPPED(status)<br> True if status was returned for a child that is currently stopped. In this case, we can execute
WSTOPSIG(status) | to fetch the signal number that caused the child to stop.<br> WIFCONTINUED(status)<br> True if status was returned for a child that has been continued after a job control stop (XSI option; waitpid only).

[p8_3.c](p8_3.c)
[p8_4.c](p8_4.c)


waitpid函数中pid参数的意义：
* `pid == -1`		等待任一子进程，就这一方面而言，waitpid 和 wait是等效的
* `pid > 0`			等待进程ID等于pid的子进程
* `pid == 0`		等待进程组ID等于调用者进程的组ID。
* `pid < 0`			等待进程组ID等于pid绝对值的任意子进程。

options参数的取值可以是0 或 以下值的“或”运算组合

Constant | Description
--- | ---
WCONTINUED | If the implementation supports job control, the status of any child specified by pid that has been continued after being stopped, but whose status has not yet been reported, is returned (XSI option).
WNOHANG | The waitpid function will not block if a child specified by pid is not immediately available. In this case, the return value is 0.
WUNTRACED | If the implementation supports job control, the status of any child specified by pid that has stopped, and whose status has not been reported since it has stopped, is returned. The WIFSTOPPED macro determines whether the return value corresponds to a stopped child process.

waitid
---

```c
#include <sys/wait.h>
int waitid(idtype_t idtype, id_t id, siginfo_t *infop, int options);
		//	Returns: 0 if OK, −1 on error
```

这是XSI扩展中定义的函数。功能与waitpid类似，但更灵活。

idtype的类型：

Constant | Description
--- | ---
P_PID | Wait for a particular process: id contains the process ID of the child to wait for.
P_PGID | Wait for any child process in a particular process group: id contains the process group ID of the children to wait for.
P_ALL | Wait for any child process: id is ignored.


optioins的取值可以是以下宏的按位或运算结果。

Constant | Description
--- | ---
WCONTINUED | Wait for a process that has previously stopped and has been continued, and whose status has not yet been reported.
WEXITED | Wait for processes that have exited.
WNOHANG | Return immediately instead of blocking if there is no child exit status available.
WNOWAIT | Don’t destroy the child exit status. The child’s exit status can be retrieved by a subsequent call to wait, waitid, or waitpid.
WSTOPPED | Wait for a process that has stopped and whose status has not yet been reported.

wait3 wait4
---

```c
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>

pid_t wait3(int *statloc, int options, struct rusage *rusage);
pid_t wait4(pid_t pid, int *statloc, int options, struct rusage *rusage); 
		//	Both return: process ID if OK, 0, or −1 on error
```

参数rusage用于返回终止进程和其所有子进程的资源使用情况。资源统计信息包括，用户CPU时间总量，系统CPU时间总量，页面错误次数，接受到的信号次数。

竞争条件
---

当多个进程企图对共享数据进行操作时，而最后的结果又取决于进程的执行顺序，则认为发生了**竞争条件**(race condition)。

面对竞争条件，需要一定的同步机制。

[p_6.c](p_6.c)

**exec函数**
---

当进程调用exec函数时，该进程执行的程序将完全替换成新程序，而新程序则从main函数开始执行。因为exec并不创建新进程，所以进程ID不会发生变化。
exec函数只是用一个新程序替换了当前进程的正文，数据，堆和栈。

```c
#include <unistd.h>
int execl(const char *pathname, const char *arg0, ... /* (char *)0 */ );
int execv(const char *pathname, char *const argv[]);
int execle(const char *pathname, const char *arg0, ... /* (char *)0, char *const envp[] */ );
int execve(const char *pathname, char *const argv[], char *const envp[]); 
int execlp(const char *filename, const char *arg0, ... /* (char *)0 */ ); 
int execvp(const char *filename, char *const argv[]);
int fexecve(int fd, char *const argv[], char *const envp[]);
		//	All seven return: −1 on error, no return on success
```

以上函数的*第一个*不同点是，前四个函数是以pathname做参数，接下来两个函数是以filename做参数，最后一个函数是以文件描述符做参数。

对于filename参数：
* 如果其中包含`/`，则将其视为pathname，与前两个函数一致。
* 否则，将从环境变量PATH中搜索filename表示的可执行文件。

环境变量PATH的示例：
`PATH=/bin:/usr/bin:/usr/local/bin/:.`

*注意*：将当前目录`.`放入环境变量PATH中，尤其是考前的位置，是有安全风险的。利用这一点，攻击者可以在某个目录里防止命名与常用命令相同（如`ls`）的恶意脚本，导致普通用户再执行常用命令时，错误的执行了恶意脚本。

调用`execlp`和`execvp`时，如果找到的文件不是由链接器生成的可执行二进制文件，则会假设找到的是shell脚本，并调用`/bin/sh`去解析执行找到的文件。

使用`fexecve`函数，可以精确的判断要执行的文件，因为必须要先调用open函数获取所指定文件的描述符，这个过程中同时也可以执行对文件权限的检查，同时也可以避免利用`IFS`攻击(修改shell默认的命令行分隔符为`/`)执行与路径名相同的恶意脚本。

以上函数的*第二个*不同点是参数的传递方式:

* 函数命名中包含字母l的，是以参数列表的形式将参数传递给新程序。其特点是函数参数个数不定。
* 函数命名中包含字母v的，是以数组的形式将参数传递给新程序。

*最后一个*不同点是向新程序传递环境表，
* 以e结尾的三个函数，使用数组向新程序传递环境变量。
* 其他四个函数，使用全局变量environ向新程序传递环境变量。

每个系统对参数表和环境表的长度都有限制，这种限制由ARG_MAX给出，POSIX.1系统要求至少4096个字节。

执行exec后，**进程ID**不会发生变化，同样不变的还有，父进程ID，实际用户ID，实际组ID，附加组ID，进程组ID，会话ID，控制终端，闹钟尚余留的时间，当前工作目录，根目录，文件模式创建屏蔽字，文件锁，进程信号屏蔽，未处理信号，资源限制，tms_utime, tms_stime, tms_cutime, and tms_cstime

exec执行后，有效用户ID是否改变，取决于程序文件的 set-user-ID 和 set-group-ID

这七个函数的关系如下图：
![relationship_of_the_seven_exec.png](relationship_of_the_seven_exec.png)

[p8_7.c](p8_7.c)

更改用户ID和组ID
---

UNIX系统使用最小特权模型来保障系统安全，通常应用程序应当只具有完成给定任务所需的最小权限。当应用需要提升特权或者访问本无权限的资源时，可以通过修改修改用户ID和组ID完成提权。

```c
#include <unistd.h> 
int setuid(uid_t uid); 
int setgid(gid_t gid);
		//	Both return: 0 if OK, −1 on error
```

可以通过以上函数来修改用户ID和组ID，当然，提权是有严格的条件限制的(以下针对UID)：

* 如果应用程序具有超级用户权限，setuid会修改real user ID(ruid), effective user ID(euid), and saved set-user-ID(suid) 为uid；
* 如果应用程序没有超级用户权限，但是uid等于ruid 或 suid, setuid 会修改 euid 为 uid, 其他两个uid不变；
* 如果以上两个条件都不满足，errno 将被设置为EPERM，并返回-1

以上第二条的实际意义是允许进程的euid在 ruid 和 suid 之间切换，ruid 是启动进程的用户ID, suid 是启动进程时 euid 的副本，euid 是应用程序文件的所有者ID，当所有者ID与启动进程的用户ID不一致时，需要文件设置set-user-ID位

关于内核所维护的三个uid，还需知道：

* 只有超级用户才可以更改real user ID。通常，ruid是在用户登录时，由login程序设置，而且永远不会改变。login是一个超级用户进程，其调用setid，会同时设置三个uid
* 仅当程序文件设置了 set-user-ID 位，exec函数才会设置进程的euid，如果没有设置set-user-ID 位，则exec不会修改euid，而是维持其原值。任何时候可以调用setuid来将euid设置为ruid或suid，但不能设置为其他任意值。
* suid是由exec复制euid得来，如果程序文件设置了set-user-ID位，则exec设置进程euid为文件的所有者ID，同时，复制euid的副本到suid。

suid仅仅是启动时edui的一个副本，因为后续程序可能调用setuid去修改euid的值为ruid，或者恢复euid原来的值，所以suid仅是一个备份启动时的euid的功能，也所以系统没有提供函数去获取suid的当前值。

#### setreuid and setregid Functions

```c
#include <unistd.h>
int setreuid(uid_t ruid, uid_t euid); 
int setregid(gid_t rgid, gid_t egid);
		//	Both return: 0 if OK, −1 on error
```

历史上，BSD提供以上函数来交换ruid和euid的值，如果不需要改变那个uid，可以将其对应的参数设置为-1

> The rule is simple: an unprivileged user can always swap between the real user ID and the effective user ID. 
> This allows a set-user-ID program to swap to the user’s normal permissions and swap back again later for set-user-ID operations. 

#### seteuid and setegid Functions

```c
#include <unistd.h> 
int seteuid(uid_t uid); 
int setegid(gid_t gid);
		//	Both return: 0 if OK, −1 on error
```

这对函数和setuid, setgid类似，但是只修改euid和egid，

> An unprivileged user can set its effective user ID to either its real user ID or its saved set-user-ID. 
> For a privileged user, only the effective user ID is set to uid.

![img](images/various_user_ids.png)

所有对group ID的函数与对user ID的函数的功能类似。

解释器文件
---

interpreter files, These files are text files that begin with a line of the form
`#! pathname [ optional-argument ]`

pathname通常是绝对路径，不对其使用PATH进行路径搜索。

对于解释器文件的识别，是由内核执行系统调用exec完成的。真正被内核执行的文件不是该解释器文件，而是由pathname指定的可执行文件(interpreter)。

一般系统会限制解释器文件的第一行的长度。

[p8_10](p8_10.wak)

system函数
---

```c
#include <stdlib.h>
int system(const char *cmdstring);
		//	Returns: (see below)
```

如果cmdstring为NULL，则仅当命令处理程序可用时，system返回非0值，这一特征可用于检测系统是否支持system函数。

system函数会调用fork, exec, waitpid三个函数，返回值有三种类型：
* If either the `fork` fails or `waitpid` returns an error other than EINTR, system returns −1 with `errno` set to indicate the error.
* If the `exec` fails, implying that the shell can’t be executed, the return value is as if the shell had executed `exit`(127).
* Otherwise, all three functions—`fork`, `exec`, and `waitpid`—succeed, and the return value from system is the termination `status` of the shell, in the format specified for `waitpid`.

make在编译结束后，会自动删除自认为的中间文件（intermediate files）[关于makefile](https://peter.bourgon.org/blog/2009/10/10/gnu-make-and-deleting-intermediate-files.html)

#### Set-User-ID

在以前的系统中，system调用会将父进程的特权传递给子进程，这会造成安全问题，新版系统基本都已拒绝这种做法。
> The system function should never be used from a set-user-ID or a set-group-ID program.

[p8_13.c](p8_13.c)<br>
[p8_14.c](p8_14.c)

进程会计
---

每当进程结束时，内核就写一个会计记录，典型的会计记录包含总量较小的二进制数据。由于没有统一的标准，各系统对该功能的实现有区别。

`acct`函数用于启动和禁止进程会计。系统命令accton是基于该函数实现的。

会计记录的数据结构定义在<sys/acct.h>中

```c
typedef  u_short comp_t;  /* 3-bit base 8 exponent; 13-bit fraction */
struct acct {
	char   ac_flag; /* flag (see Figure 8.26) */
	char   ac_stat; /* termination status (signal & core flag only) */
	/* (Solaris only) */
	uid_t  ac_uid; /* real user ID */
	gid_t  ac_gid; /* real group ID */
	dev_t  ac_tty; /* controlling terminal */
	time_t ac_btime; /* starting calendar time */
	comp_t ac_utime; /* user CPU time */
	comp_t ac_stime; /* system CPU time */
	comp_t ac_etime; /* elapsed time */
	comp_t ac_mem; /* average memory usage */
	comp_t ac_io; /* bytes transferred (by read and write) */
	/* "blocks" on BSD systems */
	comp_t ac_rw; /* blocks read or written */
	/* (not present on BSD systems) */
	char   ac_comm[8]; /* command name: [8] for Solaris, */
					/* [10] for Mac OS X, [16] for FreeBSD, and */
					/* [17] for Linux */
};
```

内核会在进程表中保存进程的数据，在进程终止时，将会计记录写入会计文件(该文件通常是/var/account/acct)，所以会计文件中的记录顺序对应的是进程终止的顺序，而不是启动顺序。

[p8_15.c](p8_15.c)<br>
[p8_16.c](p8_16.c)

用户标识
---

```c
#include <unistd.h>
char *getlogin(void);
		//	Returns: pointer to string giving login name if OK, NULL on error
```

用于获取用户登录时的用户名。

进程调度
---

A process could choose to run with lower priority by adjusting its nice value (thus a process could be ‘‘nice’’ and reduce its share of the CPU by adjusting its nice value). Only a privileged process was allowed to increase its scheduling priority.

In the Single UNIX Specification, nice values range from 0 to (2*NZERO)−1, Lower nice values have higher scheduling priority.

```c
#include <unistd.h> 
int nice(int incr);
		//	Returns: new nice value − NZERO if OK, −1 on error
```

The incr argument is added to the nice value of the calling process. If incr is too large, the system silently reduces it to the maximum legal value. Similarly, if incr is too small, the system silently increases it to the minimum legal value.

Because −1 is a legal successful return value, we need to clear errno before calling nice and check its value if nice returns −1.

```c
#include <sys/resource.h>
int getpriority(int which, id_t who);
		//	Returns: nice value between −NZERO and NZERO−1 if OK, −1 on error
```

getpriority 可以用于获取一个或者一组相关进程的nice值

which 可取三个值：
* `PRIO_PROCESS` to indicate a process, 
* `PRIO_PGRP` to indicate a process group, 
* `PRIO_USER` to indicate a user ID.

who 取值为0时，如果which为PRIO_USER，则取调用进程的实际用户ID的nice值（？）否则表示取当前进程，进程组的nice值，(依据which的取值）。
当which的值指向进程组时，返回的是优先级最高的进程的nice值。

```c
#include <sys/resource.h>
int setpriority(int which, id_t who, int value);
		//	Returns: 0 if OK, −1 on error
```

which 和 who 的取值如上。
The value is added to NZERO and this becomes the new nice value.

[p8_17.c](p8_17.c)

进程时间
---

```c
#include <sys/times.h>

struct tms {
	clock_t  tms_utime;  /* user CPU time */
	clock_t  tms_stime;  /* system CPU time */
	clock_t  tms_cutime; /* user CPU time, terminated children */
	clock_t  tms_cstime; /* system CPU time, terminated children */
};

clock_t times(struct tms *buf);
		//	Returns: elapsed wall clock time in clock ticks if OK, −1 on error
```

[p8_18.c](p8_18.c)


		
