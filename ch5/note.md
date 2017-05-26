标准I/O库
===

标准I/O库是ISO C标准的一部分

流和文件对象
---

第三章中的所有I/O函数都是针对文件描述符的，标准I/O库是围绕文件流进行的。

对于ASCII字符，一个字符编码为一个字节；对于国际字符，一个字符通常需要多个字节来编码表示。<br>
标准I/O库对于这两种情况都可以处理。一个流的定向（orientation）决定了其读写行为是单字节还是多字节。<br>
一个流被创建后，第一个作用于其的函数若是单字节函数，流就定向为**字节定向**(byte oriented)。若是多字节函数(函数声明在 <wchar.h> )，流就定向为**宽定向*(wide oriented)。

只有两个函数可以修改流的定向，`freopen`函数会清空流的定向；`fwide`函数可以设置流的定向（但不能修改已确定定向的流）。

```c
#include <stdio.h>
#include <wchar.h>
int fwide(FILE *fp, int mode);
		/*	Returns: positive if stream is wide oriented, 
		 *	negative if stream is byte oriented,
		 *	or 0 if stream has no orientation
		 */
```

参数 mode:<br>
	* 如果值为负数，fp 将被设置为 byte oriented
	* 如果值为整数，fp 将被设置为 wide oriented
	* 如果值为0，fwide不会修改fp的定向，但会返回当前的定向。

fwide 不会修改已定向的流，也不会直接返回错误码。要检查fwide函数执行是否异常，需要在函数调用前清空，errno的值，在函数调用后检查errno的值。

进程预定义的三个文件流对象: stdin, stdout, stderr.

缓冲
----

标准I/O库提供的缓冲功能，减少了对write和read等系统调用的调用次数。

标准I/O库提供三种缓冲类型：<br>
	* **全缓冲** 缓冲区填满后才执行真正的I/O操作。一个流上执行第一次I/O操作时，相应的标准I/O库会调用malloc函数分配所需的缓冲区。
	* **行缓冲** 在输入和输出中遇到换行符时，标准I/O库的函数才执行真正的I/O操作。行缓冲的两个特殊情况：
		1. 标准I/O库用于行缓冲的空间大小是固定的，所以当缓冲区满了，即是没有遇到换行符，也会发生实际的I/O操作。
		2. 任何时候，从不带缓冲的流或行缓冲的流得到输入数据，所有的行缓冲数据会先被冲洗(flush)。
	* **不缓冲** 即不对字符串进行缓冲存储。标准出错流通常是不带缓冲的。

flush: 通过调用`fflush`函数，强制执行I/O操作。

标准输入和标准输出如果不涉及交互式设备时，是全缓冲，否则，一般系统都默认是行缓冲。

```c
#include <stdio.h>
void setbuf(FILE *restrict fp, char *restrict buf );
int setvbuf(FILE *restrict fp, char *restrict buf, int mode, size_t size);
		//	Returns: 0 if OK, nonzero on error
```

`setbuf`可以开启或关闭缓冲区，如果传递的参数buf指向一个长度为`BUFSIZ`的缓冲区，则开启缓冲，如果buf为NULL，则关闭缓冲。
对于`setvbuf`，可以通过参数mode设置缓冲的类型，
	>_IOFBF fully buffered 
	>_IOLBF line buffered 
	>_IONBF unbuffered

* 如果mode为_IONBF，则buf和size忽略；
* 如果mode为_IOFBF或_IOLBF，则buf和size为缓冲区的地址和大小；在一些系统中，提供给标准I/O库buf不会全部用来做缓冲（可能用来记录其他信息）,所以实际可用缓冲区大小，要比size值小。
* 如果mode为_IOFBF或_IOLBF，且buf为NULL，标准I/O库会自行分配大小一般为BUFSIZ的缓冲区。

**注意**：如果缓冲区是函数内部定义的局部数组（也叫automatic variable），因为局部变量是分配在栈空间，函数退出时，会自动清空栈空间，所以流必须在函数退出前关闭，否则会发生异常。

强制刷新流：
```c
#include <stdio.h> 
int fflush(FILE *fp);
		// Returns: 0 if OK, EOF on error;
```

如果入参fp为NULL，所有输出流都会被强制刷新。


打开流
---

```c
#include <stdio.h>
FILE *fopen(const char *restrict pathname, const char *restrict type);
FILE *freopen(const char *restrict pathname, const char *restrict type, FILE *restrict fp);
FILE *fdopen(int fd, const char *type);
		//	All three return: file pointer if OK, NULL on error
```

