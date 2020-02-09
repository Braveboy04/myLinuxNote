# linux-IPCs
Linux进程间通信方式汇总

# 目前已包含的方式
1. 管道(PIPE) 无名管道
2. FIFO(有名管道)
3. XSI消息队列
4. XSI信号量
5. XSI共享内存
6. POSIX信号量
7. 域套接字(Domain Socket)
8. 信号(Signal)
9. 互斥量(Mutex)
10. sendmsg

其中信号(signal)和信号量(semaphore)本质上并不算是进程间通信方式，应该是进程间同步的方式，但是也可以起到一定的通信作用，故也列在上面。

另外普通的mutex是作用线程间同步用的，但是可以将进程A和进程B共享的内存中初始化一个mutex，这样就可以用将此mutex用作进程间通信用了。


## 无名管道 Pipe
>  同主机进程间数据交互机制： 
* 无名管道(PIPE)：只有创建该管道的程序，才能够找到该管道(亲缘进程通信) 
* 单向传输
* 控制进程只有pipe的一端
* pipe的阻塞操作
*fd[0] 读  fd[1]写 
SIGPIPE 
* 无名管道的内核资源在通信两进程退出后会自动释放，不能像普通文件一样存储信息，不能使用lseek来修改当前pipe的使用位置 
* 

read write 返回-1 
会收到sigpipe信号 



这是我当时的一个猜想，根据猜想写的一个验证程序：窗口一直被一个进程所占用“子进程”，父进程需要打印的数据一直在堆积。图1 的运行结果也是一直在子进程中操作，在50秒内无任何父进程的打，印当父进程退出的时候10次父进程打印一次性全部输出了



## 有名管道 fifo
* FIFO 是依赖于文件系统，

> 依赖于文件系统，像普通文件一样具有磁盘路径，文件权限和其他属性，所有程序都可以通过path找到有名管道
* fifo是一个文件，存储信息在内存，当两个进程都消失，数据消息,文件只有接口的作用
* mkfifo
	int mkfifo(const char* pathname, mode_t mode);		
* 单项通信	     
* 只能够进行少量数据传输
* 只能一对一进行传输
* 两个进程，中途退出了一个进程，未退出的一端如果是写操作的话，返回sigpipe信号 
* 未退出的一端如果是阻塞读操作的话，该读操作不会继续阻塞，直接返回0





# system v ipc 

ipcs 

* key:通信双方约定好的KEY值，32位的整形数据
* id :系统会为IPC分配一个唯一的ID，通信双方都需要知道ID才能使用当前IPC方法，创建者会返回当前ID值，
```c
 	#include <sys/types.h>
  	#include <sys/ipc.h>
	key_t ftok(const char *pathname, int proj_id);
	
    .  proj_id 
    key 31-24 proj_id 低8位
	key 23-16 st_dev属性的低8位
	key 15-0  st_ino属性的低16位
	
```
* 对象 
* 拥有者及权限对象
```c
struct ipc_perm
{
	__kernel_key_t	key;   
	__kernel_uid_t	uid;
	__kernel_gid_t	gid;
	__kernel_uid_t	cuid;
	__kernel_gid_t	cgid;
	__kernel_mode_t	 mode; 
	unsigned short	seq;
};

```
## XSI消息队列
* 对象：
	struct msqid_ds
```c
	struct msqid_ds {
	   struct ipc_perm msg_perm;     /* Ownership and permissions */
	   time_t          msg_stime;    /* Time of last msgsnd(2) */
	   time_t          msg_rtime;    /* Time of last msgrcv(2) */
           time_t          msg_ctime;    /* Time of last change */
	   unsigned long   __msg_cbytes; /* Current number of bytes in
	                                    queue (nonstandard) */
	   msgqnum_t       msg_qnum;     /* Current number of messages
	                                    in queue */
	   msglen_t        msg_qbytes;   /* Maximum number of bytes
	                                    allowed in queue */
	   pid_t           msg_lspid;    /* PID of last msgsnd(2) */
	   pid_t           msg_lrpid;    /* PID of last msgrcv(2) */
	   struct msg *    msq_first ;
 	   struct msg *    msq_last ;
	};

```

