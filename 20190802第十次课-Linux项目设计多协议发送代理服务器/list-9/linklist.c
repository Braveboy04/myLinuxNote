/******************************************************************************
*
*  Copyright (C), 2001-2005, Huawei Tech. Co., Ltd.
*
*******************************************************************************
*  File Name     : linklist.c
*  Version       : Initial Draft
*  Author        : chandler
*  Created       : 2019/7/31
*  Last Modified :
*  Description   : linklist lib
*  Function List :
*
*       1.                db_list_create
*       2.                __db_list_delete
*       3.                __db_list_insert_after
*       4.                __db_list_insert_before
*       5.                __db_list_modify
*       6.                __db_list_search
*       7.                __db_list_travel
*       8.                __db_list_visit
*
*  History:
* 
*       1.  Date         : 2019/7/31
*           Author       : chandler
*           Modification : Created file
*
******************************************************************************/
/*==============================================*
 *      include header files                    *
 *----------------------------------------------*/
#include "link_list.h"
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
*   Prototype    : db_list_create
*   Description  : create linklist
*   Input        : void
*   Output       : None
*   Return Value : db_list_pt
*   Calls        : 
*   Called By    : 
*
*   History:
* 
*       1.  Date         : 2019/7/29
*           Author       : chandler
*           Modification : Created function
*
*****************************************************************************/
db_list_pt db_list_create( void )
{
    db_list_pt  list_head=NULL;
    list_head = (db_list_pt)malloc(sizeof(db_list_t));
    if (list_head ==NULL)
    {
        errno = ENOMEM;
        return NULL;
    }
    list_head->limit_size = 0;
    list_head->head=(db_lnode_pt)malloc(sizeof(db_lnode_t));
    if (list_head->head ==NULL)
    {
        errno = ENOMEM;
        return NULL;
    }
    list_head->head->next = list_head->head->prev =NULL; 
    list_head->head->data = NULL;
    list_head->tail=list_head->head;
    return list_head;
}


/*****************************************************************************
*   Prototype    : __db_list_insert_before
*   Description  : insert from brefore
*   Input        : db_list_t ** list_head
*                  int num
*                  void * new_node_data
*   Output       : None
*   Return Value : static inline int
*   Calls        : 
*   Called By    : 
*
*   History:
* 
*       1.  Date         : 2019/7/29
*           Author       : chandler
*           Modification : Created function
*
*****************************************************************************/
static inline int __db_list_insert_before(db_list_t ** list_head, int num, void * new_node_data )
{
    u32 counter = 1; 
    db_lnode_pt current=NULL;
    db_lnode_pt new_node=NULL;

    if(*list_head==NULL || list_head==NULL)
    {
        errno = EINVAL;
        return -1;
    }
    if((* list_head)->limit_size!=0)
    {
        new_node = (db_lnode_pt)malloc(sizeof(db_lnode_t));
        if(new_node==NULL)
        {
            errno = ENOMEM;
            return -1;
        }
        new_node->data = new_node_data;
        new_node->prev = new_node->next = NULL;
        if(num > 0 && num <=(* list_head)->limit_size)
        {
            current = (*list_head)->head;
            while ( counter < num)
            {
               counter++;
               current = current->next;
            }
            if(counter == 1)
            {
                (*list_head)->head->prev = new_node;
                new_node->next = (*list_head)->head;
                (*list_head)->head = new_node;
                (*list_head)->limit_size ++;
                return 0
            }
            current->next->prev = new_node;
            new_node->prev=current->prev;
            current->prev=new_node;
            new_node->next = current;
            (* list_head)->limit_size++;
            return 0;
        }
        else
        {
            errno = EINVAL;
            free(new_node);
            new_node = NULL;
            return -1;
        }
    }
    else
    {
        if(num!=0)
        {
            errno = EINVAL;
            return -1;
        }
        (*list_head)->head->data = new_node_data;
        (*list_head)->limit_size++;
        return 0;
    }
}


/*****************************************************************************
*   Prototype    : __db_list_insert_after
*   Description  : insert from after
*   Input        : db_list_t ** list_head
*                  int num
*                  void * new_node_data
*   Output       : None
*   Return Value : static inline int
*   Calls        : 
*   Called By    : 
*
*   History:
* 
*       1.  Date         : 2019/7/31
*           Author       : chandler
*           Modification : Created function
*
*****************************************************************************/
static inline int __db_list_insert_after( db_list_t ** list_head, int num, void * new_node_data)
{
    u32 counter = 1; 
    db_lnode_pt current=NULL;
    db_lnode_pt new_node=NULL;

    if(*list_head==NULL || list_head==NULL)
    {
        errno = EINVAL;
        return -1;
    }
    if((* list_head)->limit_size!=0)
    {
        new_node = (db_lnode_pt)malloc(sizeof(db_lnode_t));
        if(new_node==NULL)
        {
            errno = ENOMEM;
            return -1;
        }
        new_node->data = new_node_data;
        new_node->prev = new_node->next = NULL;
        if(num > 0 && num <=(* list_head)->limit_size)
        {
            current = (*list_head)->head;
            while ( counter!=num-1)
            {
               counter++;
               current = current->next;
            }
            if(current->next==NULL)
            {
                (*list_head)->tail = new_node;
                current->next =new_node;
                new_node->prev =current;
                (*list_head)->limit_size ++;
                return 0
            }
            new_node->prev =current;
            new_node->next = current->next;
            current->next->prev=new_node;
            current->next=new_node;
            (*list_head)->limit_size++;
            return 0;
        }
        else
        {
            errno = EINVAL;
            free(new_node);
            new_node = NULL;
            return -1;
        }
    }
    else
    {
        if(num!=0)
        {
            errno = EINVAL;
            return -1;
        }
        (*list_head)->head->data = new_node_data;
        (*list_head)->limit_size++;
        return 0;
    }
}

