/******************************************************************************

                  版权所有 (C), 2017-2017, chandler

 ******************************************************************************
  文 件 名   : kindle_mem_noblock.c
  版 本 号   : 初稿
  作    者   : chandler
  生成日期   : 2017年4月2日
  最近修改   :
  功能描述   : 内核字符设备驱动
  函数列表   :
*
*       1.                ExitKindleMemDevice
*       2.                InitKindleMemDevice
*       3.                IoctlKindleMemDev
*       4.                OpenKindleMemDev
*       5.                ReadKindleMemDev
*       6.                ReleaseKindleMemDev
*       7.                SeekKindleMemDev
*       8.                SetupKindleMemDev
*       9.                WriteKindleMemDev
*
#include <linux/module.h>
  修改历史   :
  1.日    期   : 2017年4月2日
    作    者   : chandler
    修改内容   : 创建文件

******************************************************************************/

/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 内部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 全局变量                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 模块级变量                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/

#include <linux/fs.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/sched.h>
#include <linux/wait.h>
//#include <asm/atomic.h>

#define KINDLE_MEM_MINOR_COUNT   0x01
#define KINDLE_MEM_MINOR_BASE      0x00
#define KINDLE_MEM_SIZE                     0x1000
#define KINDLE_MEM_MAGIC                   'k'
#define KINDLE_MEM_CLEAR                  _IO(KINDLE_MEM_MAGIC,0) 
#define KINDLE_MEM_TEST                     _IO(KINDLE_MEM_MAGIC,1)
#define KINDLE_MEM_START_POS         0x0
#define KINDLE_MEMT_CURRENT_POS  0x1


typedef struct KindleMemDev{
     int                        major;
     int                        minor;
     struct cdev                cdev;
     spinlock_t                 lock;
     int                        OpenCnt;
     rwlock_t                   rwlock;
	 struct mutex		 		mutex;
	 unsigned int				len;
	 wait_queue_head_t			r_wait;
	 wait_queue_head_t			w_wait;
     unsigned char       		mem[KINDLE_MEM_SIZE];
}T_KindleMemDev;

typedef T_KindleMemDev *  PT_KindleMemDev;

PT_KindleMemDev  g_ptKindleMemDev;

static int OpenKindleMemDev(struct inode *inode, struct file *filp)
{
    filp->private_data = g_ptKindleMemDev;
#if 0
    spin_lock(&g_ptKindleMemDev->lock);      
    //如果打开计数不为0说明有进程已经打开了该设备，故解锁退出
    if(g_ptKindleMemDev->OpenCnt)      
    {
        spin_unlock(&g_ptKindleMemDev->lock);   
        return -EBUSY;
    }
    g_ptKindleMemDev->OpenCnt++;
    spin_unlock(&g_ptKindleMemDev->lock);    
#endif
    printk("KindleMemDev Open\n");    
    return 0;
}

static int ReleaseKindleMemDev (struct inode *inode, struct file *filp)
{
     filp->private_data = NULL;
#if 0
     spin_lock(&g_ptKindleMemDev->lock);  
     g_ptKindleMemDev->OpenCnt--;
     spin_unlock(&g_ptKindleMemDev->lock);  
#endif
     printk("KindleMemDev Release\n");
     return 0;
}


/*****************************************************************************
 函 数 名  : ReadKindleMemDev
 功能描述  : 字符设备读取函数
 输入参数  : struct file *pFile  
             char __user *pBuf   
             size_t iSize        
             loff_t *ptPos       
 输出参数  : 无
 返 回 值  : static ssize_t
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2017年4月2日
    作    者   : chandler
    修改内容   : 新生成函数

*****************************************************************************/
static ssize_t ReadKindleMemDev(struct file *pFile, char __user *pBuf, size_t iSize, loff_t *ptPos)
{
     int               iRet   =0;
     unsigned int iCount =iSize;
     //拿到私有设备结构体
     PT_KindleMemDev ptKindleDev=pFile->private_data;
	 //定义当前操作read的进程的等待队列节点
	 DECLARE_WAITQUEUE(wait,current);
	 //申请互斥体
	 mutex_lock(&ptKindleDev->mutex);
	 //把当前节点加入等待队列
	 add_wait_queue(&ptKindleDev->r_wait,&wait);
	 //进行可读判断
	 //while 的原因: 再次判断当进程唤醒之后的那一刻 是否又被其他进程抢夺资源
	 while(ptKindleDev->len==0)
	 {
		//如果当前的FIFO中是空的  没有数据
		if(pFile->f_flags & O_NONBLOCK)
		{
			iRet  = -EAGAIN;
			goto  out;
		}
		//设置可中断睡眠状态
		__set_current_state(TASK_INTERRUPTIBLE);
		mutex_unlock(&ptKindleDev->mutex);
		//将当前进程切换
		schedule();
		//当被唤醒之后
		//判断唤醒源是否为信号
		if(signal_pending(current))
			{
			iRet	= -ERESTARTSYS;
			goto out2;
		}
		//到此为止 为真正的资源释放唤醒
		mutex_lock(&ptKindleDev->mutex);
	 }
	 // 真正的读操作
	    if(iCount>ptKindleDev->len)
       	   iCount=ptKindleDev->len;
		 
	    if(copy_to_user(pBuf,ptKindleDev->mem,iCount))
	     {
	          iRet=-EFAULT;
			  goto out;
	     }
	    else
	     {
	     	 //将未读的数据放到已经读走空间中
	          memcpy(ptKindleDev->mem,ptKindleDev->mem+iCount,ptKindleDev->len-iCount);
			 //把总长度做更新
			  ptKindleDev->len -=iCount;
			  printk("Read %d bytes from %d，current_len:%d\n",iCount, ptKindleDev->len);
			  //因为读操作会释放一定空间，所以要唤醒写操作的等待队列
			  wake_up_interruptible(&ptKindleDev->w_wait);
			  
	          iRet=iCount;	         
	     }
out:
	mutex_unlock(&ptKindleDev->mutex);
out2:
	//移除读操作加入到等待队列中的节点
	remove_wait_queue(&ptKindleDev->r_wait,&wait);
	//回复到进程执行状态
	set_current_state(TASK_RUNNING);
	return iRet;
}


