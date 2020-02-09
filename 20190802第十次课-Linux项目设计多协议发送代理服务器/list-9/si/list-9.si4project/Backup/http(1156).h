/******************************************************************************
*
*  Copyright (C), 2001-2005, Huawei Tech. Co., Ltd.
*
*******************************************************************************
*  File Name     : http.h
*  Version       : Initial Draft
*  Author        : chandler
*  Created       : 2019/8/2
*  Last Modified :
*  Description   : http.c header file
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
#ifndef __HTTP_H__
#define __HTTP_H__


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

static static int http_send(db_ldata_pt pdata,void* sendbuffer);
static void init_http(db_list_pt ptlist);
static static void init_http_node(db_ldata_pt pdata );
static static void register_http_method( db_list_pt ptlist, void * pdata );

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __HTTP_H__ */
