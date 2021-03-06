进程关系
===

终端登录
---

早期，用户用哑终端（用硬连接连到主机）进行登录。终端可以是本地的（直接连接），也可以是远程的（通过调制解调器连接）。这两种方式都是经由内核中的终端设备驱动。

后来在窗口系统中，仿真出基于字符的终端（shell 命令行）。

### BSD终端登录

系统管理员创建通常名为/etc/ttys的文件，其中，每个终端都有一行记录设备名和其他传递给getty的参数

系统启动时，内核创建pid为1的进程init，init进程将系统带入多用户模式。

1. init进程检查/etc/ttys文件,为每一个允许登录的终端设备fork一个新的登录进程.
2. 在新的登陆进程中立即调用exec执行getty程序.
  - 1. getty调用open函数打开终端设备描述符,用以读写;
  - 2. 设备打开后，文件描述符0，1，2将被设置到的设备上。
  - 3. 随后getty输出类似“login: ”的字符，等待用户输入用户名。
3. 当用户输入用户名后，getty的工作完成，随后调用login程序：
    `execle("/bin/login", "login", "-p", username, (char *)0, envp);`
    getty启动login时，传递了一个简单的环境变量（包括终端的名字等信息）, -p 参数提示login保留传递给其的环境变量，也可添加别的，但不能替换。
  - 1. 根据用户名，调用getpwnam获取相应的口令文件登录项。
  - 2. 调用getpass显示"Password: "，并读取用户键入的口令
  - 3. 调用crypt加密用户键入的口令，然后与shadow password文件中的pw_passwd字段做比较。
    - 3.1 如果口令验证失败，调用exit(1)退出，terminal会通知init进程再次发起登录流程。
  - 4. 如果口令验证成功，login继续完成以下任务：
    - 4.1 切换到登录用户的home目录(chdir);
    - 4.2 修改当前终端设备的拥有者为当前登录用户(chown);
    - 4.3 修改当前终端设备的访问权限,以便当前登录用户能够读写之;
    - 4.4 通过调用`setgid`和`initgroups`设置组ID;
    - 4.5 使用目前的信息(HOME,SHELL,USER,LOGINNAME,PATH)初始化环境变量;
    - 4.6 调用`setuid`切换到当前用户,并调起其登录shell;
    - ... login 还会做更多的事情,不在此讨论.

网络登录
---