static ssize_t WriteKindleMemDev(struct file *pFile, const char __user *pBuf, size_t iSize, loff_t *ptPos)
{
      int           iRet   =0;
      unsigned int  iCount =iSize;
      //拿到私有设备结构体
      PT_KindleMemDev ptKindleDev=pFile->private_data;
	  //把当前节点封装为写等待队列节点
	  DECLARE_WAITQUEUE(wait,current);
	  mutex_lock(&ptKindleDev->mutex);
	  add_wait_queue(&ptKindleDev->w_wait,&wait);

	 while(ptKindleDev->len==KINDLE_MEM_SIZE)
	 {
		//FIFO 为满 无法写入 把当前写入的进程挂在等待队列上休眠
		if(pFile->f_flags & O_NONBLOCK)
		{
			iRet = -EAGAIN;
			goto  out;
		}
		//进行睡眠
		__set_current_state(TASK_INTERRUPTIBLE);
		//解锁一定要发生在进程调度之前
		mutex_unlock(&ptKindleDev->mutex);
		schedule();
		//判断唤醒源是否为信号
		if(signal_pending(current))
		{
			iRet	= -ERESTARTSYS;
			goto out2;
		}
		mutex_lock(&ptKindleDev->mutex);
	 }
	 //开展读操作  
      if(iCount>KINDLE_MEM_SIZE-(ptKindleDev->len))
          iCount=KINDLE_MEM_SIZE-(ptKindleDev->len);
	  	  
      if(copy_from_user(ptKindleDev->mem+ptKindleDev->len,pBuf,iCount))
      {
            iRet=-EFAULT;
			goto out;
      }
      else
      {
            ptKindleDev->len +=iCount;
			printk("Write %d bytes current len = %d\n",iCount,ptKindleDev->len);
			wake_up_interruptible(&ptKindleDev->r_wait);
            iRet        =iCount;
      }
   
out:
	 mutex_unlock(&ptKindleDev->mutex);
out2:
	remove_wait_queue(&ptKindleDev->w_wait,&wait);
	set_current_state(TASK_RUNNING);
	return iRet;
}

static long IoctlKindleMemDev(struct file *pFile, unsigned int iCmd, unsigned long iArg)
{
       //拿到私有设备结构体
       PT_KindleMemDev ptKindleDev=pFile->private_data;
       switch(iCmd)
       {
          case  KINDLE_MEM_CLEAR:
		  			mutex_lock(&ptKindleDev->mutex);
                    memset(ptKindleDev->mem,0,KINDLE_MEM_SIZE);
                    printk("Ioctl KindleMemDev is set to zero\n");
					mutex_unlock(&ptKindleDev->mutex);
                    break;
          case  KINDLE_MEM_TEST:
		  			mutex_lock(&ptKindleDev->mutex);
                    strcpy(ptKindleDev->mem,"123456789 test\n");
                    printk("Ioctl KindleMemDev Testing\n");
					mutex_unlock(&ptKindleDev->mutex);
                    break;
          default:
                    return -EINVAL;
       }
       return   0;
}

