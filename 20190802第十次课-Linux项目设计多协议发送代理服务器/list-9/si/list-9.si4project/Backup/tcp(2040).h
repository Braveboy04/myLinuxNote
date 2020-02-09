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




/*==============================================*
 *      constants or macros define              *
 *----------------------------------------------*/


/*==============================================*
 *      project-wide global variables           *
 *----------------------------------------------*/



/*==============================================*
 *      routines' or functions' implementations *
 *----------------------------------------------*/

void init_tcp(db_list_pt ptlist);
static void init_tcp_node(db_ldata_pt pdata );
static void register_tcp_method( db_list_pt ptlist, void * pdata );
static int tcp_send(db_ldata_pt pdata,void* sendbuffer);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __TCP_H__ */
