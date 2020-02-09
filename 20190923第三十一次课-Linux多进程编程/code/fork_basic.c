#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
int main(int argc,char *argv[])
{
	pid_t pid;
	if((ret=fork())==-1)
		printf("fork error");
	if(ret==0)
	{
		printf("This is child\n");//pid 
	}
	else
		printf("This is father\n");
	return 0;
}


file1
file2
file3