# Linux的调试器 

## 目标
* GDB基本命令讲解
* GDB的高级增强插件的使用 
* GDB的多线程多进程调试
* GDB破解和防破解讲解 

## 概念
* 基于命令行的调试方法 
* 所有的调试都是可以进行脚本编写的
* 能够调试所有架构的代码
* 有三种调试方法供大家选择
* GDB支持远程调试，支持与IDA进行联调 


## 安装GDB增强工具 (gef)
* GDB的版本大于7.7  
* wget -q -O- https://github.com/hugsy/gef/raw/master/scripts/gef.sh | sh
* 确保网络连通 并且成功更新ubuntu (更新source.list 使用apt-get update) 


## 尝试使用gdb进行小程序的调试
* gcc -g的选项 
* gdb gdb-test 
* 一些GDB的命令 

## GDB的命令 
#### 启动方法
* 本地普通启动 gdb <program> 
* 本地段错误文件启动 gdb <program> core 
* attch方式启动 gdb <program> <PID>
* 远程启动 gdbserver 0.0.0.0:1234 /path/to/file

#### 启动选项 
-se sys-file 

#### 基本命令
set listsize 
set args 10 20 30 40 50
path
show paths 
save breakpoint name.bp
gdb elf -x name.bp
print *array@10
print file::variable 


x/n、f、u
n 是一个正整数，表示显示内存的长度，也就是说从当前地址向后显示几个地址的内容。
f 表示显示的格式，跟print 的格式参数相同
u 表示从当前地址往后请求的字节数，如果不指定的话，GDB默认是4个bytes。u参数可以用下面的字符来代替，b表示单字节，h表示双字节，w表示四字节，g表示八字节。当我们指定了字节长度后，GDB会从指内存定的内存地址开始，读写指定字节，并把其当作一个值取出来。

ps -A 查看当前进程
strip test-1
bt 
info locals


#### Linux程序发布流程
* 确定程序是否存在符号表 
readelf -s test-1 
* 生成符号表
objcopy --only-keep-debug test-1 test-1.symbol
* 生成发布程序 
objcopy --strip-debug test-1 test-release
* 使用符号表进行程序debug
gdb -q --symbol=test-1.symbol --exec=test-release
strip test-release
*  symbol-file ./test-1.symbol





#### GDB安装插件
git clone https://github.com/gatieme/GdbPlugins.git ~/GdbPlugins

echo "source ~/GdbPlugins/peda/peda.py" > ~/.gdbinit --->破解 逆向
echo "source ~/GdbPlugins/gef/gef.py" > ~/.gdbinit ----> debug 逆向
echo "source ~/GdbPlugins/gdbinit/gdbinit" > ~/.gdbinit ---> 个人定制




#### GDB中暂停/恢复程序运行
* 断点
 条件断点
 break     if 
 info breakpoints
 delete    
 disable  
 enable  


condition 1 i==90
condition 1


* 观察点
 watch  地址
 info watchpoints
 rwatch 


* 捕捉点
catch event
throw 抛出一个C++的异常 catch throw
catch 捕捉一个C++的异常 catch catch
exec  调用系统调用exev时停止 catch exec   exe\
fork  调用系统调用fork时停止 catch fork
load/load libname 载入动态链接库时 catch load / catch load libname
unload

jpeg--->乱码 
securt.so
catch load securt.so


x/nfu  
p *array@len


暂停命令
commands bnum 
.
.
.
.
end

commands bnum 
end

xgoogle.top



多进程
	资源：内存 文件 时间片 协处理器 
	完全复制 
    fork
多线程 
	执行单元



#### 多进程调试
1. 确定gdb中的进程跟踪模式
show follow-fork-mode
show detach-on-fork 

follow-fork-mode  detach-on-fork
	parent			 on				只调试父进程，子进程正常运行
	child	   		 on			  	只调试子进程，父进程正常运行
	parent			 off			同时调试两个进程，子进程暂停在fork位置
	child			 off			同时调试两个进程，父进程暂停在fork位置

2. 进程间的切换
info inferiors
inferiors num

add-inferior [-copies n] [-exec executable]
detach 
kill 
remove


#### 多线程调试 
gcc -g thread.c -o thread -lpthread
默认跟主线程  子线程停止在creat phread的程序上 

* GDB帮你决定现在的调试走向 
info threads 正在运行的线程信息
b .... thread ...
b line thread threadno if...
* 所有线程会因为一个断点所中断


