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
  
void read_cb(struct bufferevent *bev, void *arg) {  
    #define MAX_LINE    256  
    char line[MAX_LINE+1];  
    int n;  
    evutil_socket_t fd = bufferevent_getfd(bev);  
    while (n = bufferevent_read(bev, line, MAX_LINE), n > 0) {  
        line[n] = '\0';  
        printf("fd=%u, read line: %s\n", fd, line);  
        bufferevent_write(bev, line, n);  
    }  
    puts("haha");  
}  
void write_cb(struct bufferevent *bev, void *arg) {}  
void error_cb(struct bufferevent *bev, short event, void *arg) {  
    evutil_socket_t fd = bufferevent_getfd(bev);  
    printf("fd = %u, ", fd);  
    if (event & BEV_EVENT_TIMEOUT) {  
        printf("Timed out\n");  
    } else if (event & BEV_EVENT_EOF) {  
        printf("connection closed\n");  
    } else if (event & BEV_EVENT_ERROR) {  
        printf("some other error\n");  
    }  
    bufferevent_free(bev);  
}  
  
//�ص����������ڼ������ӽ����Ŀͻ���socket  
void do_accept(evutil_socket_t fd, short event, void *arg) {  
    int client_socketfd;//�ͻ����׽���      
    struct sockaddr_in client_addr; //�ͻ��������ַ�ṹ��     
    int in_size = sizeof(struct sockaddr_in);    
    //�ͻ���socket    
    client_socketfd = accept(fd, (struct sockaddr *) &client_addr, &in_size); //�ȴ������������������ʽ��    
    if (client_socketfd < 0) {    
        puts("accpet error");    
        exit(1);  
    }    
  
    //����ת��  
    struct event_base *base_ev = (struct event_base *) arg;  
  
    //socket���ͻ�ӭ��Ϣ    
    char * msg = "Welcome to My socket";    
    int size = send(client_socketfd, msg, strlen(msg), 0);    
  
    //����һ���¼�������¼���Ҫ���ڼ����Ͷ�ȡ�ͻ��˴��ݹ���������  
    //�־����ͣ����ҽ�base_ev���ݵ�do_read�ص�������ȥ  
    //struct event *ev;  
    //ev = event_new(base_ev, client_socketfd, EV_TIMEOUT|EV_READ|EV_PERSIST, do_read, base_ev);  
    //event_add(ev, NULL);  
  
    //����һ��bufferevent  
    struct bufferevent *bev = bufferevent_socket_new(base_ev, client_socketfd, BEV_OPT_CLOSE_ON_FREE);  
    //���ö�ȡ������errorʱ��ķ���  
    bufferevent_setcb(bev, read_cb, NULL, error_cb, base_ev);    
    //��������  
    bufferevent_enable(bev, EV_READ|EV_WRITE|EV_PERSIST);  
    //����ˮλ��ÿ�ν���10���ַ�  
    bufferevent_setwatermark(bev, EV_READ, 0, 10);  
}  
  
  
//���������  
int main() {  
  
    int server_socketfd; //�����socket    
    struct sockaddr_in server_addr;   //�����������ַ�ṹ��      
    memset(&server_addr,0,sizeof(server_addr)); //���ݳ�ʼ��--����      
    server_addr.sin_family = AF_INET; //����ΪIPͨ��      
    server_addr.sin_addr.s_addr = INADDR_ANY;//������IP��ַ--�������ӵ����б��ص�ַ��      
    server_addr.sin_port = htons(8001); //�������˿ں�      
    
    //����������׽���    
    server_socketfd = socket(PF_INET,SOCK_STREAM,0);    
    if (server_socketfd < 0) {    
        puts("socket error");    
        return 0;    
    }    
  
    evutil_make_listen_socket_reuseable(server_socketfd); //���ö˿�����  
    evutil_make_socket_nonblocking(server_socketfd); //����������  
    
    //��IP    
    if (bind(server_socketfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr))<0) {    
        puts("bind error");    
        return 0;    
    }    
  
    //����,�������г��� 5    
    listen(server_socketfd, 10);    
      
    //����event_base �¼��ļ��ϣ����̵߳Ļ� ÿ���̶߳�Ҫ��ʼ��һ��event_base  
    struct event_base *base_ev;  
    base_ev = event_base_new();   
    const char *x =  event_base_get_method(base_ev); //��ȡIO��·���õ�ģ�ͣ�linuxһ��Ϊepoll  
    printf("METHOD:%s\n", x);  
  
    //����һ���¼�������Ϊ�־���EV_PERSIST���ص�����Ϊdo_accept����Ҫ���ڼ������ӽ����Ŀͻ��ˣ�  
    //��base_ev���ݵ�do_accept�е�arg����  
    struct event *ev;  
    ev = event_new(base_ev, server_socketfd, EV_TIMEOUT|EV_READ|EV_PERSIST, do_accept, base_ev);  
  
    //ע���¼���ʹ�¼����� pending�ĵȴ�״̬  
    event_add(ev, NULL);  
  
    //�¼�ѭ��  
    event_base_dispatch(base_ev);  
  
    //����event_base  
    event_base_free(base_ev);    
    return 1;  
}