freopen用于将一个指定的文件打开到一个指定的流上，通常用于修改标准输入，标准输出，标准出错的默认流，将信息输出到指定文件。<br>
fdopen用于将文件描述符转换为流对象，一般用于将管道，网络套接字等描述符转换成流对象，以便用标准I/O库中的函数进行读写操作。

	> Both fopen and freopen are part of ISO C; fdopen is part of POSIX.1, since ISO C doesn’t deal with file descriptors.

type | Description | open(2) Flags 
---| --- | ---
r or rb | open for reading | O_RDONLY 
w or wb | truncate to 0 length or create for writing | O_WRONLY\|O_CREAT\|O_TRUNC 
a or ab | append; open for writing at end of file, or create for writing | O_WRONLY\|O_CREAT\|O_APPEND 
r+ or r+b or rb+ | open for reading and writing | O_RDWR 
w+ or w+b or wb+ | truncate to 0 length or create for reading and writing | O_RDWR\|O_CREAT\|O_TRUNC
a+ or a+b or ab+ | open or create for reading and writing at end of file | O_RDWR\|O_CREAT\|O_APPEND 

b表示二进制文件，不带b的表示文本文件，UNIX系统不对此做区分。

当以读写的方式打开一个文件时，会有如下限制：<br>
	* 输出之后若要直接调用输入，需先调用`fflush`, `fseek`, `fsetpos`, or `rewind`.
	* 输入之后若要直接调用输出，需先调用`fseek`, `fsetpos`, or `rewind`, 或者输入已到文件末尾。

文件流默认打开后是全缓冲，如果流对象指向的是终端设备，则是行缓冲。文件流在打开后做任何操作前，可以调用`setbuf`或`setvbuf`来修改缓冲区设置。

打开的流可以使用`fclose`关闭

```c
#include <stdio.h> 
int fclose(FILE *fp);
		//	Returns: 0 if OK, EOF on error
```

读写流
---

打开一个流后，有三种方式(非格式化）读写流:<br>
 * Character-at-a-time I/O : `getc`, `fget`, `getchar`, `putc`, `fputc`, `putchar`
 * Line-at-a-time I/O : `gets`, `fgets`, `puts`, `fputs`
 * Direct I/O : `fread`, `fwrite`

### Character-at-a-time I/O 

 read one character

```c
#include <stdio.h> 
int getc(FILE *fp); 
int fgetc(FILE *fp); 
int getchar(void);
		//	All three return: next character if OK, EOF on end of file or error
```

getchar 等价于 getc(stdin)<br>
getc 和 fgetc的区别是，getc通常被实现为宏定义，而fgetc是函数定义。这样产生的效果是，getc的参数不能是具有副作用的表达式；getc不能以函数指针的形式作为参数传递；fgetc比getc的调用开销大。

不论是出错还是到达文件结尾，这个三个函数都返回EOF（-1），要获取详细信息，需调用如下函数：

```c
#include <stdio.h> 
int ferror(FILE *fp); 
int feof(FILE *fp);
		//	Both return: nonzero (true) if condition is true, 0 (false) otherwise
void clearerr(FILE *fp);
```

从文件流读取一个字符后，可以使用ungetc将字符放回流中（push back as a stack）。
```c
#include <stdio.h>
int ungetc(int c, FILE *fp);
		// Returns: c if OK, EOF on error
```

一次只能回从一个字符，连续多次调用该函数，再次读取时，读到的顺序与会送顺序相反。
回送的字符不必是上次读取的字符，不能回送EOF。但是即是在文件末尾，也可以回送字符。

输出函数
```c
#include <stdio.h>
int putc(int c, FILE *fp); 
int fputc(int c, FILE *fp); 
int putchar(int c);
		// All three return: c if OK, EOF on error
```

Line-at-a-time I/O
----

```c
#include <stdio.h>
char *fgets(char *restrict buf, int n, FILE *restrict fp); 
char *gets(char *buf);
		// Both return: buf if OK, NULL on end of file or error
```

n为缓冲区buf的长度，fgets最多返回n-1个字符，即是文件中一行的长度超过n个字符。缓冲区一定是以null(\0)字符结尾

gets函数不安全，应该弃用，显见其没有指定缓冲区长度的参数，易发生缓冲区溢出的问题。

```c
#include <stdio.h>
int fputs(const char *restrict str, FILE *restrict fp); 
int puts(const char *str);
		//	Both return: non-negative value if OK, EOF on error
```

fputs 将一个以null字符结尾的字符串写入文件流，但并不写入null字符。通常null字符之前是一个换行符，但这也不是必须的。

puts函数虽然没有类似gets函数的安全问题，但也不建议使用了。

标准I/O的效率
---


Direct I/O
---



