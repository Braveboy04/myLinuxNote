#ifndef _THREAD_POOL_H_
#define _THREAD_POOL_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include <signal.h>
#include <string.h>

typedef struct tp_work_arg_s tp_work_arg;
typedef struct tp_work_s tp_work;
typedef struct tp_thread_info_s tp_thread_info;
typedef struct tp_thread_pool_s tp_thread_pool;

//�̲߳���
struct tp_work_arg_s{
        void *args;
};

//�̵߳Ĺ�������
struct tp_work_s{
	void (*process_job)(void*arg);
};

//�߳̽ṹ����Ҫ���������̵߳���Ϣ
//����һ���߳�
struct tp_thread_info_s{
	pthread_t		thread_id;	//�߳�id

	int  		status;	//�߳�״̬: 1:æ  2���� 
	int need_exit;//�߳��˳���־  1����Ҫ�˳���0������Ҫ�˳�
	//��ǰ�̵߳����������ͻ�����
	pthread_cond_t          thread_cond;
	pthread_mutex_t		thread_lock;
	//�̵߳Ĵ�����
	tp_work			*th_work;
	//�������Ĳ���
	tp_work_arg		*th_arg;
	struct tp_thread_info_s *next,*pre;
};

//�̳߳ؽṹ
//����һ���̳߳�
struct tp_thread_pool_s{

	int min_th_num;		//�̳߳����̵߳���Сֵ
	int cur_th_num;		//�̳߳��е�ǰ���߳���
	int max_th_num;      //�̳߳������������߳���
	int idle_th_num;			//�߳��еĿ��е��߳���


	//�̳߳صĻ�����
	pthread_mutex_t tp_lock;
	//�����̵߳��߳�ID
	pthread_t manage_thread_id;	
	//�߳̽ṹ������
	tp_thread_info *thread_info;	//�����߳�����
};


//�����̳߳�
tp_thread_pool *creat_thread_pool(int min_num, int max_num);
//���̳߳��������
int tp_add_job(tp_thread_pool *this, tp_work *worker, tp_work_arg *arg);


#endif

