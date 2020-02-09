/*****************************************************************************
*   Prototype    : main
*   Description  : main
*   Input        : int argc
*                  char * argv[]
*   Output       : None
*   Return Value : int
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
int main( int argc, char * argv[] )
{
   db_list_pt remote_gate = db_list_create();
   init_http(remote_gate);
   init_tcp(remote_gate);
   __db_list_travel(remote_gate, inuse_to_send);
}


void inuse_to_send(db_ldata_pt pdata)
{
    int ret=0 ; 
    if(pdata->flag==INUSE)
    {
       pdata->Init(pdata);
       ret=pdata->SendFunction(pdata,"hello world");
       printf("Inuse type: %d send success",pdata->id);
    }

}
