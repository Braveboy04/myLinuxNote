#include <stdio.h>
#include <errno.h>


typedef unsigned int   u32;
typedef unsigned short u16;
typedef unsigned char  u8;


#define BUFFER_SIZE 2048

#define HTTP         0x1
#define RAW_TCP      0x2
#define UDP          0x3
#define HTTPS        0x4 

#define INUSE        0x1
#define UNUSE        0x0 


typedef struct db_ldata{
    u8 id;              //type of protocal
    u8 flag;            //enable or disable
    u8 send_buffer[BUFFER_SIZE];
    u32 (* Init)(void);
    u32 (* SendFunction)(int fd,u8 * send_buffer,u32 length);
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
        }
    }
}
