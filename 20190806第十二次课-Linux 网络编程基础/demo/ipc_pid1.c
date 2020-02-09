#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <signal.h>
#include<sys/select.h>

int OpenFile(char *path_name,int mode)
{
      int sockfd[2];
      pid_t chi_pid;
      char arg_fd[10];  //fd arg
      char arg_mode[10];// mode arg
      int status;
      if(socketpair(AF_LOCAL,SOCK_STREAM,0,sockfd)==-1)
      {
          err_sys("socketpair error!\n");
      }
      //创建子进程
     if((chi_pid=fork())==0)
     {
        //child process
        close(sockfd[0]);//child use sockfd[1]
        snprintf(arg_fd, sizeof(arg_fd),"%d",sockfd[1]);
        snprintf(arg_mode, sizeof(arg_mode),"%d",mode);
        
        execl("./pid_second","pid_second",arg_fd,arg_mode,(char *)NULL);
        
        printf("execl error\n");
         exit(EXIT_FAILURE);
     }
      close(sockfd[1]);//father use sockfd[0]

      //waitpid用法
      waitpid(chi_pid,&status,0);
      if(WIFEXITED(status)!=0)
      {
          if((status=WEXITSTATUS(status))==0)
          { 
            //child process end
            
          }
            close(sockfd[0]);
            return ;
      }
      return -1;
}



int main(int argc ,char *argv[])
{

      

}


void err_sys(const char *errmsg)
{
		perror(errmsg);
		exit(1);
}

