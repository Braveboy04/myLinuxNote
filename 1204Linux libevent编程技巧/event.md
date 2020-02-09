# Libevent开发 

* 依赖库：基于Linux中libevent库进行的事件响应
* 基本介绍：

```
Libevent是一个基于事件触发的网络库 ，跨平台的网络库，使用了select epoll kqueue编程结构，拥有高效的处理性能 ，数据处理模块
```

* 重点对象：

```c
event_base 是event集合，存储监听的event 

struct event {
    TAILQ_ENTRY (event) ev_next;//用来链接IO时间
    TAILQ_ENTRY (event) ev_active_next;//用来链接就绪事件，实际插入对应的
    TAILQ_ENTRY (event) ev_signal_next;//用来链接信号事件
    unsigned int min_heap_idx;  /* for managing timeouts事件在堆中的下标 */

    struct event_base *ev_base;

    int ev_fd;//对于I/O事件,是绑定的文件描述符;对于signal事件,是绑定的信号
    short ev_events;//event关注的事件类型
    /*
        包含三个事件：IO事件：EV_READ和EV_WRITE
                    信号事件：EV_SIGNAL
                    定时器事件：EV_TIMEOUT    
    */
    short ev_ncalls;//事件就绪执行时，调用ev_callback的次数
    short *ev_pncalls;  /* Allows deletes in callback */

    struct timeval ev_timeout;//timout事件的超时值

    int ev_pri;     /* smaller numbers are higher priority 优先级*/

    void (*ev_callback)(int, short, void *arg);//回调函数
    void *ev_arg;//回调函数的参数

    int ev_res;     /* result passed to event callback 传递给回调函数 */
    int ev_flags;//表明事件当前处于何种状态
    /*
        依据事件处于不同状态将其加入不同的链表中。
    */
}

struct event_base {
    const struct eventop *evsel;//底层具体I/O demultiplex操作函数集
    void *evbase;//存储IO复用返回的文件描述符相关 evbase
    int event_count;        /* counts number of total events总的事件数量 */
    int event_count_active; /* counts number of active events 就绪事件数量*/

    int event_gotterm;      /* Set to terminate loop */
    int event_break;        /* Set to terminate loop immediately */

    /* active event management */
    struct event_list **activequeues;//指针数组 ，就绪事件链表数组,每个元素防止一个链表头部
    int nactivequeues;//就绪事件队列个数，指针数组维度，也就是优先级个数

    /* signal handling info */
    struct evsignal_info sig;//用于管理信号

    struct event_list eventqueue;//注册事件队列
    struct timeval event_tv;

    struct min_heap timeheap;//管理定时器的小根堆

    struct timeval tv_cache;//记录时间缓存
}

    
```

* 编程流程(面向对象的流程)

```
1. 创建对象 
struct event_base *event_base_new(void)
2. 销毁对象
void event_base_free(struct event_base * base);
3. 获取当前自动选择的IO模型 
event_base_get_method 
4. 使用对象
基于事件循环的方法进行事件等待，event loop 
event_base_dispatch(struct *event_base * base)
在内部不断循环监听注册上来的事件 返回0成功 
int event_base_loop(struct event_base*base ,flag);
1. EVLOOP_ONCE 0x01
  
引起循环退出的情况：
	1.event_base中没有事件了 
	2.调用event_base_loopbreak 事件循环会停止 (立即停止)
	3.调用event_base_loopexit  (等待所有事件结束后停止)
	4. 程序错误 

特定头+len 
固定长度包
特殊接收符包

```

