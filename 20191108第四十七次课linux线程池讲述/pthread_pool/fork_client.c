#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>

//定义服务端的端口号
#define PORT_TCP 9001

int main(int argc ,char *argv[])
{
    int sockfd;
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
    if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1)
    {
        exit(1);
    }

    //设置创建好的socket
     //进行socket addr的设置
    bzero(&server_addr,sizeof(struct sockaddr_in));  //初始化为0
    server_addr.sin_family=AF_INET; //设置IPV4协议族
    //如果要设置地址和端口号，必须转化为网络字节序
    server_addr.sin_addr=*((struct in_addr *)host->h_addr); //ip地址
    //server_addr.sin_addr.s_addr=inet_addr("192.168.1.1");
     server_addr.sin_port=htons(PORT_TCP);

    if(connect(sockfd,(struct sockaddr *)(&server_addr),sizeof(struct sockaddr))==-1)
    {
        exit(1);
    }


   while(1)
   {
    //获得客户端标准输入的字符串数据
      if(fgets(ucSendBuf,1000,stdin))
      {
         iSendLen=send(sockfd,ucSendBuf,strlen(ucSendBuf),0);
         if(iSendLen<=0)
         {
              close(sockfd);
              exit(0);
         }
      }
      length=read(sockfd,buffer,1000);
      if(length)
      {
          buffer[length]='\0';
          printf("received  from ftp :%s\n",buffer);
          length=0;
      }
   }
}
