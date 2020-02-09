/******************************************************************************
*
*  Copyright (C), 2001-2005, Huawei Tech. Co., Ltd.
*
*******************************************************************************
*  File Name     : linklist.h
*  Version       : Initial Draft
*  Author        : chandler
*  Created       : 2019/7/31
*  Last Modified :
*  Description   : linklist.c header file
*  Function List :
*
*
*  History:
* 
*       1.  Date         : 2019/7/31
*           Author       : chandler
*           Modification : Created file
*
******************************************************************************/
#ifndef __LINKLIST_H__
#define __LINKLIST_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */


/*==============================================*
 *      include header files                    *
 *----------------------------------------------*/
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>


/*==============================================*
 *      constants or macros define              *
 *----------------------------------------------*/
        
#define HTTP         0x1
#define RAW_TCP      0x2
#define UDP          0x3
#define HTTPS        0x4 
        
#define INUSE        0x1
#define UNUSE        0x0 
        


/*==============================================*
 *      project-wide global variables           *
 *----------------------------------------------*/
    
typedef unsigned int   u32;
typedef unsigned short u16;
typedef unsigned char  u8;
    
    
typedef struct db_ldata{
    u8   id;              //type of protocal
    u8   flag;            //enable or disable
    int  sock_fd ;
    u8 * send_buffer    ;
    u32 (* Init)(struct db_ldata *pdata);
    int (* SendFunction)(struct db_ldata * pdata,void* sendbuffer);
}db_ldata_t;
typedef db_ldata_t * db_ldata_pt;


typedef struct db_lnode{
    void *               data;
    struct db_lnode  *   prev;
    struct db_lnode  *   next;
}db_lnode_t;
typedef db_lnode_t * db_lnode_pt;


typedef struct db_list
{
    u32         limit_size;
    db_lnode_pt head;
    db_lnode_pt tail;
}db_list_t;
typedef db_list_t * db_list_pt;

extern int errno;



/*==============================================*
 *      routines' or functions' implementations *
 *----------------------------------------------*/

db_list_pt db_list_create( void );
int __db_list_delete(db_list_t **list_head, int num );
int __db_list_insert_after( db_list_t ** list_head, int num, void * new_node_data);
int __db_list_insert_before(db_list_t ** list_head, int num, void * new_node_data );
int __db_list_modify(db_list_t **list_head, int num, void *new_node_data );
int __db_list_search(db_list_t ** list_head, void * find_data, int(*compare)(void*,void*) );
void  __db_list_travel(db_list_t * list_head, void(*do_function)(void *));
static inline void *  __db_list_visit(db_list_t ** list_head, u32 num );

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __LINKLIST_H__ */
