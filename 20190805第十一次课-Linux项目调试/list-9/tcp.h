/******************************************************************************
*
*  Copyright (C), 2001-2005, Huawei Tech. Co., Ltd.
*
*******************************************************************************
*  File Name     : tcp.h
*  Version       : Initial Draft
*  Author        : chandler
*  Created       : 2019/8/2
*  Last Modified :
*  Description   : tcp.c header file
*  Function List :
*
*
*  History:
* 
*       1.  Date         : 2019/8/2
*           Author       : chandler
*           Modification : Created file
*
******************************************************************************/
#ifndef __TCP_H__
#define __TCP_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */


/*==============================================*
 *      include header files                    *
 *----------------------------------------------*/
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "linklist.h"


/*==============================================*
 *      constants or macros define              *
 *----------------------------------------------*/
#define TCP_PORT    8081
#define TCP_SERVER  "127.0.0.1"
/*==============================================*
 *      project-wide global variables           *
 *----------------------------------------------*/

/*==============================================*
 *      routines' or functions' implementations *
 *----------------------------------------------*/

void init_tcp(db_list_pt ptlist);
static u32 init_tcp_node(db_ldata_pt pdata );
static void register_tcp_method( db_list_pt ptlist, void * pdata );
static int tcp_send(db_ldata_pt pdata,void* sendbuffer);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __TCP_H__ */
