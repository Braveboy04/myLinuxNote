/******************************************************************************
*
*  Copyright (C), 2001-2005, Huawei Tech. Co., Ltd.
*
*******************************************************************************
*  File Name     : tcp.c
*  Version       : Initial Draft
*  Author        : chandler
*  Created       : 2019/8/2
*  Last Modified :
*  Description   : tcp send file
*  Function List :
*
*       1.                init_tcp
*       2.                init_tcp_node
*       3.                register_tcp_method
*       4.                tcp_send
*
*  History:
* 
*       1.  Date         : 2019/8/2
*           Author       : chandler
*           Modification : Created file
*
******************************************************************************/

/*==============================================*
 *      include header files                    *
 *----------------------------------------------*/

#include "tcp.h"

/*==============================================*
 *      constants or macros define              *
 *----------------------------------------------*/
/*==============================================*
 *      project-wide global variables           *
 *----------------------------------------------*/

/*==============================================*
 *      routines' or functions' implementations *
 *----------------------------------------------*/

/*****************************************************************************
*   Prototype    : init_tcp_node
*   Description  : init tcp send type
*   Input        : db_ldata_pt pdata  
*   Output       : None
*   Return Value : static void
*   Calls        : 
*   Called By    : 
*
*   History:
* 
*       1.  Date         : 2019/8/2
*           Author       : chandler
*           Modification : Created function
*
*****************************************************************************/
static void init_tcp_node(db_ldata_pt pdata )
{
    if ((pdata->sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) 
    {
        printf("socker create error!\n");
        exit(0);
    }
    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port   = htons(TCP_PORT);
    if (inet_pton(AF_INET, TCP_SERVER, &servaddr.sin_addr) <= 0 )
    {
        printf("net_pton error!\n");
        exit(0);
    }
    if (connect(pdata->sock_fd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        printf("connect error!\n");
        exit(0);
    }
}

/*****************************************************************************
*   Prototype    : tcp_send
*   Description  : tcp send function
*   Input        : db_ldata_pt pdata  
*                  void* sendbuffer   
*   Output       : None
*   Return Value : static int
*   Calls        : 
*   Called By    : 
*
*   History:
* 
*       1.  Date         : 2019/8/2
*           Author       : chandler
*           Modification : Created function
*
*****************************************************************************/
static int tcp_send(db_ldata_pt pdata,void* sendbuffer)
{
    int ret = write(pdata->sock_fd,sendbuffer,strlen(sendbuffer));
    if(ret<0)
    {
        printf("send error:%d result:%s'\n",errno, strerror(errno));
        exit(0);
    }
    else
    {
        printf("send success,send length :%d \n",ret);
    }
    return ret;
}


static db_ldata_t tcp_node= {
	.id		= RAW_TCP,
    .flag   = INUSE,
    .Init   = init_tcp_node,
    .SendFunction = tcp_send,
};

/*****************************************************************************
*   Prototype    : register_tcp_method
*   Description  : tcp register function
*   Input        : db_list_pt ptlist  
*                  void * pdata       
*   Output       : None
*   Return Value : static void
*   Calls        : 
*   Called By    : 
*
*   History:
* 
*       1.  Date         : 2019/8/2
*           Author       : chandler
*           Modification : Created function
*
*****************************************************************************/
static void register_tcp_method( db_list_pt ptlist, void * pdata )
{
     if(*ptlist==NULL || ptlist==NULL)
    {
        errno = EINVAL;
        return ;
    }
    __db_list_insert_before(&ptlist,1,pdata);
}

/*****************************************************************************
*   Prototype    : init_tcp
*   Description  : tcp send typer register
*   Input        : db_list_pt ptlist  
*   Output       : None
*   Return Value : void
*   Calls        : 
*   Called By    : 
*
*   History:
* 
*       1.  Date         : 2019/8/2
*           Author       : chandler
*           Modification : Created function
*
*****************************************************************************/
void init_tcp(db_list_pt ptlist)
{
    register_tcp_method(ptlist,(void *)&tcp_node);
}


