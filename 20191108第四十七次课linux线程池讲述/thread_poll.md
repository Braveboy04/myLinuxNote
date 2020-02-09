# Thread_poll 应用
## 基本原理 
生成预派生线程池，创建任务池，所有的外部请求都投放入任务池
管理线程---讲任务分发给空闲线程
线程执行某个任务后，线程不会被释放
监管线程---线程监管，动态创建 销毁--->动态平衡 
任务线程---获取任务并且添加到任务池中 

## 三个管理线程。和一个池的概念
### 对象
* 任务池对象 
```
typedef struct task_queue 
{
  pthread_mutex_t mutex;
  pthread_cond_t cond;          /* when no task, the manager thread wait for ;when a new task come, signal. */
  struct task_node *head;       /* point to the task_link. */
  int number;                   /* current number of task, include unassinged and assigned but no finished. */
} TASK_QUEUE_T;

typedef struct task_node 
{
  void *arg;                    /* fun arg. */
  void *(*fun) (void *);        /* the real work of the task. */
  pthread_t tid;                /* which thread exec this task. */
  int work_id;                  /* task id. */
  int flag;                     /* 1: assigned, 0: unassigned. */
  struct task_node *next;        
  pthread_mutex_t mutex;        /* when modify this ds and exec the work,lock the task ds. */
} TASK_NODE;


```

* 线程池对象
```
typedef struct pthread_node 
{
  pthread_t tid;               /* the pid of this thread in kernel,the value is  syscall return . */
  int flag;                    /*  1:busy, 0:free. */
  struct task_node *work;      /*  if exec a work, which work. */
  struct pthread_node *next;
  struct pthread_node *prev;
  pthread_cond_t cond;        /* when assigned a task, signal this child thread by manager. */
  pthread_mutex_t mutex; 

} THREAD_NODE;

typedef struct task_queue 
{
  pthread_mutex_t mutex;
  pthread_cond_t cond;          /* when no task, the manager thread wait for ;when a new task come, signal. */
  struct task_node *head;       /* point to the task_link. */
  int number;                   /* current number of task, include unassinged and assigned but no finished. */
} TASK_QUEUE_T;
```


### 主线程
* 创建管理线程
* 创建任务管理线程
* 监管线程
* 创建任务队列
* 创建线程池 
```
    task_queue_head = (TASK_QUEUE_T *) malloc (sizeof (TASK_QUEUE_T));
  
  task_queue_head->head = NULL;  

    task_queue_head->number = 0;  

    pthread_cond_init (&task_queue_head->cond, NULL); 
  
  pthread_mutex_init (&task_queue_head->mutex, NULL);  

```


* 创建线程池对象
```
    /*init the pthread_queue_idle */ 
     pthread_queue_idle =
    (PTHREAD_QUEUE_T *) malloc (sizeof (PTHREAD_QUEUE_T));
  
  pthread_queue_idle->number = 0; 
  
  pthread_queue_idle->head = NULL;  
  
  pthread_queue_idle->rear = NULL;  
  
  pthread_mutex_init (&pthread_queue_idle->mutex, NULL);  

     pthread_cond_init (&pthread_queue_idle->cond, NULL);  

    /*init the pthread_queue_busy */ 
    pthread_queue_busy =
    (PTHREAD_QUEUE_T *) malloc (sizeof (PTHREAD_QUEUE_T));
  
  pthread_queue_busy->number = 0;  
  
  pthread_queue_busy->head = NULL; 
  
  pthread_queue_busy->rear = NULL; 
  
  pthread_mutex_init (&pthread_queue_busy->mutex, NULL);  

  pthread_cond_init (&pthread_queue_busy->cond, NULL);  

  /*create thread poll */ 
  create_pthread_pool ();
所有的线程创建后，都阻塞在自己的条件变量上 
```
* 创建任务管理线程




### 任务管理线程
* 创建任务对象
* 封装任务
* 加入任务池
 面向对象编程 
* 通知管理线程(条件变量)
通过条件变量激活管理线程

### 管理线程
* 阻塞等待任务池发来的条件 变量
* 从任务池中取任务
* 从线程池中取空闲线程
* 使用空闲线程执行任务
* 任务节点 和 线程节点相联系 
* 激活工作的子线程
* 

### 监控线程 
* 监控管理线程池空闲线程数量
* 监控管理线程池资源情况

### 工作子线程
* 注意优化方法 
 直接调用刚刚释放的线程 





### 项目设计
1. 把握框架 
2. 细节到模块 
3. 流程分析 
	

对象分析：	
typedef struct {
    ad_thread *pool_maintainer;
    ad_queue *thread_queue;
    ad_queue *request_queue;
}ad_thread_pool;	

typedef struct {
    int node_count;
    ad_node *head;
    ad_node *last;
    ad_queue_mutex *mutex;
    ad_queue_cond *cond;
}ad_queue;

typedef struct node {
    void *data;
    struct node *next;
    struct node *previous;
}ad_node;


ad_queue_mutex request_mutex = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;

  pthread_mutex_t request_mutex
pthread_mutex_init(*request_mutex);



typedef struct tpool {
    int             shutdown;                    /* 线程池是否销毁 */
    int             max_thr_num;                /* 最大线程数 */
    pthread_t       *thr_id;                    /* 线程ID数组 */
    tpool_work_t    *queue_head;                /* 线程链表 */
    pthread_mutex_t queue_lock;                    
    pthread_cond_t  queue_ready;
}tpool_t;