gcc -g thread.c -o thread -lpthread

show non-stop 

set scheduler-locking off|on|step 

off 不锁定任何线程 
on 锁定其他线程，只有当前线程执行


#### display 自动显示
display 
undisplay  num
info display 

#### 运行中改变程序的运行
print i = 50
jump address
call func

### 开启core文件 
ulimit -c 查看当前文件句柄限制
sudo ulimit -c unlimited   设置无限大可生成core
https://blog.csdn.net/wkd_007/article/details/79757289


#### Crack Me

chmod a+x
file pass_guess
strings pass_guess



#### peda 
aslr on
aslr 
elfheader

下载IDA PRO 安装


1.找到程序的入口地址
	
0x1acda8
call   0x80484ad
print $eip=0x80484ad



逆向破解中的关系：
静态--->流程 功能 断点 关键函数的解析 
动态--->查看内存 修改内存  控制程序执行流程



NX 栈不可执行
PIE(alsr)
relro

dumprop
elfheader
elfsymbol

#### 反调试技术

加壳 /加花 

模糊头等 方法 



# 破解







./elfa xxxx 

  8 #include<stdio.h>
  8 #include<stdio.h>
  9 #include<unistd.h>
 10 #include<sys/types.h>
 11 #include<sys/wait.h>
 12 
 13 int main()
 14 {
 15     pid_t pid = fork();//创建子进程
 16 
 17     if(pid == -1)
 18     {
 19         perror("fork error");
 20         return -1;
 21     }
 22     else if(pid == 0)//child
 23     {
 24         printf("i am a child:my pid is %d,my father is %d\n",getpid(),getppid());
 25     }
 26     else//father
 27     {
 28         printf("i am a father:my pid is %d\n",getpid());
 29         wait(NULL);//等待子进程
 30     }
 31 
 32     return 0;
 33 
 34 }

  8 #include<stdio.h>
  9 #include<pthread.h>
 10 
 11 void* thread1(void* arg)
 12 {
 13     printf("i am thread1,my tid is %u\n",pthread_self());
 14     return NULL;
 15 }
 16 
 17 void* thread2(void* arg)
 18 {
 19     printf("i am thread2,my tid is %u\n",pthread_self());
 20     return NULL;
 21 }
 22 
 23 int main()
 24 {
 25     pthread_t tid1,tid2;
 26     pthread_create(&tid1,NULL,thread1,NULL);//创建线程1
 27     pthread_create(&tid2,NULL,thread2,NULL);//创建线程2
 28 
 29     pthread_join(tid1,NULL);//等待线程1
 30     pthread_join(tid2,NULL);//等待线程2 
 31 
 32     return 0;
 33 }



RELRO

GOT--->只读
printf  my_address 







func my_address  hook 



void fun(char *s)
{

	char buf[0x100];
	strcpy(buf,s)

}



## 多线程实例调试
启动参数 
webd -d -p 8000 -s 4444 -e

rdi rsi rdx rcx r8 r9 

 netstat -aptn




openssl 的安装

作业：
	自行调试webd，调试过程截图并且汇总一个md 
时间：
	下周六--3月16日 

list

ls -l /root/

rdi 

rsi 

rdx

rcx

r8 

r9



## 多进程调试

set follow-fork-mode parent/child

set detach-on-fork on/off 

info inferiors  被GDB调试的每个inferior 的信息 

detach inferiors id 



## 多线程调试 

thread thread-id 

info threads

thread apply [thread-id-list] cmd

thread apply [1-5] p/d temp

set print thread-events

set scheduler-locking off/on/step 



## 逆向破解

1. 动态调试与静态分析相结合 

2. 找到验证函数 

   2.1 动态调试的内存特征值 

   ​		60----base1

   ​					59--- base2

   ​								58-----base3

   ​												57-------base4

   ​																												base  n



## 漏洞挖掘与代码防护

1. 什么叫漏洞 

   1. 1  因代码没有完整的校验，造成了内存结构的破坏

2. 如何发现漏洞

     2.1  确定危险函数，

3. 如何利用漏洞

4. 如何防止漏洞

   ​	4.1 NX ：栈不可执行(dep )--rop

   ​	4.2 aslr：指令地址随机化 ----->offset是固定的

   ​	4.3 canary   -fstack-protector-all"A -fno-stack-protectorZ 

   ​	4.4 RELRO 