/*****************************************************************************
*   Prototype    : __db_list_delete
*   Description  : delete node
*   Input        : db_list_t **list_head
*                  int num
*   Output       : None
*   Return Value : static inline void
*   Calls        : 
*   Called By    : 
*
*   History:
* 
*       1.  Date         : 2019/7/31
*           Author       : chandler
*           Modification : Created function
*
*****************************************************************************/
static inline void __db_list_delete(db_list_t **list_head, int num )
{
    int counter =1;
    db_lnode_pt  current=NULL;
    db_lnode_pt  tmp;
    if(*list_head==NULL || list_head==NULL)
    {
        errno = EINVAL;
        return -1;
    }
    current = (* list_head)->head;
    while(counter<num)
    {
        counter++;
        current=current->next;
    }
    if(counter==1)
    {
        tmp = (*list_head)->head;
        (*list_head)->head =(*list_head)->head->next;
        free(tmp);
        tmp=NULL;
        (*list_head)->head->prev=NULL;
        (*list_head)->limit_size--;
        return ;
    }
    if((* list_head)->limit_size==counter)
    {
        tmp = (*list_head)->tail;
        (*list_head)->tail =(*list_head)->tail->prev;
        free(tmp);
        tmp=NULL;
        (*list_head)->tail->next=NULL;
        (*list_head)->limit_size--;
        return ;
    }
    tmp = current;
    current->next->prev=current->prev;
    current->prev->next=current->next;
    free(tmp);
    tmp=NULL;
    (*list_head)->limit_size--;
}

/*****************************************************************************
*   Prototype    : __db_list_modify
*   Description  : modify node
*   Input        : db_list_t *list_head
*                  int num
*                  void *new_node_data
*   Output       : None
*   Return Value : static inline void
*   Calls        : 
*   Called By    : 
*
*   History:
* 
*       1.  Date         : 2019/7/31
*           Author       : chandler
*           Modification : Created function
*
*****************************************************************************/
static inline void __db_list_modify( db_list_t *list_head, int num, void *new_node_data )
{
    int counter =0;
    db_lnode_pt current=NULL;
    if(list_head == NULL || num <0 || num > list_head->limit_size){
       errno = EINVAL;
       return -1;
    }
    current= list_head->head;
    while (counter!=num-1)
    {
        current = current->next;
        counter++;
    }
    current->data = new_node_data;
}


/*****************************************************************************
*   Prototype    :  __db_list_visit
*   Description  : search node
*   Input        : db_list_t ** list_head
*                  int num
*   Output       : None
*   Return Value : static inline void *
*   Calls        : 
*   Called By    : 
*
*   History:
* 
*       1.  Date         : 2019/7/31
*           Author       : chandler
*           Modification : Created function
*
*****************************************************************************/
static inline void *  __db_list_visit( db_list_t ** list_head, int num )
{
    int counter;
    db_lnode_pt  current=(*list_head)->head;
    if(num<0||num>(*list_head)->limit_size)
    {
        errno = EINVAL;
        return -1;
    }
    for ( counter = 0 ; counter<num  ; counter++ )
    {
        current=current->next;
    }
    return current->data;
}


/*****************************************************************************
*   Prototype    :  __db_list_travel
*   Description  : travel all node
*   Input        : db_list_t * list_head
*                  void(*do_function)(void *)
*   Output       : None
*   Return Value : static inline void
*   Calls        : 
*   Called By    : 
*
*   History:
* 
*       1.  Date         : 2019/7/31
*           Author       : chandler
*           Modification : Created function
*
*****************************************************************************/
static inline void  __db_list_travel( db_list_t * list_head, void(*do_function)(void *))
{
    u32 i =0;
    if(list_head->limit_size <0 || list_head==NULL)
    {
        errno = EINVAL;
        return -1;
    }
    for ( i = 0 ; i<list_head->limit_size ; i++ )
    {
        (*do_function)(__db_list_visit(list_head,i))
    }
}

/*****************************************************************************
*   Prototype    : __db_list_search
*   Description  : search data 
*   Input        : db_list_t ** list_head
*                  void * find_data
*                  int(*compare)(void*,void*)
*   Output       : None
*   Return Value : static inline int
*   Calls        : 
*   Called By    : 
*
*   History:
* 
*       1.  Date         : 2019/7/31
*           Author       : chandler
*           Modification : Created function
*
*****************************************************************************/
static inline int __db_list_search( db_list_t ** list_head, void * find_data, int(*compare)(void*,void*) )
{
    int counter =1 ;
    db_lnode_pt current=NULL;
    if((*list_head)==NULL || list_head==NULL)
    {
        errno = EINVAL;
        return -1;
    }
    current =(*list_head)->head;
    while ( compare(current->data,find_data)!=0 && current->next!=NULL )
    {
        current=current->next;
        counter++;
    }
    if (current->next==NULL && compare(current->data,find_data)!=0)
    {
        return 0;
    }
    return counter;
}

