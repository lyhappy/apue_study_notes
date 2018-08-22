进程关系
===

终端登录
---

早期，用户用哑终端（用硬连接连到主机）进行登录。终端可以是本地的（直接连接），也可以是远程的（通过调制解调器连接）。这两种方式都是经由内核中的终端设备驱动。

后来在窗口系统中，仿真出基于字符的终端（shell 命令行）。

### BSD终端登录

0. 系统管理员创建通常名为/etc/ttys的文件，其中，每个终端都有一行记录设备名和其他传递给getty的参数
1. 系统启动时，内核创建pid为1的进程init，init进程将系统带入多用户模式。
1.1 init进程读取/etc/ttys文件，为每一个允许登录的终端设备调用fork, 并用exec执行getty程序。init进程为超级用户权限，ruid和euid均为0，fork出来的子进程，执行getty后，也是超级用户权限。
2. getty对终端设备调用open函数，用以读写，如果设备是一个modem，则open函数将等待modem拨号成功后返回。
3. 设备打开后，文件描述符0，1，2将被设置到的设备上。
4. 随后getty输出类似“login: ”的字符，等待用户输入用户名。
5. 当用户输入用户名后，getty的工作完成，随后调用login程序：
`execle("/bin/login", "login", "-p", username, (char *)0, envp);`
5.1 getty启动login时，传递了一个简单的环境变量（包括终端的名字等信息）, -p 参数提示login保留传递给其的环境变量，也可添加别的，但不能替换。
6. login程序也具有超级用户权限，且要做很多事。
6.1 根据用户名，调用getpwnam获取相应的口令文件登录项。
6.2 调用getpass显示"Password: "，并读取用户键入的口令
6.3 调用crypt加密用户键入的口令，然后与shadow password文件中的pw_passwd字段做比较。
6.4 如果口令验证失败，调用exit(1)退出，terminal会通知init进程再次发起登录流程。
6.5 如果口令验证成功，login继续完成以下任务：
	6.5.1 Change to our home directory (chdir)
	6.5.2 Change the ownership of our terminal device (chown) so we own it
	6.5.3 Change the access permissions for our terminal device so we have permission to read from and write to it
	6.5.4 Set our group IDs by calling setgid and initgroups
	6.5.5 Initialize the environment with all the information that login has: our home directory (HOME), shell (SHELL), user name (USER and LOGNAME), and a default path (PATH)
	6.5.6 Change to our user ID (setuid) and invoke our login shell, as in 
		`execl("/bin/sh", "-sh", (char *)0);`

网络登录
---








