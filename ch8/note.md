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
WIFSIGNALED(status) | True if status was returned for a child that terminated abnormally, by receipt of a signal that it didn’t catch. In this case, we can execute<br>
WTERMSIG(status)<br>
to fetch the signal number that caused the termination.<br>
Additionally, some implementations (but not the Single UNIX Specification) define the macro
WCOREDUMP(status) | that returns true if a core file of the terminated process was generated.<br>
WIFSTOPPED(status)<br>
True if status was returned for a child that is currently stopped. In this case, we can execute
WSTOPSIG(status) | to fetch the signal number that caused the child to stop.<br>
WIFCONTINUED(status)<br>
True if status was returned for a child that has been continued after a job control stop (XSI option; waitpid only).



