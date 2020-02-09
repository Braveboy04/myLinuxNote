# Linux多进程管理

## 数据处理模块的目的
* 完成业务逻辑
> 解析协议方法
> http ssl ftp 
> web 


* 效率提高
> 多进程
  进程的基本概念
  进程控制的API
  高级进程 
  IPC
> 多线程
  线程的基本概念
  线程的控制API
  线程的互斥
  线程的同步
> 池
  连接池
  内存池
  线程池
  进程池

## 进程的基本概念
* Linux中事务管理的基本单元  代表资源的总和 

* Linux进程的属性

独立性

### 进程状态
```
define TASK_RUNNING		    0
define TASK_INTERRUPTIBLE	1
define TASK_UNINTERRUPTIBLE	2
define TASK_STOPPED	     	4
define TASK_ZOMBIE		   8
``` 
![](https://i.imgur.com/gSp0isA.png)

### 进程基本属性
#### 进程号 

pid_t getpid(void);
pid_t getppid(void);
getpgid();

#### 用户号 
pid_t getpgid(pid_t pid);
int setuid(uid_t uid);
euid uid 

#### 会话号
pid_t getsid(pid_t pid);
setsid（void）；


#### 控制终端
一个会话可以有一个控制终端，控制进程就是打开终端的进程
终端发送的信号，只有前台进程组的进程会收到，后台不处理
ctl + \ 

tcgetpgrp
tcgetsid


#### 进程用户属性
UID EUID写
0 root
0 - 1000 
1000


setuid --->临时将该进程设置为文件拥有权限
1000 
1001
uid   euid
1001  1000

ping ---icmp---raw--->root

ping root root

1000 0 
passwd 
chmod u-s /usr/bin/passwd


### 进程的管理及控制

#### 进程创建
fork 
pid_t fork(void);
> 0
==0

fork对于IOFILE中的缓存进行拷贝 

vfork  ,在必要的时候才新申请内存


### 进程的代码执行
execve（/bin/sh）
fork 
vfork : 共享父进程代码段 数据段 
process 1

process 2


process(a,b,c,d,e,f)
process(argv)


/bin/sh ls -l -a
execl(/bin/sh,sh,ls,-l,-a);
execlp(sh,sh,ls,-l,-a);
execle(/bin/sh,sh,ls,-l,-a,env);

execv(/bin/sh,argv[]);

		文件名  路径名  参数表 使用argv			
execl  			1		1		
execlp   1				1
execle			1		1
execv			1				1	
execvp	1						1	
execve			1		1

       int execl(const char *path, const char *arg, ...);
       int execlp(const char *file, const char *arg, ...);
			会从$PATH环境变量中找到目录
       int execle(const char *path, const char *arg,
                  ..., char * const envp[]);
       int execv(const char *path, char *const argv[]);
       int execvp(const char *file, char *const argv[]);
       int execve(const char *file, char *const argv[],
                   char *const envp[]);


### 回收进程资源
exit(0) _exit(0);
return 

int on_exit(void (*function)(int , void *), void *arg);

wait 
waitpid

### 特殊进程讲解
#### 守护进程
> 代码设计的守护进程
* 调用umask将文件模式创建屏蔽字设置为0
* 调用fork，父进程退出。保证子进程不是话首进程，从而保证后续不和其他终端关联
* 设置新会话。
* 
	service xinetd restart


#### 日志进程
1.查看日志进程

* 自行进行日志的文件创建 添加 管理
* 使用日志守护进程

rsyslogd 
 ps -aux|grep syslogd

通过修改syslog的配置文件进行日志守护进程的使用
cat /etc/rsyslog.conf
 openlog(argv[0],LOG_NDELAY|LOG_PID,LOG_DAEMON);	

 void openlog(const char *ident, int option, int facility);

LOG_PID  日志包含当前进程的pid
LOG_PERROR  stderr 


/var/log/dmesg      内核引导信息日志
/var/log/message    标准系统错误信息日志
/var/log/maillog    邮件系统信息日志
/var/log/cron       计划任务日志
/var/log/secure     安全信息日志


LOG_AUTH       security/authorization messages

LOG_AUTHPRIV   security/authorization messages (private)

LOG_CRON       clock daemon (cron and at)

LOG_DAEMON     system daemons without separate facility value

LOG_FTP        ftp daemon

LOG_KERN       kernel messages (these can't be generated from user pro‐
              cesses)

LOG_LOCAL0 through LOG_LOCAL7
              reserved for local use

LOG_LPR        line printer subsystem

LOG_MAIL       mail subsystem

LOG_NEWS       USENET news subsystem

LOG_SYSLOG     messages generated internally by syslogd(8)

LOG_USER (default)
              generic user-level messages

LOG_UUCP       UUCP subsystem


产生一条日志信息
void syslog(int priority, const char *format, ...);

LOG_EMERG      system is unusable

LOG_ALERT      action must be taken immediately

LOG_CRIT       critical conditions

LOG_ERR        error conditions

LOG_WARNING    warning conditions

LOG_NOTICE     normal, but significant, condition

LOG_INFO       informational message

LOG_DEBUG      debug-level message


LOG_KERN   /vae


#### 使用系统守护进程，实现授时服务器



vi /etc/xinetd.d/
service date_mine
{
	disable		= no
	type		= UNLISTED
	socket_type	= stream
	protocol	= tcp
	user		= chandler
	wait		= no
	port 		= 5859
	bind		= 127.0.0.1
	server		= /bin/date
}
service xinetd start
service xinetd restart


ping 