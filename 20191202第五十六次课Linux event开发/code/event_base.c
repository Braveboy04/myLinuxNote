#include <stdio.h>  
#include <stdlib.h>  
#include <unistd.h>  
#include <sys/types.h>      
#include <sys/socket.h>      
#include <netinet/in.h>      
#include <arpa/inet.h>     
#include <string.h>  
#include <fcntl.h>   
  
#include <event2/event.h>  
#include <event2/bufferevent.h>  
  
int main() {  
    puts("init a event_base!");  
    struct event_base *base; //����һ��event_base  
    base = event_base_new(); //��ʼ��һ��event_base  
    const char *x =  event_base_get_method(base); //�鿴�����ĸ�IO��·����ģ�ͣ�linuxһ����epoll  
    printf("METHOD:%s\n", x);  
    int y = event_base_dispatch(base); //�¼�ѭ������Ϊ�������û��ע���¼������Ի�ֱ���˳�  
    event_base_free(base);  //����libevent  
    return 1;  
}