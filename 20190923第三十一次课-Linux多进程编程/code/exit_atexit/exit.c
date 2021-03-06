#include<stdio.h>
#include<errno.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>

int g_val = 0;

void test_exit(int status,void *arg)
{
	printf("befor exit\n");
	printf("exit %d\n",status);
	printf("arg=%s \n",(char *)arg);
}
int main()
{
    pid_t id = fork();
    if(id < 0){
        exit(1);
    }
    else if(id == 0){
        //child
        printf("child is run,child pid is %d\n",getpid());
        sleep(1);
		char *str="test";
		on_exit(test_exit,(void*)str);
        exit(100);
    }
    else{
        printf("father is run , father pid is: %d\n",getpid());
    }

    int ret;
    if(wait(&ret) < 0){
        printf("wait error , error code is : %d\n",errno);
        return 1;
    }
    printf("wait success,status code is : %d\n",ret);
    return 0;
}

