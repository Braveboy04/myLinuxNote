# Linux 多线程编程

## 线程的概念
 从用户空间资源
* 线程不代表资源，是程序的最小执行单元------线程与线程间只是栈不同 
* 进程代表资源，是程序调度的最小单元--------进程与进程是全复制
 从内核空间资源
* Linux应用层中，进程和线程是一样的 

* 线程的管理，线程与进程的区别
* 线程的同步与互斥
* 线程的信号处理 
* 线程的属性 

## 线程的管理
> linux中使用第三方的库进行多线程实现，-lpthread 
> gcc -g -o xxxx xxx.c -lpthread

pthread_self() 获得线程的tid
SYS_gettid    获得线程的tpid

### 线程创建 

int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                          void *(*start_routine) (void *), void *arg);

pthread_t *thread 线程的id 线程在进程中容器里的编号
pthread_attr_t *attr  NULL 
void *(*start_routine) (void *) 线程执行函数
void *arg 传递给线程的参数

pmap



### 线程退出与等待
void pthread_exit(void *retval);

* 自行调用pthread_exit
* 其他线程调用 pthread_cancel 
* 线程执行结束
* 创建线程的进程退出  
* 其中一个线程执行了exec类函数，因为会替换当前进程所有的地址空间
子线程退出仅释放自己私有空间，私有栈空间

int pthread_join(pthread_t thread, void **retval);




void pthread_cleanup_push(void (*routine)(void *), void *arg);
void pthread_cleanup_pop(int execute);
解决线程终止或者异常终止时，释放资源的问题


### 线程取消
> 线程能否被取消要看两点：
* 线程是否具有可取消属性---默认可以被取消
* 线程如果设置为到可取消点才能被取消时，线程不会被立刻取消 
  
	int pthread_cancel(pthread_t thread);
	

线程的取消状态属性 
int pthread_setcancelstate(int state, int *oldstate);
 可取消   不可取消
PTHREAD_CANCEL_ENABLE  可取消属性 
PTHREAD_CANCEL_DISABLE---->未决取消

线程的取消类型属性
int pthread_setcanceltype(int type, int *oldtype);

立刻被取消  PTHREAD_CANCEL_ASYNCHRONOUS

只有到达一定取消点，才会取消  PTHREAD_CANCEL_DEFERRED


### 线程的私有数据

TSD私有数据，同名但是不同内存地址的私有数据结构 

* 创建 
int pthread_key_create(pthread_key_t *key, void (*destructor)(void*));
int pthread_key_delete(pthread_key_t key);

* 读写
 void *pthread_getspecific(pthread_key_t key);
int pthread_setspecific(pthread_key_t key, const void *value);


## 线程的互斥

### 互斥锁通信机制 
* pthread_mutex_t  mutex  ;
* int pthread_mutex_destroy(pthread_mutex_t *mutex);
* int pthread_mutex_init(pthread_mutex_t *restrict mutex,
              const pthread_mutexattr_t *restrict attr);
* int pthread_mutex_lock(pthread_mutex_t *mutex);
* int pthread_mutex_trylock(pthread_mutex_t *mutex);
* int pthread_mutex_unlock(pthread_mutex_t *mutex);

int a

上锁 
a=10
解锁 

条件变量通信机制
pthread_mutex_t work_mutex 
int i=3;
int j=7;

thread A            					thread B
pthread_mutex_lock	  pthread_mutex_lock
					 					 if(i==j)
{										  {
				  

	i++;			     do_something();
	j--;			    
}									      }
pthread_mutex_unlock  pthread_mutex__unlock



thread A            		thread B
pthread_lock			pthread_lock
									pthread_cond_wait	

​									if(i==j)
{								  {
​					  

	i++;			  do_something();
	j--;			    
 if(i==j)
pthread_cond_signal
}								 }
pthread_unlock	    pthread_unlock

### 条件变量 

* 对象：pthread_cond_t  condtion 

