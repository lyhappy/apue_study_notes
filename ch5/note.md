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

[p5_1.c](p5_1.c)
[p5_2.c](p5_2.c)

Function | User CPU (seconds) | System CPU (seconds) | Clock time (seconds) | Bytes of program text 
--- | --- | --- | --- |---
best time from Figure 3.6 | 0.05 | 0.29 | 3.18
fgets, fputs | 2.27 | 0.30 | 3.49 | 143 
getc, putc | 8.45 | 0.29 | 10.33 | 114 
fgetc, fputc | 8.16 | 0.40 | 10.18 | 114
single byte time from Figure 3.6 | 134.61 | 249.94 | 394.95

Direct I/O (Binary I/O)
---

```c
#include <stdio.h>
size_t fread(void *restrict ptr, size_t size, size_t nobj,
		FILE *restrict fp);
size_t fwrite(const void *restrict ptr, size_t size, size_t nobj,
		FILE *restrict fp);
		//	Both return: number of objects read or written
```

在通过网络互联的异构系统中，二进制I/O操作的文件会有兼容性问题。

定位流
---

```c
#include <stdio.h> 
long ftell(FILE *fp);
		//	Returns: current file position indicator if OK, −1L on error 
int fseek(FILE *fp, long offset, int whence);
		//	Returns: 0 if OK, −1 on error
void rewind(FILE *fp);
```


对于二进制文件，定位的单位为byte, whence 的取值和 lseek 相同， SEEK_SET, SEEK_CUR, SEEK_END<br>
对于文本文件（非UNIX系统, 文本文件以特定格式存储）, whence 只能取值 SEEK_SET, offset只能取值0或ftell返回的值。<br>
rewind 用于重定位到文件开始。

```c
#include <stdio.h> 
off_t ftello(FILE *fp);
		//	Returns: current file position indicator if OK, (off_t)−1 on error 
int fseeko(FILE *fp, off_t offset, int whence);
		//	Returns: 0 if OK, −1 on error
```

off_t 类型的长度可以大于32位。

```c
#include <stdio.h>
int fgetpos(FILE *restrict fp, fpos_t *restrict pos); 
int fsetpos(FILE *fp, const fpos_t *pos);
		//	Both return: 0 if OK, nonzero on error
```

format I/O
---

```c
#include <stdio.h>
int printf(const char *restrict format, ...);
int fprintf(FILE *restrict fp, const char *restrict format, ...); 
int dprintf(int fd, const char *restrict format, ...);
		//	All three return: number of characters output if OK, negative value if output error
int sprintf(char *restrict buf, const char *restrict format, ...); 
		//	Returns: number of characters stored in array if OK, negative value if encoding error
int snprintf(char *restrict buf, size_t n, const char *restrict format, ...);
		//	Returns: number of characters that would have been stored in array 
		//	if buffer was large enough, negative value if encoding error
```

format specification:
	`%[flags][fldwidth][precision][lenmodifier]convtype`

Flag | Description
--- | ---
’ | (apostrophe) format integer with thousands grouping characters 
- | left-justify the output in the field
+ | always display sign of a signed conversion
 (space) | prefix by a space if no sign is generated
# | convert using alternative form (include 0x prefix for hexadecimal format, for example) 
0 | prefix with leading zeros instead of padding with spaces

Length modifier | Description
--- | --- 
hh | signed or unsigned char
h | signed or unsigned short
l | signed or unsigned long or wide character 
ll | signed or unsigned long long 
j | intmax_t or uintmax_t
z | size_t
t | ptrdiff_t
L | long double


Conversion type | Description
--- | ---
d,i | signed decimal
o | unsigned octal
u | unsigned decimal
x,X | unsigned hexadecimal
f,F | double floating-point number
e,E | double floating-point number in exponential format
g,G | interpreted as f, F, e, or E, depending on value converted
a,A | double floating-point number in hexadecimal exponential format 
c | character (with l length modifier, wide character)
s | string (with l length modifier, wide character string)
p | pointer to a void
n | pointer to a signed integer into which is written the number of characters written so far 
% | a % character
C | wide character (XSI option, equivalent to lc)
S | wide character string (XSI option, equivalent to ls)

```c
#include <stdarg.h>
#include <stdio.h>
int vprintf(const char *restrict format, va_list arg);
int vfprintf(FILE *restrict fp, const char *restrict format,
		va_list arg);

int vdprintf(int fd, const char *restrict format, va_list arg);
		//	All three return: number of characters output if OK, negative value if output error
int vsprintf(char *restrict buf, const char *restrict format, va_list arg);
		//	Returns: number of characters stored in array if OK, negative value if encoding error

int vsnprintf(char *restrict buf, size_t n,
const char *restrict format, va_list arg);
		//	Returns: number of characters that would have been stored in array 
		//	if buffer was large enough, negative value if encoding error
```

