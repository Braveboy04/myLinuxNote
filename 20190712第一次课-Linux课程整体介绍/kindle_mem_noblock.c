/******************************************************************************

                  ��Ȩ���� (C), 2017-2017, chandler

 ******************************************************************************
  �� �� ��   : kindle_mem_noblock.c
  �� �� ��   : ����
  ��    ��   : chandler
  ��������   : 2017��4��2��
  ����޸�   :
  ��������   : �ں��ַ��豸����
  �����б�   :
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
  �޸���ʷ   :
  1.��    ��   : 2017��4��2��
    ��    ��   : chandler
    �޸�����   : �����ļ�

******************************************************************************/

/*----------------------------------------------*
 * �ⲿ����˵��                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �ⲿ����ԭ��˵��                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �ڲ�����ԭ��˵��                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ȫ�ֱ���                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ģ�鼶����                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ��������                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �궨��                                       *
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
    //����򿪼�����Ϊ0˵���н����Ѿ����˸��豸���ʽ����˳�
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
 �� �� ��  : ReadKindleMemDev
 ��������  : �ַ��豸��ȡ����
 �������  : struct file *pFile  
             char __user *pBuf   
             size_t iSize        
             loff_t *ptPos       
 �������  : ��
 �� �� ֵ  : static ssize_t
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��4��2��
    ��    ��   : chandler
    �޸�����   : �����ɺ���

*****************************************************************************/
static ssize_t ReadKindleMemDev(struct file *pFile, char __user *pBuf, size_t iSize, loff_t *ptPos)
{
     int               iRet   =0;
     unsigned int iCount =iSize;
     //�õ�˽���豸�ṹ��
     PT_KindleMemDev ptKindleDev=pFile->private_data;
	 //���嵱ǰ����read�Ľ��̵ĵȴ����нڵ�
	 DECLARE_WAITQUEUE(wait,current);
	 //���뻥����
	 mutex_lock(&ptKindleDev->mutex);
	 //�ѵ�ǰ�ڵ����ȴ�����
	 add_wait_queue(&ptKindleDev->r_wait,&wait);
	 //���пɶ��ж�
	 //while ��ԭ��: �ٴ��жϵ����̻���֮�����һ�� �Ƿ��ֱ���������������Դ
	 while(ptKindleDev->len==0)
	 {
		//�����ǰ��FIFO���ǿյ�  û������
		if(pFile->f_flags & O_NONBLOCK)
		{
			iRet  = -EAGAIN;
			goto  out;
		}
		//���ÿ��ж�˯��״̬
		__set_current_state(TASK_INTERRUPTIBLE);
		mutex_unlock(&ptKindleDev->mutex);
		//����ǰ�����л�
		schedule();
		//��������֮��
		//�жϻ���Դ�Ƿ�Ϊ�ź�
		if(signal_pending(current))
			{
			iRet	= -ERESTARTSYS;
			goto out2;
		}
		//����Ϊֹ Ϊ��������Դ�ͷŻ���
		mutex_lock(&ptKindleDev->mutex);
	 }
	 // �����Ķ�����
	    if(iCount>ptKindleDev->len)
       	   iCount=ptKindleDev->len;
		 
	    if(copy_to_user(pBuf,ptKindleDev->mem,iCount))
	     {
	          iRet=-EFAULT;
			  goto out;
	     }
	    else
	     {
	     	 //��δ�������ݷŵ��Ѿ����߿ռ���
	          memcpy(ptKindleDev->mem,ptKindleDev->mem+iCount,ptKindleDev->len-iCount);
			 //���ܳ���������
			  ptKindleDev->len -=iCount;
			  printk("Read %d bytes from %d��current_len:%d\n",iCount, ptKindleDev->len);
			  //��Ϊ���������ͷ�һ���ռ䣬����Ҫ����д�����ĵȴ�����
			  wake_up_interruptible(&ptKindleDev->w_wait);
			  
	          iRet=iCount;	         
	     }
out:
	mutex_unlock(&ptKindleDev->mutex);
out2:
	//�Ƴ����������뵽�ȴ������еĽڵ�
	remove_wait_queue(&ptKindleDev->r_wait,&wait);
	//�ظ�������ִ��״̬
	set_current_state(TASK_RUNNING);
	return iRet;
}