* 初始化方法：
```
#include <pthread.h>

int pthread_cond_destroy(pthread_cond_t *cond);
int pthread_cond_init(pthread_cond_t * cond,const pthread_condattr_t * attr);

pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
```
* 条件变量等待 
```
等待条件变量 
#include <pthread.h>

int pthread_cond_timedwait(pthread_cond_t *restrict cond,pthread_mutex_t *restrict mutex,
								const struct timespec *restrict abstime);
	int pthread_cond_wait(pthread_cond_t *restrict cond,
					pthread_mutex_t *restrict mutex);
```

* 条件变量通知
```
通知条件变量
#include <pthread.h>

int pthread_cond_broadcast(pthread_cond_t *cond);
int pthread_cond_signal(pthread_cond_t *cond);
```

* 注意：条件变量要配合互斥体进行操作

### 读写锁

* 如果当前线程读数据 则允许其他线程进行读操作 但不允许写操作
* 如果当前线程写数据 则其他线程的读写都不允许操作 

RCU  内核中RCU 做链表的操作 

*在mutex的基础上 区分了 读锁定 和写锁定 
如果某线程申请了读锁定 其他线程依旧可以申请读锁  不能申请写锁定
如果某线程申请了写锁定 则其他线程不能申请读锁定  也不能申请写锁定

pthread_rwlock_t 

int pthread_rwlock_destroy(pthread_rwlock_t *rwlock);
int pthread_rwlock_init(pthread_rwlock_t *restrict rwlock,
              const pthread_rwlockattr_t *restrict attr);

读锁定 
int pthread_rwlock_rdlock(pthread_rwlock_t *rwlock);



int pthread_rwlock_tryrdlock(pthread_rwlock_t *rwlock);
写锁定 
int pthread_rwlock_trywrlock(pthread_rwlock_t *rwlock);



int pthread_rwlock_wrlock(pthread_rwlock_t *rwlock);



a[]；

read start 
				write
							
read end
				update	



### 线程信号 
* 每个线程可以向其他线程发送信号 pthread_kill 
* 每个信号都有信号屏蔽集合
* 同进程下所有线程共享对某信号的处理方法  
	

线程信号发送

 int pthread_kill(pthread_t thread, int sig);
sig -->0  向某个线程调用pthread_kill，并且第二个参数为0  

pthread_kill(2,0)

int pthread_sigmask(int how, const sigset_t *set, sigset_t *oldset);

SIG_BLOCK
SIG_UNBLOCK
SIG_SETMASK 

不论给进程中的哪个线程发送SIG_KILL  SIG_STOP 则当前进程中的所有线程都会推出 



### 线程属性 
```
typedef struct __pthread_attr_s
{
    int __detachstate;    设置可取消属性 

    int __schedpolicy;    调度策略

    struct __sched_param __schedparam;

    int __inheritsched;

    int __scope;

    size_t __guardsize;
    int __stackaddr_set;

    void *__stackaddr;
    size_t __stacksize;表示堆栈的大小。

}pthread_attr_t; 
```
pthread_t pthread_self(void);
	
__detachstate
int pthread_attr_setdetachstate(pthread_attr_t *attr, int detachstate);
int pthread_attr_getdetachstate(pthread_attr_t *attr, int *detachstate);

struct __pthread_attr_s attr;
pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
pthrea_create(tid,&attr,func,argv);

PTHREAD_CREATE_DETACHED--->不能被等待 
PTHREAD_CREATE_JOINABLE--->

int pthread_attr_setstacksize(pthread_attr_t *attr, size_t stacksize);
int pthread_attr_getstacksize(pthread_attr_t *attr, size_t *stacksize);




https://blog.csdn.net/lg1259156776/article/details/52273929
















汇编 python web运维（php jsp html） linux  c/c++ java  安卓 
安装使用黑客软件


php代码审计 webshell 菜刀
html代码审计
pwm  做逆向分析--分析栈溢出 堆溢出漏洞
# 介绍黑客大赛
 ctf 
 xctf
 flag
 defcon ctf 

# 介绍特训班的课时
# 介绍特训班学习内容
# 介绍网络安全 





