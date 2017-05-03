CH4 文件和目录
===

## stat、fstat and lstat

```c 
#include <sys/stat.h>
int stat(const char * restrict pathname, struct stat * restrict buf);
int fstat(int filedes, struct stat * buf);
int lstat(const char * restrict pathname, struct stat * restrict buf);
```
lstat函数类似于stat，当文件是一个符号链接时，lstat返回的是符号链接的信息，而不是链接所指文件的信息。

```c
struct stat {
	mode_t	st_mode;	/* file type & mode (permissions) */
	ino_t 	st_ino;		/* i-node number (serial number) */
	dev_t	st_dev;		/* device number (file system) */
	dev_t	st_rdev;	/* device number for special files */
	nlink_t st_nlink;	/* number of links */
	uid_t	st_uid;		/* user ID of owner */
	gid_t	st_gid;		/* group ID of owner */
	off_t	st_size;	/* size in bytes, for regular files */
	time_t	st_atime;	/* time of last access */
	time_t	st_mtime;	/* time of last modification */
	time_t	st_ctime;	/* time of last file status change */
	blksize_t	st_blksize;	/* best I/O block size */
	blkcnt_tst_blocks;	/* number of disk blocks allocated */
};
```

## 文件类型

使用命令`ls -l`可以查看文件属性，结果的第一列的第一个字符表示文件的属性
* **普通文件**(regular file) 一般用`-`表示，可以是文本文件，也可以是二进制文件
* **目录文件**(directory file) 一般用`d`表示，对于一个目录文件，任何具有权限的进程都可以读，但只有内核可以直接写目录文件。
* **块特殊文件**(block special file) 一般用`b`表示，提供对设备带缓冲的访问，每次访问以固定长度为单位进行。
* **字符特殊文件**(character special file) 一般用`c`表示，提供对设备不带缓冲的访问，每次访问长度可变。
* **FIFO**(named pipe) 一般用`p`表示，用于进程间通信。
* **套接字**(socket) 一般用`s`表示，用于进程间网络通信。
* **符号链接**(symbolic link) 一般用`l`表示，指向另一个文件。

文件类型信息包含在stat结构体的st_mode成员中，

```c
#define S_IFMT  00170000 		// 以下七个标记的掩码
#define S_IFSOCK 0140000 		// 套接字
#define S_IFLNK  0120000		// 符号链接 
#define S_IFREG  0100000		// 普通文件 
#define S_IFBLK  0060000 		// 块特殊文件
#define S_IFDIR  0040000 		// 目录文件
#define S_IFCHR  0020000 		// 字符特殊文件
#define S_IFIFO  0010000 		// 管道或FIFO
#define S_ISUID  0004000 		// 设置用户ID， 下节讲到
#define S_ISGID  0002000 		// 设置组ID, 下节讲到
#define S_ISVTX  0001000
```

针对如上标记，有一组宏用来快速判断。
```c
#define S_ISLNK(m)  (((m) & S_IFMT) == S_IFLNK)
#define S_ISREG(m)  (((m) & S_IFMT) == S_IFREG)
#define S_ISDIR(m)  (((m) & S_IFMT) == S_IFDIR)
#define S_ISCHR(m)  (((m) & S_IFMT) == S_IFCHR)
#define S_ISBLK(m)  (((m) & S_IFMT) == S_IFBLK)
#define S_ISFIFO(m) (((m) & S_IFMT) == S_IFIFO)
#define S_ISSOCK(m) (((m) & S_IFMT) == S_IFSOCK) 
```

## 设置用户ID和设置组ID

与进程相关的用户ID和组ID:
* 实际用户ID和实际组ID标识我们究竟是谁。这两个字段的内容取自口令文件的登录项。
* 有效用户ID和有效组ID以及附加组ID决定了我们的文件访问权限。
* 保存的设置用户ID和保存的设置组ID包含了有效用户ID和有效组ID的副本，参见8.1节 setuid 函数

通常，有效用户ID等于实际用户ID，有效组ID等于实际组ID。

当被执行的文件，表示其文件类型信息的stat->st_mode被设置了 S_ISUID 和 S_ISGID 两个标记时，进程的有效用户ID将被设置为文件所有者的用户ID，有效组ID将被设置为文件的组所有者ID

一个典型的场景就是passwd命令，任一普通用户都有修改其口令的权限，但是口令文件（/etc/passwd 和 /etc/shadow）只有超级管理员权限才可以读写，这就存在矛盾了。passwd命令通过设置文件的SUID属性，使用用户在执行其期间具有了短暂的root权限。


**安全问题**
设置了SUID属性的可执行文件，会使执行文件的用户具有与文件拥有者同等的权限。当一些文件拥有者为root的可执行文件设置了SUID属性时，如果其存在漏洞，则可被用于提升用户权限。

## 文件访问权限

代表文件访问权限的标记位：

```c
/* File mode */
/* Read, write, execute/search by owner */
#define	S_IRWXU		0000700		/* [XSI] RWX mask for owner */
#define	S_IRUSR		0000400		/* [XSI] R for owner */
#define	S_IWUSR		0000200		/* [XSI] W for owner */
#define	S_IXUSR		0000100		/* [XSI] X for owner */
/* Read, write, execute/search by group */
#define	S_IRWXG		0000070		/* [XSI] RWX mask for group */
#define	S_IRGRP		0000040		/* [XSI] R for group */
#define	S_IWGRP		0000020		/* [XSI] W for group */
#define	S_IXGRP		0000010		/* [XSI] X for group */
/* Read, write, execute/search by others */
#define	S_IRWXO		0000007		/* [XSI] RWX mask for other */
#define	S_IROTH		0000004		/* [XSI] R for other */
#define	S_IWOTH		0000002		/* [XSI] W for other */
#define	S_IXOTH		0000001		/* [XSI] X for other */
```

chmod 命令可用于修改文件的访问权限。

当用名字打开一个文件时，对该名字包含的每一级目录，包括可能隐含的当前目录，都应具有可执行权限。对于目录，其执行权限位常被称为搜索位。

* 文件的读权限决定了能否打开该文件进行读操作，这与open函数的O_RDONLY和O_RDWR标志相关.
* 文件的写权限决定了能否打开该文件进行写操作，这与open函数的O_WRONLY和O_RDWR标志相关.
* 在open函数中对一个文件指定O_TRUNC标志时，必须具有对该文件的写权限。
* 在一个目录创建文件时，必须具有对该目录的写权限和执行权限。
* 在删除一个文件时，必须对包含该文件的目录具有写权限和执行权限，对文件本身不要求写权限和读权限。
* 使用exec函数执行一个文件时，文件必须具有可执行权限，同时必须是一个普通文件。 

内核对文件访问权限的测试：