static ssize_t WriteKindleMemDev(struct file *pFile, const char __user *pBuf, size_t iSize, loff_t *ptPos)
{
      int           iRet   =0;
      unsigned int  iCount =iSize;
      //�õ�˽���豸�ṹ��
      PT_KindleMemDev ptKindleDev=pFile->private_data;
	  //�ѵ�ǰ�ڵ��װΪд�ȴ����нڵ�
	  DECLARE_WAITQUEUE(wait,current);
	  mutex_lock(&ptKindleDev->mutex);
	  add_wait_queue(&ptKindleDev->w_wait,&wait);

	 while(ptKindleDev->len==KINDLE_MEM_SIZE)
	 {
		//FIFO Ϊ�� �޷�д�� �ѵ�ǰд��Ľ��̹��ڵȴ�����������
		if(pFile->f_flags & O_NONBLOCK)
		{
			iRet = -EAGAIN;
			goto  out;
		}
		//����˯��
		__set_current_state(TASK_INTERRUPTIBLE);
		//����һ��Ҫ�����ڽ��̵���֮ǰ
		mutex_unlock(&ptKindleDev->mutex);
		schedule();
		//�жϻ���Դ�Ƿ�Ϊ�ź�
		if(signal_pending(current))
		{
			iRet	= -ERESTARTSYS;
			goto out2;
		}
		mutex_lock(&ptKindleDev->mutex);
	 }
	 //��չ������  
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
       //�õ�˽���豸�ṹ��
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
      //�õ�˽���豸�ṹ��
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
      //���õ�ǰ�豸�ṹ���е�cdev����
        cdev_init(&ptKindleMemDev->cdev, &g_tKindleMemFops);
        ptKindleMemDev->cdev.owner=THIS_MODULE;
     // �ύ���úõ�Cdev�ṹ��
       iRet=cdev_add(&ptKindleMemDev->cdev, tDevno,KINDLE_MEM_MINOR_COUNT);
       if(iRet)
       {
          printk(KERN_NOTICE "Error %d adding KindleMem%d",iRet,iMinor);
       }
       memset(ptKindleMemDev->mem,0,KINDLE_MEM_SIZE);
       //�����������г�ʼ��
      spin_lock_init(&ptKindleMemDev->lock);
      ptKindleMemDev->OpenCnt=0;
      rwlock_init(&ptKindleMemDev->rwlock);
	  mutex_init(&ptKindleMemDev->mutex);
	  //��ʼ����ǰFIFO����Ϊ0
	  ptKindleMemDev->len=0;
	  //��ʼ����ǰ�ĵȴ�����
	  init_waitqueue_head(&ptKindleMemDev->r_wait);
	  init_waitqueue_head(&ptKindleMemDev->w_wait);
}
static int __init InitKindleMemDevice(void)
{
      int iRet;

       // �����豸�ṹ��
      g_ptKindleMemDev=kzalloc(sizeof(T_KindleMemDev),GFP_KERNEL);
      if(!g_ptKindleMemDev)
      {
          iRet=-ENOMEM;
          goto fail_malloc;
      }
      
      dev_t tDevno=MKDEV(g_ptKindleMemDev->major, 0);
      //����Cdev�ṹ��
      if(g_ptKindleMemDev->major)
      {
          //�Ѿ������cdev�ṹ��
          iRet=register_chrdev_region(g_ptKindleMemDev->major,KINDLE_MEM_MINOR_COUNT,"KindleMem");
      }
      else
      {
           //���û�з����cdev�ṹ��
           iRet=alloc_chrdev_region(&tDevno,KINDLE_MEM_MINOR_BASE ,KINDLE_MEM_MINOR_COUNT,"KindleMem");
           g_ptKindleMemDev->major=MAJOR(tDevno);
      }
      if(iRet<0)
          return iRet;
     
      //���豸�ṹ��������
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

//����һ��������
spinlock_t lock;

//��ʼ��������
spin_lock_init(lock);

//���������
spin_lock(spinlock_t * lock);


//�ٽ�������

//�ͷ�������
spin_unlock(spinlock_t * lock);

//���Ի��������
spin_trylock(spinlock_t * lock);
//�õ��᷵��true
//û�õ��᷵��false

//�������ر��ж�
spin_lock_irq(spinlock_t * lock);
spin_unlock_irq(spinlock_t * lock)
spin_lock_irqsave(lock, flags)
spin_unlock_irqrestore(spinlock_t * lock, unsigned long flags)
#endif
#if 0
//����ͳ�ʼ����д������
rwlock_t rwlock;
rwlock_init(&rwlock);

//������
void read_lock(&rwlock);
void read_lock_irq(&rwlock);
void read_lock_irqsave(&rwlock);

//������
void read_unlock(&rwlock);
void read_unlock_irq(&rwlock);
void read_lock_irqrestore(&rwlock);

//д����
void write_lock(&rwlock);
void write_lock_irq(&rwlock);
void write_lock_irqsave(&rwlock);

//д����
void write_unlock(&rwlock);
void write_unlock_irq(&rwlock);
void write_lock_irqrestore(&rwlock);

#endif

#if 0
//��ȡ/����һ��˳����
void write_seqlock(seqlock_t * sl);
void write_seqlock_irqsave(seqlock_t * sl);

//�ͷ�˳����
void write_sequnlock(seqlock_t * sl);
void write_sequnlock_irqrestore(seqlock_t * sl);

//������
read_seqbegin(const seqlock_t * sl);
read_seqbegin_or_lock_irqsave(seqlock_t * lock, int * seq)
	#endif
#if 0
//����ȴ�����
wait_queue_head_t WaitQueue;
//��ʼ���ȴ�����
init_waitqueue_head(&WaitQueue);

//���岢��ʼ���ȴ����еĿ�ݷ�ʽ

DECLARE_WAIT_QUEUE_HEAD(WaitQueue)

//����һ�����н��   �ѽ��̷�װ��һ���ȴ������ϵĽڵ�
DECLARE_WAITQUEUE(name, tsk)

//��� �Ƴ��ȴ����нڵ�
void add_wait_queue(wait_queue_head_t * q, wait_queue_t * wait)
void remove_wait_queue(wait_queue_head_t * q, wait_queue_t * wait)

//�ȴ��¼�
wait_event(queue, condition); //�����ж�˯��״̬
wait_event_interruptible(wq, condition); //���ж�˯��״̬
wait_event_timeout(wq, condition, timeout);
wait_event_interruptible_timeout(wq, condition, timeout);

//���Ѷ���
void wake_up(&WaitQueue);


static ssize_t WriteKindleMemDev(struct file *pFile, const char __user *pBuf, size_t iSize, loff_t *ptPos)
{
	//�ѵ�ǰwrite�Ľ��̶���Ϊһ���ȴ����еĽڵ�
	DECLARE_WAITQUEUE(wait,current);
	//����õĶ�������Ӹýڵ�
	add_wait_queue(&xxx_wait,&wait);
	
	//�豸��д����
	do{
		//�ж��Ƿ��д
		avail=device_writeable();
		if(avail<0)
		{
		//�ж��Ƿ��Է�������ʽ��
		// open("file_name",flags)
		//flags: O_NONBLOCK O_RWDR
		   if(file->f_flags & O_NONBLOCK)
		   	{
			   ret=-EAGAIN;
			   break;
		   }
		   //���õ�ǰ����Ϊ���ж�˯��
		   //wait_event(&xxx_wait, 1);
		  __set_current_state(TASK_INTERRUPTIBLE);
		   //����ǰ���̽����л�
		  schedule(void);
		   //�жϵ�ǰ�����Ǳ��źŻ���?
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
//numfds ���м���ļ����fd +1
//��ʼ��FD_SET
FD_ZERO(fd_set *readfds);
//��ν�fd��ӵ�fd_set��
FD_SET(int fd,fd_set *readfds);
//��δ�fd_set���һ��fd
FD_CLR(int fd,fd_set *readfds);
//�ж�readfds�е�ĳ��fd�ɶ�
FD_ISSET(int fd,fd_set *readfds);

//poll����
int poll(struct pollfd *fd,nfds_t nfds,int timeout);
int epoll_create(int size);
int epfd=epoll_create(5);
epoll_ctl(int epfd,int op,int fd,struct epoll_event *event);
//op
EPOLL_CTL_ADD
EPOLL_CTL_DEL
EPOLL_CTL_MOD
//struct epoll_event *event
//����Ҫ��ص�fd���¼�����
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
// epoll�ȴ��¼����� events�Ǵ��ں˷��صĵ���ʱ�� maxevents��ص�����fd���� ����ֵ�ǵ����¼��ĸ���
int epoll_wait(int epfd,struct epoll_event *events,int maxevents,int timeout);

