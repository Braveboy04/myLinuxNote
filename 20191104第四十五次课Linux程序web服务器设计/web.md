# Linux Server Framework
## 主要模块 /communicate
C10K

![](http://i.imgur.com/wxPUaml.jpg)

这份代码框架包括（没有具体业务实现）

* reactor模式
* IO复用，统一信号源   IO通信模块 
	多线程accept 
	select  poll  
	epoll 
	
* 线程池，连接池    数据处理
* 消息队列。各个模块都是通过消息队列完成，异步插入
* 使用函数指针实现C的回调机制
* Linux下基本tcp通信

## 线程安全log系统 /spirit_log
* 线程锁
* pthread_key

1.  标准的日志格式----->syslog
2.  日志流量方式，传回日志服务器 
3.  自定义日志文件写入 
4.  

## 连接池conn_pool
* 连接池及队列
* 使用回调方式

## CGI程序 
web 
* reactor
* 日志
* 参数解析
* 配置解析 
* 访问控制模块 
* CGI 

参数的传递：通过环境变量 
数据的传输通道：标准输入输出重定向进行 


http://192.168.199.232:8000/cgi-bin/getAuth?email=123%40qq.com&password=123456



*Get模式CGI参数传递
1. get模式中参数是在URL的后面绑定的
2. 传递参数设置了固定的环境变量QUERY_STRING 
3. CGI程序从环境变量QUERY_STRING 中获取参数



# syslog

1. 标准是统一的 



## 配置解析模块

1. 自行设置getconfig函数
2. 使用标准的json配置模块来实现 

<https://blog.csdn.net/mengdeguodu_/article/details/90203798>

jsonc 

```
Linux 
1.下载json-c的源码

JSON下载地址:https://github.com/json-c/json-c/wiki

2.解压json-c的源码包

tar -xvf json-c-0.10.tar.gz

3.配置json-c的源码包

(1)在x86平台配置

./configure --prefix=/usr

(2)在ARM平台配置

./configure --prefix=安装路径 --host=交叉开发工具链前缀

例如安装路径为"/home/linux/json/install" 交叉开发工具链前缀为arm-none-linux-gnueabi-,则指定configuer参数如下:

./configure --prefix=/home/linux/json/install --host=arm-none-linux-gnueabi

4.编译

make

5.安装

make install

```

```c
#include "stdlib.h"
#include "stdio.h"
#include "./cJSON.h"
 
#define FIELD_TYPE_DOUBLE	0 //DOUBLE有符号整形
#define FIELD_TYPE_DOUBLE_U	1 //DOUBLE无符号整形
#define FIELD_TYPE_FLOAT	2 //FLOAT有符号整形
#define FIELD_TYPE_FLOAT_U	3 //FLOAT无符号整形
#define FIELD_TYPE_INT		4 //有符号整形(4字节)
#define FIELD_TYPE_INT_U	5 //无符号整形(4字节)
#define FIELD_TYPE_SHORT	6 //有符号短整形(2字节)
#define FIELD_TYPE_SHORT_U	7 //无符号短整形(2字节)
#define FIELD_TYPE_CHAR		8 //有符号字符(2字节)
#define FIELD_TYPE_CHAR_U	9 //无符号字符(2字节)
#define FIELD_TYPE_STRING	10 //字符串
#define FTILD_TYPE_TIME		11 //时间
 
//描述：js格式解析，只处理一层的js格式
struct TJS{
	char *Field;  //客户定义的字段
	int FieldType; //字段的格式类型，如整形，字符串
	cJSON *pJON; //json解析之后的字符串数据，实际使用时需将字符串转化成对应字段Field的数据
};
 
//根据客户定义的js字段填充如下结构体
struct TJS g_TJsCfg[] = {
	{"startTime", FTILD_TYPE_TIME},
	{"endTime", FTILD_TYPE_TIME},
	{"UUID", FIELD_TYPE_STRING},
	{"subDeviceID", FTILD_TYPE_TIME},
	{"UserType", FIELD_TYPE_INT},
	{"CredenceType", FIELD_TYPE_INT},
	{"credenceNo", FIELD_TYPE_STRING},
	{"userName", FIELD_TYPE_STRING},
	{"userID", FIELD_TYPE_STRING},
	{"opTime", FTILD_TYPE_TIME},
	{"placeNo", FIELD_TYPE_STRING},
	{"placeLockNo", FIELD_TYPE_STRING},
};
 
int g_iJsCfgNum = sizeof(g_TJsCfg)/sizeof(g_TJsCfg[0]);
 
//描述：以下根据具体的需求提取数据，这里只是纯粹的用于验证JSON解析的数据是否正确
void printf_js(cJSON *pJSON, int FieldType)
{
	switch(FieldType)
	{
	case FIELD_TYPE_DOUBLE:
	case FIELD_TYPE_DOUBLE_U:	
		printf("Name: %s, Data:%s\n", pJSON->string, pJSON->valuestring);
		break;
	case FIELD_TYPE_FLOAT:
		break;
	case FIELD_TYPE_FLOAT_U:
		break;
	case FIELD_TYPE_INT:
	case FIELD_TYPE_INT_U:
		printf("Name: %s, Data:%s\n", pJSON->string, pJSON->valuestring);
		break;
	case FIELD_TYPE_SHORT:
		break;
	case FIELD_TYPE_SHORT_U:
		break;
	case FIELD_TYPE_CHAR:
		break;
	case FIELD_TYPE_CHAR_U:
		break;
	case FIELD_TYPE_STRING:
	case FTILD_TYPE_TIME:
		printf("Name: %s, Data:%s\n", pJSON->string, pJSON->valuestring);
		break;
	default:
		printf("Unsupport fmt:%d\n", FieldType);
	}
}
 
void parse_js(void)
{
	//假设接收到客户的js字段数据为
	char *pRevJson = "{\"startTime\":\"2017-11-10 08:00:01\",\
		\"endTime\":\"2020-11-10 08:00:01\",\
		\"UUID\":\"ca23bc4456de223de11a3a7a8ac1d2e3\",\
		\"subDeviceID\":\"2020-11-10 08:00:01\",\
		\"UserType\":\"1\",\
		\"CredenceType\":\"5\",\
		\"credenceNo\":\"粤A123456\",\
		\"userName\":\"张三\",\
		\"userID\":\"1002\",\
		\"opTime\":\"2017-11-10 08:00:01\",\
		\"placeNo\":\"CA12001\",\
		\"placeLockNo\":\"1234566\"\
	}";
	
	int idx;
 
	
	//1. 初始化接收到的客户端json数据
	cJSON *root = cJSON_Parse(pRevJson); 
	if(!root) {
		printf("get root faild !\n");
		return;
	}
	//2. 解析客户的数据
	for (idx=0; idx<g_iJsCfgNum; idx++)
		g_TJsCfg[idx].pJON = cJSON_GetObjectItem(root, g_TJsCfg[idx].Field);
	
	//3. 提取客户的数据
	for (idx=0; idx<g_iJsCfgNum; idx++)
	{
		if (g_TJsCfg[idx].pJON)
			printf_js(g_TJsCfg[idx].pJON, g_TJsCfg[idx].FieldType);
		else
			printf("Name:%s, No data!\n", g_TJsCfg[idx].pJON->string);
	}
	
	//4. 释放内存，防止内存泄露！！！
	if (root)
		cJSON_Delete(root);
}
 
int main(int argc, char **argv)
{
	parse_js();
	
	return 0;

}

.htaccsee 
```



## 配置服务器后台运行

1. 自行设计daemon

2. 使用系统的daemon 函数



## web服务的http与https共存问题

1. 开启两个进程，分别在两个进程中监听2个端口 





## web的访问控制机制

192.168.1.0/24

192.168.1.23   &   255.255.255.0  ----->192.168.1.0 





## http的协议解析 

1. http的协议状态机  
2. url解析---->hash表    协议状态机提取url和参数----》将url生成hash---》进行hash表查询---->从表中找到对应的条目并获得url 
3. 



## 错误响应



```c
static void clienterror(int fd, char *cause, char *errnum, 
		 char *shortmsg, char *longmsg) 
{
    char buf[MAXLINE], body[MAXBUF];

    /* Build the HTTP response body */
    sprintf(body, "<html><title>Tiny Error</title>");
    sprintf(body, "%s<body bgcolor=""ffffff"">\r\n", body);
    sprintf(body, "%s%s: %s\r\n", body, errnum, shortmsg);
    sprintf(body, "%s<p>%s: %s\r\n", body, longmsg, cause);
    sprintf(body, "%s<hr><em>The Tiny Web server</em>\r\n", body);

    /* Print the HTTP response */
    sprintf(buf, "HTTP/1.0 %s %s\r\n", errnum, shortmsg);
    sprintf(buf, "%sContent-type: text/html\r\n",buf);
    sprintf(buf, "%sContent-length: %d\r\n\r\n",buf,(int)strlen(body));

    #ifdef HTTPS 
    if(ishttps)
    {
	SSL_write(ssl,buf,strlen(buf));
	SSL_write(ssl,body,strlen(body));
    }
    else
    #endif
    {
    	Rio_writen(fd, buf, strlen(buf));
	Rio_writen(fd, body, strlen(body));
    }
}
```



##  权限检测

1. 检测文件是否存在 
2. 检测获取的文件的权限

3. 文件属性检测

    网页木马---php asp 

   /xx/xx/xxxx.php

   .htaccess 

4. url的合法性  

   /../../../../../../../../../../../etc/passwd 

   xx/xx/xx/xx/../../../../../../../../../../../etc/passwd  

   xxx/xx/xxx/xx.htm?passwd=1212||1==1 

   xxx/xx/xxx/xx.htm?passwd=1212&&SELECT * FROM XXX_TABLE 

   

   if($_GET == passwd)

   {

   ​	do_something ;

   }

   if(passwd==1212||1==1 )

   {

   ​	do_something ;

   }

xxxx/xxxx/xxxx.exe 

/xxx/xxx/xx/xxx.png 



## 目录服务器

1. 遍历目录并且展示  



## Linux文件快速发送

mmap(addr, len, prot, flags, fd, offset)  

1 .将某个文件的内容映射到一个指定的地址上 



## cgi

192.168.199.183/cgi-bin/getAuth.html

调用方法：

​	/cgi-bin/Auth.

   /cgi-win/**cgiprogram.exe**

传递参数：

1. CGI请求

 	1. post
      	1. post 的数据体的长度获取是依靠Content-Length: 解析出来的 
 	2. get
      	1. /cgi-bin/getAuth.html?passwd=123456&&username=admin
           	1. setenv("QUERY_STRING", cgiargs, 1); 

2. CGI进程

3. 环境变量
4. 标准IO重定向
5. CGI程序启动 
6. 守护进程
7. 

实现交互：



​	

## 线程池

