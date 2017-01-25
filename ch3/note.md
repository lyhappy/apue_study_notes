CH3 文件I/O
===

0. 五个基本的I/O函数
	open, create, read, write, close

1. 文件描述符
	内核使用文件描述符维护进程打开的文件，每当进程调用open或create时，内核会向进程返回一个文件描述符，是一个整型值。
	UNIX系统shell使用描述符0表示标准输入, 1表示标准输出，2表示标准出错。POSIX在<unistd.h>中定义了相应的三个常量: STDIN_FILENO, STDOUT_FILENO, STDERR_FILENO.
	文件描述符的变化是0~OPEN_MAX
	
1.1 系统支持的最大文件描述符。
	使用ulimit系统命令可以查看一些系统参数，其中`ulimit -n`返回进程支持的最大打开文件数。
	* 查看系统支持的最大文件数: `cat /proc/sys/fs/file-max`
		* 临时修改: `echo 1000000 > /proc/sys/fs/file-max`
		* 永久修改: 在/etc/sysctl.conf中增加配置项: `fs.file-max = 1000000`
	* 查看进程支持的最大文件数: ulimit -n 
		* 临时修改: `ulimit -n 1000000`
		* 永久修改: 在/etc/security/limits.conf中增加以下配置项：
			> * hard nofile 1000000
			* soft nofile 1000000
			root hard nofile 1000000
			root soft nofile 1000000

	Linux系统2.4.22对每个进程的文件描述符数的硬限制默认是1048576, 在修改/etc/security/limits.conf中的配置项时，不应超过该值。如要超过，应该先修改`echo 2000000 > /proc/sys/fs/nr_open`

2. open

	```c
	#include <fcntl.h>
	int open(const char *filepath, int oflag, ... /* mode_t mode */);
	```
	
	oflag 表示open的多个选项，可取值如下：
	* O_RDONLY 只读打开
	* O_WRONLY 只写打开
	* O_RDWR   读、写打开
	以上三种只能取一种
	* O_APPEND 追加方式打开
	* O_CREAT  若文件不存在，则创建，此时需要指定第三个参数，指定新文件的访问权限。
	* O_EXCL   使用O_CREAT时，如果文件存在，则会出错，使用此选项时，会先检查文件是否存在，若不存在，则会创建，其特点是使检查和创建成为一个院子操作。
	* O_TRUNC  如果文件存在，且以只写或读写的方式打开时，则将其长度截断为0。
	* O_NOCTTY 如果filepath指的是一个终端设备，则不将该设备分配作为此进程的控制终端
	* O_NONBLOCK 如果filepath指定的是一个FIFO, 一个块特殊文件或一个字符特殊文件，则此选项为文件的本次打开操作和后续的I/O操作设置非阻塞模式。

3. creat
	```c
	#include <fcntl.h>
	int creat(const char * filepath, mode_t mode);
	```
	creat函数的功能与如下open调用方式是相同的。
	`open(pathname, O_WRONLY | O_CREAT | O_TRUNC, mode);`
	
4. close
	```c
	#include <unistd.h>
	int close(int filedes);
	```
	
5. lseek
	```c
	#include <unistd.h>
	off_t lseek(int filedes, off_t offset, int where);
	```
	where 可以取值：
	SEEK_SET, 表示从文件开始处计算偏移
	SEEK_CUR, 表示从文件当前位置开始计算偏移
	SEEK_END, 表示从文件末尾开始计算偏移

6. read
	```c
	#include <unistd.h>
	ssize_t read(int filedes, void *buf, size_t nbytes);
	```
	从打开的文件读取数据，如果成功，返回读取到的字节数，如果已经到文件结尾，返回0
	
7. write
	```c
	#include <unistd.h>
	ssize_t write(int filedes, const void *buf, size_t nbytes);
	```
	若成功，返回实际写入的字节数，失败返回-1，通常失败的原因是磁盘已写满或超过给定进程的最大文件限制。

	