static loff_t SeekKindleMemDev(struct file *pFile, loff_t iOffset, int iOrig)
{
      loff_t  iRet    =0;
      //拿到私有设备结构体
     PT_KindleMemDev ptKindleDev=pFile->private_data;
     switch(iOrig)
     {
         case KINDLE_MEM_START_POS:
                 if(iOffset<0)
                 {
                        iRet=-EINVAL;
                        break;
                 }
                if((unsigned int)iOffset >KINDLE_MEM_SIZE )
                {
                        iRet=-EINVAL;
                        break;
                 }
                pFile->f_pos = (unsigned int )iOffset;
                iRet               = pFile->f_pos;
                break;
          case  KINDLE_MEMT_CURRENT_POS:
               if((pFile->f_pos+iOffset )> KINDLE_MEM_SIZE)
               {
                        iRet=-EINVAL;
                         break;
               }
               if((pFile->f_pos+iOffset ) <0)
               {
                          iRet=-EINVAL;
                          break;
               }
               pFile->f_pos +=iOffset;
               iRet                   =pFile->f_pos;
               break;
            default :
                iRet=-EINVAL;
                break;
     }
     return  iRet;
}

static const struct file_operations g_tKindleMemFops={
         .owner             =   THIS_MODULE,
         .open               =   OpenKindleMemDev,
         .release            =   ReleaseKindleMemDev,
         .read                =   ReadKindleMemDev,
         .write               =   WriteKindleMemDev,
         .unlocked_ioctl  =   IoctlKindleMemDev,
         .llseek               =   SeekKindleMemDev,
};

static void SetupKindleMemDev(PT_KindleMemDev ptKindleMemDev,int iMinor)
{  
        int   iRet;
        dev_t tDevno=MKDEV(ptKindleMemDev->major, iMinor);
      //设置当前设备结构体中的cdev变量
        cdev_init(&ptKindleMemDev->cdev, &g_tKindleMemFops);
        ptKindleMemDev->cdev.owner=THIS_MODULE;
     // 提交设置好的Cdev结构体
       iRet=cdev_add(&ptKindleMemDev->cdev, tDevno,KINDLE_MEM_MINOR_COUNT);
       if(iRet)
       {
          printk(KERN_NOTICE "Error %d adding KindleMem%d",iRet,iMinor);
       }
       memset(ptKindleMemDev->mem,0,KINDLE_MEM_SIZE);
       //对自旋锁进行初始化
      spin_lock_init(&ptKindleMemDev->lock);
      ptKindleMemDev->OpenCnt=0;
      rwlock_init(&ptKindleMemDev->rwlock);
	  mutex_init(&ptKindleMemDev->mutex);
	  //初始化当前FIFO长度为0
	  ptKindleMemDev->len=0;
	  //初始化当前的等待队列
	  init_waitqueue_head(&ptKindleMemDev->r_wait);
	  init_waitqueue_head(&ptKindleMemDev->w_wait);
}
static int __init InitKindleMemDevice(void)
{
      int iRet;

       // 分配设备结构体
      g_ptKindleMemDev=kzalloc(sizeof(T_KindleMemDev),GFP_KERNEL);
      if(!g_ptKindleMemDev)
      {
          iRet=-ENOMEM;
          goto fail_malloc;
      }
      
      dev_t tDevno=MKDEV(g_ptKindleMemDev->major, 0);
      //分配Cdev结构体
      if(g_ptKindleMemDev->major)
      {
          //已经分配过cdev结构体
          iRet=register_chrdev_region(g_ptKindleMemDev->major,KINDLE_MEM_MINOR_COUNT,"KindleMem");
      }
      else
      {
           //如果没有分配过cdev结构体
           iRet=alloc_chrdev_region(&tDevno,KINDLE_MEM_MINOR_BASE ,KINDLE_MEM_MINOR_COUNT,"KindleMem");
           g_ptKindleMemDev->major=MAJOR(tDevno);
      }
      if(iRet<0)
          return iRet;
     
      //对设备结构体做设置
      SetupKindleMemDev(g_ptKindleMemDev,KINDLE_MEM_MINOR_BASE);
      
      return 0;

fail_malloc:
    unregister_chrdev_region(tDevno,KINDLE_MEM_MINOR_COUNT );
    return iRet;
}

module_init(InitKindleMemDevice);

static void __exit ExitKindleMemDevice(void)
{
    cdev_del(&g_ptKindleMemDev->cdev);
    kfree(g_ptKindleMemDev);
    unregister_chrdev_region(MKDEV(g_ptKindleMemDev->major,0),KINDLE_MEM_MINOR_COUNT);
}
module_exit(ExitKindleMemDevice);
MODULE_AUTHOR("Chandler 20170310");
MODULE_LICENSE("GPL v2");

#if 0

//定义一个自旋锁
spinlock_t lock;

//初始化自旋锁
spin_lock_init(lock);

//获得自旋锁
spin_lock(spinlock_t * lock);


//临界区操作

//释放自旋锁
spin_unlock(spinlock_t * lock);

//尝试获得自旋锁
spin_trylock(spinlock_t * lock);
//拿到会返回true
//没拿到会返回false

