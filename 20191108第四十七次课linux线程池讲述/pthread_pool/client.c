#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#define PORT_TCP 9001
struct info
{
	char flag ;			//1:file attribute; 2:file content
	char buf[256];		//require file name
	int local_begin;	//if get attribute, no used
	int length;			//if get attribute, no used
};

int main(int argc,char *argv[])
{
	 int client_fd;
    char buffer[1024];
    int length;
    int iSendLen;
   unsigned char ucSendBuf[1024];
   struct hostent *host;
   struct sockaddr_in server_addr;//ftp addr
    if(argc!=2)
   {
        printf("./%s hostname \n",argv[0]);
        exit(0);
    }
  //把用户传入的IP地址进行转化
  if((host=gethostbyname(argv[1]))==NULL)
  {
      exit(1);
  }
  //创建客户端的SOCKET程序 
    if((client_fd=socket(AF_INET,SOCK_STREAM,0))==-1)
    {
        exit(1);
    }

    //设置创建好的socket
     //进行socket addr的设置
    bzero(&server_addr,sizeof(struct sockaddr_in));  //初始化为0
    server_addr.sin_family=AF_INET; //设置IPV4协议族
    //如果要设置地址和端口号，必须转化为网络字节序
    server_addr.sin_addr=*((struct in_addr *)host->h_addr); //ip地址
     server_addr.sin_port=htons(PORT_TCP);

    if(connect(client_fd,(struct sockaddr *)(&server_addr),sizeof(struct sockaddr))==-1)
    {
        exit(1);
    }

	struct info sendinfo;
	
	sendinfo.flag = 1;
	sprintf(sendinfo.buf,"%s",argv[2]);
	
	if(-1 == send(client_fd,&sendinfo, sizeof(sendinfo),0))
	{
		perror("send");exit(EXIT_FAILURE);
	}
	
	char buf[1024];
	memset(buf,'\0',1024);
	if(-1 == recv(client_fd,buf,1024,0))
	{
		perror("recv");exit(EXIT_FAILURE);
	}	
	
	close(client_fd);
	if(-1==(client_fd=socket(AF_INET,SOCK_STREAM,0)))
	{
		perror("socket");exit(EXIT_FAILURE);
	}
	

	if(-1 == connect(client_fd,(struct sockaddr *)&server_addr,sizeof(server_addr)))
	{
		perror("connect");exit(EXIT_FAILURE);
	}
	
	sendinfo.flag = 2;
	sendinfo.local_begin = 0;
	sendinfo.length = atoi (buf);

	if(-1 == send(client_fd,&sendinfo, sizeof(sendinfo),0))
	{
		perror("send");exit(EXIT_FAILURE);
	}
	
	int file_fd;
	file_fd = open(argv[3],O_WRONLY|O_CREAT,0644);
	
	lseek(file_fd,sendinfo.local_begin,SEEK_SET);
	
	printf("length = %d\n",ntohl(sendinfo.length));
	int ret = 0;
	int sum = 0;
	do
	{
		ret = recv(client_fd,buf,1024,0);
		write(file_fd,buf,ret);
		sum  = sum + ret;
		printf("sum=%d\n",sum);
		getchar();
	}while(sum != ntohl(sendinfo.length));
	close(client_fd);
	close(file_fd);
}