```c
#include <stdio.h>
int scanf(const char *restrict format, ...);
int fscanf(FILE *restrict fp, const char *restrict format, ...);
int sscanf(const char *restrict buf, const char *restrict format, ...);
		//	All three return: number of input items assigned, 
		//	EOF if input error or end of file before any conversion
```

conversion specification:
		`%[*][fldwidth][m][lenmodifier]convtype`

Conversion type | Description
--- | ---
d | signed decimal, base 10
i | signed decimal, base determined by format of input
o | unsigned octal (input optionally signed)
u | unsigned decimal, base 10 (input optionally signed)
x,X | unsigned hexadecimal (input optionally signed)
a,A,e,E,f,F,g,G | floating-point number
c | character (with l length modifier, wide character)
s | string (with l length modifier, wide character string)
[ | matches a sequence of listed characters, ending with ]
[ˆ | matches all characters except the ones listed, ending with ]
p | pointer to a void
n | pointer to a signed integer into which is written the number of characters read so far 
% | a % character
C | wide character (XSI option, equivalent to lc)
S | wide character string (XSI option, equivalent to ls)

```c
#include <stdarg.h>
#include <stdio.h>
int vscanf(const char *restrict format, va_list arg);
int vfscanf(FILE *restrict fp, const char *restrict format,
		va_list arg);
int vsscanf(const char *restrict buf, const char *restrict format, va_list arg);
		//	All three return: number of input items assigned, EOF if input error or end of file before any conversion
```

实现细节
---

```c
#include <stdio.h> 
int fileno(FILE *fp);
		//	Returns: the file descriptor associated with the stream
```

通过fileno可以获取文件流对应的文件描述符，当然这不是标准I/O库的函数。

[p5_3](p5_3)

临时文件
---

```c
#include <stdio.h>
char *tmpnam(char *ptr);
FILE *tmpfile(void);
		//	Returns: pointer to unique pathname Returns: file pointer if OK, NULL on error
```

`tmpnam`生成一个与现有文件不重名且有效的路径字符串，每次调用产生的字符串不相同。最多可以调用TMP_MAX次，标准C规定至少25次，一般的实现远大于这个数。
如果传入的参数ptr为NULL，则生成的文件名存储在一个静态数组，地址以返回值的形式给出，且后续的调用会覆盖静态数组中的值，所以需要及时取出其中的字符串。
使用tmpname生成文件名，然后再创建文件，由于不是一个原子操作，所以会有同步的问题。

`tmpfile`会生成一个临时文件，并在关闭文件或进程退出时自动删除。

XSI 定义的额外两个函数

```c
#include <stdlib.h>
char *mkdtemp(char *template);
		//	Returns: pointer to directory name if OK, NULL on error 
int mkstemp(char *template);
		//	Returns: file descriptor if OK, −1 on error
```

The `mkdtemp` function creates a directory with a unique name.<br>
The `mkstemp` function creates a regular file with a unique name.<br>
This string is a pathname whose last six characters are set to XXXXXX.<br>

mkdtemp 创建的文件夹，访问权限为：S_IRUSR | S_IWUSR | S_IXUSR	0700

Memory Streams
---

```c
#include <stdio.h>
FILE *fmemopen(void *restrict buf, size_t size,
const char *restrict type);
		//	Returns: stream pointer if OK, NULL on error
```

将一段内存转换成流对象，以标准I/O的方式操作。

whenever a memory stream is opened for append, the current file position is set to the first null byte in the buffer.<br>
如果传入的参数buf为NULL，则fmemopen会自行分配一段内存空间，但调用者无法获取其地址，即如果type参数为只读或者只写，那么调用者是无法以直接访问内存的嗯方式写或者读取其内容。这样一般是没有意义的。<br>
在流中写内容后，并调用`fclose`, `fflush`, `fseek`, `fseeko`, or `fsetpos` 会在流末尾写入null字符。

* Of the four platforms covered in this book, only Linux 3.2.0 provides support for memory streams.

```c
#include <stdio.h>
FILE *open_memstream(char **bufp, size_t *sizep); #include <wchar.h>
FILE *open_wmemstream(wchar_t **bufp, size_t *sizep);
		//	Both return: stream pointer if OK, NULL on error
```

These two functions differ from fmemopen in several ways:<br>
* The stream created is only open for writing.<br>
* We can’t specify our own buffer, but we can get access to the buffer’s address and size through the bufp and sizep arguments, respectively.<br>
* We need to free the buffer ourselves after closing the stream.<br>
* The buffer will grow as we add bytes to the stream.<br>

bufp是二级指针，即随着对流的操作，缓冲区会自动增长。所以每次调用fflush或fclose后，缓冲区可能会重新分配，需关注bufp的值，其值可能为指向新的内存空间的指针。

标准I/O的替代品
---

sfio, fio, ASI