#### 优点和缺点
优点：
	1.  消息队列是两个不相关进程之间传递数据的有效渠道.
    2. 与命名管道相比,具有的优势是:
			独立于发送以及接收进程而存在;
			消除命名管道的打开以及关闭而存在的困难;
			可以提前查看紧急信息;
		    避免命名管道的同步以及阻塞问题;
缺点:
		   与命名管道一样,每个数据块都有一个最大长度限制;
		   系统中所有队列包含的全部数据块长度也有一个上限;

* 创建对象：
   int msgget(key_t key, int msgflg);
```c
   #include <sys/types.h>
   #include <sys/ipc.h>
   #include <sys/msg.h>
   int msgget(key_t key, int msgflg);
```

   key_t key 
   int msgflg 确定消息队列的访问权限，


```c
IPC_CREAT  找到当前的key对应的IPC，如果没有IPC则进行创建
IPC_EXCL   如果存在则返回失败
IPC_NOWAIT  返回值为当前ipc对应的id值 
```
* 设置对象：
   int msgctl(int msqid,int cmd,struct msqid_ds *buf);
	cmd:
		IPC_RMID  删除 
		IPC_SET  设置ipc_perm参数 
		IPC_STAT 获取ipc_perm参数
		IPC_INFO 获取ipc信息  和ipcs
* 使用对象：
  1. 发送
       int msgsnd(int msqid,const void *msg_ptr,size_t sz,int msgflg);
      struct msgbuf 
	{
       long mtype;       /* message type, must be > 0 */
       char mtext[1];    /* message data */
      };
	
  2. 接收
    
       ssize_t msgrcv(int msqid, void *msgp, size_t msgsz, long msgtyp,
                      int msgflg);


msgtyp 0   默认接收第一条消息 
msgtyp > 0 接收第一条msgtyp消息
msgtyp < 0 接收小于msgtyp绝对值的消息






msgmni 最大消息队列数 16 
msgmax 最大消息长度(字节数) 8192 
msgmnb 消息队列中的最大字节数 16384 
ipcrm -q key




## XSI信号量
#### 概念：
* 实现进程间同步
* 表示系统可用资源的个数


#### 对象：
```
struct semid_ds {
	struct ipc_perm sem_perm;  /* Ownership and permissions */
	time_t          sem_otime; /* Last semop time */
	time_t          sem_ctime; /* Last change time */
	unsigned long   sem_nsems; /* No. of semaphores in set */
	struct   sem *	sem_base 
};
struct sem{
	int semval  信号量的值
	int sempid  最近一个操作的进程号
}
```
#### 对象操作
* 创建对象： 
 int semget(key_t k,int n,int semflag);
* 设置对象：
 int semctl(int sid,int semnum,int cmd,union semun a);

	
	union semun {
	   int              val;    /* Value for SETVAL */
	   struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
	   unsigned short*  array;  /* Array for GETALL, SETALL */
	   struct seminfo  *__buf;  /* Buffer for IPC_INFO
	                               (Linux-specific) */
	};
cmd  如果对于整个信号量集合，则有如下设置方法
IPC_STAT  ipc_perm
IPC_SET  ipc_perm
IPC_RMID  
IPC_INFO  信号量的系统信息

int semctl(int sid,int semnum,int cmd,.......);

cmd  如果对于单个信号量集合，则有如下设置方法
GETPID 返回操作IPC的PID 
GETVAL 
GETALL
SETVAL 
SETALL 

* 使用对象
* 
```
int semop(int s,struct sembuf *sb,size _t n);

struct sembuf{
unsigned short int sem_num; 
short int sem_op; 
short int sem_flg;};

```
sem_op

	+ 表示进行加
	- 表示进行减