//锁定被关闭中断
spin_lock_irq(spinlock_t * lock);
spin_unlock_irq(spinlock_t * lock)
spin_lock_irqsave(lock, flags)
spin_unlock_irqrestore(spinlock_t * lock, unsigned long flags)
#endif
#if 0
//定义和初始化读写自旋锁
rwlock_t rwlock;
rwlock_init(&rwlock);

//读锁定
void read_lock(&rwlock);
void read_lock_irq(&rwlock);
void read_lock_irqsave(&rwlock);

//读解锁
void read_unlock(&rwlock);
void read_unlock_irq(&rwlock);
void read_lock_irqrestore(&rwlock);

//写锁定
void write_lock(&rwlock);
void write_lock_irq(&rwlock);
void write_lock_irqsave(&rwlock);

//写解锁
void write_unlock(&rwlock);
void write_unlock_irq(&rwlock);
void write_lock_irqrestore(&rwlock);

#endif

#if 0
//获取/锁定一个顺序锁
void write_seqlock(seqlock_t * sl);
void write_seqlock_irqsave(seqlock_t * sl);

//释放顺序锁
void write_sequnlock(seqlock_t * sl);
void write_sequnlock_irqrestore(seqlock_t * sl);

//读锁定
read_seqbegin(const seqlock_t * sl);
read_seqbegin_or_lock_irqsave(seqlock_t * lock, int * seq)
	#endif
#if 0
//定义等待队列
wait_queue_head_t WaitQueue;
//初始化等待队列
init_waitqueue_head(&WaitQueue);

//定义并初始化等待队列的快捷方式

DECLARE_WAIT_QUEUE_HEAD(WaitQueue)

//定义一个队列结点   把进程封装成一个等待队列上的节点
DECLARE_WAITQUEUE(name, tsk)

//添加 移除等待队列节点
void add_wait_queue(wait_queue_head_t * q, wait_queue_t * wait)
void remove_wait_queue(wait_queue_head_t * q, wait_queue_t * wait)

//等待事件
wait_event(queue, condition); //不可中断睡眠状态
wait_event_interruptible(wq, condition); //可中断睡眠状态
wait_event_timeout(wq, condition, timeout);
wait_event_interruptible_timeout(wq, condition, timeout);

//唤醒队列
void wake_up(&WaitQueue);


static ssize_t WriteKindleMemDev(struct file *pFile, const char __user *pBuf, size_t iSize, loff_t *ptPos)
{
	//把当前write的进程定义为一个等待队列的节点
	DECLARE_WAITQUEUE(wait,current);
	//向定义好的队列中添加该节点
	add_wait_queue(&xxx_wait,&wait);
	
	//设备读写操作
	do{
		//判断是否可写
		avail=device_writeable();
		if(avail<0)
		{
		//判断是否以非阻塞方式打开
		// open("file_name",flags)
		//flags: O_NONBLOCK O_RWDR
		   if(file->f_flags & O_NONBLOCK)
		   	{
			   ret=-EAGAIN;
			   break;
		   }
		   //设置当前进程为可中断睡眠
		   //wait_event(&xxx_wait, 1);
		  __set_current_state(TASK_INTERRUPTIBLE);
		   //将当前进程进行切换
		  schedule(void);
		   //判断当前进程是被信号唤醒?
		   if(signal_pending(current))
		   	{
			  ret=-ERESTARTSYS;
			  break;
		   }
		}

	}while(avail<0)
	device_write();
	return ret;
}
	#endif

//noblock
int select(int numfds,fd_set *readfds,fd_set *writefds,fd_set *exceptfds,struct timeval *timeout);
//numfds 所有监控文件最高fd +1
//初始化FD_SET
FD_ZERO(fd_set *readfds);
//如何将fd添加到fd_set中
FD_SET(int fd,fd_set *readfds);
//如何从fd_set清除一个fd
FD_CLR(int fd,fd_set *readfds);
//判断readfds中的某个fd可读
FD_ISSET(int fd,fd_set *readfds);

//poll机制
int poll(struct pollfd *fd,nfds_t nfds,int timeout);
int epoll_create(int size);
int epfd=epoll_create(5);
epoll_ctl(int epfd,int op,int fd,struct epoll_event *event);
//op
EPOLL_CTL_ADD
EPOLL_CTL_DEL
EPOLL_CTL_MOD
//struct epoll_event *event
//代表要监控的fd的事件类型
struct epoll_event {
	__u32 events;
	__u64 data;
} EPOLL_PACKED;
EPOLLIN
EPOLLOUT
EPOLLPRI
EPOLLERR
EPOLLUP
EPOLLET
// epoll等待事件到达 events是从内核返回的到达时间 maxevents监控的最多的fd个数 返回值是到达事件的个数
int epoll_wait(int epfd,struct epoll_event *events,int maxevents,int timeout);