0 表示测试是否为0
* 如果当前信号量的操作后的值为负数，则进行阻塞，一直到满足条件为止 
sem_flg

IPC_NOWAIT 如果操作信号集合中任意一个失败，立即返回，并且不会对其他的信号量做操作
SEM_UNDO ：进程退出后，该进程对sem进行的操作将被撤销 

struct sembuf sops[4]
sops[0].sem_num  =1
sops[0].sem_op   =-1			/ P操作/
sops[0].sem_flag  = 0
sops[1].sem_num   =2
sops[1].sem_op    =1		/ V操作/
sops[1].sem_flag  =0

#### demo
* 库存容量一定



## XSI共享内存

* 数据量最大
* 传输最快
* mmap 

#### 共享内存对象
```

struct shmid_ds {
	struct ipc_perm		shm_perm;	/* operation perms */
	int	  shm_segsz;	/* size of segment (bytes) */
	__kernel_time_t		shm_atime;	/* last attach time */
	__kernel_time_t		shm_dtime;	/* last detach time */
	__kernel_time_t		shm_ctime;	/* last change time */
	__kernel_ipc_pid_t	shm_cpid;	/* pid of creator */
	__kernel_ipc_pid_t	shm_lpid;	/* pid of last operator */
	unsigned short		shm_nattch;	/* no. of current attaches */
}
```

#### 创建共享内存
   int shmget(key_t key, size_t size, int shmflg);
共享内存的权限
SHM_R
SHM_W 

#### 设置共享内存
   int shmctl(int shmid, int cmd, struct shmid_ds *buf);
SHM_INFO
```
 struct shm_info {
     int           used_ids; /* # of currently existing
                                segments */
     unsigned long shm_tot;  /* Total number of shared
                                memory pages */
     unsigned long shm_rss;  /* # of resident shared
                                memory pages */
     unsigned long shm_swp;  /* # of swapped shared
                                memory pages */
     unsigned long swap_attempts;
                             /* Unused since Linux 2.4 */
     unsigned long swap_successes;
                             /* Unused since Linux 2.4 */
 };

```
SHM_STAT
SHM_LOCK
SHM_UNLOCK


#### 使用对象
void *shmat(int shmid, const void *shmaddr, int shmflg);

char * buf



SHM_RDONLY
SHM_REMAP
SHM_EXEC

int shmdt(const void *shmaddr);

#### 注意
1.父子进程的共享内存约定
fork函数
exec执行一个新的程序  自动卸载
exit




### socketpair

如何利用select处理信号 或者事件






### 思考题 
如何在进程间进行文件句柄传递

#### 进行文件句柄的传输
```
sendmsg
recvmsg
ssize_t sendmsg(int sockfd, const struct msghdr *msg, int flags);
ssize_t recvmsg(int sockfd, struct msghdr *msg, int flags);

 struct msghdr {
           void         *msg_name;       /* optional address */
           socklen_t     msg_namelen;    /* size of address */
           struct iovec *msg_iov;        /* scatter/gather array */
           size_t        msg_iovlen;     /* # elements in msg_iov */
           void         *msg_control;    /* ancillary data, see below */
           size_t        msg_controllen; /* ancillary data buffer len */
           int           msg_flags;      /* flags on received message */
       };

    struct iovec {                    /* Scatter/gather array items */
               void  *iov_base;              /* Starting address */
               size_t iov_len;               /* Number of bytes to transfer */
           };

   struct cmsghdr {
       socklen_t     cmsg_len;     /* data byte count, including hdr */
       int           cmsg_level;   /* originating protocol */
       int           cmsg_type;    /* protocol-specific type */
   /* followed by
       unsigned char cmsg_data[]; */
   };

local_fd=socket(AF_LOCAL,SOCK_STREAM,0)

   struct sockaddr_un serveraddr;
```